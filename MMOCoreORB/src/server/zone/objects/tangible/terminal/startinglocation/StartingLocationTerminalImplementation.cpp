/*
 * StartingLocationTerminalImplementation.cpp
 *
 *  Created on: 12/08/2009
 *      Author: victor
 */

#include "server/zone/objects/tangible/terminal/startinglocation/StartingLocationTerminal.h"
#include "server/zone/objects/creature/CreatureObject.h"
#include "server/zone/managers/player/PlayerManager.h"
#include "server/zone/objects/player/PlayerObject.h"
#include "server/zone/objects/player/sui/listbox/SuiListBox.h"
#include "server/zone/objects/player/sui/callbacks/ExpSelectSuiCallback.h"
#include "server/zone/objects/player/sui/callbacks/FactionSelectSuiCallback.h"

void StartingLocationTerminalImplementation::initializeTransientMembers() {
	TerminalImplementation::initializeTransientMembers();

	setLoggingName("StartingLocationTerminal");
}

int StartingLocationTerminalImplementation::handleObjectMenuSelect(CreatureObject* player, byte selectedID) {
	//System::out << "entering start terminal radial call" << endl;
	if (selectedID != 20 || !authorizationState) { // not use object or not authorized
		if (!authorizationState)
			info("handleObjectMenuSelect: not authorized");

		return 1;
	}

	if(player->getSelectedExpMode() == 0 ){
		if(!player->getPlayerObject()->hasSuiBoxWindowType(SuiWindowType::XP_SELECT)){
			ManagedReference<PlayerObject*> ghost = player->getPlayerObject();
			ManagedReference<SuiListBox*> xpBox = new SuiListBox(player, SuiWindowType::XP_SELECT, 0);
			xpBox->setCallback(new ExpSelectSuiCallback(player->getZoneServer()));
			xpBox->setPromptTitle("Welcome");
			xpBox->setPromptText("Please select experience rate.");
			xpBox->addMenuItem("Normal: 1x Exp",0);
			//xpBox->addMenuItem("Elevated: 5x Exp",1);
			//xpBox->addMenuItem("Fast: 10x Exp",2);
			ghost->addSuiBox(xpBox);
			player->sendMessage(xpBox->generateMessage());
			return 1;
		}else{
			return 1;
		}
	}
	
	if(player->getSelectedExpMode() >= 0 ){
 		if(!player->getPlayerObject()->hasSuiBoxWindowType(SuiWindowType::FACTION_SELECT)){
 			ManagedReference<PlayerObject*> ghost = player->getPlayerObject();
 			ManagedReference<SuiListBox*> factionBox = new SuiListBox(player, SuiWindowType::FACTION_SELECT, 0);
 			factionBox->setCallback(new FactionSelectSuiCallback(player->getZoneServer()));
 			factionBox->setPromptTitle("Please select a faction.");
 			factionBox->setPromptText("Please choose carefully.");
 			factionBox->addMenuItem("Neutral",0);
 			factionBox->addMenuItem("Imperial",1);
 			factionBox->addMenuItem("Rebel",2);
 			ghost->addSuiBox(factionBox);
 			player->sendMessage(factionBox->generateMessage());
 			return 1;
 		}else{
 			return 1;
 		}
 	}
	
	server->getPlayerManager()->sendStartingLocationsTo(player);

	return 0;
}
