#pragma once 
#include <Playground.h>

namespace olc
{
	class PixelGameEngine;
}


class UIBase
{
public:
	virtual ~UIBase() = default;

	void Draw(Playground* workbench)
	{
		if (mHidden)
		{
			return;
		}
		DrawInternal(workbench);
	}

	bool Update(olc::PixelGameEngine* pge)
	{
		if (mHidden)
		{
			return false;
		}
		return UpdateInternal(pge);
	}

	void Hide() { mHidden = true; }
	void Show() { mHidden = false;}

protected:
	virtual void DrawInternal(Playground* playground) const = 0;
	virtual bool UpdateInternal(olc::PixelGameEngine* pge) = 0;



private:
	bool mHidden = false;

};