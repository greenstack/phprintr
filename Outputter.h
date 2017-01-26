#pragma once
#include <string>
#include <sstream>
#include "Flags.h"

#ifdef _WIN32
#include <windows.h>
#endif

class Outputter
{
protected:
	bool inString;
	int columns;
public:
	Outputter();
	~Outputter();
	std::string fabricateWhitespace(int tabCount);
	void processCharacter(const char&, const Flags&, std::ostream&);
	void setConsoleTextColor(bool, std::ostream&);
};

