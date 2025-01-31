#pragma once
#include <yape2d/MassPointsData.h>

namespace yape2d
{
	class Constrain
	{
	public:
		virtual float Residual(MassPointsData& data) = 0;
		virtual float Jacobian(MassPointsData& data, unsigned int id) = 0;
	};
}