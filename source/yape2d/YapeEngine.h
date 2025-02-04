#pragma once
#include <vector>
#include <unordered_map>
#include <functional>
#include <memory>
#include <yape2d/Constrain.h>

#include <yape2d/Vector2D.h>
#include <yape2d/MassPointsData.h>
#include <yape2d/Force.h>

namespace yape2d
{
	class YapeEngine
	{
	public:
		void Reset();
		void SetGravity(float gravity);
		void SetFriction(float friction);

		void AddPointMass(unsigned int id, float mass, float x, float y);
		void AddSpring(unsigned int id1, unsigned int id2, float length, float stiffness);
		void AddDistanceConstrain(unsigned int id1, unsigned int id2, float distance);
		void AddFixedPositionConstrain(unsigned int id1, vec2f position);

		void Update(float dt);

		vec2f GetPosition(unsigned int id) const;

		float CalculateEnergy() const;
		float GetTime() const { return mTime; }
		int GetNumberConstrains() const { return mConstrains.size(); }
		int GetNumberForces() const { return mForces.size(); }
		void SetConstrainSolverIterations(int count) { mConstrainSolver_Iterations = count; }
		void SetConstrainCorrectionWeight(float weight) { mConstrainCorrectionWeigth = weight; }

	private:
		void ResetForces();
		void ApplyForces();
		void ApplyConstrains();

		void SemiImplicitEulerSolver(float dt);
		void UpdatePositions(float dt);
		void UpdateVelocities(float dt);

	private:
		float mTime = 0.0f;

		float mGravity = 0.0f;
		float mFriction = 0.0f;
		float mMaxTimeStep = 0.01f;
		int mConstrainSolver_Iterations = 5;
		float mConstrainCorrectionWeigth = 50.0f;

		MassPointsData mData;
		std::vector<std::unique_ptr<Force>> mForces;
		std::vector<std::unique_ptr<Constrain>> mConstrains;
	};
}