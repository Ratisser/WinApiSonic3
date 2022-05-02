#include "EditLevelController.h"

#include <RSRenderer.h>
#include <RSCollision.h>
#include <RSWindow.h>
#include <RSLevelManager.h>
#include <sstream>

#include "eCollisionCheckGroup.h"
#include "Ring.h"
#include "Monitor.h"

EditLevelController::EditLevelController()
	: cursorRenderer_(nullptr)
	, cursorCollision_(nullptr)
	, focusedRing_(nullptr)
	, focusedMonitor_(nullptr)
	, mode_(eEditMode::RING_EDIT)
{
}

EditLevelController::~EditLevelController()
{
	SAFE_DELETE(cursorRenderer_);
	SAFE_DELETE(cursorCollision_);

	std::list<Ring*>::iterator iter = ringList_.begin();
	std::list<Ring*>::iterator endIter = ringList_.end();

	while (iter != endIter)
	{
		delete* iter;
		*iter = nullptr;
		iter++;
	}
	ringList_.clear();

	std::list<Monitor*>::iterator monitorIter = monitorList_.begin();
	std::list<Monitor*>::iterator monitorEndIter = monitorList_.end();

	while (monitorIter != monitorEndIter)
	{
		delete *monitorIter;
		*monitorIter = nullptr;
		monitorIter++;
	}
	monitorList_.clear();
}

void EditLevelController::Start()
{
	SetRenderOrder(10000);
	cursorRenderer_ = new RSRenderer(this);
	cursorRenderer_->SetImage("cursor.bmp");

	cursorCollision_ = new RSCollision(this, eCollisionType::CIRCLE, static_cast<int>(eCollisionCheckGroup::CURSOR));
	cursorCollision_->SetRadius(3.f);
	level_->PushCollision(cursorCollision_);

	// 초기 위치 설정
	level_->SetCamPos({ 2880.0f, 1096.0f });
	level_->SetCamMovement(level_->GetCamSize().halffloat4() * -1);

	// 파일 로드
	LoadFile();

	if (false == RSInput::GetInstance().IsKeyAvailable("Up"))
	{
		RSInput::GetInstance().CreateKey("Up", VK_UP);
	}
	if (false == RSInput::GetInstance().IsKeyAvailable("Down"))
	{
		RSInput::GetInstance().CreateKey("Down", VK_DOWN);
	}
	if (false == RSInput::GetInstance().IsKeyAvailable("Left"))
	{
		RSInput::GetInstance().CreateKey("Left", VK_LEFT);
	}
	if (false == RSInput::GetInstance().IsKeyAvailable("Right"))
	{
		RSInput::GetInstance().CreateKey("Right", VK_RIGHT);
	}
	if (false == RSInput::GetInstance().IsKeyAvailable("W"))
	{
		RSInput::GetInstance().CreateKey("W", 'W');
	}
	if (false == RSInput::GetInstance().IsKeyAvailable("S"))
	{
		RSInput::GetInstance().CreateKey("S", 'S');
	}
	if (false == RSInput::GetInstance().IsKeyAvailable("A"))
	{
		RSInput::GetInstance().CreateKey("A", 'A');
	}
	if (false == RSInput::GetInstance().IsKeyAvailable("D"))
	{
		RSInput::GetInstance().CreateKey("D", 'D');
	}
	if (false == RSInput::GetInstance().IsKeyAvailable("ClickLeft"))
	{
		RSInput::GetInstance().CreateKey("ClickLeft", VK_LBUTTON);
	}
	if (false == RSInput::GetInstance().IsKeyAvailable("ClickRight"))
	{
		RSInput::GetInstance().CreateKey("ClickRight", VK_RBUTTON);
	}
	if (false == RSInput::GetInstance().IsKeyAvailable("ClickMiddle"))
	{
		RSInput::GetInstance().CreateKey("ClickMiddle", VK_MBUTTON);
	}
	if (false == RSInput::GetInstance().IsKeyAvailable("Save"))
	{
		RSInput::GetInstance().CreateKey("Save", 'Q');
	}
	if (false == RSInput::GetInstance().IsKeyAvailable("1"))
	{
		RSInput::GetInstance().CreateKey("1", '1');
	}
	if (false == RSInput::GetInstance().IsKeyAvailable("2"))
	{
		RSInput::GetInstance().CreateKey("2", '2');
	}
	if (false == RSInput::GetInstance().IsKeyAvailable("3"))
	{
		RSInput::GetInstance().CreateKey("3", '3');
	}
	if (false == RSInput::GetInstance().IsKeyAvailable("4"))
	{
		RSInput::GetInstance().CreateKey("4", '4');
	}
	if (false == RSInput::GetInstance().IsKeyAvailable("5"))
	{
		RSInput::GetInstance().CreateKey("5", '5');
	}
	if (false == RSInput::GetInstance().IsKeyAvailable("6"))
	{
		RSInput::GetInstance().CreateKey("6", '6');
	}
	if (false == RSInput::GetInstance().IsKeyAvailable("7"))
	{
		RSInput::GetInstance().CreateKey("7", '7');
	}
}

void EditLevelController::UpdateBefore()
{
	ringUpdateBefore();
	monitorUpdateBefore();
}

void EditLevelController::Update()
{
	float deltaTime = RSTimer::GetInstance().GetDeltaTime();
	const float speed = 600.f;
	float movement = speed * deltaTime;

	if (true == RSInput::GetInstance().IsKeyPress("6"))
	{
		mode_ = eEditMode::RING_EDIT;
		return;
	}
	else if (true == RSInput::GetInstance().IsKeyPress("7"))
	{
		mode_ = eEditMode::MONITOR_EDIT;
		return;
	}

	// 저장
	if (true == RSInput::GetInstance().IsKeyDown("Save"))
	{
		SaveFile();
	}

	// W S A D
	if (true == RSInput::GetInstance().IsKeyPress("W"))
	{
		level_->SetCamMovement({ 0.0f, -movement });
	}
	else if (true == RSInput::GetInstance().IsKeyPress("S"))
	{
		level_->SetCamMovement({ 0.0f, movement });
	}

	if (true == RSInput::GetInstance().IsKeyPress("A"))
	{
		level_->SetCamMovement({ -movement, 0.0f });
	}
	else if (true == RSInput::GetInstance().IsKeyPress("D"))
	{
		level_->SetCamMovement({ movement, 0.0f });
	}

	switch (mode_)
	{
	case EditLevelController::eEditMode::RING_EDIT:
		ringUpdate();
		break;
	case EditLevelController::eEditMode::MONITOR_EDIT:
		monitorUpdate();
		break;
	default:
		break;
	}
}

void EditLevelController::UpdateAfter()
{
	ringUpdateAfter();
	monitorUpdateAfter();
}

void EditLevelController::Render()
{
	monitorRender();
	ringRender();

	cursorRenderer_->Render();

	RSImage* backBuffer = RSResourceManager::GetInstance().GetBackBuffer();
	std::stringstream ss;
	ss << "Location : " << location_.x << ", " << location_.y;
	TextOutA(backBuffer->GetDC(), 5, 5, ss.str().c_str(), static_cast<int>(ss.str().size()));
	//ss.clear();
	ss.str("");

	switch (mode_)
	{
	case EditLevelController::eEditMode::RING_EDIT:
		ss << "Edit Mode : Ring";
		break;
	case EditLevelController::eEditMode::MONITOR_EDIT:
		ss << "Edit Mode : Monitor";
		break;
	default:
		break;
	}

	TextOutA(backBuffer->GetDC(), 5, 25, ss.str().c_str(), static_cast<int>(ss.str().size()));
}

void EditLevelController::Cleanup()
{
}

void EditLevelController::SaveFile()
{
	char buffer[256]{ 0, };
	GetCurrentDirectoryA(256, buffer);
	std::string path = buffer;
	if (-1 == path.find("Build"))
	{
		path += "\\..\\Build";
	}

	path += "\\";
	path += "sonic.dat";

	FILE* file = nullptr;

	if (0 != fopen_s(&file, path.c_str(), "wb"))
	{
		RSDebug::MsgBoxError("File open failed");
		return;
	}

	// 링 저장
	int listSize = static_cast<int>(ringList_.size());
	fwrite(&listSize, sizeof(int), 1, file);

	std::list<Ring*>::iterator iter = ringList_.begin();
	std::list<Ring*>::iterator endIter = ringList_.end();

	while (iter != endIter)
	{
		Ring* currentRing = *iter;

		float4 currentLocation = currentRing->GetLocation();
		int x = currentLocation.ix();
		int y = currentLocation.iy();

		fwrite(&x, sizeof(int), 1, file);
		fwrite(&y, sizeof(int), 1, file);

		iter++;
	}

	// 모니터 저장
	listSize = static_cast<int>(monitorList_.size());
	fwrite(&listSize, sizeof(int), 1, file);

	std::list<Monitor*>::iterator monitorIter = monitorList_.begin();
	std::list<Monitor*>::iterator monitorEndIter = monitorList_.end();

	while (monitorIter != monitorEndIter)
	{
		Monitor* currentMonitor = *monitorIter;

		float4 currentLocation = currentMonitor->GetLocation();
		int x = currentLocation.ix();
		int y = currentLocation.iy();
		int type = static_cast<int>(currentMonitor->monitorType_);

		fwrite(&x, sizeof(int), 1, file);
		fwrite(&y, sizeof(int), 1, file);
		fwrite(&type, sizeof(int), 1, file);

		monitorIter++;
	}


	fclose(file);

	RSDebug::MsgBoxInfo("Save successful");
}

void EditLevelController::LoadFile()
{
	char buffer[256]{ 0, };
	GetCurrentDirectoryA(256, buffer);
	std::string path = buffer;
	if (-1 == path.find("Build"))
	{
		path += "\\..\\Build";
	}

	path += "\\";
	path += "sonic.dat";

	FILE* file = nullptr;

	if (0 != fopen_s(&file, path.c_str(), "rb"))
	{
		fopen_s(&file, path.c_str(), "wb");
		if (file != nullptr)
		{
			fclose(file);
		}
		return;
	}

	int listSize = 0;
	fread(&listSize, sizeof(int), 1, file);

	for (int i = 0; i < listSize; ++i)
	{
		int x = 0;
		int y = 0;

		fread(&x, sizeof(int), 1, file);
		fread(&y, sizeof(int), 1, file);

		Ring* newRing = new Ring();
		newRing->SetLocation({ static_cast<float>(x), static_cast<float>(y) });
		newRing->SetLevel(level_);
		newRing->Start();
		newRing->state_ = eRingState::EDIT;
		ringList_.push_front(newRing);
	}

	listSize = 0;
	fread(&listSize, sizeof(int), 1, file);
	for (int i = 0; i < listSize; ++i)
	{
		int x = 0;
		int y = 0;
		int type = 0;

		fread(&x, sizeof(int), 1, file);
		fread(&y, sizeof(int), 1, file);
		fread(&type, sizeof(int), 1, file);
		Monitor* newMonitor = new Monitor();
		newMonitor->SetLevel(level_);
		newMonitor->SetLocation({ static_cast<float>(x), static_cast<float>(y) });
		newMonitor->Start();
		newMonitor->SetMonitorType(static_cast<eMonitor>(type));
		newMonitor->state_ = Monitor::eMonitorState::EDIT;
		monitorList_.push_front(newMonitor);
	}

	fclose(file);
}

void EditLevelController::ringUpdateBefore()
{
	std::list<Ring*>::iterator iter = ringList_.begin();
	std::list<Ring*>::iterator endIter = ringList_.end();

	while (iter != endIter)
	{
		(*iter)->UpdateBefore();
		iter++;
	}
}

void EditLevelController::ringUpdate()
{
	// 위 아래 왼쪽 오른쪽 방향키
	if (nullptr != focusedRing_)
	{
		if (true == RSInput::GetInstance().IsKeyDown("Up"))
		{
			focusedRing_->SetMovement(float4::UP);
			SetMovement(float4::UP);
		}
		else if (true == RSInput::GetInstance().IsKeyDown("Down"))
		{
			focusedRing_->SetMovement(float4::DOWN);
			SetMovement(float4::DOWN);
		}

		if (true == RSInput::GetInstance().IsKeyDown("Left"))
		{
			focusedRing_->SetMovement(float4::LEFT);
			SetMovement(float4::LEFT);
		}
		else if (true == RSInput::GetInstance().IsKeyDown("Right"))
		{
			focusedRing_->SetMovement(float4::RIGHT);
			SetMovement(float4::RIGHT);
		}
	}

	// 마우스 눌렀을 때
	if (true == RSInput::GetInstance().IsKeyDown("ClickLeft"))
	{
		POINT mousePoint;
		GetCursorPos(&mousePoint);
		POINT clientPosition = mousePoint;
		ScreenToClient(RSWindow::GetInstance().GetHandle(), &clientPosition);

		float4 windowClientSize = RSWindow::GetInstance().GetClientSize();
		float4 renderingResolution = RSWindow::GetInstance().GetViewportSize();

		// 클라이언트 영역 밖을 클릭했을 경우 취소한다.
		if (clientPosition.x < 0 || clientPosition.x > windowClientSize.ix()
			|| clientPosition.y < 0 || clientPosition.y > windowClientSize.iy())
		{
			return;
		}

		clientPosition.x = static_cast<LONG>((static_cast<float>(clientPosition.x) / windowClientSize.x) * renderingResolution.x);
		clientPosition.y = static_cast<LONG>((static_cast<float>(clientPosition.y) / windowClientSize.y) * renderingResolution.y);

		float4 inGameLocation = level_->GetCamPos();
		inGameLocation = inGameLocation - level_->GetCamSize().halffloat4();

		inGameLocation.x = inGameLocation.x + static_cast<float>(clientPosition.x);
		inGameLocation.y = inGameLocation.y + static_cast<float>(clientPosition.y);

		SetLocation(inGameLocation);

		Ring* newRing = new Ring();
		newRing->SetLocation(inGameLocation);
		newRing->SetLevel(level_);
		newRing->Start();
		newRing->state_ = eRingState::EDIT;
		ringList_.push_front(newRing);

		SetLocation(inGameLocation);
		focusedRing_ = newRing;
	}
	else if (true == RSInput::GetInstance().IsKeyPress("ClickRight"))
	{
		focusedRing_ = nullptr;
		POINT mousePoint;
		GetCursorPos(&mousePoint);
		POINT clientPosition = mousePoint;
		ScreenToClient(RSWindow::GetInstance().GetHandle(), &clientPosition);

		float4 windowClientSize = RSWindow::GetInstance().GetClientSize();
		float4 renderingResolution = RSWindow::GetInstance().GetViewportSize();

		// 클라이언트 영역 밖을 클릭했을 경우 취소한다.
		if (clientPosition.x < 0 || clientPosition.x > windowClientSize.ix()
			|| clientPosition.y < 0 || clientPosition.y > windowClientSize.iy())
		{
			return;
		}

		clientPosition.x = static_cast<LONG>((static_cast<float>(clientPosition.x) / windowClientSize.x) * renderingResolution.x);
		clientPosition.y = static_cast<LONG>((static_cast<float>(clientPosition.y) / windowClientSize.y) * renderingResolution.y);

		float4 inGameLocation = level_->GetCamPos();
		inGameLocation = inGameLocation - level_->GetCamSize().halffloat4();

		inGameLocation.x = inGameLocation.x + static_cast<float>(clientPosition.x);
		inGameLocation.y = inGameLocation.y + static_cast<float>(clientPosition.y);

		SetLocation(inGameLocation);

		std::list<Ring*>::iterator iter = ringList_.begin();
		std::list<Ring*>::iterator endIter = ringList_.end();

		while (iter != endIter)
		{
			Ring* currentRing = *iter;
			if (nullptr != currentRing->collision_->GetCollisionGroupCheckOne(static_cast<int>(eCollisionCheckGroup::CURSOR)))
			{
				iter = ringList_.erase(iter);
				break;
			}

			iter++;
		}
	}
	else if (true == RSInput::GetInstance().IsKeyDown("ClickMiddle"))
	{
		POINT mousePoint;
		GetCursorPos(&mousePoint);
		POINT clientPosition = mousePoint;
		ScreenToClient(RSWindow::GetInstance().GetHandle(), &clientPosition);

		float4 windowClientSize = RSWindow::GetInstance().GetClientSize();
		float4 renderingResolution = RSWindow::GetInstance().GetViewportSize();

		// 클라이언트 영역 밖을 클릭했을 경우 취소한다.
		if (clientPosition.x < 0 || clientPosition.x > windowClientSize.ix()
			|| clientPosition.y < 0 || clientPosition.y > windowClientSize.iy())
		{
			return;
		}

		clientPosition.x = static_cast<LONG>((static_cast<float>(clientPosition.x) / windowClientSize.x) * renderingResolution.x);
		clientPosition.y = static_cast<LONG>((static_cast<float>(clientPosition.y) / windowClientSize.y) * renderingResolution.y);

		float4 inGameLocation = level_->GetCamPos();
		inGameLocation = inGameLocation - level_->GetCamSize().halffloat4();

		inGameLocation.x = inGameLocation.x + static_cast<float>(clientPosition.x);
		inGameLocation.y = inGameLocation.y + static_cast<float>(clientPosition.y);

		SetLocation(inGameLocation);

		std::list<Ring*>::iterator iter = ringList_.begin();
		std::list<Ring*>::iterator endIter = ringList_.end();

		while (iter != endIter)
		{
			Ring* currentRing = *iter;
			if (nullptr != currentRing->collision_->GetCollisionGroupCheckOne(static_cast<int>(eCollisionCheckGroup::CURSOR)))
			{
				focusedRing_ = currentRing;
				SetLocation(focusedRing_->GetLocation());
				break;
			}

			iter++;
		}
	}

	std::list<Ring*>::iterator iter = ringList_.begin();
	std::list<Ring*>::iterator endIter = ringList_.end();

	while (iter != endIter)
	{
		(*iter)->Update();
		iter++;
	}
}

void EditLevelController::ringUpdateAfter()
{
	std::list<Ring*>::iterator iter = ringList_.begin();
	std::list<Ring*>::iterator endIter = ringList_.end();

	while (iter != endIter)
	{
		(*iter)->UpdateAfter();
		iter++;
	}
}

void EditLevelController::ringRender()
{
	std::list<Ring*>::iterator iter = ringList_.begin();
	std::list<Ring*>::iterator endIter = ringList_.end();

	while (iter != endIter)
	{
		(*iter)->Render();
		iter++;
	}
}

void EditLevelController::monitorUpdateBefore()
{
	std::list<Monitor*>::iterator iter = monitorList_.begin();
	std::list<Monitor*>::iterator endIter = monitorList_.end();

	while (iter != endIter)
	{
		(*iter)->UpdateBefore();
		iter++;
	}
}

void EditLevelController::monitorUpdate()
{
	// 위 아래 왼쪽 오른쪽 방향키
	if (nullptr != focusedMonitor_)
	{
		if (RSInput::GetInstance().IsKeyDown("1"))
		{
			focusedMonitor_->SetMonitorType(eMonitor::RING);
		}
		else if (RSInput::GetInstance().IsKeyDown("2"))
		{
			focusedMonitor_->SetMonitorType(eMonitor::ELECTRIC);
		}
		else if (RSInput::GetInstance().IsKeyDown("3"))
		{
			focusedMonitor_->SetMonitorType(eMonitor::FIRE);
		}
		else if (RSInput::GetInstance().IsKeyDown("4"))
		{
			focusedMonitor_->SetMonitorType(eMonitor::BUBBLE);
		}
		else if (RSInput::GetInstance().IsKeyDown("5"))
		{
			focusedMonitor_->SetMonitorType(eMonitor::LIFE);
		}

		if (true == RSInput::GetInstance().IsKeyDown("Up"))
		{
			focusedMonitor_->SetMovement(float4::UP);
			SetMovement(float4::UP);
		}
		else if (true == RSInput::GetInstance().IsKeyDown("Down"))
		{
			focusedMonitor_->SetMovement(float4::DOWN);
			SetMovement(float4::DOWN);
		}

		if (true == RSInput::GetInstance().IsKeyDown("Left"))
		{
			focusedMonitor_->SetMovement(float4::LEFT);
			SetMovement(float4::LEFT);
		}
		else if (true == RSInput::GetInstance().IsKeyDown("Right"))
		{
			focusedMonitor_->SetMovement(float4::RIGHT);
			SetMovement(float4::RIGHT);
		}
	}

	// 마우스 눌렀을 때
	if (true == RSInput::GetInstance().IsKeyDown("ClickLeft"))
	{
		POINT mousePoint;
		GetCursorPos(&mousePoint);
		POINT clientPosition = mousePoint;
		ScreenToClient(RSWindow::GetInstance().GetHandle(), &clientPosition);

		float4 windowClientSize = RSWindow::GetInstance().GetClientSize();
		float4 renderingResolution = RSWindow::GetInstance().GetViewportSize();

		// 클라이언트 영역 밖을 클릭했을 경우 취소한다.
		if (clientPosition.x < 0 || clientPosition.x > windowClientSize.ix()
			|| clientPosition.y < 0 || clientPosition.y > windowClientSize.iy())
		{
			return;
		}

		clientPosition.x = static_cast<LONG>((static_cast<float>(clientPosition.x) / windowClientSize.x) * renderingResolution.x);
		clientPosition.y = static_cast<LONG>((static_cast<float>(clientPosition.y) / windowClientSize.y) * renderingResolution.y);

		float4 inGameLocation = level_->GetCamPos();
		inGameLocation = inGameLocation - level_->GetCamSize().halffloat4();

		inGameLocation.x = inGameLocation.x + static_cast<float>(clientPosition.x);
		inGameLocation.y = inGameLocation.y + static_cast<float>(clientPosition.y);

		SetLocation(inGameLocation);

		Monitor* newMonitor = new Monitor();
		newMonitor->SetLocation(inGameLocation);
		newMonitor->SetLevel(level_);
		newMonitor->Start();
		newMonitor->state_ = Monitor::eMonitorState::EDIT;
		monitorList_.push_front(newMonitor);

		SetLocation(inGameLocation);
		focusedMonitor_ = newMonitor;
	}
	else if (true == RSInput::GetInstance().IsKeyPress("ClickRight"))
	{
		focusedRing_ = nullptr;
		POINT mousePoint;
		GetCursorPos(&mousePoint);
		POINT clientPosition = mousePoint;
		ScreenToClient(RSWindow::GetInstance().GetHandle(), &clientPosition);

		float4 windowClientSize = RSWindow::GetInstance().GetClientSize();
		float4 renderingResolution = RSWindow::GetInstance().GetViewportSize();

		// 클라이언트 영역 밖을 클릭했을 경우 취소한다.
		if (clientPosition.x < 0 || clientPosition.x > windowClientSize.ix()
			|| clientPosition.y < 0 || clientPosition.y > windowClientSize.iy())
		{
			return;
		}

		clientPosition.x = static_cast<LONG>((static_cast<float>(clientPosition.x) / windowClientSize.x) * renderingResolution.x);
		clientPosition.y = static_cast<LONG>((static_cast<float>(clientPosition.y) / windowClientSize.y) * renderingResolution.y);

		float4 inGameLocation = level_->GetCamPos();
		inGameLocation = inGameLocation - level_->GetCamSize().halffloat4();

		inGameLocation.x = inGameLocation.x + static_cast<float>(clientPosition.x);
		inGameLocation.y = inGameLocation.y + static_cast<float>(clientPosition.y);

		SetLocation(inGameLocation);

		std::list<Monitor*>::iterator iter = monitorList_.begin();
		std::list<Monitor*>::iterator endIter = monitorList_.end();

		while (iter != endIter)
		{
			Monitor* currentMonitor = *iter;
			if (nullptr != currentMonitor->collision_->GetCollisionGroupCheckOne(static_cast<int>(eCollisionCheckGroup::CURSOR)))
			{
				iter = monitorList_.erase(iter);
				break;
			}

			iter++;
		}
	}
	else if (true == RSInput::GetInstance().IsKeyDown("ClickMiddle"))
	{
		POINT mousePoint;
		GetCursorPos(&mousePoint);
		POINT clientPosition = mousePoint;
		ScreenToClient(RSWindow::GetInstance().GetHandle(), &clientPosition);

		float4 windowClientSize = RSWindow::GetInstance().GetClientSize();
		float4 renderingResolution = RSWindow::GetInstance().GetViewportSize();

		// 클라이언트 영역 밖을 클릭했을 경우 취소한다.
		if (clientPosition.x < 0 || clientPosition.x > windowClientSize.ix()
			|| clientPosition.y < 0 || clientPosition.y > windowClientSize.iy())
		{
			return;
		}

		clientPosition.x = static_cast<LONG>((static_cast<float>(clientPosition.x) / windowClientSize.x) * renderingResolution.x);
		clientPosition.y = static_cast<LONG>((static_cast<float>(clientPosition.y) / windowClientSize.y) * renderingResolution.y);

		float4 inGameLocation = level_->GetCamPos();
		inGameLocation = inGameLocation - level_->GetCamSize().halffloat4();

		inGameLocation.x = inGameLocation.x + static_cast<float>(clientPosition.x);
		inGameLocation.y = inGameLocation.y + static_cast<float>(clientPosition.y);

		SetLocation(inGameLocation);

		std::list<Monitor*>::iterator iter = monitorList_.begin();
		std::list<Monitor*>::iterator endIter = monitorList_.end();

		while (iter != endIter)
		{
			Monitor* currentMonitor = *iter;
			if (nullptr != currentMonitor->collision_->GetCollisionGroupCheckOne(static_cast<int>(eCollisionCheckGroup::CURSOR)))
			{
				focusedMonitor_ = currentMonitor;
				SetLocation(focusedMonitor_->GetLocation());
				break;
			}

			iter++;
		}
	}




	std::list<Monitor*>::iterator iter = monitorList_.begin();
	std::list<Monitor*>::iterator endIter = monitorList_.end();

	while (iter != endIter)
	{
		(*iter)->Update();
		iter++;
	}
}

void EditLevelController::monitorUpdateAfter()
{
	std::list<Monitor*>::iterator iter = monitorList_.begin();
	std::list<Monitor*>::iterator endIter = monitorList_.end();

	while (iter != endIter)
	{
		(*iter)->UpdateAfter();
		iter++;
	}
}

void EditLevelController::monitorRender()
{
	std::list<Monitor*>::iterator iter = monitorList_.begin();
	std::list<Monitor*>::iterator endIter = monitorList_.end();

	while (iter != endIter)
	{
		(*iter)->Render();
		iter++;
	}
}
