#pragma once

#include <BWAPI.h>
#include <ctime>
#include <Windows.h>
#include <sstream>
//#include <iostream>
#include <fstream>

using namespace BWAPI;

class LogHandler
{
private:

	std::string combinations[2];

	std::string logString[11];
	
	std::string currentFolder;
	int orderIndex;
	int stageIndex;
	std::string mapFileName;


	std::string dateFormat() const;


	// Create a folder for an experiment session
	void createFolder();

	/*
	* The stage-log.txt file holds offline data for the entire session like:
	* Name of the current folder
	* Filepaths the different maps
	* What stages are completed
	*/
	void createStageLog();
	void saveStageLog();
	void checkStage();
	void readFromStageLog();

public:
	LogHandler();
	~LogHandler();

	// Log data depending on the stage of the session
	void createMapLog(std::vector<std::string> commands);

	// Returns the expected map for the current stage
	std::string currentMap() const;
};

