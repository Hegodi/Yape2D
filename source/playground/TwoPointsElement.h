#pragma once

#include <olcPixelGameEngine.h>

#include <MassPoint.h>

constexpr int TwoPointElementInteractionRadius = 10;

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

	olc::vf2d GetPosition() const override
	{
		if (mP1 == nullptr)
			return { -1000.0f, -1000.0f };
		else if (mP2 == nullptr)
			return mP1->GetPosition();

		return (mP1->GetPosition() + mP2->GetPosition()) * 0.5f;
	}

	float GetLength() const { return mLength; }

	bool HasStartPoint() const { return mP1 != nullptr; }
	bool HasEndPoint() const { return mP2 != nullptr; }

	std::shared_ptr<MassPoint> GetMassPointStart() const { return mP1; }
	std::shared_ptr<MassPoint> GetMassPointEnd() const { return mP2; }

	void SetTemporaryEndPoint(olc::vf2d pos) { mTemporaryEndPoint = pos; }

	void Draw(Playground* wb, int debugLevel) override = 0;

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