#pragma once
#include <vector>
#include <unordered_map>
#include <yape2d/Vector2D.h>

namespace yape2d
{
	struct MassPointsData
	{
		std::unordered_map<unsigned int, int> mIdMap;
		std::vector<unsigned int> mIds;
		std::vector<float> mM;
		std::vector<vec2f> mP;
		std::vector<vec2f> mV;
		std::vector<vec2f> mF;
		std::vector<bool> mFixed;

		void Clear()
		{
			mIdMap.clear();
			mIds.clear();
			mM.clear();
			mP.clear();
			mV.clear();
			mF.clear();
			mFixed.clear();
		}

		void AddMassPoint(unsigned int id, float x, float y, float mass, bool isFixed)
		{

			mIdMap.emplace(id, mM.size());
			mIds.push_back(id);
			mM.push_back(mass);
			mP.push_back(vec2f(x, y));
			mV.push_back(vec2f(0.0f,0.0f));
			mF.push_back(vec2f(0.0f,0.0f));
			mFixed.push_back(isFixed);
		}
	};
}