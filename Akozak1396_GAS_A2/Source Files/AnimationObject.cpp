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
AnimationObject::AnimationObject(AnimationObject *m)
{
	gfx = m->gfx;
	width = m->width;
	height = m->height;
	frameSpeed = m->frameSpeed;

	*anchorPoint = *(m->anchorPoint);
	*location = *(m->location);

	bmp = m->bmp;
}

AnimationObject::AnimationObject(const wchar_t* filename, Graphics* gfx, floatPOINT *anchor, int numRows, int numColumns, int frameRows, int frameColumns, int framesPerRender, int repeat)
{
	frameSpeed = framesPerRender;
	this->gfx = gfx; //save the gfx parameter for later
	HRESULT hr;

	repeatTimes = repeat;

	anchorPoint = anchor;

	//Step 1: Create a WIC Factory
	IWICImagingFactory *wicFactory = NULL;
	hr = CoCreateInstance(
		CLSID_WICImagingFactory, //CLS ID of the object about to be made
		NULL, //not needed here, but just denotes this isn't part of an aggregate
		CLSCTX_INPROC_SERVER, //Indicates this DLL runs in the same process
		IID_IWICImagingFactory, //Reference to an Interface that talks to the object
		(LPVOID*)&wicFactory); //This is our pointer to the WICFactory, once set up.

	bmp.clear();	
	int x = 0, y = 0;
	D2D1_SIZE_F windowSize = gfx->GetRenderTarget()->GetSize();

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

	if (numColumns * numRows != 0)
	{
		UINT X, Y;
		wicFrame->GetSize(&X, &Y);

		double a = ((double)(windowSize.width / numRows) / X) * frameColumns;
		double b = ((double)(windowSize.height / numColumns) / Y) * frameRows;

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
		width = X * scalingFactor / frameColumns;
		height = Y * scalingFactor / frameRows;
	}

	UINT X, Y;	
	ID2D1Bitmap *frame = NULL;
	IWICBitmapSource *f = NULL;
	f = wicFrame;

	if (numColumns * numRows != 0) { f = wicScaler; }

	f->GetSize(&X, &Y);
	int frameWidth = X / frameRows, frameHeight = Y / frameColumns;

	while (y < frameColumns)
	{
		while (x < frameRows)
		{
			//Step 4: Create a WIC Converter
			IWICFormatConverter *wicConverter = NULL;
			hr = wicFactory->CreateFormatConverter(&wicConverter);

			IWICBitmapClipper *wicClipper = NULL;
			hr = wicFactory->CreateBitmapClipper(&wicClipper);

			WICRect destRect = { x * frameWidth, y * frameHeight, frameWidth, frameHeight };
			wicClipper->Initialize(f, &destRect);

			//Step 5: Configure the Converter
			wicConverter->Initialize(
				wicClipper, //Our frame from above
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
				&frame //Our destination bmp we specified earlier in the header
			);

			if (frame != nullptr)
			{
				bmp.push_back(frame);
			}			
			x++;
			if (wicClipper) wicClipper->Release();			
			if (wicConverter) wicConverter->Release();
		}
		y++;
		x = 0;
	}	
	if (wicFrame) wicFrame->Release();
	if (wicDecoder) wicDecoder->Release();
	//Let us do some private object cleanup!
	if (wicFactory) wicFactory->Release();
}

AnimationObject::~AnimationObject()
{
	/*for (auto i = bmp.begin(); i != bmp.end(); i++)
	{
		((ID2D1Bitmap*)*i)->Release();
	}*/
}

void AnimationObject::Draw()
{
	currentFrame++;
	if (currentFrame / frameSpeed >= bmp.size())
	{
		if (repeatTimes == ++currTimes)
		{
			completedAnimation = true;
		}
		else
		{
			currentFrame = 0;
		}
	}

	if (!completedAnimation)
	{
		ID2D1Bitmap* bit = bmp[int(currentFrame / frameSpeed)];
		D2D1_SIZE_F sz = bit->GetSize();
		gfx->GetDeviceContext()->DrawImage(bit, D2D1::Point2F(location->x - bit->GetSize().width / 2, location->y - bit->GetSize().height / 2));
	}
}

floatPOINT AnimationObject::Draw(floatPOINT drawloc)
{
	floatPOINT finalLoc = *location + floatPOINT{ - anchorPoint->x + drawloc.x, - anchorPoint->y + drawloc.y };
	D2D1_SIZE_F imgSize = bmp[0]->GetSize();
	//finalLoc.x = finalLoc.x - imgSize.width / 2;
	//finalLoc.y = finalLoc.y - imgSize.height / 2;
	//finalLoc.x = location->x + anchorPoint;
	//finalLoc.y = drawloc.y + location->y;

	currentFrame++;
	if (currentFrame / frameSpeed >= bmp.size())
	{
		if (repeatTimes == ++currTimes)
		{
			completedAnimation = true;
		}
		else
		{
			currentFrame = 0;
		}
	}

	if (!completedAnimation)
	{
		ID2D1Bitmap* bit = bmp[int(currentFrame / frameSpeed)];
		D2D1_SIZE_F sz = bit->GetSize();
		gfx->GetDeviceContext()->DrawImage(bit, D2D1::Point2F(finalLoc.x, finalLoc.y));
	}
	return finalLoc;
}
