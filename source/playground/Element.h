#pragma once 
#include <Playground.h>

class Element
{
public:
	explicit Element(unsigned int id) : mId(id) {}
	virtual ~Element() = default;

	virtual void Draw(Playground* workbench, int debugLevel) = 0;
	unsigned int GetId() const { return mId; }
	virtual olc::vf2d GetPosition() const = 0;

private:
	unsigned int mId = 0;


};