#pragma once
#include <Constrain.h>
#include <MassPointsData.h>

namespace yape2d
{
	class ConstrainDistance : public Constrain
	{
	public:
		ConstrainDistance(unsigned int id1, unsigned int id2, float distance)
			: mId1(id1), mId2(id2), mDistance(distance)
		{}

		void Apply(MassPointsData& data, float correctionWeight) override
		{
			auto i1 = data.mIdMap[mId1];
			auto i2 = data.mIdMap[mId2];
			vec2f dir = data.mP[i1] - data.mP[i2];
			float dst = dir.length();
			dir /= dst;

			auto& v1 = data.mV[i1];
			auto& v2 = data.mV[i2];


			auto deltaVel = v1 - v2;
			float weigth_forbiden_vel = dir.dot(deltaVel);
			auto forbiden_vel = dir * weigth_forbiden_vel * 0.5f;
			v1 -= forbiden_vel;
			v2 += forbiden_vel;

			// Apply extra velocity to correct drift
			float error_distance = dst - mDistance;
			v1 -= dir * error_distance * correctionWeight;
			v2 += dir * error_distance * correctionWeight;

		}

	private:
		unsigned int mId1 = 0;
		unsigned int mId2 = 0;
		float mDistance = 0;
	};
}