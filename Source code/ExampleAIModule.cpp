#include "ExampleAIModule.h"
#include "SpeechRecognition.h"
#include "LogHandler.h"



using namespace BWAPI;
using namespace Filter;

LogHandler* lh;

SpeechRecognition* sr;

bool disableSpeech;

std::vector<UnitType> bQueue;
std::vector<UnitType> ccQueue;
std::vector<UnitType> fQueue;
std::vector<UnitType> spQueue;
std::vector<UnitType> nQueue;

// Own function
void addToQueue(int type)
{
	switch (type)
	{
	case 1: // Command Center
	{
		ccQueue.push_back(UnitTypes::Terran_SCV);
		break;
	}

	case 2: // Barracks
	{
		bQueue.push_back(UnitTypes::Terran_Medic);
		break;
	}
	case 3:
	{
		bQueue.push_back(UnitTypes::Terran_Firebat);
		break;
	}
	case 4:
	{
		bQueue.push_back(UnitTypes::Terran_Marine);
		break;
	}
	case 5:
	{
		bQueue.push_back(UnitTypes::Terran_Ghost);
		break;
	}

	case 6: // Factory
	{
		fQueue.push_back(UnitTypes::Terran_Vulture);
		break;
	}
	case 7:
	{
		fQueue.push_back(UnitTypes::Terran_Goliath);
		break;
	}
	case 8:
	{
		fQueue.push_back(UnitTypes::Terran_Siege_Tank_Tank_Mode);
		break;
	}

	case 9: // Starport
	{
		spQueue.push_back(UnitTypes::Terran_Wraith);
		break;
	}
	case 10:
	{
		spQueue.push_back(UnitTypes::Terran_Dropship);
		break;
	}
	case 11:
	{
		spQueue.push_back(UnitTypes::Terran_Valkyrie);
		break;
	}
	case 12:
	{
		spQueue.push_back(UnitTypes::Terran_Science_Vessel);
		break;
	}
	case 13:
	{
		spQueue.push_back(UnitTypes::Terran_Battlecruiser);
		break;
	}
	case 14:
	{
		nQueue.push_back(UnitTypes::Terran_Nuclear_Missile);
		break;
	}
	}
}

std::string typeToString(int type)
{
	std::string stype = "Not added";

	switch (type)
	{
	case 1: // Command Center
	{
				stype = "SCV";
				break;
	}

	case 2: // Barracks
	{
				stype = "medic";
				break;
	}
	case 3:
	{
			  stype = "firebat";
			  break;
	}
	case 4:
	{
			  stype = "marine";
			  break;
	}
	case 5:
	{
			 stype = "ghost";
			 break;
	}

	case 6: // Factory
	{
				stype = "vulture";
				break;
	}
	case 7:
	{
			  stype = "goliath";
			  break;
	}
	case 8:
	{
			  stype = "siege tank";
			  break;
	}

	case 9: // Starport
	{
				stype = "wraith";
				break;
	}
	case 10:
	{
			   stype = "dropship";
			   break;
	}
	case 11:
	{
			   stype = "valkyrie";
			   break;
	}
	case 12:
	{
				stype = "science vessel";
				break;
	}
	case 13:
	{
			   stype = "battlecruiser";
			   break;
	}
	case 14:
	{
			   stype = "nuke";
			   break;
	}
	}
	return stype;
}

void ExampleAIModule::onStart()
{
	disableSpeech = false;

	lh = new LogHandler();

  //Broodwar->sendText("show me the money");
  //Broodwar->sendText("operation cwal");

  sr = new SpeechRecognition();

  // Print the map name.
  // BWAPI returns std::string when retrieving a string, don't forget to add .c_str() when printing!
  Broodwar << "The map is " << Broodwar->mapName() << "!" << std::endl;

  // Enable the UserInput flag, which allows us to control the bot and type messages.
  Broodwar->enableFlag(Flag::UserInput);

  // Set the command optimization level so that common commands can be grouped
  // and reduce the bot's APM (Actions Per Minute).
  Broodwar->setCommandOptimizationLevel(2);

  // Check if this is a replay
  if ( Broodwar->isReplay() )
  {

    // Announce the players in the replay
    Broodwar << "The following players are in this replay:" << std::endl;
    
    // Iterate all the players in the game using a std:: iterator
    Playerset players = Broodwar->getPlayers();
    for(auto p : players)
    {
      // Only print the player if they are not an observer
      if ( !p->isObserver() )
        Broodwar << p->getName() << ", playing as " << p->getRace() << std::endl;
    }

  }
  else // if this is not a replay
  {
    // Retrieve you and your enemy's races. enemy() will just return the first enemy.
    // If you wish to deal with multiple enemies then you must use enemies().
    if ( Broodwar->enemy() ) // First make sure there is an enemy
      Broodwar << "The matchup is " << Broodwar->self()->getRace() << " vs " << Broodwar->enemy()->getRace() << std::endl;
  }

  if (lh->currentMap().c_str()[3] == '1')
  {
	  Broodwar->sendText("show me the money");
	  Broodwar->sendText("operation cwal");
	  Broodwar->sendText("Voice commands enabled for this match");
  }
  else if (lh->currentMap().c_str()[2] == 'c' && lh->currentMap().c_str()[2] == '2')
  {
	  sr->switchSilentMode();
	  Broodwar->sendText("Voice commands disabled for this match");
  }
  else if (lh->currentMap().c_str()[2] == 'b' && lh->currentMap().c_str()[2] == '3')
  {
	  sr->switchSilentMode();
	  Broodwar->sendText("Voice commands disabled for this match");
  }
  else
  {
	  Broodwar->sendText("Voice commands enabled for this match");
  }

  // c2, b3

  if (Broodwar->mapFileName() != lh->currentMap())
  {
	  Broodwar->leaveGame();
  }
}

void ExampleAIModule::onEnd(bool isWinner)
{
  // Called when the game ends
  if ( isWinner )
  {
    // Log your win here!
  }

  if (Broodwar->mapFileName() == lh->currentMap())
  {
	  lh->createMapLog();
  }
}

void ExampleAIModule::onFrame()
{
	// Called once every game frame

	// Display the game frame rate as text in the upper left area of the screen
	Broodwar->drawTextScreen(200, 0, "FPS: %d", Broodwar->getFPS());
	Broodwar->drawTextScreen(200, 20, "Average FPS: %f", Broodwar->getAverageFPS());

	// Return if the game is a replay or is paused
	if (Broodwar->isReplay() || Broodwar->isPaused() || !Broodwar->self())
		return;

	// Prevent spamming by only running our onFrame once every number of latency frames.
	// Latency frames are the number of frames before commands are processed.
	if (Broodwar->getFrameCount() % Broodwar->getLatencyFrames() != 0)
		return;

	std::vector<int> voiceCommand;
	sr->getCommand(voiceCommand);

	if (Broodwar->getKeyState(BWAPI::Key::K_PAUSE))
	{
		if (!disableSpeech)
		{
			if (sr->switchSilentMode())
			{
				Broodwar->sendText("Voice commands disabled");
			}
			else
			{
				Broodwar->sendText("Voice commands enabled");
			}
		}
	}

	bool activateUpgrade = false;
	bool startedUpgrade = false;

	if (!voiceCommand.empty())
	{
		//if (voiceCommand.size() == 1) // Listening command
		//{
		//	if (voiceCommand[0] == 1)
		//	{
		//		Broodwar->sendText("Voice commands enabled");
		//	}
		//	else if (voiceCommand[0] == 2)
		//	{
		//		Broodwar->sendText("Voice commands disabled");
		//	}
		//}
		//else // Game command
		//{

		switch (voiceCommand[0])
		{
		case SpeechRuleType::SwitchState:
			{
				break;
			}
		case SpeechRuleType::Produce:
			{
				std::string amount = std::to_string(voiceCommand[1]);

				std::string response = "Trying to produce " + amount + " " + typeToString(voiceCommand[2]);
				if (voiceCommand[1] > 1) response += "s";

				Broodwar->sendText(response.c_str());
				for (int i = 0; i < voiceCommand[1]; i++)
				{
					addToQueue(voiceCommand[2]);
				}
				break;
			}
		case SpeechRuleType::Upgrade:
			{
				activateUpgrade = true;
				break;
			}
		}


		//}

	}


	// Iterate through all the units that we own
	for (auto &u : Broodwar->self()->getUnits())
	{
		// Ignore the unit if it no longer exists
		// Make sure to include this block when handling any Unit pointer!
		if (!u->exists())
			continue;

		// Ignore the unit if it has one of the following status ailments
		if (u->isLockedDown() || u->isMaelstrommed() || u->isStasised())
			continue;

		// Ignore the unit if it is in one of the following states
		if (u->isLoaded() || !u->isPowered() || u->isStuck())
			continue;

		// Ignore the unit if it is incomplete or busy constructing
		if (!u->isCompleted() || u->isConstructing())
			continue;

		if (u->getType() == UnitTypes::Terran_Barracks)
		{
			if (!u->isTraining() && !bQueue.empty())
			{
				u->train(bQueue.front());
				bQueue.pop_back();

				if (bQueue.empty() && Broodwar->self()->supplyTotal() == Broodwar->self()->supplyUsed())
				{
					Broodwar->sendText("Supply full, build more dupply depots!");
				}

				break;
			}
		}
		else if (u->getType() == UnitTypes::Terran_Command_Center)
		{
			if (!u->isTraining() && !ccQueue.empty())
			{
				u->train(ccQueue.front());
				ccQueue.pop_back();

				if (ccQueue.empty() && Broodwar->self()->supplyTotal() == Broodwar->self()->supplyUsed())
				{
					Broodwar->sendText("Supply full, build more dupply depots!");
				}

				break;
			}
		}
		else if (u->getType() == UnitTypes::Terran_Factory)
		{
			if (!u->isTraining() && !fQueue.empty())
			{
				u->train(fQueue.front());
				fQueue.pop_back();
				
				if (fQueue.empty() && Broodwar->self()->supplyTotal() == Broodwar->self()->supplyUsed())
				{
					Broodwar->sendText("Supply full, build more dupply depots!");
				}

				break;
			}
		}
		else if (u->getType() == UnitTypes::Terran_Starport)
		{
			if (!u->isTraining() && !spQueue.empty())
			{
				u->train(spQueue.front());
				spQueue.pop_back();

				if (spQueue.empty() && Broodwar->self()->supplyTotal() == Broodwar->self()->supplyUsed())
				{
					Broodwar->sendText("Supply full, build more dupply depots!");
				}

				break;
			}
		}
		else if (u->getType() == UnitTypes::Terran_Nuclear_Silo)
		{
			if (!u->isTraining() && !spQueue.empty())
			{
				u->train(nQueue.front());
				nQueue.pop_back();

				if (nQueue.empty() && Broodwar->self()->supplyTotal() == Broodwar->self()->supplyUsed())
				{
					Broodwar->sendText("Supply full, build more dupply depots!");
				}

				break;
			}
		}
		else if (activateUpgrade)
		{

			std::string centerName = "NULL";


			if (u->getType() == UnitTypes::Terran_Engineering_Bay)
			{
				if (voiceCommand[1] == 1)
				{
					if (voiceCommand[2] == 1)
					{
						u->upgrade(UpgradeTypes::Terran_Infantry_Weapons);
						startedUpgrade = true;
					}
					else if (voiceCommand[2] == 2)
					{
						u->upgrade(UpgradeTypes::Terran_Infantry_Armor);
						startedUpgrade = true;
					}
				}
				centerName = "Engineering Bay";
			}
			if (u->getType() == UnitTypes::Terran_Armory)
			{
				switch (voiceCommand[1])
				{
				case 2: // Vehicle
				{
					if (voiceCommand[2] == 1)
					{
						u->upgrade(UpgradeTypes::Terran_Vehicle_Weapons);
						startedUpgrade = true;
					}
					else if (voiceCommand[2] == 3)
					{
						u->upgrade(UpgradeTypes::Terran_Vehicle_Plating);
						startedUpgrade = true;
					}
					break;
				}
				case 3: // Ship
				{
					if (voiceCommand[2] == 1)
					{
						u->upgrade(UpgradeTypes::Terran_Ship_Weapons);
						startedUpgrade = true;
					}
					else if (voiceCommand[2] == 3)
					{
						u->upgrade(UpgradeTypes::Terran_Ship_Plating);
						startedUpgrade = true;
					}
					break;
				}
				}
				centerName = "Armory";
			}

			if (centerName != "NULL")
			{
				if (!startedUpgrade)
				{
					Broodwar->sendText("Could not start upgrade, dont know why");
				}

				std::string msg = centerName + " is currently upgrading " + std::to_string(u->getUpgrade());
				Broodwar->sendText(msg.c_str());

				std::stringstream ss;
				ss << "Upgrade is completed in " << u->getRemainingUpgradeTime() << " seconds";
				Broodwar->sendText(ss.str().c_str());
			}

		}

	}

	if (!startedUpgrade && activateUpgrade)
	{
		Broodwar->sendText("Could not start upgrade because facility is not built");
	}

 // closure: unit iterator
}

void ExampleAIModule::onSendText(std::string text)
{

  // Send the text to the game if it is not being processed.
  Broodwar->sendText("%s", text.c_str());


  // Make sure to use %s and pass the text as a parameter,
  // otherwise you may run into problems when you use the %(percent) character!

}

void ExampleAIModule::onReceiveText(BWAPI::Player player, std::string text)
{
  // Parse the received text
  Broodwar << player->getName() << " said \"" << text << "\"" << std::endl;
}

void ExampleAIModule::onPlayerLeft(BWAPI::Player player)
{
  // Interact verbally with the other players in the game by
  // announcing that the other player has left.
  Broodwar->sendText("Goodbye %s!", player->getName().c_str());
}

void ExampleAIModule::onNukeDetect(BWAPI::Position target)
{

  // Check if the target is a valid position
  if ( target )
  {
    // if so, print the location of the nuclear strike target
    Broodwar << "Nuclear Launch Detected at " << target << std::endl;
  }
  else 
  {
    // Otherwise, ask other players where the nuke is!
    Broodwar->sendText("Where's the nuke?");
  }

  // You can also retrieve all the nuclear missile targets using Broodwar->getNukeDots()!
}

void ExampleAIModule::onUnitDiscover(BWAPI::Unit unit)
{
}

void ExampleAIModule::onUnitEvade(BWAPI::Unit unit)
{
}

void ExampleAIModule::onUnitShow(BWAPI::Unit unit)
{
}

void ExampleAIModule::onUnitHide(BWAPI::Unit unit)
{
}

void ExampleAIModule::onUnitCreate(BWAPI::Unit unit)
{
	if (Broodwar->isReplay())
	{
		// if we are in a replay, then we will print out the build order of the structures
		if (unit->getType().isBuilding() && !unit->getPlayer()->isNeutral())
		{
			int seconds = Broodwar->getFrameCount() / 24;
			int minutes = seconds / 60;
			seconds %= 60;
			Broodwar->sendText("%.2d:%.2d: %s creates a %s", minutes, seconds, unit->getPlayer()->getName().c_str(), unit->getType().c_str());
		}
	}
	else
	{
		
	}

}

void ExampleAIModule::onUnitDestroy(BWAPI::Unit unit)
{
}

void ExampleAIModule::onUnitMorph(BWAPI::Unit unit)
{
  if ( Broodwar->isReplay() )
  {
    // if we are in a replay, then we will print out the build order of the structures
    if ( unit->getType().isBuilding() && !unit->getPlayer()->isNeutral() )
    {
      int seconds = Broodwar->getFrameCount()/24;
      int minutes = seconds/60;
      seconds %= 60;
      Broodwar->sendText("%.2d:%.2d: %s morphs a %s", minutes, seconds, unit->getPlayer()->getName().c_str(), unit->getType().c_str());
    }
  }
}

void ExampleAIModule::onUnitRenegade(BWAPI::Unit unit)
{
}

void ExampleAIModule::onSaveGame(std::string gameName)
{
  Broodwar << "The game was saved to \"" << gameName << "\"" << std::endl;
}

void ExampleAIModule::onUnitComplete(BWAPI::Unit unit)
{

}
