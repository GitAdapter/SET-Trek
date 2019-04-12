/*
*  FILE          : AminationObject.cpp
*  PROJECT       : PROG2215 - SET-TREK: The Search For Sound (Assignment #3)
*  PROGRAMMER    : Alex Kozak
*  FIRST VERSION : 2019-04-11
*  DESCRIPTION   :
*    The functions in this file define the functionality of a AnimationObject which has multiple frames as determined by
*	 a spritesheet of data. This can be repeatable, at a different render speed, and more.
*/

#pragma once
#include "VisibleObject.h"

AnimationObject::AnimationObject(AnimationObject *m)
{
	renderTarget = m->renderTarget;
	deviceContext = m->deviceContext;
	width = m->width;
	height = m->height;
	frameSpeed = m->frameSpeed;

	*anchorPoint = *(m->anchorPoint);
	*location = *(m->location);

	bmp = m->bmp;
}

AnimationObject::AnimationObject(const wchar_t* filename, ID2D1RenderTarget* rt, ID2D1DeviceContext* dc, floatPOINT *anchor, int numRows, int numColumns, int frameRows, int frameColumns, int framesPerRender, int repeat)
{
	renderTarget = rt;
	deviceContext = dc;
	frameSpeed = framesPerRender;
	this->gfx = gfx;
	HRESULT hr;

	repeatTimes = repeat;

	anchorPoint = anchor;

	IWICImagingFactory *wicFactory = NULL;
	hr = CoCreateInstance(CLSID_WICImagingFactory, NULL, CLSCTX_INPROC_SERVER, IID_IWICImagingFactory, (LPVOID*)& wicFactory);

	bmp.clear();	
	int x = 0, y = 0;
	D2D1_SIZE_F windowSize = renderTarget->GetSize();

	IWICBitmapDecoder *wicDecoder = NULL;
	hr = wicFactory->CreateDecoderFromFilename(filename, NULL, GENERIC_READ, WICDecodeMetadataCacheOnLoad, &wicDecoder);

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
			hr = wicScaler->Initialize(wicFrame, X * scalingFactor, Y * scalingFactor, WICBitmapInterpolationModeFant);
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
			IWICFormatConverter *wicConverter = NULL;
			IWICBitmapClipper *wicClipper = NULL;
			hr = wicFactory->CreateFormatConverter(&wicConverter);
			hr = wicFactory->CreateBitmapClipper(&wicClipper);

			WICRect destRect = { x * frameWidth, y * frameHeight, frameWidth, frameHeight };
			wicClipper->Initialize(f, &destRect);

			wicConverter->Initialize(wicClipper, GUID_WICPixelFormat32bppPBGRA, WICBitmapDitherTypeNone, NULL, 0.0, WICBitmapPaletteTypeCustom);
			renderTarget->CreateBitmapFromWicBitmap(wicConverter, NULL, &frame);

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
	if (wicFactory) wicFactory->Release();
}

AnimationObject::~AnimationObject()
{
	bmp.clear();
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
		deviceContext->DrawImage(bit, D2D1::Point2F(location->x - bit->GetSize().width / 2, location->y - bit->GetSize().height / 2));
	}
}

void AnimationObject::Draw(floatPOINT drawloc)
{
	floatPOINT finalLoc = *location + floatPOINT{ - anchorPoint->x + drawloc.x, - anchorPoint->y + drawloc.y };
	D2D1_SIZE_F imgSize = bmp[0]->GetSize();

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
		deviceContext->DrawImage(bit, D2D1::Point2F(finalLoc.x, finalLoc.y));
	}
}