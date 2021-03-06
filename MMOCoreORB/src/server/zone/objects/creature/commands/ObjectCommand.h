/*
				Copyright <SWGEmu>
		See file COPYING for copying conditions.*/

#ifndef OBJECTCOMMAND_H_
#define OBJECTCOMMAND_H_

#include "server/zone/objects/scene/SceneObject.h"
#include "server/zone/managers/loot/LootManager.h"
#include "server/zone/managers/crafting/CraftingManager.h"
#include "server/zone/managers/crafting/ComponentMap.h"


class ObjectCommand : public QueueCommand {
public:

	ObjectCommand(const String& name, ZoneProcessServer* server)
		: QueueCommand(name, server) {

	}

	int doQueueCommand(CreatureObject* creature, const uint64& target, const UnicodeString& arguments) const {

		if (!checkStateMask(creature))
			return INVALIDSTATE;

		if (!checkInvalidLocomotions(creature))
			return INVALIDLOCOMOTION;

		StringTokenizer args(arguments.toString());

		try {
			String commandType;
			args.getStringToken(commandType);

			if (commandType.beginsWith("createitem")) {
				String objectTemplate;
				args.getStringToken(objectTemplate);

				ManagedReference<CraftingManager*> craftingManager = creature->getZoneServer()->getCraftingManager();
				if(craftingManager == NULL) {
					return GENERALERROR;
				}

				Reference<SharedObjectTemplate*> shot = TemplateManager::instance()->getTemplate(objectTemplate.hashCode());

				if (shot == NULL || !shot->isSharedTangibleObjectTemplate()) {
					creature->sendSystemMessage("Templates must be tangible objects, or descendants of tangible objects, only.");
					return INVALIDPARAMETERS;
				}

				ManagedReference<SceneObject*> inventory = creature->getSlottedObject("inventory");

				if (inventory == NULL || inventory->isContainerFullRecursive()) {
					creature->sendSystemMessage("Your inventory is full, so the item could not be created.");
					return INVALIDPARAMETERS;
				}

				ManagedReference<TangibleObject*> object = (server->getZoneServer()->createObject(shot->getServerObjectCRC(), 1)).castTo<TangibleObject*>();

				if (object == NULL) {
					creature->sendSystemMessage("The object '" + commandType + "' could not be created because the template could not be found.");
					return INVALIDPARAMETERS;
				}

				Locker locker(object);

				object->createChildObjects();

				ManagedReference<PlayerObject*> ghost = creature->getPlayerObject();
				ManagedReference<CreatureObject*> player = cast<CreatureObject*>(creature);

				// Set Player as Crafter
				String name = player->getFirstName();
				object->setCraftersName(name);

				// Object Name
				StringBuffer customName;
				if (ghost->getAdminLevel() >= 15) {
					customName << object->getDisplayedName() << " \\#ff0000(" << player->getFirstName() << ")\\#FFFFFF";//Red for admin
				} else {
					customName << object->getDisplayedName() << " \\#00CC00(" << player->getFirstName() << ")\\#FFFFFF";//Green for player
				}
				object->setCustomObjectName(customName.toString(), false);

				String serial = craftingManager->generateSerial();
				object->setSerialNumber(serial);

				int quantity = 1;

				if (args.hasMoreTokens())
					quantity = args.getIntToken();

				if(quantity > 1 && quantity <= 100)
					object->setUseCount(quantity);

				// load visible components
				while (args.hasMoreTokens()) {
					String visName;
					args.getStringToken(visName);

					uint32 visId = visName.hashCode();
					if (ComponentMap::instance()->getFromID(visId).getId() == 0)
						continue;

					object->addVisibleComponent(visId, false);
				}

				if (inventory->transferObject(object, -1, true)) {
					inventory->broadcastObject(object, true);
				} else {
					object->destroyObjectFromDatabase(true);
					creature->sendSystemMessage("Error transferring object to inventory.");
				}
			} else if (commandType.beginsWith("createloot")) {
				String lootGroup;
				args.getStringToken(lootGroup);

				int level = 1;

				if (args.hasMoreTokens())
					level = args.getIntToken();

				ManagedReference<SceneObject*> inventory = creature->getSlottedObject("inventory");

				if (inventory == NULL || inventory->isContainerFullRecursive()) {
					creature->sendSystemMessage("Your inventory is full, so the item could not be created.");
					return INVALIDPARAMETERS;
				}

				ManagedReference<LootManager*> lootManager = creature->getZoneServer()->getLootManager();

				if (lootManager == NULL)
					return INVALIDPARAMETERS;

				lootManager->createLoot(inventory, lootGroup, level);
			}else if (commandType.beginsWith("createattachment")) {
				//Syntax exmaple /object createattachment clothing mindblast_accuracy 25
				String attachmentType;
				String skillMod;
				args.getStringToken(attachmentType);
		
				if (attachmentType != "armor" && attachmentType != "clothing"){
					creature->sendSystemMessage("You must specify armor or clothing.");
					return INVALIDPARAMETERS;
				}
 				if (args.hasMoreTokens())
					args.getStringToken(skillMod);
				
				int skillBonus = 1;
				if (args.hasMoreTokens())
					skillBonus = args.getIntToken();
 				ManagedReference<SceneObject*> inventory = creature->getSlottedObject("inventory");
 				if (inventory == NULL || inventory->isContainerFullRecursive()) {
					creature->sendSystemMessage("Your inventory is full, so the item could not be created.");
					return INVALIDPARAMETERS;
				}
 				ManagedReference<LootManager*> lootManager = creature->getZoneServer()->getLootManager();
 				
 				if (lootManager == NULL)
					return INVALIDPARAMETERS;
					
				Reference<LootItemTemplate*> itemTemplate = NULL;
				LootGroupMap* lootGroupMap = LootGroupMap::instance();
				if (attachmentType == "armor")
					itemTemplate = lootGroupMap->getLootItemTemplate("attachment_armor");
				else
					itemTemplate = lootGroupMap->getLootItemTemplate("attachment_clothing");
							
				ManagedReference<SceneObject*> ca = lootManager->createLootAttachment(itemTemplate,skillMod, skillBonus); 
 				if (ca != NULL){
					Attachment* attachment = cast<Attachment*>(ca.get());
								
					if (attachment != NULL){
						Locker objLocker(attachment);
						if (inventory->transferObject(ca, -1, true, true)) { //Transfer tape to player inventory
							inventory->broadcastObject(ca, true);
						} else {
							ca->destroyObjectFromDatabase(true);
							creature->sendSystemMessage("Unable to place Skill Attachment in player's inventory!");
							return INVALIDPARAMETERS;
						}
						
					}
							
				}
 			}
			 else if (commandType.beginsWith("createresource")) {
				String resourceName;
				args.getStringToken(resourceName);

				int quantity = 100000;

				if (args.hasMoreTokens())
					quantity = args.getIntToken();

				ManagedReference<ResourceManager*> resourceManager = server->getZoneServer()->getResourceManager();
				resourceManager->givePlayerResource(creature, resourceName, quantity);
			} else if (commandType.beginsWith("createarealoot")) {
				String lootGroup;
				args.getStringToken(lootGroup);

				int range = 32;
				if (args.hasMoreTokens())
					range = args.getIntToken();

				if( range < 0 )
					range = 32;

				if( range > 128 )
					range = 128;

				int level = 1;
				if (args.hasMoreTokens())
					level = args.getIntToken();

				ManagedReference<LootManager*> lootManager = creature->getZoneServer()->getLootManager();
				if (lootManager == NULL)
					return INVALIDPARAMETERS;

				Zone* zone = creature->getZone();
				if (zone == NULL)
					return GENERALERROR;

				// Find all objects in range
				SortedVector<QuadTreeEntry*> closeObjects;
				CloseObjectsVector* closeObjectsVector = (CloseObjectsVector*) creature->getCloseObjects();
				if (closeObjectsVector == NULL) {
					zone->getInRangeObjects(creature->getPositionX(), creature->getPositionY(), range, &closeObjects, true);
				} else {
					closeObjectsVector->safeCopyTo(closeObjects);
				}

				// Award loot group to all players in range
				for (int i = 0; i < closeObjects.size(); i++) {
					SceneObject* targetObject = static_cast<SceneObject*>(closeObjects.get(i));

					if (targetObject->isPlayerCreature() && creature->isInRange(targetObject, range)) {

						CreatureObject* targetPlayer = cast<CreatureObject*>(targetObject);
						Locker tlock( targetPlayer, creature );

						ManagedReference<SceneObject*> inventory = targetPlayer->getSlottedObject("inventory");
						if (inventory != NULL) {
							if( lootManager->createLoot(inventory, lootGroup, level) )
								targetPlayer->sendSystemMessage( "You have received a loot item!");
						}

						tlock.release();
					}
				}
			} else if (commandType.beginsWith("checklooted")) {
				ManagedReference<LootManager*> lootManager = creature->getZoneServer()->getLootManager();
				if (lootManager == NULL)
					return INVALIDPARAMETERS;

				creature->sendSystemMessage("Number of Legendaries Looted: " + String::valueOf(lootManager->getLegendaryLooted()));
				creature->sendSystemMessage("Number of Exceptionals Looted: " + String::valueOf(lootManager->getExceptionalLooted()));
				creature->sendSystemMessage("Number of Magical Looted: " + String::valueOf(lootManager->getYellowLooted()));
			}
		} catch (Exception& e) {
			creature->sendSystemMessage("SYNTAX: /object createitem <objectTemplatePath> [<quantity>]");
			creature->sendSystemMessage("SYNTAX: /object createresource <resourceName> [<quantity>]");
			creature->sendSystemMessage("SYNTAX: /object createloot <loottemplate> [<level>]");
			creature->sendSystemMessage("SYNTAX: /object createarealoot <loottemplate> [<range>] [<level>]");
			creature->sendSystemMessage("SYNTAX: /object createattachment <armor/clothing> <skillModName> [<bonus>]");
			creature->sendSystemMessage("SYNTAX: /object checklooted");

			return INVALIDPARAMETERS;
		}


		return SUCCESS;
	}

};

#endif //OBJECTCOMMAND_H_
