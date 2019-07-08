#include "SpelSyltPCH.h"

#include "sfml_libs.h"
#ifdef _DEBUG
#pragma comment(lib, "../External/RapidJson_Debug")
#else
#pragma comment(lib, "../External/RapidJson_Release")
#endif


#include "Application.h"

void LoggingSetup()
{
	/*
	Uncomment the log level you want
	Verbose		= Log all logs
	Log			= Log all logs except verbose
	Warning		= Log warnings and errors only
	Error		= Log errors only

	Note: Only the last uncommented log level will be regarded!
	*/
	SET_LOG_LEVEL_VERBOSE;
	//SET_LOG_LEVEL_LOG;
	//SET_LOG_LEVEL_WARNING;
	//SET_LOG_LEVEL_ERROR;

	/* Uncomment this line to output all logs withing the set log level to file */
	//LOG_TO_FILE;
}

int main()
{
	LoggingSetup();

	CApplication App;
	App.Initialize();
	while (App.Run());

	return 0;
}