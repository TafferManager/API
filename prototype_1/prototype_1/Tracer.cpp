#include "Tracer.h"

ProtoTracer::ProtoTracer()
{
	tracerStream.open("proto_1.log", std::ios::in | std::ios::app);
	tracerStream << "========================== New instance of proto_1 opened ==========================" << std::endl;
}

ProtoTracer::~ProtoTracer()
{
	WriteLogEntry("Going to close tracer file proto_1.log");
	tracerStream.close();
}

void ProtoTracer::WriteLogEntry(std::string entry)
{
	time_t rawtime;
	struct tm * timeinfo;
	char buffer[80];

	time(&rawtime);
	timeinfo = localtime(&rawtime);

	strftime(buffer, sizeof(buffer), "%d-%m-%Y %I:%M:%S", timeinfo);
	std::string timeStr(buffer);
	tracerStream << timeStr << " --- " << entry << std::endl;
}