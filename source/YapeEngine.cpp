#include <YapeEngine.h>

#include <assert.h>

#include <ForceSpring.h>
#include <ConstrainDistance.h>
#include <ConstrainFixedPosition.h>

namespace yape2d
{
	void YapeEngine::Reset()
	{
		mData.Clear();
		mConstrains.clear();
		mForces.clear();
	}

	void YapeEngine::SetGravity(float gravity)
	{
		mGravity = gravity;
	}

	void YapeEngine::SetFriction(float friction)
	{
		mFriction = friction;
	}

	void YapeEngine::AddPointMass(unsigned int id, float mass, float x, float y)
	{
		mData.AddMassPoint(id, x, y, mass);
	}

	void YapeEngine::AddSpring(unsigned int id1, unsigned int id2, float length, float stiffness)
	{
		mForces.push_back(std::make_unique<ForceSpring>(id1, id2, length, stiffness));
	}

	void YapeEngine::AddDistanceConstrain(unsigned int id1, unsigned int id2, float distance)
	{
		mConstrains.push_back(std::make_unique<ConstrainDistance>(id1, id2, distance));
	}
	void YapeEngine::AddFixedPositionConstrain(unsigned int id1, vec2f position)
	{
		mConstrains.push_back(std::make_unique<ConstrainFixedPosition>(id1, position));
	}

	void YapeEngine::Update(float dt)
	{
		float t = 0;
		while (t < dt)
		{
			SemiImplicitEulerSolver(mMaxTimeStep);
			ApplyConstrains();
			t += mMaxTimeStep;
		}

		float lastDt = dt - (t - mMaxTimeStep);
		SemiImplicitEulerSolver(mMaxTimeStep);
		ApplyConstrains();

		mTime += dt;

	}

	vec2f YapeEngine::GetPosition(unsigned int id) const
	{
		auto it = mData.mIdMap.find(id);
		assert(it != mData.mIdMap.end());
		int ind = it->second;
		return mData.mP[ind];
	}

	float YapeEngine::CalculateEnergy() const
	{
		static float Kmax = 0.0;
		static float Tmax = 0.0;

		float T = 0.0;
		float K = 0.0;
		for (int i = 0; i < mData.mM.size(); i++)
		{
			T += mData.mM[i] * mData.mV[i].length2();
			K = mData.mM[i] * mGravity * mData.mP[i].y;
		}
		T *= 0.5;

		if (T > Tmax) Tmax = T;

		for (auto const& force : mForces)
		{
			K += force->CalculateEnergy(mData);
		}
		if (K > Kmax) Kmax = K;

		return T + K;
	}

	void YapeEngine::SemiImplicitEulerSolver(float dt)
	{
		ResetForces();

		UpdatePositions(dt);

		ApplyForces();

		UpdateVelocities(dt);
	}

	void YapeEngine::UpdatePositions(float dt)
	{
		for (int i = 0; i < mData.mM.size(); i++)
		{
			mData.mP[i] += mData.mV[i] * dt;
		}
	}

	void YapeEngine::UpdateVelocities(float dt)
	{
		for (int i = 0; i < mData.mM.size(); i++)
		{
			auto a = mData.mF[i] / mData.mM[i];
			a.y -= mGravity;
			a -= mData.mV[i] * mFriction;
			mData.mV[i] += a * dt;
		}
	}

	void YapeEngine::ResetForces()
	{
		memset(mData.mF.data(), 0, mData.mF.size() * sizeof(vec2f));
	}

	void YapeEngine::ApplyForces()
	{
		for (auto const& force : mForces)
		{
			force->ApplyForce(mData);
		}
	}

	void YapeEngine::ApplyConstrains()
	{
		for (int i = 0; i < mConstrainSolver_Iterations; i++)
		{
			for (auto const& constrain : mConstrains)
			{
				constrain->Apply(mData, mConstrainCorrectionWeigth);
			}
		}
	}

}
