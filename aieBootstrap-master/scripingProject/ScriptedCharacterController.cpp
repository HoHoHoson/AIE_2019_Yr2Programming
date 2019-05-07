#include "ScriptedCharacterController.h"

#include <fstream>
#include <sstream>
#include <iterator>
#include <iostream>

#include <algorithm>

#include "Character.h"
ScriptedCharacterController::ScriptedCharacterController(Character& ControlledCharacter)
	: m_controlledCharacter(&ControlledCharacter)
	, m_commandLocation(0)
	, m_isWaiting(false)
	, m_remainingWaitTime(0.0f)
{
	setupCommands();
}

void ScriptedCharacterController::update(float deltaTime)
{
	if (m_isScriptLoaded && !m_isWaiting)
	{
		const CommandData& nextCommand = m_commandsToExectute[m_commandLocation];

		auto it = m_AllowedCommands.find(nextCommand[0]);

		if (it != m_AllowedCommands.end())
		{
			it->second(nextCommand);
		}
		else
		{
			std::cout << "Invalid Command, '" << nextCommand[0] << "'\n";
		}

		m_commandLocation++;

		if (m_commandLocation >= m_commandsToExectute.size())
			m_commandLocation = 0;
	}

	if (m_isWaiting)
	{
		m_remainingWaitTime -= deltaTime;
		
		if (m_remainingWaitTime <= 0.0f)
			m_isWaiting = false;
	}
}

void ScriptedCharacterController::loadScript(std::string script)
{
	m_isScriptLoaded = false;
	m_scriptPath = script;
	m_commandsToExectute.clear();

	std::ifstream scriptFile(script, std::ios::in);

	std::string text;
	while (scriptFile.good())
	{
		if (std::getline(scriptFile, text))
		{
			m_commandsToExectute.push_back(splitString(text));
		}
	}

	if (!m_commandsToExectute.empty())
	{
		m_isScriptLoaded = true;
		m_commandLocation = 0;
		m_isWaiting = false;
	}
}

void ScriptedCharacterController::reloadScript()
{
	loadScript(m_scriptPath);
}

void ScriptedCharacterController::setupCommands()
{
	m_AllowedCommands["SET_SPRITE"] = [&](const CommandData& data) { onSetSprite(data); };
	m_AllowedCommands["SET_STATE"] = [&](const CommandData& data) { onSetState(data); };
	m_AllowedCommands["WAIT"] = [&](const CommandData& data) { onWait(data); };
	m_AllowedCommands["SPEAK"] = [&](const CommandData& data) { onSpeak(data); };
}

void ScriptedCharacterController::onSetSprite(const CommandData & data)
{
	assert(data.size() == 2);
	assert(
		  data[1] == "adventurer" ||
		  data[1] == "female" ||
		  data[1] == "player" ||
		  data[1] == "zombie" ||
		  data[1] == "soldier"
		  );

	m_controlledCharacter->setSprite(data[1]);
}

void ScriptedCharacterController::onSetState(const CommandData& data)
{
	assert(data.size() == 2);

	std::unordered_map<std::string, Character::State> states;
	states["idle"] = Character::IDLE;
	states["talking"] = Character::TALKING;
	states["face_back"] = Character::FACE_BACK;
	states["hurt"] = Character::HURT;

	assert(states.find(data[1]) != states.end());

	m_controlledCharacter->setState(states[data[1]]);
}

void ScriptedCharacterController::onWait(const CommandData& data)
{
	assert(data.size() == 2);

	float waitTime = (float)std::atof(data[1].c_str());
	assert(waitTime > 0.0f);
	
	m_isWaiting = true;
	m_remainingWaitTime = waitTime;
}

void ScriptedCharacterController::onSpeak(const CommandData& data)
{
	assert(data.size() >= 3);

	float speakTime = (float)std::atof(data[1].c_str());
	assert(speakTime > 0.0f);

	std::string toSpeak;
	for (unsigned int i = 2; i < data.size(); ++i)
		toSpeak.append(data[i] + " ");

	m_isWaiting = true;
	m_remainingWaitTime = speakTime;
	m_controlledCharacter->speak(toSpeak, speakTime);
}

std::vector<std::string> ScriptedCharacterController::splitString(std::string str)
{
	std::istringstream iss(str);
	return std::vector<std::string>(std::istream_iterator<std::string>(iss),
				std::istream_iterator<std::string>());
}
