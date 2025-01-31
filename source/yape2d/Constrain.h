#pragma once
#include <yape2d/MassPointsData.h>

namespace yape2d
{
	class Constrain
	{
	public:
		virtual void Apply(MassPointsData& data, float correctionWeight = 0.0f) = 0;
	};
}