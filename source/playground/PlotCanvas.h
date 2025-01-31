#pragma once

#include <olcPixelGameEngine.h>
#include <deque>

class PlotCanvas
{
public:

	void SetSize(int width, int height)
	{
		mWidth = width;
		mHeight = height;
	}

	void AddValue(double value)
	{
		if (mValues.empty())
		{
			mBaseValue = value;
		}

		if (value < mMinY)
		{
			mMinY = value - fabs(value) * 0.05f;
		}
		if (value > mMaxY)
		{
			mMaxY = value + fabs(value) * 0.05f;
		}
		mValues.push_back(value);
		if (mValues.size() >= mWidth)
		{
			mValues.pop_front();
		}
	}

	float ConvertToLocaY(float value)
	{
		return (1.0f -  (value - mMinY) / (mMaxY - mMinY)) * mHeight;
	}


	void Draw(olc::PixelGameEngine* pge, int x0, int y0)
	{
		pge->FillRect(x0, y0, mWidth, mHeight, olc::DARK_GREY);

		float dx = mWidth / (mValues.size()+1);

		float yBase = y0 + ConvertToLocaY(mBaseValue);
		pge->DrawLine(x0, yBase, x0 + mWidth, yBase, olc::BLACK);

		float x = x0;
		for (auto const& v : mValues)
		{
			float y = y0 + ConvertToLocaY(v);
			x += dx;
			pge->DrawLine(x, y, x + dx, y, olc::YELLOW);
		}
	}

private:
	int mWidth = 200;
	int mHeight = 100;


	float mMinY = FLT_MAX;
	float mMaxY = -FLT_MAX;

	float mBaseValue = 0.0f;
	std::deque<float> mValues;



};