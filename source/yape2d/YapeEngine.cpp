#include <yape2d/YapeEngine.h>

#include <assert.h>

#include <yape2d/SpringForce.h>
#include <yape2d/ConstrainDistance.h>

namespace yape2d
{
	void YapeEngine::Reset()
	{
		mData.Clear();
	}

	void YapeEngine::SetGravity(float gravity)
	{
		mGravity = gravity;
		mHasGravity = abs(mGravity) > FLT_EPSILON;
	}

	void YapeEngine::AddPointMass(unsigned int id, float mass, float x, float y, bool fixedPosition)
	{
		mData.AddMassPoint(id, x, y, mass, fixedPosition);
	}

	void YapeEngine::AddSpring(unsigned int id1, unsigned int id2, float length, float stiffness)
	{
		mForces.push_back(std::make_unique<SpringForce>(id1, id2, length, stiffness));
	}

	void YapeEngine::AddDistanceConstrain(unsigned int id1, unsigned int id2, float distance)
	{
		mConstrains.push_back(std::make_unique<ConstrainDistance>(id1, id2, distance));
	}

	void YapeEngine::Update(float dt)
	{

		//ApplyConstrains();

		SemiImplicitEulerSolver(dt);
		//RungeKutta4(dt);

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
			if (mHasGravity)
			{
				K = mData.mM[i] * mGravity * mData.mP[i].y;
			}
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
			if (mData.mFixed[i])
			{
				mData.mV[i] = { 0.0f, 0.0f };
				continue;
			}

			mData.mP[i] += mData.mV[i] * dt;
		}
	}

	void YapeEngine::UpdateVelocities(float dt)
	{
		for (int i = 0; i < mData.mM.size(); i++)
		{
			if (mData.mFixed[i])
			{
				continue;
			}

			auto a = mData.mF[i] / mData.mM[i];
			a.y -= mGravity;
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
	}

}
