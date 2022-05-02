#pragma once

#include <map>
#include <string>

#include "RSDebug.h"
#include "RSNameHelper.h"
#include "RSTimer.h"

class StateInfo
{
public:
	std::string StateName;
	float StateTime;
};

class StateReturn
{
public:
	StateReturn()
		: NextStateName_()
		, DelayForNextState_(0.0f)
	{
	}
	StateReturn(const std::string& _nextStateName, float _delayForNextState = 0.0f)
		: NextStateName_(_nextStateName)
		, DelayForNextState_(_delayForNextState)
	{
	}

	StateReturn(const char* _nextStateName, float _delayForNextState = 0.0f)
		: NextStateName_(_nextStateName)
		, DelayForNextState_(_delayForNextState)
	{

	}

	~StateReturn()
	{
	}

public:
	std::string NextStateName_;
	float DelayForNextState_;
};


template<typename FSMType>
class RSGameFSM
{
public:
	class State;

public:
	RSGameFSM();
	~RSGameFSM();

	RSGameFSM(const RSGameFSM& _other) = delete;
	RSGameFSM(RSGameFSM&& _other) = delete;

	RSGameFSM& operator=(const RSGameFSM& _other) = delete;
	RSGameFSM& operator=(const RSGameFSM&& _other) = delete;

	void Update();

	bool CreateState(const std::string& _stateName, void(FSMType::* _startState)(StateInfo), StateReturn(FSMType::* _updateState)(StateInfo));
	void ChangeState(const std::string& _stateName);
	State* FindState(const std::string& _stateName)
	{
		typename std::map<std::string, State*>::iterator findIter = allState_.find(_stateName);
		typename std::map<std::string, State*>::iterator endIter = allState_.end();
		if (endIter == findIter)
		{
			return nullptr;
		}

		return nullptr;
	}

private:
	std::map<std::string, State*> allState_;
	State* currentState_;
	StateReturn nextState_;

};


template<typename FSMType>
class RSGameFSM<FSMType>::State
{
	friend RSGameFSM;
public:
	State(std::string _stateName, void(FSMType::* _startState)(StateInfo), StateReturn(FSMType::* _updateState)(StateInfo));
	~State()
	{
	}


private:
	void (FSMType::* startState_)(StateInfo _state);
	StateReturn(FSMType::* updateState_)(StateInfo _state);

	std::string stateName_;
	float stateTime_;
};

template<typename FSMType>
inline RSGameFSM<FSMType>::RSGameFSM()
	: currentState_(nullptr)
	, allState_()
{
}

template<typename FSMType>
inline RSGameFSM<FSMType>::~RSGameFSM()
{
}

template<typename FSMType>
inline void RSGameFSM<FSMType>::Update()
{
	float deltaTime = RSTimer::GetInstance().GetDeltaTime();
	if (nullptr == currentState_)
	{
		RSDebug::AssertFalse();
		return;
	}


	if (nextState_.DelayForNextState_ > 0.0f)
	{
		nextState_.DelayForNextState_ -= deltaTime;
	}
	else
	{
		if (nextState_.NextStateName_.size() > 0)
		{
			currentState_ = FindState(nextState_.NextStateName_);
			if (nullptr == currentState_)
			{
				RSDebug::AssertFalse();
				return;
			}

			currentState_->startState_();
		}
	}




	currentState_->updateState({ currentState_->stateName_, currentState_->stateTime_ });
}

template<typename FSMType>
inline bool RSGameFSM<FSMType>::CreateState(const std::string& _stateName, void(FSMType::* _startState)(StateInfo), StateReturn(FSMType::* _updateState)(StateInfo))
{
	if (nullptr != FindState(_stateName))
	{
		RSDebug::AssertFalse();
		return false;
	}

	State* newState = new State(_stateName, _startState, _updateState);

	allState_.insert(std::pair<std::string, RSGameFSM<FSMType>::State*>(_stateName, newState));

	return true;
}

template<typename FSMType>
inline void RSGameFSM<FSMType>::ChangeState(const std::string& _stateName)
{

}




template<typename FSMType>
RSGameFSM<FSMType>::State::State(std::string _stateName, void(FSMType::* _startState)(StateInfo), StateReturn(FSMType::* _updateState)(StateInfo))
	: stateName_(_stateName)
	, stateTime_(0.0f)
	, startState_(_startState)
	, updateState_(_updateState)
{
}


; // <- 이 세미콜론을 제거하면 뜨는 오류
// 심각도	코드	설명	프로젝트	파일	줄	비표시 오류(Suppression) 상태
// 오류(활성)	E2923	PCH 경고 : 헤더 중지가 파일 범위가 아닙니다.IntelliSense PCH 파일이 생성되지 않았습니다.