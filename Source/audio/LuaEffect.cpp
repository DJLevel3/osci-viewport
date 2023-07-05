#include "LuaEffect.h"
#include "../lua/LuaParser.h"

Vector2 LuaEffect::apply(int index, Vector2 input, double value, double frequency, double sampleRate) {
	int fileIndex = audioProcessor.getCurrentFileIndex();
	if (fileIndex == -1) {
		return input;
	}
	std::shared_ptr<LuaParser> parser = audioProcessor.getCurrentFileParser()->getLua();
	if (parser != nullptr) {
		parser->setVariable("slider_" + name.toLowerCase(), value);
	}
	return input;
}