#include "Brainfuck.h"

Brainfuck::Brainfuck(std::istream & in, std::ostream & out) : _mem({ 0 }), _ptr(0), _in(in), _out(out), _maxPtr(0), _dbg(false), _step(false), _cancel(false)
{
}

Brainfuck::Brainfuck() : Brainfuck(std::cin, std::cout)
{
}

void Brainfuck::interpretString(std::string code)
{
	try
	{
		for (unsigned int i = 0; i < code.length(); i++)
		{
			if (_step && std::string(VALID_OPS).find(code[i - 1]) != std::string::npos)
			{
				_step = false;
				breakpoint(code[i - 1]);
			}
			if (_cancel)
			{
				_cancel = false;
				break;
			}

			switch (code.at(i))
			{
			case '>':
				if (++_ptr >= MEMSIZE)
				{
					_out << "Interpretation error: pointer exceeds memory size (" << MEMSIZE << ")" << std::endl;
					return;
				}
				if (_ptr > _maxPtr)
					_maxPtr = _ptr;
				break;
			case '<':
				if (_ptr-- == 0)
				{
					_out << "Interpretation error: pointer can't be negative" << std::endl;
					return;
				}
				break;
			case '+':
				_mem.at(_ptr)++;
				break;
			case '-':
				_mem.at(_ptr)--;
				break;
			case ',':
				if (&_in == &std::cin)
					_mem.at(_ptr) = _getch();
				else
				{
					_mem.at(_ptr) = _in.get();
					_in.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
				}
				break;
			case '.':
				_out << _mem.at(_ptr);
				break;
			case '[':
				if (_mem.at(_ptr) == 0)
				{
					int bracketsCount = 1;
					while (bracketsCount != 0)
					{
						if (code.at(++i) == '[')
							bracketsCount++;
						else if (code.at(i) == ']')
							bracketsCount--;
					}
				}
				break;
			case ']':
				if (_mem.at(_ptr) != 0)
				{
					int bracketsCount = -1;
					while (bracketsCount != 0)
					{
						if (code.at(--i) == '[')
							bracketsCount++;
						else if (code.at(i) == ']')
							bracketsCount--;
					}
				}
				break;

			case '!':
				if (_dbg)
				{
					if (i != 0)
						breakpoint(code[i-1]);
					else
						breakpoint(0);
				}
				break;
			}
		}

		_step = false;
		_cancel = false;
	}
	catch (std::out_of_range& e)
	{
		_out << "Interpretation error: invalid brackets" << std::endl;
		reset();
	}
}

void Brainfuck::reset()
{
	_ptr = 0;
	_mem = { 0 };
	_maxPtr = 0;
	_dbg = false;
	_step = false;
	_cancel = false;
}

unsigned short Brainfuck::getPtr()
{
	return _ptr;
}

byte Brainfuck::getFromMem(unsigned short index)
{
	return _mem.at(index);
}

bool Brainfuck::getDbg()
{
	return _dbg;
}

void Brainfuck::setDbg(bool dbg)
{
	_dbg = dbg;
}

bool Brainfuck::getStepMode()
{
	return _step;
}

void Brainfuck::setStepMode(bool step)
{
	_step = step;
}

void Brainfuck::breakpoint(char lastCode)
{
	std::string cmd;

	std::cout << std::endl << "***********************************" << std::endl << std::endl;
	std::cout << "BREAKPOINT" << std::endl << std::endl;
	if (std::string(VALID_OPS).find(lastCode) != std::string::npos)
		std::cout << "Last command: " << lastCode << std::endl << std::endl;
	std::cout << "Pointer: " << _ptr << std::endl << std::endl;
	std::cout << "Memory: " << std::endl << std::endl;
	for (unsigned int i = 0; i <= _maxPtr; i++)
	{
		if (i != 0 && i % BYTES_PER_LINE == 0)
			std::cout << std::endl << std::endl;
		std::cout << "[" << i << "]:" << static_cast<unsigned int>(_mem[i]) << "	";
	}
	if (_maxPtr == 0 || _maxPtr % BYTES_PER_LINE != 0)
		std::cout << std::endl << std::endl;

	std::cout << "Press S for next step, C to cancel all following commands, or Enter to continue" << std::endl << std::endl;
	std::cout << "***********************************" << std::endl << std::endl;
	
	char ch;
	bool cont = true;
	do
	{
		ch = _getch();
		std::cin.clear();
		switch (ch)
		{
		case 's': case 'S':
			_step = true;
			cont = false;
			break;
		case 'c': case 'C':
			_cancel = true;
			cont = false;
			break;
		case '\r': case '\n':
			cont = false;
			break;
		}
	} while (cont);
}
