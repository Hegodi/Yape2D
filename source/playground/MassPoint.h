#pragma once
#include <olcPixelGameEngine.h>

#include <Element.h>

static unsigned int sNextStaticIdForMassPoint = 1000;

class MassPoint : public Element
{
public:
	MassPoint() : Element(sNextStaticIdForMassPoint++) 
	{
		SetMass(1.0f);
	}

	void SetPosition(float x, float y) { mPosition = { x, y }; }
	void SetPosition(olc::vf2d pos) { mPosition = pos; }

	void SetMass(float mass)
	{
		mMass = 1.0f;
		mRadius = 10 * sqrt(mass);
	}
	float GetMass() const { return mMass; }
	olc::vf2d GetPosition() const { return mPosition; }

	void Draw(Playground* wb) override
	{
		if (mIsFixed)
		{
			wb->FillRect(wb->FromWorldToScreen(mPosition) - olc::vf2d(mRadius, mRadius), olc::vf2d(2*mRadius, 2*mRadius), olc::DARK_RED);
		}
		else
		{
			wb->FillCircle(wb->FromWorldToScreen(mPosition), mRadius, olc::DARK_GREEN);
		}
	}

	float GetRadius() const { return mRadius; }

	void SetFixed() { mIsFixed = true; }
	bool IsFixed() const { return mIsFixed; }

private:
	olc::vf2d mPosition = { 0.0f, 0.0f };
	float mMass = 0.0f;
	float mRadius = 0.0f;
	bool mIsFixed = false;
};