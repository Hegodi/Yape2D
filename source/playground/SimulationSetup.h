#pragma once
#include <vector>
#include <memory>

class MassPoint;
class TwoPointsElement;

class SimulationSetup
{
public:
	void Reset();
	void Save(const char* filename);
	bool Load(const char* filename);

	std::shared_ptr<MassPoint> FindMassPoint(unsigned int id) const;

	std::vector<std::shared_ptr<MassPoint>> mMassPoints;
	std::vector<std::shared_ptr<TwoPointsElement>> mTwoPointsElements;

	float mGravity;
	float mFriction;

};