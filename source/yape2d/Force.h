#pragma once
#include <yape2d/MassPointsData.h>
#include <yape2d/Vector2D.h>

namespace yape2d
{
	class Force
	{
	public:
		virtual ~Force() = default;
		virtual void ApplyForce(MassPointsData& data) const = 0;
		virtual vec2f CalculateForceOver(MassPointsData const& data, unsigned int id) const = 0;
		virtual float CalculateEnergy(MassPointsData const& data) const = 0;
		virtual bool InvolveParticle(unsigned int id) const = 0;

	};
}
