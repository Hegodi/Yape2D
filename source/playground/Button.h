#pragma once
#include <olcPixelGameEngine.h>
#include <functional>

using OnClickCallback = std::function<void(void)>;

class Button
{
public:
	Button(int x, int y, int width, int height, const char* text, OnClickCallback onClick)
		: mPos(x, y)
		, mSize(width, height)
	{
		mBBmin = mPos - mSize / 2;
		mBBmax = mPos + mSize / 2;
	}

	void Update(olc::PixelGameEngine* pge)
	{
		auto pos = pge->GetMousePos();

		olc::Pixel color = olc::GREY;
		if (pos.x > mBBmin.x && pos.x < mBBmax.x && pos.y > mBBmin.y && pos.y < mBBmax.y)
		{
			color = olc::YELLOW;
			if (pge->GetMouse(0).bPressed && mCallback != nullptr)
			{
				mCallback();
			}
		}

		pge->FillRect(mPos, mSize, color);
	}

private:
	olc::vi2d mBBmin;
	olc::vi2d mBBmax;
	olc::vi2d mPos;
	olc::vi2d mSize;
	OnClickCallback mCallback = nullptr;
};