#pragma once

#include <olcPixelGameEngine.h>

#include <MassPoint.h>
#include <TwoPointsElement.h>

class Rod : public TwoPointsElement
{
public:

	void Draw(Playground* wb, int debugLevel) override
	{
		if (!HasStartPoint())
		{
			return;
		}

		olc::vf2d start = wb->FromWorldToScreen(mP1->GetPosition());
		olc::vf2d end = wb->FromWorldToScreen(HasEndPoint() ? mP2->GetPosition() : mTemporaryEndPoint);

		wb->DrawLine(start , end, olc::GREEN);

		if (debugLevel > 0)
		{
			wb->DrawCircle(wb->FromWorldToScreen(GetPosition()), TwoPointElementInteractionRadius, olc::WHITE);
		}
	}
};