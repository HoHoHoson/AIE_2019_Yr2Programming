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


std::vector<std::string> ScriptedCharacterController::splitString(std::string str)
{
	std::istringstream iss(str);
	return std::vector<std::string>(std::istream_iterator<std::string>(iss),
				std::istream_iterator<std::string>());
}
