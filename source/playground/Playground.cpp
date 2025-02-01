#include <Playground.h>
#include <MassPoint.h>
#include <Spring.h>
#include <UI/UIButton.h>

Playground::Playground()
{
	sAppName = "Parphys - Playground";
}

bool Playground::OnUserCreate()
{
	mOffset = { ScreenWidth() * 0.5f, ScreenHeight() * 0.5f };


	int buttonX = 5;
	int buttonY = 5;
	const int buttonWidth = 100;
	const int buttonHeight = 30;

	mUIButtonPlayPause = std::make_shared<UIButton>(buttonX, buttonY, buttonWidth, buttonHeight, "Run", [this]() 
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


	buttonY = 100;
	{
		auto btn = std::make_shared<UIButton>(buttonX, buttonY, buttonWidth, buttonHeight, "Mass Point", [this]() { SwitchEditMode(EditMode::AddMass);});
		mUIElements.push_back(btn);
		mUIButtonsEdit.push_back(btn);
	}
	{
		buttonY += 35;
		auto btn = std::make_shared<UIButton>(buttonX, buttonY, buttonWidth, buttonHeight, "Spring", [this]() { SwitchEditMode(EditMode::AddSpring);});
		mUIElements.push_back(btn);
		mUIButtonsEdit.push_back(btn);
	}
	{
		buttonY += 35;
		auto btn = std::make_shared<UIButton>(buttonX, buttonY, buttonWidth, buttonHeight, "Rod", [this]() { SwitchEditMode(EditMode::AddTwoPointsElement);});
		mUIElements.push_back(btn);
		mUIButtonsEdit.push_back(btn);
	}

	buttonX = ScreenWidth() - 105;
	buttonY = 5;
	{
		auto btn = std::make_shared<UIButton>(buttonX, buttonY, buttonWidth, buttonHeight, "Clear All", [this]() {ResetSetup(); });
		mUIElements.push_back(btn);
		mUIButtonsEdit.push_back(btn);
	}
	{
		buttonY += 35;
		auto btn = std::make_shared<UIButton>(buttonX, buttonY, buttonWidth, buttonHeight, "Double Pend", [this]() {InitDoublePendulum(); });
		mUIElements.push_back(btn);
		mUIButtonsEdit.push_back(btn);
	}
	{
		buttonY += 35;
		auto btn = std::make_shared<UIButton>(buttonX, buttonY, buttonWidth, buttonHeight, "Springs", [this]() {InitSpringTests(); });
		mUIElements.push_back(btn);
		mUIButtonsEdit.push_back(btn);
	}

	ResetSetup();
	return true;
}

bool Playground::OnUserUpdate(float fElapsedTime)
{
	Clear(olc::BLACK);

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
			UpdateEditMode();
		}
	}


	return true;
}

void Playground::DrawElements()
{
	for (auto const& ele : mMassPoints)
	{
		ele->Draw(this);
	}
	for (auto const& spr : mTwoPointsElements)
	{
		spr->Draw(this);
	}
}

bool Playground::UpdateUI()
{
	for (auto& btn : mUIElements)
	{
		if (btn->Update(this))
		{
			// If one element return true (i.e has been clicked on)
			// we don't check more
			return true;
		}
	}
	return false;
}

void Playground::DrawUI()
{
	for (auto& btn : mUIElements)
	{
		btn->Draw(this);
	}
}

void Playground::SwitchEditMode(EditMode mode)
{
	if (mEditMode == mode)
	{
		return;
	}

	mEditMode = mode;
	mElementSelected = nullptr;
}

void Playground::UpdateEditMode()
{
	if (GetKey(olc::M).bPressed)
	{
		SwitchEditMode(static_cast<EditMode>((static_cast<int>(mEditMode) + 1) % static_cast<int>(EditMode::COUNT)));
	}

	switch (mEditMode)
	{
	case Playground::EditMode::AddMass:
		UpdateAddPoint();
		break;
	case Playground::EditMode::AddSpring:
		UpdateAddTwoPoints();
		break;
	case Playground::EditMode::AddTwoPointsElement:
		UpdateAddTwoPoints();
		break;
	default:
		break;
	}

	if (mElementSelected != nullptr)
	{
		mElementSelected->Draw(this);
	}

}

void Playground::UpdateSimulation(float dt)
{
	mPhysicsEngine.Update(dt);

	for (auto& mp : mMassPoints)
	{
		auto pos = mPhysicsEngine.GetPosition(mp->GetId());
		mp->SetPosition(pos.x, pos.y);
	}
}

void Playground::UpdateAddPoint()
{
	if (GetMouse(0).bPressed)
	{
		auto mp = std::make_shared<MassPoint>();
		mp->SetPosition(FromScreenToWorld(GetMousePos()));
		mMassPoints.push_back(mp);
	}
	else if (GetMouse(1).bPressed)
	{
		auto mp = FindMassPoint(GetMousePos());
		if (mp != nullptr)
		{
			std::erase_if(mTwoPointsElements, [mp](std::shared_ptr<TwoPointsElement> tpe) { return tpe->GetMassPointEnd() == mp || tpe->GetMassPointStart() == mp; });
			mIsSimulationDirty = true;

			for (auto it = mMassPoints.begin(); it != mMassPoints.end(); it++)
			{
				if (*it == mp)
				{
					mMassPoints.erase(it);
					break;
				}
			}
		}
	}
}

void Playground::UpdateAddTwoPoints()
{
	if (mElementSelected != nullptr)
	{
		auto spring = std::dynamic_pointer_cast<TwoPointsElement>(mElementSelected);
		spring->SetTemporaryEndPoint(FromScreenToWorld(GetMousePos()));

	}
	if (GetMouse(0).bPressed)
	{
		auto mp = FindMassPoint(GetMousePos());
		if (mp != nullptr)
		{
			
			if (mElementSelected == nullptr)
			{
				std::shared_ptr<TwoPointsElement> tpe = mEditMode == EditMode::AddSpring ? std::make_shared<Spring>() : std::make_shared<TwoPointsElement>();
				tpe->SetPointStart(mp);
				tpe->SetTemporaryEndPoint(FromScreenToWorld(GetMousePos()));
				mElementSelected = tpe;
			}
			else
			{
				auto tpe = std::dynamic_pointer_cast<TwoPointsElement>(mElementSelected);
				tpe->SetPointEnd(mp);

				auto it = std::find_if(mTwoPointsElements.begin(), mTwoPointsElements.end(), [tpe](std::shared_ptr<TwoPointsElement> const& e) {return e->HasTheSameConnections(tpe); });
				if (it == mTwoPointsElements.end())
				{
					mTwoPointsElements.push_back(tpe);
					mIsSimulationDirty = true;
					mElementSelected = nullptr;
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
		mElementSelected = nullptr;
	}
}

void Playground::StartSimulation()
{
	if (mIsSimulationDirty)
	{
		mPhysicsEngine.Reset();

		for (auto const& mp : mMassPoints)
		{
			auto pos = mp->GetPosition();
			mPhysicsEngine.AddPointMass(mp->GetId(), mp->GetMass(), pos.x, pos.y);
			if (mp->IsFixed())
			{
				mPhysicsEngine.AddFixedPositionConstrain(mp->GetId(), yape2d::vec2f(pos.x, pos.y));
			}
		}

		for (auto const& tpe : mTwoPointsElements)
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

	mIsSimulationRunning = true;

	for (auto btn : mUIButtonsEdit)
	{
		btn->Hide();
	}
}

void Playground::StopSimulation()
{
	mIsSimulationRunning = false;
	for (auto btn : mUIButtonsEdit)
	{
		btn->Show();
	}
}

void Playground::ShowInfo()
{
	std::string info;
	info = "Mass Points " + std::to_string(mMassPoints.size()) + "\n";
	info += "Two Points Elements: " + std::to_string(mTwoPointsElements.size()) + "\n";

	DrawString(10, ScreenHeight() - 50 , info, olc::WHITE);
}

std::shared_ptr<MassPoint> Playground::FindMassPoint(olc::vf2d pos)
{
	for (auto const& mp : mMassPoints)
	{
		if ((pos - FromWorldToScreen(mp->GetPosition())).mag() < mp->GetRadius())
		{
			return mp;
		}
	}
	return nullptr;
}

void Playground::ResetSetup()
{
	StopSimulation();
	mMassPoints.clear();
	mTwoPointsElements.clear();
	mPhysicsEngine.Reset();
	mIsSimulationDirty = true;
}

void Playground::InitSpringTests()
{
	StopSimulation();
	auto mp1 = std::make_shared<MassPoint>();
	mp1->SetPosition(olc::vf2d(0.0f, 3.0f));
	mp1->SetFixed();
	mMassPoints.push_back(mp1);

	auto mp2 = std::make_shared<MassPoint>();
	mp2->SetPosition(olc::vf2d(1.4f, 1.5f));
	mMassPoints.push_back(mp2);

	auto mp3 = std::make_shared<MassPoint>();
	mp3->SetPosition(olc::vf2d(-1.2f, 0.0f));
	mMassPoints.push_back(mp3);

	{
		auto rod = std::make_shared<Spring>();
		rod->SetPointStart(mp1);
		rod->SetPointEnd(mp2);
		mTwoPointsElements.push_back(rod);
	}

	{
		auto rod = std::make_shared<Spring>();
		rod->SetPointStart(mp2);
		rod->SetPointEnd(mp3);
		mTwoPointsElements.push_back(rod);
	}
	mIsSimulationDirty = true;
}

void Playground::InitDoublePendulum()
{
	StopSimulation();
	auto mp1 = std::make_shared<MassPoint>();
	mp1->SetPosition(olc::vf2d(0.0f, 3.0f));
	mp1->SetFixed();
	mMassPoints.push_back(mp1);

	auto mp2 = std::make_shared<MassPoint>();
	mp2->SetPosition(olc::vf2d(1.4f, 1.5f));
	mMassPoints.push_back(mp2);

	auto mp3 = std::make_shared<MassPoint>();
	mp3->SetPosition(olc::vf2d(-1.2f, 0.0f));
	mMassPoints.push_back(mp3);

	{
		auto rod = std::make_shared<TwoPointsElement>();
		rod->SetPointStart(mp1);
		rod->SetPointEnd(mp2);
		mTwoPointsElements.push_back(rod);
	}

	{
		auto rod = std::make_shared<TwoPointsElement>();
		rod->SetPointStart(mp2);
		rod->SetPointEnd(mp3);
		mTwoPointsElements.push_back(rod);
	}

	mPhysicsEngine.SetGravity(9.8);
	mIsSimulationDirty = true;
}

olc::vf2d Playground::FromScreenToWorld(olc::vf2d const& pos) const
{
	return olc::vf2d((pos.x - mOffset.x) / mScale, -(pos.y - mOffset.y) / mScale);
}

olc::vf2d Playground::FromWorldToScreen(olc::vf2d const& pos) const
{
	return olc::vf2d(pos.x * mScale + mOffset.x, - pos.y * mScale + mOffset.y);
}
