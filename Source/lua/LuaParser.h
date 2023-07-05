#pragma once
#include "../shape/Vector2.h"
#include <JuceHeader.h>
#include "../shape/Shape.h"

struct lua_State;
class LuaParser {
public:
	LuaParser(juce::String script);
	~LuaParser();

	Vector2 draw();
	void setVariable(juce::String variableName, double value);

private:
	void parse();

	int functionRef = -1;
	long step = 1;
	lua_State* L;
	juce::String script;
	std::atomic<bool> updateVariables = false;
	juce::SpinLock variableLock;
	std::vector<juce::String> variableNames;
	std::vector<double> variables;
};