#pragma once
#include <olcPixelGameEngine.h>
#include <olcPGEX_Font.h>
#include <functional>
#include <UI/UIBase.h>

using OnClickCallback = std::function<void(void)>;

class UIButton : public UIBase
{
public:
	UIButton(int x, int y, int width, int height, const char* text, OnClickCallback onClick)
		: mPos(x, y)
		, mSize(width, height)
		, mText(text)
		, mCallback(onClick)
	{
		mBBmin = mPos;
		mBBmax = mPos + mSize;

		mPosText = olc::vi2d(mPos.x + mSize.x / 2 - 8 * mText.length() / 2, mPos.y + mSize.y / 2 - 4);
	}

	bool UpdateInternal(olc::PixelGameEngine* pge) override
	{
		auto pos = pge->GetMousePos();
		if (pos.x > mBBmin.x && pos.x < mBBmax.x && pos.y > mBBmin.y && pos.y < mBBmax.y)
		{
			mColor = olc::YELLOW;
			if (pge->GetMouse(0).bPressed && mCallback != nullptr)
			{
				mCallback();
				return true;
			}
		}
		else
		{
			mColor = mColorDefault;

		}
		return false;
	}

	void DrawInternal(Playground* pg) const override
	{
		pg->FillRect(mPos, mSize, mColor);
		if (mFont)
		{
			mFont->DrawStringDecal(mPosText, mText, olc::BLACK);
		}
		else
		{
			pg->DrawString(mPosText, mText, olc::BLACK);
		}
	}

	void SetText(const char* text) { mText = text; }

	void SetDefaultColor(olc::Pixel color) { mColorDefault = color; }
	void ResetDefaultColor() { mColorDefault = olc::GREY; }


private:
	olc::Pixel mColor = olc::GREY;
	olc::Pixel mColorDefault = olc::GREY;
	std::string mText;
	olc::vi2d mBBmin;
	olc::vi2d mBBmax;
	olc::vi2d mPos;
	olc::vi2d mPosText;
	olc::vi2d mSize;
	OnClickCallback mCallback = nullptr;
	std::shared_ptr<olc::Font> mFont = nullptr;
};