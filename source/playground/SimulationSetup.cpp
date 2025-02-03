#include <SimulationSetup.h>
#include <MassPoint.h>
#include <TwoPointsElement.h>
#include <pugixml/pugixml.hpp>
#include <Spring.h>
#include <Rod.h>

namespace 
{
	constexpr const char* NodeName_Root = "Yape2dPlayground";
	constexpr const char* NodeName_Spring = "Spring";
	constexpr const char* NodeName_Rod = "Rod";
	constexpr const char* NodeName_MassPoint = "MassPoint";
	constexpr const char* AttributeName_Gravity = "gravity";
	constexpr const char* AttributeName_Friction = "friction";
}

void SimulationSetup::Reset()
{
	mMassPoints.clear();
	mTwoPointsElements.clear();
}

void SimulationSetup::Save(const char* filename)
{
	pugi::xml_document doc;

	// Add a root node
	pugi::xml_node root = doc.append_child(NodeName_Root);

	// General settings
	root.append_attribute(AttributeName_Gravity).set_value(mGravity);
	root.append_attribute(AttributeName_Friction).set_value(mFriction);

	for (auto const& mp : mMassPoints)
	{
		auto pos = mp->GetPosition();
		auto node = root.append_child(NodeName_MassPoint);
		node.append_attribute("id").set_value(mp->GetId());
		node.append_attribute("x").set_value(pos.x);
		node.append_attribute("y").set_value(pos.y);
		node.append_attribute("m").set_value(mp->GetMass());
		node.append_attribute("fix").set_value(mp->IsFixed());
	}

	for (auto const& tpe : mTwoPointsElements)
	{
		pugi::xml_node node;
		if (auto rod = std::dynamic_pointer_cast<Rod>(tpe))
		{
			node = root.append_child(NodeName_Rod);

		}
		else if (auto spring = std::dynamic_pointer_cast<Spring>(tpe))
		{
			node = root.append_child(NodeName_Spring);
			node.append_attribute("k").set_value(spring->GetElasticConstant());
			node.append_attribute("length").set_value(tpe->GetLength());
		}
		node.append_attribute("id").set_value(tpe->GetId());
		node.append_attribute("mpId1").set_value(tpe->GetMassPointStart()->GetId());
		node.append_attribute("mpId2").set_value(tpe->GetMassPointEnd()->GetId());
	}

	if (doc.save_file(filename))
	{
		printf("Simulation saved");
	}
	else
	{
		printf("ERROR: cannot save the simulation");
	}
}

bool SimulationSetup::Load(const char* filename)
{
	Reset();

	pugi::xml_document doc;
	pugi::xml_parse_result result = doc.load_file(filename);
	if (result.status != pugi::xml_parse_status::status_ok)
	{
		return false;
	}

	pugi::xml_node root = doc.child(NodeName_Root);
	if (root.empty())
	{
		return false;
	}

	mGravity = root.attribute(AttributeName_Gravity).as_float();
	mFriction = root.attribute(AttributeName_Friction).as_float();

	// First load all mass points
	for (auto child : root.children())
	{
		const std::string key = child.name();
		if (key == NodeName_MassPoint)
		{
			auto id = child.attribute("id").as_uint();
			auto mp = std::make_shared<MassPoint>(id);
			mp->SetPosition(child.attribute("x").as_float(), child.attribute("y").as_float());
			mp->SetMass(child.attribute("m").as_float());
			if (child.attribute("fix").as_bool())
				mp->SetFixed();

			mMassPoints.push_back(mp);
		}
	}


	// Then the rest of elements that references mass points
	for (auto child : root.children())
	{
		const std::string key = child.name();
		if (key == NodeName_Rod)
		{
			auto rod = std::make_shared<Rod>();
			auto id = child.attribute("id").as_uint();
			auto mp1 = FindMassPoint(child.attribute("mpId1").as_uint());
			auto mp2 = FindMassPoint(child.attribute("mpId2").as_uint());
			rod->SetPointStart(mp1);
			rod->SetPointEnd(mp2);
			mTwoPointsElements.push_back(rod);
		}
		else if (key == NodeName_Spring)
		{
			auto spring = std::make_shared<Spring>();
			auto id = child.attribute("id").as_uint();
			auto mp1 = FindMassPoint(child.attribute("mpId1").as_uint());
			auto mp2 = FindMassPoint(child.attribute("mpId2").as_uint());
			spring->SetPointStart(mp1);
			spring->SetPointEnd(mp2);
			spring->SetLength(child.attribute("length").as_float());
			spring->SetElasticConstant(child.attribute("k").as_float());
			mTwoPointsElements.push_back(spring);
		}
	}

	return true;

}

std::shared_ptr<MassPoint> SimulationSetup::FindMassPoint(unsigned int id) const
{
	for (auto mp : mMassPoints)
	{
		if (mp->GetId() == id)
			return mp;
	}

	return nullptr;
}
