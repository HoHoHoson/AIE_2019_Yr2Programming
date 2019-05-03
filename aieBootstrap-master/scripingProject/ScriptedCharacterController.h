#pragma once
#include <string>
#include <vector>
#include <functional>
#include <unordered_map>

class Character;

class ScriptedCharacterController
{
public:
	ScriptedCharacterController() = delete;
	ScriptedCharacterController(Character& ControlledCharacter);
	~ScriptedCharacterController() = default;
	
	void loadScript(std::string script);
	void reloadScript();
private:
	std::vector<std::string> splitString(std::string str);

	Character* m_controlledCharacter;
	bool m_isScriptLoaded;
	std::string m_scriptPath;
	unsigned int m_commandLocation;

	bool m_isWaiting;
	float m_remainingWaitTime;

	using CommandData = std::vector<std::string>;
	using CommandList = std::vector<CommandData>;
	CommandList m_commandsToExectute;

};
