#pragma once
#include <MassPointsData.h>

namespace yape2d
{
	class Constrain
	{
	public:
		virtual void Apply(MassPointsData& data, float correctionWeight = 0.0f) = 0;
		virtual ~Constrain() = default;
	};
}