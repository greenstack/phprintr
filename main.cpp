#include <iostream>
#include <fstream>
#include <sys/stat.h>
#include "Outputter.h"

bool fileExists(const std::string& fileName);
void resetConsole();

int main(int argc, char** argv) 
{
#ifdef _WINDOWS_
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	HANDLE hstdout = GetStdHandle(STD_OUTPUT_HANDLE);
	GetConsoleScreenBufferInfo(hstdout, &csbi);
#endif

	const int MIN_ARG_COUNT = 2;
	const int COLOR_ARG_COUNT = 3;
	Flags flags;
	Outputter output;
	// Program setup
	// Make sure there's enough input parameters.
	if (argc < MIN_ARG_COUNT) 
	{
		std::cerr << "Please specify a valid file to read." << std::endl;
		return 1;
	}
	const std::string fileName = argv[1];
	// Make sure input file exists
	if (!fileExists(fileName)) 
	{
		std::cerr << "Couldn't find the file you wanted. sorry" << std::endl;
		return 2;
	}

	// Read the flags
	if (argc > MIN_ARG_COUNT) 
	{
		for (int i = MIN_ARG_COUNT; i < argc; i++)
		{
			std::string arg = std::string(argv[i]);
			if ( "-c" == arg || "--color" == arg) 
			{
				std::cout << argv[i];
				flags.showColors = true;
			}
			else if ("-f" == arg || "--file" == arg) 
			{
				std::cout << " " << arg;
				flags.writeToFile = true;
				flags.showColors = false; //We don't want gibberish in the output file
				// The file name should be just after the file flag
				i++;
				// If no file is specified... the program will just crash.
				if (i == argc) {
					std::cerr << "Please provide an output file name." << std::endl;
					return 3;
				}
				flags.outputFileName = argv[i];
				std::cout << " " << argv[i];
				break; //no other flahs matter at this point.
			}
		}
		std::cout << "\n";
	}
	// End setup
	// Program execution
	std::cout << "Printing contents of: " << fileName << '\n';
	std::stringstream ss;
	int columns = 0;
	char current;
	std::ifstream array_file(fileName, std::fstream::in);
	while (array_file >> std::noskipws >> current) 
	{
		if (flags.writeToFile)
			output.processCharacter(current, flags, ss);
		else
			output.processCharacter(current, flags, std::cout);
	}
	array_file.close();
	if (flags.writeToFile) 
	{
		std::ofstream output;
		output.open(flags.outputFileName);
		output << ss.str();
		output.close();
		std::cout << "The formatted array was written to " << flags.outputFileName;
	}
	else 
	{
		std::cout << ss.str() << std::endl;
		//system("pause");
	}
	// End execution
}

//From stack overflow: http://stackoverflow.com/a/6296808/4500719
//I have no idea how this works but it does so thanks
bool fileExists(const std::string& filename) 
{
	struct stat buf;
	return (stat(filename.c_str(), &buf) != -1);
}
