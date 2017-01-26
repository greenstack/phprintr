#include "Outputter.h"



Outputter::Outputter()
{
	inString = false;
	columns = 0;
}


Outputter::~Outputter()
{
}

std::string Outputter::fabricateWhitespace(int tabCount) {
	std::string tabsBase = "  ";
	std::string whitespace = "";
	for (int i = 0; i < tabCount; i++) {
		whitespace += tabsBase;
	}
	return whitespace;
}

void Outputter::processCharacter(const char& current, const Flags& flags, std::ostream& stream)
{
	//This won't account for escaped quote characters.
	if (current == '"')
	{
		inString = !inString;
	}
	else if (current == '(')
	{
		columns++;
	}
	else if (current == '[')
	{
		setConsoleTextColor(flags.showColors, stream);
		stream << '\n' << fabricateWhitespace(columns);
	}
	else if (current == ')')
	{
		columns--;
		setConsoleTextColor(flags.showColors, stream);
		stream << '\n' << fabricateWhitespace(columns);
	}
	else if (current == '\n')
	{
		return;
	}
	stream << current;
}

void Outputter::setConsoleTextColor(bool doColor, std::ostream& ss)
{
	if (!doColor)
	{
		return;
	}
	const int COLOR_COUNT = 7;

	int colorIndex = columns % COLOR_COUNT;
#ifdef __unix
	const std::string colors[COLOR_COUNT] =
	{
		"\033[36;1m", //white
		"\033[34;1m", //blue
		"\033[36;1m", //cyan
		"\033[32;1m", //green
		"\033[33;1m", //yellow
		"\033[31;1m", //red
		"\033[35;1m", //magenta
	};
	ss << colors[colorIndex];
#elif defined(_WINDOWS_)
	// colors won't rotate. why? ...oh yeah.
	HANDLE hstdin = GetStdHandle(STD_INPUT_HANDLE);
	HANDLE hstdout = GetStdHandle(STD_OUTPUT_HANDLE);
	const int colors[COLOR_COUNT] =
	{
		0x08, //gray
		0x0F, //white
		0x0B, //aqua
		0x0A, //green
		0x0E, //yellow
		0x0C, //red
		0x0D, //purple
	};
	SetConsoleTextAttribute(hstdout, colors[colorIndex]);
#endif
}