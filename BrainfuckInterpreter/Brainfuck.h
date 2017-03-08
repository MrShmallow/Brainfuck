#pragma once

#include <array>
#include <iostream>
#include <fstream>
#include <conio.h>
#include <string>

#define MEMSIZE 65536
#define BYTES_PER_LINE 8
#define VALID_OPS "><+-,.[]!"

typedef unsigned char byte;

class Brainfuck
{
public:
	Brainfuck();
	Brainfuck(std::istream& in, std::ostream& out);
	void interpretString(std::string code);
	void reset();
	unsigned short getPtr();
	byte getFromMem(unsigned short index);
	bool getDbg();
	void setDbg(bool dbg = true);
	bool getStepMode();
	void setStepMode(bool step = true);

private:
	unsigned short _ptr, _maxPtr;
	bool _dbg, _step, _cancel;
	std::array<byte, MEMSIZE> _mem;
	std::istream& _in;
	std::ostream& _out;

	void breakpoint(char lastCode);
};