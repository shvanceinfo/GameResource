#include "Fsm.h"

FSM::FSM(std::string state, std::function<void()> onEnter)
{
	_currentState = state;
	_previousState = "";
	this->addState(state, onEnter);
}

FSM * FSM::create(std::string state, std::function<void()> onEnter)
{
	FSM* fsm = new FSM(state, onEnter);
	if ( fsm && fsm->init())
	{
		fsm->autorelease();
		return fsm;
	}
	else
	{
		CC_SAFE_DELETE(fsm);
		return nullptr;
	}
}

bool FSM::isContainState(std::string stateName)
{
	return _states.find(stateName) != _states.end();
}

void FSM::printState()
{
	cocos2d::log("FSM::printState:list of statesS");
	for (auto ptr = _states.begin(); ptr != _states.end(); ptr++)
	{
		cocos2d::log(ptr->c_str());
	}
}

void FSM::changeToState(std::string state)
{
	if (isContainState(state))
	{
		_previousState = _currentState;
		_currentState = state;
		cocos2d::log("FSM::changeToState:%s->%s", _previousState.c_str(), _currentState.c_str());
		if (_onEnter[state])
		{
			_onEnter[state]();
		}
		else
		{
			cocos2d::log("FSM::changeToState:no such state as %s, state unchanged", state.c_str());
		}
	}
}

FSM* FSM::addState(std::string state, std::function<void()> onEnter /* = nullptr */)
{
	if (""==state)
	{
		cocos2d::log("FSM::addState:sate can't be empty string!");
		return nullptr;
	}

	_states.insert(state);
	_onEnter.insert(std::pair<std::string, std::function<void()>>(state, onEnter));
	return this;
}

FSM * FSM::addEvent(std::string eventName, std::string from, std::string to)
{
	if ( ""==eventName)
	{
		cocos2d::log("FSM::addEvent:eventName can't be empty!");
		return nullptr;
	}

	if ( !isContainState(from))
	{
		cocos2d::log("FSM::addEvent: from state %s does not exit", from.c_str());
		return nullptr;
	}

	if ( !isContainState(to))
	{
		cocos2d::log("FSM::addEvent:to state %s does not exit", to.c_str());
		return nullptr;
	}

	std::unordered_map<std::string, std::string> &oneEvent = _events[eventName];
	oneEvent[from] = to;
	return this;
}

bool FSM::canDoEvent(std::string eventName)
{
	return _events[eventName].find(_currentState) != _events[eventName].end();
}

void FSM::doEvent(std::string eventName)
{
	if (canDoEvent(eventName))
	{
		cocos2d::log("FSM::doEvent:doing event %s", eventName.c_str());
		changeToState(_events[eventName][_currentState]);
	}
	else
	{
		cocos2d::log("FSM::doEvent: cannot do event %s", eventName.c_str());
	}
}

void FSM::setOnEnter(std::string state, std::function<void()> onEnter)
{
	if (isContainState(state))
	{
		_onEnter[state] = onEnter;
	}
	else
	{
		cocos2d::log("FSM::setOnEnter:no state named %s", state.c_str());
	}
}

bool FSM::init()
{
	this->addState("walking", [](){cocos2d::log("Enter walking"); })
		->addState("attacking", [](){cocos2d::log("Enter attacking"); })
		->addState("dead", [](){cocos2d::log("Enter dead"); })
		->addState("beingHit", [](){cocos2d::log("Enter beingHit"); });

	this->addEvent("walk", "idle", "walking")
		//->addEvent("walk", "attacking", "walking")
		->addEvent("attack", "idle", "attacking")
		->addEvent("attack", "walking", "attacking")
		->addEvent("die", "idle", "dead")
		->addEvent("die", "walking", "dead")
		->addEvent("die", "attacking", "dead")
		->addEvent("stop", "walking", "idle")
		->addEvent("stop", "attacking", "idle")
		//->addEvent("walk", "walking", "walking");
		->addEvent("walk", "walking", "walking")
		->addEvent("beHit", "idle", "beingHit")
		->addEvent("beHit", "walking", "beingHit")
		->addEvent("die", "beingHit", "dead")
		->addEvent("stop", "beingHit", "idle")
		->addEvent("stop", "idle", "idle");

	return true;
}