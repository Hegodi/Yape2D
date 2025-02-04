#include <Playground.h>
#include <MassPoint.h>
#include <Spring.h>
#include <Rod.h>
#include <UI/UIButton.h>
#include <UI/UISlider.h>
#include <vector>

namespace
{
	const olc::Pixel ColorBackground = olc::Pixel(8, 22, 91);
	const olc::Pixel ColorGrid = olc::Pixel(53, 59, 91);

}

Playground::Playground()
{
	sAppName = "Yape 2D - Playground";
}

bool Playground::OnUserCreate()
{
	mSetup = std::make_unique<SimulationSetup>();

	mFont = std::make_shared<olc::Font>("../../data/Badaga.png");

	int posX = 5;
	int posY = 5;
	const int buttonWidth = 100;
	const int buttonHeight = 20;

	mUIButtonPlayPause = std::make_shared<UIButton>(posX, posY, buttonWidth, buttonHeight, "Run", [this]() 
	{
		if (mIsSimulationRunning)
		{
			mUIButtonPlayPause->SetText("Run");
			StopSimulation();
		}
		else
		{
			mUIButtonPlayPause->SetText("Stop");
			StartSimulation();
		}
	});
	mUIElements.push_back(mUIButtonPlayPause);

	{
		posX += buttonWidth + 10;
		auto btn = std::make_shared<UIButton>(posX, posY, buttonWidth, buttonHeight, "Reset Camera", [this]() {ResetCamera(); });
		mUIElements.push_back(btn);
	}
	{
		posX += buttonWidth + 10;
		mUIButtonMoveCamera = std::make_shared<UIButton>(posX, posY, buttonWidth, buttonHeight, "Move Camera", [this]()
		{
			mMoveMoveCamera = !mMoveMoveCamera;
			if (mMoveMoveCamera)
				mUIButtonMoveCamera->SetDefaultColor(olc::GREEN);
			else
				mUIButtonMoveCamera->ResetDefaultColor();
				
		});
		mUIElements.push_back(mUIButtonMoveCamera);
	}
	{
		posX += buttonWidth + 10;
		posY += 10;
		auto slide = std::make_shared<UISlider>(posX, posY, buttonWidth, buttonHeight, "Zoom");
		slide->SetLimits(10.0f, 200.0f);
		slide->SetValue(mScale);
		slide->SetCallback([this](float value) {mScale = value; });
		mUIElements.push_back(slide);
	}


	posX = 5;
	posY = 50;
	{
		auto btn = std::make_shared<UIButton>(posX, posY, buttonWidth, buttonHeight, "Edit", [this]() { SwitchEditMode(EditMode::Edit);});
		btn->SetName("Edit");
		mUIElements.push_back(btn);
		mUIElementsEdit.push_back(btn);
	}
	{
		posY += buttonHeight + 5;
		auto btn = std::make_shared<UIButton>(posX, posY, buttonWidth, buttonHeight, "Add Anchor", [this]() { SwitchEditMode(EditMode::AddAnchor);});
		btn->SetName("AddAnchor");
		mUIElements.push_back(btn);
		mUIElementsEdit.push_back(btn);
	}
	{
		posY += buttonHeight + 5;
		auto btn = std::make_shared<UIButton>(posX, posY, buttonWidth, buttonHeight, "Add Mass", [this]() { SwitchEditMode(EditMode::AddMass);});
		btn->SetName("AddMass");
		mUIElements.push_back(btn);
		mUIElementsEdit.push_back(btn);
	}
	{
		posY += buttonHeight + 5;
		auto btn = std::make_shared<UIButton>(posX, posY, buttonWidth, buttonHeight, "Add Spring", [this]() { SwitchEditMode(EditMode::AddSpring);});
		btn->SetName("AddSpring");
		mUIElements.push_back(btn);
		mUIElementsEdit.push_back(btn);
	}
	{
		posY += buttonHeight + 5;
		auto btn = std::make_shared<UIButton>(posX, posY, buttonWidth, buttonHeight, "Add Rod", [this]() { SwitchEditMode(EditMode::AddRod);});
		btn->SetName("AddRod");
		mUIElements.push_back(btn);
		mUIElementsEdit.push_back(btn);
	}
	{
		posY += 50;
		mUISliderGravity = std::make_shared<UISlider>(posX, posY, 100, 20, "Gravity");
		mUISliderGravity->SetLimits(0.0f, 20.0f);
		mUIElements.push_back(mUISliderGravity);
		mUIElementsEdit.push_back(mUISliderGravity);
		posY += 35;
	}
	{
		mUISliderFriction = std::make_shared<UISlider>(posX, posY, 100, 20, "Friction");
		mUISliderFriction->SetLimits(0.0f, 5.0f);
		mUISliderFriction->SetValue(0.1f);
		mUIElements.push_back(mUISliderFriction);
		mUIElementsEdit.push_back(mUISliderFriction);
	}

	posX = 5;
	posY = ScreenHeight() - buttonHeight - 5;
	{
		auto btn = std::make_shared<UIButton>(posX, posY, buttonWidth, buttonHeight, "Clear All", [this]() {ResetSetup(); });
		mUIElements.push_back(btn);
		mUIElementsEdit.push_back(btn);
	}
	{
		posX += buttonWidth + 10;
		auto btn = std::make_shared<UIButton>(posX, posY, buttonWidth, buttonHeight, "Example A", [this]()
			{
				LoadSetup("example_01.xml");
			});
		mUIElements.push_back(btn);
		mUIElementsEdit.push_back(btn);
	}
	{
		posX += buttonWidth + 5;
		auto btn = std::make_shared<UIButton>(posX, posY, buttonWidth, buttonHeight, "Example B", [this]() 
			{
				LoadSetup("example_02.xml");
			});
		mUIElements.push_back(btn);
		mUIElementsEdit.push_back(btn);
	}

#ifndef EMSCRIPTEN
	{
		posX = ScreenWidth() - buttonWidth - 5;
		posY = ScreenHeight() - buttonHeight - 5;
		auto btn = std::make_shared<UIButton>(posX, posY, buttonWidth, buttonHeight, "Save", [this]() {
			mSetup->mFriction = mUISliderFriction->GetValue();
			mSetup->mGravity = mUISliderGravity->GetValue();
			mSetup->Save("../../data/test.xml");
			
		});
		mUIElements.push_back(btn);
		mUIElementsEdit.push_back(btn);

		posX -= buttonWidth + 5;
		auto btn2 = std::make_shared<UIButton>(posX, posY, buttonWidth, buttonHeight, "Load", [this]() {
			LoadSetup("test.xml");
		});
		mUIElements.push_back(btn2);
		mUIElementsEdit.push_back(btn2);
	}
#endif // !EMSCRIPTEN


	ResetSetup();
	SwitchEditMode(EditMode::Edit);
	return true;
}

bool Playground::OnUserUpdate(float fElapsedTime)
{
	Clear(ColorBackground);

	DrawElements();
	DrawUI();
	bool interactionWithUI = UpdateUI();

	if (mIsSimulationRunning)
	{
		UpdateSimulation(fElapsedTime);
	}
	else
	{
		if (!interactionWithUI)
		{
			UpdateEditor();
		}
	}

	return true;
}

void Playground::DrawElements()
{
	int debugLevel = mIsSimulationRunning ? 0 : 1; 
	for (auto const& ele : mSetup->mMassPoints)
	{
		ele->Draw(this, debugLevel);
	}
	for (auto const& spr : mSetup->mTwoPointsElements)
	{
		spr->Draw(this, debugLevel);
	}
}

bool Playground::UpdateUI()
{
	for (auto& btn : mUIElements)
	{
		if (btn->Update(this))
		{
			return true;
		}
	}

	for (auto slider : mUISlidersSelected)
	{
		if (slider->Update(this))
		{
			return true;
		}
	}

	if (UpdateCamera())
	{
		return true;
	}


	return false;
}

void Playground::DrawUI()
{
	for (auto& btn : mUIElements)
	{
		btn->Draw(this);
	}
	for (auto slider : mUISlidersSelected)
	{
		slider->Draw(this);
	}
}

void Playground::SwitchEditMode(EditMode mode)
{
	if (mEditMode == mode)
	{
		return;
	}

	mEditMode = mode;
	mElementToAdd = nullptr;

	std::string buttonName;
	switch (mEditMode)
	{
	case Playground::EditMode::Edit:
		buttonName = "Edit";
		break;
	case Playground::EditMode::AddAnchor:
		buttonName = "AddAnchor";
		break;
	case Playground::EditMode::AddMass:
		buttonName = "AddMass";
		break;
	case Playground::EditMode::AddSpring:
		buttonName = "AddSpring";
		break;
	case Playground::EditMode::AddRod:
		buttonName = "AddRod";
		break;
	default:
		break;
	}

	for (auto ele : mUIElementsEdit)
	{
		auto btn = std::dynamic_pointer_cast<UIButton>(ele);
		if (btn == nullptr)
			continue;
		if (btn->GetName() == buttonName)
		{
			btn->SetDefaultColor(olc::DARK_GREEN);
		}
		else
		{
			btn->ResetDefaultColor();
		}
	}
}

void Playground::UpdateEditor()
{
	if (GetKey(olc::M).bPressed)
	{
		SwitchEditMode(static_cast<EditMode>((static_cast<int>(mEditMode) + 1) % static_cast<int>(EditMode::COUNT)));
	}

	switch (mEditMode)
	{
	case Playground::EditMode::Edit:
		UpdateEditMode();
		break;
	case Playground::EditMode::AddAnchor:
		UpdateAddPoint();
		break;
	case Playground::EditMode::AddMass:
		UpdateAddPoint();
		break;
	case Playground::EditMode::AddSpring:
		UpdateAddTwoPoints();
		break;
	case Playground::EditMode::AddRod:
		UpdateAddTwoPoints();
		break;
	default:
		break;
	}

	if (mElementToAdd != nullptr)
	{
		mElementToAdd->Draw(this, 0);
	}

	if (mElementSelected != nullptr)
	{
		const int RadiusSelected = 15;
		DrawCircle(FromWorldToScreen(mElementSelected->GetPosition()), RadiusSelected, olc::RED);
	}
}

void Playground::UpdateSimulation(float dt)
{
	auto start = std::chrono::high_resolution_clock::now();

	mPhysicsEngine.Update(dt);


	for (auto& mp : mSetup->mMassPoints)
	{
		auto pos = mPhysicsEngine.GetPosition(mp->GetId());
		mp->SetPosition(pos.x, pos.y);
	}

	auto end = std::chrono::high_resolution_clock::now();
	std::chrono::duration<float> elapsed = end - start;

	ShowInfo(elapsed.count());
}

void Playground::UpdateAddPoint()
{
	if (GetMouse(0).bPressed)
	{
		auto mp = std::make_shared<MassPoint>();
		if (mEditMode == EditMode::AddAnchor)
		{
			mp->SetFixed();
		}
		mp->SetPosition(FromScreenToWorld(GetMousePos()));
		mSetup->mMassPoints.push_back(mp);
	}
}

void Playground::UpdateAddTwoPoints()
{
	if (mElementToAdd != nullptr)
	{
		auto tpe = std::dynamic_pointer_cast<TwoPointsElement>(mElementToAdd);
		tpe->SetTemporaryEndPoint(FromScreenToWorld(GetMousePos()));

	}
	if (GetMouse(0).bPressed)
	{
		auto mp = FindMassPoint(GetMousePos());
		if (mp != nullptr)
		{
			
			if (mElementToAdd == nullptr)
			{
				std::shared_ptr<TwoPointsElement> tpe = nullptr;
				if (mEditMode == EditMode::AddSpring)
					tpe = std::make_shared<Spring>();
				else 
					tpe = std::make_shared<Rod>();

				tpe->SetPointStart(mp);
				tpe->SetTemporaryEndPoint(FromScreenToWorld(GetMousePos()));
				mElementToAdd = tpe;
			}
			else
			{
				auto tpe = std::dynamic_pointer_cast<TwoPointsElement>(mElementToAdd);
				tpe->SetPointEnd(mp);

				auto it = std::find_if(mSetup->mTwoPointsElements.begin(), mSetup->mTwoPointsElements.end(), [tpe](std::shared_ptr<TwoPointsElement> const& e) {return e->HasTheSameConnections(tpe); });
				if (it == mSetup->mTwoPointsElements.end())
				{
					mSetup->mTwoPointsElements.push_back(tpe);
					mIsSimulationDirty = true;
					mElementToAdd = nullptr;
				}
				else
				{
					tpe->SetPointEnd(nullptr);
				}
			}
		}
	}
	else if (GetMouse(1).bPressed)
	{
		mElementToAdd = nullptr;
	}
}

void Playground::UpdateEditMode()
{
	if (auto ele = FindAnyElement(GetMousePos()))
	{
		if (GetMouse(0).bPressed)
		{
			OnSelectElement(ele);
		}
		else if (GetMouse(1).bPressed)
		{
			if (ele == mElementSelected)
			{
				OnSelectElement(nullptr);
			}

			if (auto mp = std::dynamic_pointer_cast<MassPoint>(ele))
			{
				for (auto it = mSetup->mTwoPointsElements.begin(); it != mSetup->mTwoPointsElements.end();)
				{
					std::shared_ptr<TwoPointsElement> const& tpe = *it;
					if (tpe->GetMassPointEnd() == mp || tpe->GetMassPointStart() == mp)
					{
						it = mSetup->mTwoPointsElements.erase(it);
					}
					else
					{
						it++;
					}
				}

				mSetup->mMassPoints.erase(std::remove(mSetup->mMassPoints.begin(), mSetup->mMassPoints.end(), mp), mSetup->mMassPoints.end());
				mIsSimulationDirty = true;
			}
			else if (auto tpe = std::dynamic_pointer_cast<TwoPointsElement>(ele))
			{
				mSetup->mTwoPointsElements.erase(std::remove(mSetup->mTwoPointsElements.begin(), mSetup->mTwoPointsElements.end(), tpe), mSetup->mTwoPointsElements.end());
				mIsSimulationDirty = true;
			}
		}
	}


}

bool Playground::UpdateCamera()
{
	if (!mMoveMoveCamera)
	{
		return false;
	}

	if (GetMouse(0).bPressed)
	{
		mLastMousePosition = GetMousePos();
		mIsMovingCamera = true;
		return true;
	}

	if (mIsMovingCamera)
	{
		if (GetMouse(0).bHeld)
		{
			auto pos = GetMousePos();
			olc::vf2d delta = pos - mLastMousePosition;
			if (delta.mag() > 0)
			{
				mLastMousePosition = pos;
				mOffset.x += delta.x;
				mOffset.y += delta.y;
			}
		}
		else
		{
			mIsMovingCamera = false;
		}
		return true;
	}

	mScale += GetMouseWheel() / 100.0f;
	return false;

}


void Playground::StartSimulation()
{
	OnSelectElement(nullptr);
	if (mIsSimulationDirty)
	{
		mPhysicsEngine.Reset();

		for (auto const& mp : mSetup->mMassPoints)
		{
			auto pos = mp->GetPosition();
			mPhysicsEngine.AddPointMass(mp->GetId(), mp->GetMass(), pos.x, pos.y);
			if (mp->IsFixed())
			{
				mPhysicsEngine.AddFixedPositionConstrain(mp->GetId(), yape2d::vec2f(pos.x, pos.y));
			}
		}

		for (auto const& tpe : mSetup->mTwoPointsElements)
		{
			if (auto sp = std::dynamic_pointer_cast<Spring>(tpe))
			{
				mPhysicsEngine.AddSpring(sp->GetMassPointStart()->GetId(), sp->GetMassPointEnd()->GetId(), sp->GetLength(), sp->GetElasticConstant());
			}
			else
			{
				mPhysicsEngine.AddDistanceConstrain(tpe->GetMassPointStart()->GetId(), tpe->GetMassPointEnd()->GetId(), tpe->GetLength());
			}
		}
	}

	mPhysicsEngine.SetGravity(mUISliderGravity->GetValue());
	mPhysicsEngine.SetFriction(mUISliderFriction->GetValue());
	mIsSimulationRunning = true;

	for (auto btn : mUIElementsEdit)
	{
		btn->Hide();
	}
}

void Playground::StopSimulation()
{
	mIsSimulationRunning = false;
	SwitchEditMode(EditMode::Edit);
	for (auto btn : mUIElementsEdit)
	{
		btn->Show();
	}
}

void Playground::ShowInfo(float delta)
{
	std::string info;
	info  = "Mass Points         : " + std::to_string(mSetup->mMassPoints.size()) + "\n";
	info += "Two Points Elements : " + std::to_string(mSetup->mTwoPointsElements.size()) + "\n";
	info += "Constrains          : " + std::to_string(mPhysicsEngine.GetNumberConstrains()) + "\n";
	info += "Forces              : " + std::to_string(mPhysicsEngine.GetNumberForces()) + "\n";
	info += "Physics update frame: " + std::to_string(delta) + "ms \n";

	DrawString(ScreenWidth() - 270, 10, info, olc::WHITE);
}

std::shared_ptr<MassPoint> Playground::FindMassPoint(olc::vf2d pos)
{
	for (auto const& mp : mSetup->mMassPoints)
	{
		if ((pos - FromWorldToScreen(mp->GetPosition())).mag() < mp->GetRadius())
		{
			return mp;
		}
	}
	return nullptr;
}

std::shared_ptr<TwoPointsElement> Playground::FindTwoPointsElement(olc::vf2d pos)
{
	for (auto const& tpe : mSetup->mTwoPointsElements)
	{
		if ((pos - FromWorldToScreen(tpe->GetPosition())).mag() < TwoPointElementInteractionRadius)
		{
			return tpe;
		}
	}
	return nullptr;
}

std::shared_ptr<Element> Playground::FindAnyElement(olc::vf2d pos)
{
	for (auto const& mp : mSetup->mMassPoints)
	{
		if ((pos - FromWorldToScreen(mp->GetPosition())).mag() < mp->GetRadius())
		{
			return mp;
		}
	}

	for (auto const& pte : mSetup->mTwoPointsElements)
	{
		if ((pos - FromWorldToScreen(pte->GetPosition())).mag() < TwoPointElementInteractionRadius)
		{
			return pte;
		}
	}
	return nullptr;
}

void Playground::ResetSetup()
{
	ResetCamera();
	StopSimulation();
	mSetup->Reset();
	mPhysicsEngine.Reset();
	mIsSimulationDirty = true;
	mUISliderGravity->SetValue(9.8f);
	mUISliderFriction->SetValue(0.1f);
}

void Playground::InitBenchamarkA()
{
	ResetCamera();
	StopSimulation();
	mSetup->Reset();

	auto mp1 = std::make_shared<MassPoint>();
	mp1->SetPosition(olc::vf2d(-1.0f, 0.0f));
	mp1->SetFixed();
	mSetup->mMassPoints.push_back(mp1);

	auto mp2 = std::make_shared<MassPoint>();
	mp2->SetPosition(olc::vf2d(0.0f, 0.0f));
	mSetup->mMassPoints.push_back(mp2);

	auto mp3 = std::make_shared<MassPoint>();
	mp3->SetPosition(olc::vf2d(1.0f, 0.0f));
	mp3->SetFixed();
	mSetup->mMassPoints.push_back(mp3);

	{
		auto rod = std::make_shared<Rod>();
		rod->SetPointStart(mp1);
		rod->SetPointEnd(mp2);
		mSetup->mTwoPointsElements.push_back(rod);
	}

	{
		auto rod = std::make_shared<Rod>();
		rod->SetPointStart(mp2);
		rod->SetPointEnd(mp3);
		mSetup->mTwoPointsElements.push_back(rod);
	}
	mIsSimulationDirty = true;
	mUISliderGravity->SetValue(9.8f);
}

void Playground::InitBenchamarkB()
{
	ResetCamera();
	StopSimulation();
	mSetup->Reset();

	auto mp1 = std::make_shared<MassPoint>();
	mp1->SetPosition(olc::vf2d(0.0f, 2.5f));
	mp1->SetFixed();
	mSetup->mMassPoints.push_back(mp1);

	auto mp2 = std::make_shared<MassPoint>();
	mp2->SetPosition(olc::vf2d(1.4f, 1.5f));
	mSetup->mMassPoints.push_back(mp2);

	auto mp3 = std::make_shared<MassPoint>();
	mp3->SetPosition(olc::vf2d(-1.2f, 0.0f));
	mSetup->mMassPoints.push_back(mp3);

	{
		auto rod = std::make_shared<Rod>();
		rod->SetPointStart(mp1);
		rod->SetPointEnd(mp2);
		mSetup->mTwoPointsElements.push_back(rod);
	}

	{
		auto rod = std::make_shared<Rod>();
		rod->SetPointStart(mp2);
		rod->SetPointEnd(mp3);
		mSetup->mTwoPointsElements.push_back(rod);
	}

	mUISliderGravity->SetValue(9.8f);
	mIsSimulationDirty = true;
}


void Playground::OnSelectElement(std::shared_ptr<Element> element)
{
	if (element == mElementSelected)
	{
		return;
	}
	mElementSelected = element;
	
	mUISlidersSelected.clear();
	if (element == nullptr)
	{
		return;
	}

	int posX = ScreenWidth() - 200;
	int posY = 15;
	if (auto mp = std::dynamic_pointer_cast<MassPoint>(element))
	{
		if (!mp->IsFixed())
		{
			auto sliderMass = std::make_shared<UISlider>(posX, posY, 100, 25, "Mass");
			sliderMass->SetLimits(0.1f, 20.0f);
			sliderMass->SetValue(mp->GetMass());
			sliderMass->SetCallback([this](float value) { 
				std::dynamic_pointer_cast<MassPoint>(mElementSelected)->SetMass(value); 
				mIsSimulationDirty = true;
				});
			mUISlidersSelected.push_back(sliderMass);
		}
	}
	else if (auto spring = std::dynamic_pointer_cast<Spring>(element))
	{
		auto sliderLength = std::make_shared<UISlider>(posX, posY, 100, 25, "Length");
		sliderLength->SetLimits(0.1f, 5.0f);
		sliderLength->SetValue(spring->GetLength());
		sliderLength->SetCallback([this](float value) { 
			std::dynamic_pointer_cast<Spring>(mElementSelected)->SetLength(value); 
			mIsSimulationDirty = true;
			});
		mUISlidersSelected.push_back(sliderLength);

		posY += 30;
		auto sliderElastic = std::make_shared<UISlider>(posX, posY, 100, 25, "k");
		sliderElastic->SetLimits(1.0f, 100.0f);
		sliderElastic->SetValue(spring->GetElasticConstant());
		sliderElastic->SetCallback([this](float value) { 
			std::dynamic_pointer_cast<Spring>(mElementSelected)->SetElasticConstant(value); 
			mIsSimulationDirty = true;
			});
		mUISlidersSelected.push_back(sliderElastic);
	}
}

void Playground::ResetCamera()
{
	mMoveMoveCamera = false;
	mUIButtonMoveCamera->ResetDefaultColor();
	mOffset = { ScreenWidth() * 0.5f, ScreenHeight() * 0.5f };
	mScale = 100.0f;
}

void Playground::LoadSetup(const char* filename)
{
	mSetup->Load((std::string("../../data/") + std::string(filename)).c_str());
	mIsSimulationDirty = true;
	mUISliderGravity->SetValue(mSetup->mGravity);
	mUISliderFriction->SetValue(mSetup->mFriction);
}

olc::vf2d Playground::FromScreenToWorld(olc::vf2d const& pos) const
{
	return olc::vf2d((pos.x - mOffset.x) / mScale, -(pos.y - mOffset.y) / mScale);
}

olc::vf2d Playground::FromWorldToScreen(olc::vf2d const& pos) const
{
	return olc::vf2d(pos.x * mScale + mOffset.x, - pos.y * mScale + mOffset.y);
}
