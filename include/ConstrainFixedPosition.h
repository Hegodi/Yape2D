#pragma once
#include <Constrain.h>
#include <MassPointsData.h>

namespace yape2d
{
	class ConstrainFixedPosition : public Constrain
	{
	public:
		ConstrainFixedPosition(unsigned int id1, vec2f position)
			: mId1(id1), mPosition(position)
		{}

		void Apply(MassPointsData& data, float correctionWeight) override
		{
			auto i1 = data.mIdMap[mId1];
			data.mV[i1] = { 0.0f, 0.0f };
			data.mP[i1] = mPosition;
		}

	private:
		unsigned int mId1 = 0;
		vec2f mPosition = { 0.0f, 0.0f };
	};
}