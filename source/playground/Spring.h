#pragma once

#include <olcPixelGameEngine.h>

#include <Element.h>
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

	void Draw(Playground* wb) override
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

		const int number_lines = length > 100 ? 20 : 10;
		const float width = 30.0f;
		const float delta = length / number_lines;

		wb->DrawLine(start, end, olc::YELLOW);
		for (int i = 0; i < number_lines; i++)
		{
			olc::vf2d p0 = i == 0 ? start : start + dir * i * delta + nor * (0.5f - i % 2) * width;
			olc::vf2d p1 = i == (number_lines-1) ? end : start + dir * (i+1) * delta + nor * (0.5f - (i+1) % 2) * width;
			wb->DrawLine(p0, p1, olc::YELLOW);
		}
	}


private:
	float mElasticConstant = 5.0f;
};