#pragma once
#include "Application.h"

class ProtoTracer
{
private:
	std::ofstream tracerStream;
public:
	ProtoTracer();
	~ProtoTracer();
	void WriteLogEntry(std::string entry);
};