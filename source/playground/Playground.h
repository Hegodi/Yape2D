#pragma once
#include <olcPixelGameEngine.h>
#include <Button.h>
#include <yape2d/YapeEngine.h>


class Element;
class MassPoint;
class TwoPointsElement;

class Playground : public olc::PixelGameEngine
{
public:
	Playground();

public:
	bool OnUserCreate() override;
	bool OnUserUpdate(float fElapsedTime) override;

	olc::vf2d FromScreenToWorld(olc::vf2d const& pos) const;
	olc::vf2d FromWorldToScreen(olc::vf2d const& pos) const;

private:
	enum class EditMode
	{
		AddMass,
		AddSpring,
		AddTwoPointsElement,
		COUNT
	};

	void DrawElements();

	void SwitchEditMode(EditMode mode);

	void UpdateEditMode();
	void UpdateSimulation(float dt);

	void UpdateAddPoint();
	void UpdateAddTwoPoints();

	void StartSimulation();
	void StopSimulation();

	std::shared_ptr<MassPoint> FindMassPoint(olc::vf2d pos);

	void ResetSetup();
	void InitSpringTests();
	void InitDoublePendulum();

private:

	bool mIsSimulationDirty = false;

	bool mWithGravity = false;
	bool mIsSimulationRunning = false;
	EditMode mEditMode = EditMode::COUNT;

	std::shared_ptr<Element> mElementSelected = nullptr;

	std::vector<std::shared_ptr<MassPoint>> mMassPoints;
	std::vector<std::shared_ptr<TwoPointsElement>> mTwoPointsElements;

	std::vector<std::shared_ptr<Button>> mButtons;

	std::vector<std::shared_ptr<Button>> mButtonsEdit;

	std::shared_ptr<Button> mButtonPlayPause;

	yape2d::YapeEngine mPhysicsEngine;

	float mScale = 100.0;
	olc::vf2d mOffset = { 0.0f, 0.0f };
}; 
