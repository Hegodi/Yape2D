#pragma once

#include <olcPixelGameEngine.h>

#include <Element.h>
#include <MassPoint.h>

class TwoPointsElement : public Element
{
public:
	TwoPointsElement() : Element(0) 
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
			mLength = (mP2->GetPosition() - mP1->GetPosition()).mag();
		}
	}

	float GetLength() const { return mLength; }

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

		wb->DrawLine(start , end, olc::GREEN);
	}

	bool HasTheSameConnections(std::shared_ptr<TwoPointsElement> other)
	{
		return ((other->mP1 == mP1 && other->mP2 == mP2) || (other->mP2 == mP1 && other->mP1 == mP2));
	}

protected:
	std::shared_ptr<MassPoint> mP1 = nullptr; 
	std::shared_ptr<MassPoint> mP2 = nullptr; 
	olc::vf2d mTemporaryEndPoint = { 0.0f, 0.0f };
	float mLength = 0.0f;
};