#pragma once
#include <olcPixelGameEngine.h>
#include <yape2d/YapeEngine.h>
#include <SimulationSetup.h>

class Element;
class MassPoint;
class TwoPointsElement;
class UIBase;
class UIButton;
class UISlider;


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
		Edit,
		AddAnchor,
		AddMass,
		AddSpring,
		AddRod,
		COUNT
	};

	void DrawElements();
	bool UpdateUI();
	void DrawUI();

	void SwitchEditMode(EditMode mode);

	void UpdateEditor();
	void UpdateSimulation(float dt);

	void UpdateAddPoint();
	void UpdateAddTwoPoints();
	void UpdateEditMode();
	bool UpdateCamera();

	void StartSimulation();
	void StopSimulation();

	void ShowInfo();

	std::shared_ptr<MassPoint> FindMassPoint(olc::vf2d pos);
	std::shared_ptr<TwoPointsElement> FindTwoPointsElement(olc::vf2d pos);
	std::shared_ptr<Element> FindAnyElement(olc::vf2d pos);

	void ResetSetup();
	void InitBenchamarkA();
	void InitBenchamarkB();

	void OnSelectElement(std::shared_ptr<Element> element);
	void ResetCamera();

private:

	bool mIsSimulationDirty = false;
	bool mIsSimulationRunning = false;

	EditMode mEditMode = EditMode::COUNT;

	std::shared_ptr<Element> mElementToAdd = nullptr;
	std::shared_ptr<Element> mElementSelected = nullptr;

	std::unique_ptr<SimulationSetup> mSetup;

	// UI
	bool mMovingCamera = false;
	std::vector<std::shared_ptr<UIBase>> mUIElements;

	std::vector<std::shared_ptr<UIBase>> mUIElementsEdit;

	std::shared_ptr<UIButton> mUIButtonPlayPause;
	std::shared_ptr<UIButton> mUIButtonMoveCamera;
	std::shared_ptr<UISlider> mUISliderGravity = nullptr;
	std::shared_ptr<UISlider> mUISliderFriction = nullptr;

	std::vector < std::shared_ptr<UISlider>> mUISlidersSelected;

	yape2d::YapeEngine mPhysicsEngine;

	float mScale = 100.0;
	olc::vf2d mOffset = { 0.0f, 0.0f };
	olc::vf2d mLastMousePosition = { 0.0f, 0.0f };

	bool mIsMovingCamera = false;
	bool mMoveMoveCamera = false;
}; 
