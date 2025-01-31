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
		, mText(text)
		, mCallback(onClick)
	{
		mBBmin = mPos;
		mBBmax = mPos + mSize;

		mPosText = olc::vi2d(mPos.x + mSize.x / 2 - 8 * mText.length() / 2, mPos.y + mSize.y / 2 - 4);
	}

	bool Update(olc::PixelGameEngine* pge)
	{
		if (mHidden)
			return false;

		auto pos = pge->GetMousePos();

		olc::Pixel color = olc::GREY;
		if (pos.x > mBBmin.x && pos.x < mBBmax.x && pos.y > mBBmin.y && pos.y < mBBmax.y)
		{
			color = olc::YELLOW;
			if (pge->GetMouse(0).bPressed && mCallback != nullptr)
			{
				mCallback();
				return true;
			}
		}

		pge->FillRect(mPos, mSize, color);
		pge->DrawString(mPosText, mText, olc::BLACK);
		return false;
	}

	void SetText(const char* text) { mText = text; }

	void Hide() { mHidden = true; }
	void Show() { mHidden = false;}

private:
	bool mHidden = false;
	std::string mText;
	olc::vi2d mBBmin;
	olc::vi2d mBBmax;
	olc::vi2d mPos;
	olc::vi2d mPosText;
	olc::vi2d mSize;
	OnClickCallback mCallback = nullptr;
};