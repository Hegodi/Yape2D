#pragma once
#include <yape2d/Constrain.h>
#include <yape2d/MassPointsData.h>

namespace yape2d
{
	class ConstrainDistance : public Constrain
	{
	public:
		ConstrainDistance(unsigned int id1, unsigned int id2, float distance)
			: mId1(id1), mId2(id2), mDistance(distance)
		{}

		float Residual(MassPointsData& data)
		{
			auto i1 = data.mIdMap[mId1];
			auto i2 = data.mIdMap[mId2];
			//float dx = data.mX[i1] - data.mX[i2];
			//float dy = data.mY[i1] - data.mY[i2];
			//return fabs(dx * dx + dy * dy - mDistance * mDistance);
			return 0.0f;
		}

		float Jacobian(MassPointsData& data, unsigned int id)
		{
			return 0.0f;
		}

	private:
		unsigned int mId1 = 0;
		unsigned int mId2 = 0;
		float mDistance = 0;
	};
}