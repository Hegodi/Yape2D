#include <Playground.h>
#include <MassPoint.h>
#include <Spring.h>

Playground::Playground()
{
	sAppName = "Parphys - Playground";
}

bool Playground::OnUserCreate()
{
	mOffset = { ScreenWidth() * 0.5f, ScreenHeight() * 0.5f };
	mPlotEnergy.SetSize(300, 150);
	//InitTest();
	InitSpringPendulum();
	//InitSpringDoublePendulum();
	return true;
}

bool Playground::OnUserUpdate(float fElapsedTime)
{
	Clear(olc::BLACK);

	for (auto& btn : mButtons)
	{
		btn.Update(this);
	}

	DrawElements();

	if (mShowPlotEnergy)
	{
		mPlotEnergy.Draw(this, ScreenWidth() - 310, ScreenHeight() - 160);
	}

	if (GetKey(olc::P).bPressed) mShowPlotEnergy != mShowPlotEnergy;

	if (mIsSimulationRunning)
	{
		DrawString(10, ScreenHeight() - 10 , "Pulse SPACE to stop the simulation and enter edit mode", olc::WHITE);
		UpdateSimulation(fElapsedTime);
	}
	else
	{
		DrawString(10, ScreenHeight() - 10 , "Pulse SPACE to run the simulation", olc::WHITE);
		UpdateEditMode();
	}

	std::string info;
	info = "Mass Points " + std::to_string(mMassPoints.size()) + "\n";
	info += "Springs: " + std::to_string(mSprings.size()) + "\n";

	DrawString(10, ScreenHeight() - 50 , info, olc::WHITE);

	if (GetKey(olc::SPACE).bPressed)
	{
		if (!mIsSimulationRunning)
		{
			StartSimulation();
		}
		else
		{
			mIsSimulationRunning = false;
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
	for (auto const& spr : mSprings)
	{
		spr->Draw(this);
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
	switch (mEditMode)
	{
	case Playground::EditMode::AddMass:
		break;
	case Playground::EditMode::AddSpring:
		break;
	default:
		break;
	}

}

void Playground::UpdateEditMode()
{
	if (GetKey(olc::M).bPressed)
	{
		SwitchEditMode(static_cast<EditMode>((static_cast<int>(mEditMode) + 1) % static_cast<int>(EditMode::COUNT)));
	}

	std::string txt = "Edit Mode: ";

	switch (mEditMode)
	{
	case Playground::EditMode::AddMass:
		txt += "Add Mass Point";
		UpdateAddMass();
		break;
	case Playground::EditMode::AddSpring:
		UpdateAddSpring();
		txt += "Add Spring";
		break;
	default:
		break;
	}
	txt += "\nPusa M to switch";
	DrawString(10, 10, txt, olc::WHITE);

	if (mElementSelected != nullptr)
	{
		mElementSelected->Draw(this);
	}

}

void Playground::UpdateSimulation(float dt)
{
	mPhysicsEngine.Update(dt);
	double energy = mPhysicsEngine.CalculateEnergy();
	char energy_txt[64];
	sprintf(energy_txt, "Energy: %13.6e (%.1f)", energy, mPhysicsEngine.GetTime());
	DrawString(olc::vi2d(ScreenWidth() - 260, 10), energy_txt);
	mPlotEnergy.AddValue(energy);

	for (auto& mp : mMassPoints)
	{
		auto pos = mPhysicsEngine.GetPosition(mp->GetId());
		mp->SetPosition(pos.x, pos.y);
	}
}

void Playground::UpdateAddMass()
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
			std::erase_if(mSprings, [mp](std::shared_ptr<Spring> spring) { return spring->GetMassPointEnd() == mp || spring->GetMassPointStart() == mp; });
		}

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

void Playground::UpdateAddSpring()
{
	if (mElementSelected != nullptr)
	{
		auto spring = std::dynamic_pointer_cast<Spring>(mElementSelected);
		spring->SetTemporaryEndPoint(FromScreenToWorld(GetMousePos()));

	}
	if (GetMouse(0).bPressed)
	{
		auto mp = FindMassPoint(GetMousePos());
		if (mp != nullptr)
		{
			
			if (mElementSelected == nullptr)
			{
				auto spring = std::make_shared<Spring>();
				spring->SetPointStart(mp);
				spring->SetTemporaryEndPoint(FromScreenToWorld(GetMousePos()));
				mElementSelected = spring;
			}
			else
			{
				auto spring = std::dynamic_pointer_cast<Spring>(mElementSelected);
				spring->SetPointEnd(mp);

				auto it = std::find_if(mSprings.begin(), mSprings.end(), [spring](std::shared_ptr<Spring> const& spr) {return spr->HasTheSameConnections(spring); });
				if (it == mSprings.end())
				{
					mSprings.push_back(spring);
					mElementSelected = nullptr;
				}
				else
				{
					spring->SetPointEnd(nullptr);
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
	mPhysicsEngine.Reset();

	for (auto const& mp : mMassPoints)
	{
		auto pos = mp->GetPosition();
		mPhysicsEngine.AddPointMass(mp->GetId(), mp->GetMass(), pos.x, pos.y, mp->IsFixed());
	}

	for (auto const& sp : mSprings)
	{
		mPhysicsEngine.AddSpring(sp->GetMassPointStart()->GetId(), sp->GetMassPointEnd()->GetId(), sp->GetLength(), sp->GetElasticConstant());
	}


	mIsSimulationRunning = true;
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

void Playground::InitTest()
{
	auto mp1 = std::make_shared<MassPoint>();
	mp1->SetPosition(olc::vf2d(-1.5f, 0.0f));
	mp1->SetFixed();
	mMassPoints.push_back(mp1);

	auto mp2 = std::make_shared<MassPoint>();
	mp2->SetPosition(olc::vf2d(0.3f, 0.0f));
	mMassPoints.push_back(mp2);

	auto mp3 = std::make_shared<MassPoint>();
	mp3->SetPosition(olc::vf2d(1.5f, 0.0f));
	mMassPoints.push_back(mp3);

	{
		auto spring = std::make_shared<Spring>();
		spring->SetPointStart(mp1);
		spring->SetPointEnd(mp2);
		spring->SetLength(1.0f);
		mSprings.push_back(spring);
	}

	{
		auto spring = std::make_shared<Spring>();
		spring->SetPointStart(mp3);
		spring->SetPointEnd(mp2);
		spring->SetLength(1.0f);
		mSprings.push_back(spring);
	}
}

void Playground::InitSpringPendulum()
{
	auto mp1 = std::make_shared<MassPoint>();
	mp1->SetPosition(olc::vf2d(0.0f, 2.0f));
	mp1->SetFixed();
	mMassPoints.push_back(mp1);

	auto mp2 = std::make_shared<MassPoint>();
	mp2->SetPosition(olc::vf2d(0.3f, 1.0f));
	mMassPoints.push_back(mp2);

	auto spring = std::make_shared<Spring>();
	spring->SetPointStart(mp1);
	spring->SetPointEnd(mp2);
	spring->SetLength(1.0f);
	spring->SetElasticConstant(10.0);
	mSprings.push_back(spring);

	mPhysicsEngine.SetGravity(9.8);
}

void Playground::InitSpringDoublePendulum()
{
	auto mp1 = std::make_shared<MassPoint>();
	mp1->SetPosition(olc::vf2d(0.0f, 2.0f));
	mp1->SetFixed();
	mMassPoints.push_back(mp1);

	auto mp2 = std::make_shared<MassPoint>();
	mp2->SetPosition(olc::vf2d(0.4f, 1.0f));
	mMassPoints.push_back(mp2);

	auto mp3 = std::make_shared<MassPoint>();
	mp3->SetPosition(olc::vf2d(-0.2f, 0.0f));
	mMassPoints.push_back(mp3);

	{
		auto spring = std::make_shared<Spring>();
		spring->SetPointStart(mp1);
		spring->SetPointEnd(mp2);
		spring->SetLength(1.0f);
		spring->SetElasticConstant(10.0);
		mSprings.push_back(spring);
	}

	{
		auto spring = std::make_shared<Spring>();
		spring->SetPointStart(mp2);
		spring->SetPointEnd(mp3);
		spring->SetLength(1.0f);
		spring->SetElasticConstant(10.0);
		mSprings.push_back(spring);
	}

	mPhysicsEngine.SetGravity(9.8);
}

olc::vf2d Playground::FromScreenToWorld(olc::vf2d const& pos) const
{
	return olc::vf2d((pos.x - mOffset.x) / mScale, -(pos.y - mOffset.y) / mScale);
}

olc::vf2d Playground::FromWorldToScreen(olc::vf2d const& pos) const
{
	return olc::vf2d(pos.x * mScale + mOffset.x, - pos.y * mScale + mOffset.y);
}
