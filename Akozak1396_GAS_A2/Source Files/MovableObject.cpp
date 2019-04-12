/*
*  FILE          : MovableObject.cpp
*  PROJECT       : PROG2215 - SET-TREK: The Search For Sound (Assignment #3)
*  PROGRAMMER    : Alex Kozak
*  FIRST VERSION : 2019-04-11
*  DESCRIPTION   :
*    The functions in this file define the functionality of a MovableObject which (as the name
*	 implies) is an object which moves. It has a speed, position, calculatable destination, and more.
*/

#include "VisibleObject.h"

// prototypes
void changeIndiv(float *, float, float);
bool isTouching(MovableObject*);
float getShipAngle(floatPOINT start, floatPOINT end);

MovableObject::MovableObject(MovableObject *m)
{
	// take all vital pieces of info from the provided MovableObject class and save them to the local oject
	width = m->width;
	height = m->height;
	angle = m->angle;

	speed = new floatPOINT { 0.0f, 0.0f };
	*speed = *m->speed;

	if (m->location->x > m->desintation->x)
	{
		speed->x = -speed->x;
	}

	if (m->location->y > m->desintation->y)
	{
		speed->y = -speed->y;
	}

	*anchorPoint = *(m->anchorPoint);
	*location = *(m->location);	
	desintation = new floatPOINT();
	*desintation = *m->location;

	deviceContext = m->deviceContext;
	renderTarget = m->renderTarget;

	bmp = m->bmp;
	*chromakeyEffect = *(m->chromakeyEffect);
}

MovableObject::MovableObject(const wchar_t* filename, ID2D1RenderTarget* rt, ID2D1DeviceContext* dc, bool isS, floatPOINT *anchor, D2D1_VECTOR_3F chroma, int numRows, int numColumns)
{	
	renderTarget = rt;
	deviceContext = dc;
	this->gfx = gfx;
	bmp = NULL;
	HRESULT hr;

	floatPOINT zeroFP = floatPOINT{ 0.0f, 0.0f };
	anchorPoint = anchor;
	angle = 0;
	speed = &zeroFP;

	IWICImagingFactory *wicFactory = NULL;
	hr = CoCreateInstance(CLSID_WICImagingFactory, NULL, CLSCTX_INPROC_SERVER, IID_IWICImagingFactory, (LPVOID*)& wicFactory);

	IWICBitmapDecoder *wicDecoder = NULL;
	hr = wicFactory->CreateDecoderFromFilename(filename, NULL, GENERIC_READ, WICDecodeMetadataCacheOnLoad, &wicDecoder);

	IWICBitmapFrameDecode* wicFrame = NULL;
	hr = wicDecoder->GetFrame(0, &wicFrame);

	IWICBitmapScaler *wicScaler = NULL;
	hr = wicFactory->CreateBitmapScaler(&wicScaler);
	
	if (numColumns * numRows != 0)
	{
		D2D1_SIZE_F windowSize = renderTarget->GetSize();
		UINT X, Y;
		wicFrame->GetSize(&X, &Y);

		// determine which angle needs to be th smallet to determine the scaling factor to use
		double a = (double)(windowSize.width / numRows) / X;
		double b = (double)(windowSize.height / numColumns) / Y;
		double scalingFactor = min(a, b);

		// if numColums and numRows are 1, simply stretch to fill the screen, not shrink to fit.
		if (numColumns * numRows == 1) scalingFactor = max(a, b);

		if (SUCCEEDED(hr))
		{
			hr = wicScaler->Initialize(wicFrame, X * scalingFactor, Y * scalingFactor, WICBitmapInterpolationModeFant);
		}
		width = X * scalingFactor;
		height = Y * scalingFactor;
	}


	IWICFormatConverter *wicConverter = NULL;
	hr = wicFactory->CreateFormatConverter(&wicConverter);

	if (numColumns * numRows == 0)
	{
		hr = wicConverter->Initialize(wicFrame, GUID_WICPixelFormat32bppPBGRA, WICBitmapDitherTypeNone, NULL, 0.0, WICBitmapPaletteTypeCustom);
	}
	else
	{
		hr = wicConverter->Initialize(wicScaler, GUID_WICPixelFormat32bppPBGRA, WICBitmapDitherTypeNone, NULL, 0.0, WICBitmapPaletteTypeCustom);
	}

	// create final bitmap
	renderTarget->CreateBitmapFromWicBitmap(wicConverter, NULL, &bmp);

	// generate final chromakeyed effect, to be used later if needed
	chromakeyEffect = NULL;
	deviceContext->CreateEffect(CLSID_D2D1ChromaKey, &chromakeyEffect);
	chromakeyEffect->SetInput(0, bmp);
	chromakeyEffect->SetValue(D2D1_CHROMAKEY_PROP_COLOR, chroma);
	chromakeyEffect->SetValue(D2D1_CHROMAKEY_PROP_TOLERANCE, 0.8f);
	chromakeyEffect->SetValue(D2D1_CHROMAKEY_PROP_INVERT_ALPHA, false);
	chromakeyEffect->SetValue(D2D1_CHROMAKEY_PROP_FEATHER, false);

	if (wicFactory) wicFactory->Release();
	if (wicDecoder) wicDecoder->Release();
	if (wicConverter) wicConverter->Release();
	if (wicFrame) wicFrame->Release();
	if (wicScaler) wicScaler->Release();
}

MovableObject::~MovableObject(){}

void changeToward(floatPOINT *sp, floatPOINT ep, floatPOINT speed)
{
	// change both aspects of the floatPOINT towards teh destination by factor speed
	changeIndiv((float *)(&sp->x), ep.x, speed.x);
	changeIndiv((float *)(&sp->y), ep.y, speed.y);
}

void changeIndiv(float *sp, float ep, float speed)
{
	// change an individual x/y float based on direction and speed
	float ydiff = *sp - ep;
	if (abs(ydiff) < speed && abs(ydiff) > 0) { *sp = ep; }
	else if (ydiff > 0) { *sp -= speed; }
	else if (ydiff < 0) { *sp += speed; }
}

void MovableObject::moveObject(bool isEnemy)
{
	// move the object based on its location and destination, calculating speed.
	getShipSpeed(*this->location, *this->desintation, this->speed, isEnemy);
	this->angle = getShipAngle(*this->location, *this->desintation);
	changeToward(this->location, *this->desintation, *this->speed);
}

float getShipAngle(floatPOINT start, floatPOINT end)
{
	// get deltas
	float delta_x = start.x - end.x;
	float delta_y = start.y - end.y;
	
	// get radian direction from location to destination
	float theta_radians = atan2(delta_y, delta_x);

	// determine if the radians need to be corrected in a direction or not
	if (theta_radians < 0)
		theta_radians = abs(theta_radians);
	else
		theta_radians = 2 * M_PI - theta_radians;

	// return the degree conversion
	return -((180.0f * theta_radians) / M_PI) + 180;
}

void MovableObject::getShipSpeed(floatPOINT start, floatPOINT end, floatPOINT *speed, bool isEnemy)
{
	// find deltas
	float ax = start.x, ay = start.y, bx = end.x, by = end.y;
	float deltaX = abs(ax - bx);
	float deltaY = abs(ay - by);

	// only get a speed if there is a need to move anywhere
	if (deltaX > 0.1f || deltaY > 0.1f)
	{
		float bss = baseShipSpeed;
		double distance = sqrt(SQUARE(ax - bx) + SQUARE(ay - by));
		if (isEnemy && distance < renderTarget->GetSize().width / 5)
		{
			// close enough so enemy gets speed boost
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

void MovableObject::Draw()
{
	// draw the image given its internal parameters, done for background mostly
	deviceContext->DrawImage(bmp, D2D1::Point2F(location->x - bmp->GetSize().width / 2, location->y - bmp->GetSize().height / 2));
}

void MovableObject::Draw(floatPOINT drawloc, bool shouldChroma, float rotation)
{
	// Generate the on-screen location to draw the object
	floatPOINT location;
	location.x = drawloc.x - anchorPoint->x;
	location.y = drawloc.y - anchorPoint->y;

	// set the rotation to the desired location, from the center
	deviceContext->SetTransform(D2D1::Matrix3x2F::Rotation(rotation, D2D1::Point2F(location.x, location.y)));

	// determine if to draw the chromakeyed object or not, account for the width/height of the image
	if (shouldChroma) 
	{
		deviceContext->DrawImage(chromakeyEffect, D2D1::Point2F(location.x - width / 2, location.y - height / 2));
	}
	else
	{
		deviceContext->DrawImage(bmp, D2D1::Point2F(location.x - width / 2, location.y - height / 2));
	}

	// reset the rotation to 0
	deviceContext->SetTransform(D2D1::Matrix3x2F::Rotation(0, D2D1::Point2F(location.x, location.y)));
}