#include "LogHandler.h"

std::string LogHandler::dateFormat() const
{
	time_t t = time(0);
	struct tm * now = localtime(&t);

	// Polish format
	now->tm_year += 1900;

	std::stringstream date;
	date << now->tm_year << "-" << now->tm_mday << "-" << now->tm_hour << "-" << now->tm_min;
	return date.str();
}

void LogHandler::createFolder()
{
	currentFolder = dateFormat();
	std::string completed = "experiment-logs/" + currentFolder;
	CreateDirectory(completed.c_str(), NULL);
}

void LogHandler::createStageLog()
{
	std::ofstream log;
	log.open("experiment-logs/stage-log.txt");

	if (orderIndex == 0)
	{
		orderIndex = 1;
	}
	else if (orderIndex == 1)
	{
		orderIndex = 0;
	}

	log << orderIndex << std::endl;									// Line 1
	log << currentFolder << std::endl;								// Line 2
	log << std::endl;

	log << "Trial:" << std::endl;									// Line 4
	log << combinations[orderIndex] << "1" << ".scm" << std::endl;	// Line 5
	log << std::endl;

	log << "Experimental with speech:" << std::endl;				// Line 7
	log << combinations[orderIndex] << "2" << ".scm" << std::endl;	// Line 8
	log << std::endl;

	log << "Experimental without speech:" << std::endl;				// Line 10
	log << combinations[orderIndex] << "3" << ".scm" << std::endl;	// Line 11

	log.close();
}

void LogHandler::saveStageLog()
{
	std::ofstream log;
	log.open("experiment-logs/stage-log.txt");

	for (size_t i = 0; i < 11; i++)
	{
		log << logString[i];
		log << std::endl;
	}

	log.close();
}

void LogHandler::checkStage()
{
	logString[stageIndex] += "X";
}

void LogHandler::readFromStageLog()
{
	// Stage 1: Trial with voice completed
	// Stage 2: Experimental with voice completed
	// Stage 3: Experimental without voice completed

	std::ifstream log("experiment-logs/stage-log.txt");
	if (log.is_open())
	{
		for (size_t i = 0; i < 11; i++)
		{
			std::getline(log, logString[i]);
		}

		orderIndex = std::stoi(logString[0]);
		currentFolder = logString[1];

		char mark = 'X';
		if (logString[9].back() == mark)
		{
			// Create new folder, log and reload
			createFolder();
			createStageLog();
			readFromStageLog();
		}
		else if (logString[6].back() == mark)
		{
			// set last map
			stageIndex = 9;
			mapFileName = logString[10];
		}
		else if (logString[3].back() == mark)
		{
			// set second map
			stageIndex = 6;
			mapFileName = logString[7];
		}
		else
		{
			// set first map
			stageIndex = 3;
			mapFileName = logString[4];
		}

		Broodwar << "Expected map: " << mapFileName.c_str() << std::endl;
	}
	else
	{
		// Create new folder, log and reload
		createFolder();
		createStageLog();
		readFromStageLog();
	}
}

LogHandler::LogHandler()
{
	// Stage 1: Trial with voice completed
	// Stage 2: Experimental with voice completed
	// Stage 3: Experimental without voice completed

	combinations[0] = "abc";
	combinations[1] = "acb";

	readFromStageLog();
}
LogHandler::~LogHandler(){}


void LogHandler::createMapLog()
{
	// Create file
	std::string filePath = "experiment-logs/";
	filePath += currentFolder;
	filePath.append("/");

	// Stage 1: Trial with voice completed
	// Stage 2: Experimental with voice completed
	// Stage 3: Experimental without voice completed

	std::ofstream log;
	log.open(filePath + Broodwar->mapName() + "_" + dateFormat() + ".txt");

	// if (speech)
	// Log speech data

	// if (experiment)
	// Log game data

	log << "Structures data" << std::endl;
	log << "Building score:" << Broodwar->self()->getBuildingScore() << std::endl;
	log << "Razing score:" << Broodwar->self()->getRazingScore() << std::endl;
	// Numbers of structures built are missing

	log << std::endl;
	log << "Units data" << std::endl;
	log << "Unit score:" << Broodwar->self()->getUnitScore() << std::endl;
	log << "Kill score:" << Broodwar->self()->getKillScore() << std::endl;
	log << "Produced units:" << Broodwar->self()->completedUnitCount() << std::endl;
	log << "Lost units:" << Broodwar->self()->deadUnitCount() << std::endl;
	log << "Killed units:" << Broodwar->self()->killedUnitCount() << std::endl;
	// Number of produced units are missing

	log << std::endl;
	log << "Resources data" << std::endl;
	log << "Gathered gas:" << Broodwar->self()->gatheredGas() << std::endl;
	log << "Gathered minerals:" << Broodwar->self()->gatheredMinerals() << std::endl;
	log << "Refunded gas:" << Broodwar->self()->refundedGas() << std::endl;
	log << "Refunded minerals:" << Broodwar->self()->refundedMinerals() << std::endl;
	log << "Spent gas:" << Broodwar->self()->spentGas() << std::endl;
	log << "Spent minerals:" << Broodwar->self()->spentMinerals() << std::endl;
	log << std::endl;
	log << "Uncategorized data" << std::endl;
	log << "Custom score:" << Broodwar->self()->getCustomScore() << std::endl;
	log << "Time:" << Broodwar->elapsedTime() << std::endl;
	log << "APM?:" << Broodwar->getAPM() << std::endl;
	log << std::endl;



	log.close();

	checkStage();

	saveStageLog();
}

std::string LogHandler::currentMap() const
{
	return mapFileName;
}