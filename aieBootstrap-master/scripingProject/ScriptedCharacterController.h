#pragma once
#include <string>
#include <vector>
#include <functional>
#include <unordered_map>

class Character;

using CommandData = std::vector<std::string>;
using CommandList = std::vector<CommandData>;

class ScriptedCharacterController
{
public:
	ScriptedCharacterController() = delete;
	ScriptedCharacterController(Character& ControlledCharacter);
	~ScriptedCharacterController() = default;
	
	void update(float deltaTime);

	void loadScript(std::string script);
	void reloadScript();

	void setupCommands();

	void onSetSprite(const CommandData& data);
	void onSetState(const CommandData& data);
	void onWait(const CommandData& data);
	void onSpeak(const CommandData& data);

private:
	std::vector<std::string> splitString(std::string str);

	Character* m_controlledCharacter;
	bool m_isScriptLoaded;
	std::string m_scriptPath;
	unsigned int m_commandLocation;

	bool m_isWaiting;
	float m_remainingWaitTime;

	CommandList m_commandsToExectute;

	using ScriptFunction = std::function<void(const CommandData&)>;
	std::unordered_map<std::string, ScriptFunction> m_AllowedCommands;
};
