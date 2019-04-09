#pragma once
#include "AnimationObject.h"

/****************************************************
The concept behind this class is that it will be passed
a filename and graphics object/rendertarget, then, will
proceed to create the needed WIC components to read,
decode, and then encode the bitmap file from disk into
a compatible D2D bitmap.

We need this approach to be able to address pretty much
any bitmap from disk/resources into the game and use it
within Directx (D2D specifically for now)

*******************************************************/
AnimationObject::AnimationObject()
{

}

AnimationObject::AnimationObject(AnimationObject *m)
{
	width = m->width;
	height = m->height;
	angle = m->angle;

	*anchorPoint = *(m->anchorPoint);
	*desintation = *(m->desintation);
	*location = *(m->location);

	*bmp = *(m->bmp);
}

AnimationObject::AnimationObject(std::vector<const wchar_t*> filename, Graphics* gfx, floatPOINT *anchor, int numRows, int numColumns)
{
	this->gfx = gfx; //save the gfx parameter for later
	bmp = NULL; //This needs to be NULL to start off
	HRESULT hr;

	anchorPoint = anchor;

	//Step 1: Create a WIC Factory
	IWICImagingFactory *wicFactory = NULL;
	hr = CoCreateInstance(
		CLSID_WICImagingFactory, //CLS ID of the object about to be made
		NULL, //not needed here, but just denotes this isn't part of an aggregate
		CLSCTX_INPROC_SERVER, //Indicates this DLL runs in the same process
		IID_IWICImagingFactory, //Reference to an Interface that talks to the object
		(LPVOID*)&wicFactory); //This is our pointer to the WICFactory, once set up.

//Step 2: Create a Decoder to read file into a WIC Bitmap
	IWICBitmapDecoder *wicDecoder = NULL;
	hr = wicFactory->CreateDecoderFromFilename(
		filename, //The filename we passed in already
		NULL, //This can be used to indicate other/preferred decoders. Not something we need.
		GENERIC_READ, //indicates we're reading from the file, vs writing, etc.
		WICDecodeMetadataCacheOnLoad, //Needed, but would only help if we were keeping this in WIC
		&wicDecoder); //Our pointer to the Decoder we've setup

//Step 3: Read a 'frame'. We're really just moving the whole image into a frame here
	IWICBitmapFrameDecode* wicFrame = NULL;
	hr = wicDecoder->GetFrame(0, &wicFrame); //0 here means the first frame... or only one in our case
	//Now, we've got a WICBitmap... we want it to be a D2D bitmap

	IWICBitmapScaler *wicScaler = NULL;
	hr = wicFactory->CreateBitmapScaler(&wicScaler);

	D2D1_SIZE_F windowSize = gfx->GetRenderTarget()->GetSize();
	UINT X, Y;
	wicFrame->GetSize(&X, &Y);


	double a = (double)(windowSize.width / numRows) / X;
	double b = (double)(windowSize.height / numColumns) / Y;

	double scalingFactor = min(a, b);

	if (numColumns * numRows == 1) scalingFactor = max(a, b);

	if (SUCCEEDED(hr))
	{
		hr = wicScaler->Initialize(
			wicFrame,
			X*scalingFactor,
			Y*scalingFactor,
			WICBitmapInterpolationModeFant
		);
	}

	width = X * scalingFactor;
	height = Y * scalingFactor;


	//Step 4: Create a WIC Converter
	IWICFormatConverter *wicConverter = NULL;
	hr = wicFactory->CreateFormatConverter(&wicConverter);

	//Step 5: Configure the Converter
	hr = wicConverter->Initialize(
		wicScaler, //Our frame from above
		GUID_WICPixelFormat32bppPBGRA, //Pixelformat
		WICBitmapDitherTypeNone, //not important for us here
		NULL, //indicates no palette is needed, not important here
		0.0, //Alpha Transparency, can look at this later
		WICBitmapPaletteTypeCustom //Not important for us here
	);

	//Step 6: Create the D2D Bitmap! Finally!
	gfx->GetRenderTarget()->CreateBitmapFromWicBitmap(
		wicConverter, //Our friend the converter
		NULL, //Can specify D2D1_Bitmap_Properties here, not needed now
		&bmp //Our destination bmp we specified earlier in the header
	);

	//Let us do some private object cleanup!
	if (wicFactory) wicFactory->Release();
	if (wicDecoder) wicDecoder->Release();
	if (wicConverter) wicConverter->Release();
	if (wicFrame) wicFrame->Release();
	if (wicScaler) wicScaler->Release();
}

AnimationObject::~AnimationObject()
{
	if (bmp) bmp->Release();
}

void AnimationObject::getShipSpeed(floatPOINT start, floatPOINT end, floatPOINT *speed, bool isEnemy)
{
	float ax = start.x, ay = start.y, bx = end.x, by = end.y;
	float deltaX = abs(ax - bx);
	float deltaY = abs(ay - by);

	if (deltaX > 0.1f || deltaY > 0.1f)
	{
		float bss = baseShipSpeed;
		double distance = sqrt(SQUARE(ax - bx) + SQUARE(ay - by));
		if (isEnemy && distance < gfx->GetRenderTarget()->GetSize().width / 5)
		{
			bss *= 1.1f;
		}
		speed->x = (bss / (deltaX + deltaY)) * deltaX;
		speed->y = (bss / (deltaX + deltaY)) * deltaY;

		// Correcting for rounding errors
		float diff = bss - sqrtf(pow(speed->y, 2) + pow(speed->x, 2));
		speed->x += diff / 2;
		speed->y += diff / 2;
	}
}

void AnimationObject::Draw(floatPOINT drawloc, bool shouldChroma, float rotation, D2D1_VECTOR_3F vector)
{

	floatPOINT location;
	location.x = drawloc.x - anchorPoint->x;
	location.y = drawloc.y - anchorPoint->y;
	if (shouldChroma)
	{
		ID2D1Effect *chromakeyEffect = NULL;

		//D2D1_VECTOR_3F vector{ 0.0f, 1.0f, 0.0f };
		gfx->GetDeviceContext()->CreateEffect(CLSID_D2D1ChromaKey, &chromakeyEffect);

		chromakeyEffect->SetInput(0, bmp);
		chromakeyEffect->SetValue(D2D1_CHROMAKEY_PROP_COLOR, vector);
		chromakeyEffect->SetValue(D2D1_CHROMAKEY_PROP_TOLERANCE, 0.8f);
		chromakeyEffect->SetValue(D2D1_CHROMAKEY_PROP_INVERT_ALPHA, false);
		chromakeyEffect->SetValue(D2D1_CHROMAKEY_PROP_FEATHER, false);

		gfx->GetDeviceContext()->SetTransform(D2D1::Matrix3x2F::Rotation(rotation, D2D1::Point2F(location.x, location.y)));
		gfx->GetDeviceContext()->DrawImage(chromakeyEffect, D2D1::Point2F(location.x - bmp->GetSize().width / 2, location.y - bmp->GetSize().height / 2));
		gfx->GetDeviceContext()->SetTransform(D2D1::Matrix3x2F::Rotation(0, D2D1::Point2F(location.x, location.y)));

		if (chromakeyEffect) chromakeyEffect->Release();
	}
	else
	{
		gfx->GetRenderTarget()->DrawBitmap(
			bmp, //Bitmap we built from WIC
			D2D1::RectF(location.x, location.y,
				bmp->GetSize().width, bmp->GetSize().height), //Destination rectangle
			0.8f, //Opacity or Alpha
			D2D1_BITMAP_INTERPOLATION_MODE::D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR,
			//Above - the interpolation mode to use if this object is 'stretched' or 'shrunk'. 
			//Refer back to lecture notes on image/bitmap files
			D2D1::RectF(location.x, location.y, bmp->GetSize().width, bmp->GetSize().height) //Source Rect
		);
	}
}
