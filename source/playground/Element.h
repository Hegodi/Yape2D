#pragma once 
#include <Playground.h>

class Element
{
public:
	explicit Element(unsigned int id) : mId(id) {}
	virtual ~Element() = default;

	virtual void Draw(Playground* workbench) = 0;
	unsigned int GetId() const { return mId; }

private:
	unsigned int mId = 0;


};