#pragma once

#include <olcPixelGameEngine.h>

#include <Element.h>
#include <MassPoint.h>

class Spring : public Element
{
public:
	Spring() : Element(0) 
	{
	}

	void SetPointStart(std::shared_ptr<MassPoint> point)
	{
		mP1 = point;
	}
	void SetPointEnd(std::shared_ptr<MassPoint> point)
	{
		mP2 = point;
		if (mP2 != nullptr)
		{
			mLength = (mP2->GetPosition() - mP1->GetPosition()).mag() * 0.8;
		}
	}

	float GetElasticConstant() const { return mElasticConstant; }
	void SetElasticConstant(double value) { mElasticConstant = value; }
	float GetLength() const { return mLength; }
	void SetLength(float length) { mLength = length; }

	bool HasStartPoint() const { return mP1 != nullptr; }
	bool HasEndPoint() const { return mP2 != nullptr; }

	std::shared_ptr<MassPoint> GetMassPointStart() const { return mP1; }
	std::shared_ptr<MassPoint> GetMassPointEnd() const { return mP2; }

	void SetTemporaryEndPoint(olc::vf2d pos) { mTemporaryEndPoint = pos; }

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

	bool HasTheSameConnections(std::shared_ptr<Spring> other)
	{
		return ((other->mP1 == mP1 && other->mP2 == mP2) || (other->mP2 == mP1 && other->mP1 == mP2));
	}

private:
	std::shared_ptr<MassPoint> mP1 = nullptr; 
	std::shared_ptr<MassPoint> mP2 = nullptr; 
	olc::vf2d mTemporaryEndPoint = { 0.0f, 0.0f };
	float mElasticConstant = 5.0f;
	float mLength = 0.0f;
};