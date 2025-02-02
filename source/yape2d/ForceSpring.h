#include <yape2d/MassPointsData.h>
#include <yape2d/Force.h>

namespace yape2d
{

	class ForceSpring : public Force
	{
	public:
		ForceSpring(unsigned int id1, unsigned int id2, float length, float stiffness)
			: mId1(id1)
			, mId2(id2)
			, mLength(length)
			, mStiffness(stiffness)
		{}

		void ApplyForce(MassPointsData & data) const override
		{
			auto force = CalculateForceOver(data, mId1);

			size_t i1 = data.mIdMap[mId1];
			size_t i2 = data.mIdMap[mId2];

			data.mF[i1] += force;
			data.mF[i2] -= force;
		}

		vec2f CalculateForceOver(MassPointsData const& data, unsigned int id) const override
		{
			vec2f force = { 0.0, 0.0f };
			if (!InvolveParticle(id))
			{
				return force;
			}

			auto it1 = data.mIdMap.find(mId1);
			auto it2 = data.mIdMap.find(mId2);
			if (it1 == data.mIdMap.end() || it2 == data.mIdMap.end())
			{
				std::cerr << "ERROR: cannot find mass points with ids: " << mId1 << " and " << mId2 << std::endl;
				return force;
			}

			size_t i1 = it1->second;
			size_t i2 = it2->second;
			auto const& pos1 = data.mP[i1];
			auto const& pos2 = data.mP[i2];

			auto dstVec = pos2 - pos1;

			auto dst = dstVec.length();
			float delta = dst - mLength;
			auto force_mag = mStiffness * delta;

			force = (dstVec / dst) * force_mag;

			if (mId2 == id)
			{
				force *= -1;
			}
			return force;
		}

		float CalculateEnergy(MassPointsData const& data) const override
		{
			size_t i1 = data.mIdMap.find(mId1)->second;
			size_t i2 = data.mIdMap.find(mId2)->second;
			auto dst = (data.mP[i2] - data.mP[i1]).length();
			auto delta = dst - mLength;
			return mStiffness * delta * delta * 0.5;
		}

		bool InvolveParticle(unsigned int id) const override
		{
			return mId1 == id || mId2 == id;
		}

	private:
		unsigned int mId1 = 0;
		unsigned int mId2 = 0;
		float mLength = 0.0;
		float mStiffness = 0.0;
	};
}