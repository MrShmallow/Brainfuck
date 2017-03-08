#include "Brainfuck.h"

void interpret();
void runFile(int argc, char** argv);

int main(int argc, char** argv)
{
	if (argc < 2)
		interpret();
	else
		runFile(argc, argv);
	
	return 0;
}

void interpret()
{
	std::string code;
	Brainfuck bf;

	std::cout << "Brainfuck Interpreter and Debugger" << std::endl;
	std::cout << "Type 'help' for list of available commands" << std::endl << std::endl;

	do
	{
		std::cout << ">> ";
		getline(std::cin, code);
		std::string cmd;
		if (code == "?" || code == "h" || code == "help")
		{
			std::cout << "Available commands:" << std::endl;
			std::cout << "h	help			show available commands" << std::endl;
			std::cout << "p	pointer			get value of pointer" << std::endl;
			std::cout << "m	memory <index>		get value in index" << std::endl;
			std::cout << "d	debug			toggle debugging" << std::endl;
			std::cout << "r	reset			reset brainfuck" << std::endl;
			std::cout << "q	quit			quit brainfuck" << std::endl;
		}
		else if (code == "p" || code == "pointer")
		{
			std::cout << "pointer = " << bf.getPtr() << std::endl;
		}
		else if ((cmd = code.substr(0, code.find(' '))) == "m" || cmd == "memory")
		{
			unsigned short index = atoi(code.substr(code.find(' ') + 1).c_str());
			std::cout << "memory[" << index << "] = " << static_cast<unsigned short>(bf.getFromMem(index)) << std::endl;
		}
		else if (code == "d" || code == "debug")
		{
			bool prevVal = bf.getDbg();
			if (prevVal)
			{
				bf.setDbg(false);
				bf.setStepMode(false);
				std::cout << "Debugging is now disabled" << std::endl;
			}
			else
			{
				bf.setDbg(true);
				std::cout << "Debugging is now enabled" << std::endl;
			}
		}
		else if (code == "r" || code == "reset")
		{
			bf.reset();
			std::cout << "Brainfuck has restarted" << std::endl;
		}
		else if (code != "q" && code != "quit")
		{
			bf.interpretString(code);
			if (code != "")
				std::cout << std::endl;
		}
	} while (code != "q" && code != "quit");
}

void runFile(int argc, char** argv)
{
	std::string code;
	Brainfuck bf;

	{
		std::ifstream infile(argv[1], std::ios::in);
		if (!infile)
		{
			std::cout << "File couldn't be opened" << std::endl;
			return;
		}
		code = std::string((std::istreambuf_iterator<char>(infile)), std::istreambuf_iterator<char>());
	}

	if (argc > 2 && std::string(argv[2]) == "-d")
		bf.setDbg();

	bf.interpretString(code);
}