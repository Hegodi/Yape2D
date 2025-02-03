#pragma once

#include <olcPixelGameEngine.h>

#include <MassPoint.h>
#include <TwoPointsElement.h>

class Spring : public TwoPointsElement
{
public:
	Spring() 
	{
	}

	float GetElasticConstant() const { return mElasticConstant; }
	void SetElasticConstant(double value) { mElasticConstant = value; }
	void SetLength(float length) { mLength = length; }

	void Draw(Playground* wb, int debugLevel) override
	{
		if (!HasStartPoint())
		{
			return;
		}

		olc::vf2d start = wb->FromWorldToScreen(mP1->GetPosition());
		olc::vf2d end = wb->FromWorldToScreen(HasEndPoint() ? mP2->GetPosition() : mTemporaryEndPoint);

		olc::vf2d dir = end - start;
		float length = dir.mag();
		dir /= length;
		olc::vf2d nor = { dir.y, -dir.x };

		const int number_lines = mLength * 10;
		if (number_lines == 0)
		{
			wb->DrawLine(start, end, olc::YELLOW);
		}
		const float width = 5 + mElasticConstant * 0.5f;
		const float delta = length / number_lines;

		for (int i = 0; i < number_lines; i++)
		{
			olc::vf2d p0 = i == 0 ? start : start + dir * i * delta + nor * (0.5f - i % 2) * width;
			olc::vf2d p1 = i == (number_lines-1) ? end : start + dir * (i+1) * delta + nor * (0.5f - (i+1) % 2) * width;
			wb->DrawLine(p0, p1, olc::YELLOW);
		}

		if (debugLevel > 0)
		{
			wb->DrawCircle(wb->FromWorldToScreen(GetPosition()), TwoPointElementInteractionRadius, olc::WHITE);
			auto pos = GetPosition();
			olc::vf2d dir = (mP1->GetPosition() - mP2->GetPosition()).norm();
			olc::vf2d p1 = wb->FromWorldToScreen(pos + dir * mLength * 0.5f);
			olc::vf2d p2 = wb->FromWorldToScreen(pos - dir * mLength * 0.5f);
			wb->DrawLine(p1, p2, olc::RED);
			wb->FillCircle(p1, 5, olc::RED);
			wb->FillCircle(p2, 5, olc::RED);
		}
	}


private:
	float mElasticConstant = 10.0f;
};