#pragma once
#include <olcPixelGameEngine.h>
#include <functional>
#include <UI/UIBase.h>
#include <assert.h>
#include <stdlib.h>

using OnChangeCallback = std::function<void(float value)>;
class UISlider : public UIBase
{
public:
	UISlider(int x, int y, int width, int height, const char* label)
		: mLabel(label)
	{
		mRailPos = { x, y };
		mRailSize = { width, 3 };

		mCursorSize = { 8, height };
		mCursorPos.y = mRailPos.y - mCursorSize.y/2;

		mLabelPos.x = mRailPos.x + mRailSize.x + 8;
		mLabelPos.y = mRailPos.y - 2;


		SetValue(mValue);
	}

	void SetCallback(OnChangeCallback callback)
	{
		mChangeCallback = callback;
	}

	bool UpdateInternal(olc::PixelGameEngine* pge) override
	{
		auto pos = pge->GetMousePos();
		if (mIsDragging)
		{
			if (pge->GetMouse(0).bHeld)
			{
				UpdateValue(pge->GetMouseX());
			}
			else
			{
				mColor = olc::GREY;
				mIsDragging = false;
			}

		}
		else
		{

			if (pos.x > mCursorBBmin.x && pos.x < mCursorBBmax.x && pos.y > mCursorBBmin.y && pos.y < mCursorBBmax.y)
			{
				mColor = olc::WHITE;
				if (pge->GetMouse(0).bPressed)
				{
					mColor = olc::YELLOW;
					mIsDragging = true;
					return true;
				}
			}
			else
			{
				mColor = olc::GREY;
				mIsDragging = false;
			}
		}
		return false;
	}

	void DrawInternal(Playground* pg) const override
	{
		pg->FillRect(mRailPos, mRailSize, olc::DARK_GREY);
		pg->FillRect(mCursorPos, mCursorSize, mColor);
		char buffer[128];
		snprintf(buffer, sizeof(buffer), "%s %.1f", mLabel.c_str(), mValue);
		pg->DrawString(mLabelPos, buffer, olc::WHITE);
	}

	void UpdateValue(float xPos)
	{
		if (xPos < mRailPos.x)
			xPos = mRailPos.x;
		if (xPos > mRailPos.x + mRailSize.x)
			xPos = mRailPos.x + mRailSize.x;

		float weight = (xPos - mRailPos.x) / (mRailSize.x);
		float valueNew = mMinValue + weight * (mMaxValue - mMinValue);

		if (mValue != valueNew)
		{
			mValue = valueNew;
			if (mChangeCallback != nullptr)
				mChangeCallback(valueNew);
		}
		mCursorPos.x = xPos;
		mCursorBBmin = mCursorPos;
		mCursorBBmax = mCursorBBmin + mCursorSize;
	}

	void SetValue(float value)
	{
		mValue = value;
		float pos = mRailPos.x + ((mValue - mMinValue) / (mMaxValue - mMinValue)) * mRailSize.x;
		UpdateValue(pos);
	}

	void SetLimits(float minValue, float maxValue)
	{
		mMinValue = minValue;
		mMaxValue = maxValue;
		if (mMaxValue <= mMinValue)
		{
			mMaxValue = mMinValue * 2;
		}
		SetValue(mValue);
	}

	void SetLabel(const char* label) { mLabel = label; }

	float GetValue() const { return mValue; }

private:

	OnChangeCallback mChangeCallback = nullptr;
	bool mIsDragging = false;

	olc::Pixel mColor = olc::GREY;

	olc::vi2d mCursorPos;
	olc::vi2d mCursorSize;
	olc::vi2d mCursorBBmin;
	olc::vi2d mCursorBBmax;

	olc::vi2d mRailPos;
	olc::vi2d mRailSize;

	std::string mLabel;
	olc::vi2d mLabelPos;

	float mMinValue = 0.0f;
	float mMaxValue = 1.0f;
	float mIncrement = 0.1f;
	float mValue = 0.5f;
};