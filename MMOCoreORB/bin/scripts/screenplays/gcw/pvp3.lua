local ObjectManager = require("managers.object.object_manager")

pvp = ScreenPlay:new {
	numberOfActs = 1,
  	questString = "pvp",
  	questdata = Object:new {
    	activePlayerName = "initial",
    	}
}
  
registerScreenPlay("pvp", true)
  
function pvp:start()
    	self:spawnActiveAreas()
end
  
function pvp:spawnActiveAreas()
	local pSpawnArea = spawnSceneObject("rori", "object/active_area.iff", -5115, 80, -2339, 0, 0, 0, 0, 0)
    
	if (pSpawnArea ~= nil) then
		local activeArea = LuaActiveArea(pSpawnArea)
	        activeArea:setCellObjectID(0)
	        activeArea:setRadius(220)
	        createObserver(ENTEREDAREA, "pvp", "notifySpawnArea", pSpawnArea)
	        createObserver(EXITEDAREA, "pvp", "notifySpawnAreaLeave", pSpawnArea)
	    end
end
 
function pvp:notifySpawnArea(pActiveArea, pMovingObject)
	
	if (not SceneObject(pMovingObject):isCreatureObject()) then
		return 0
	end
	
	return ObjectManager.withCreatureObject(pMovingObject, function(player)
		if (player:isAiAgent()) then
			return 0
		end
		
		if (player:isImperial() or player:isRebel()) then
			player:broadcastToServer("\\#00E604" .. player:getFirstName() .. "\\#63C8F9 Has entered the Rori  PVP Zone!")
			player:sendSystemMessage("You have entered the Rori PvP zone!")
			player:setFactionStatus(2)
		else
			player:sendSystemMessage("You must be Rebel or Imperial or Neutral to enter the PvP zone!")
			player:teleport(-5108, 81, -2106, 0)
		end
		return 0
	end)
end

function pvp:notifySpawnAreaLeave(pActiveArea, pMovingObject)
	
	if (not SceneObject(pMovingObject):isCreatureObject()) then
		return 0
	end
	
	return ObjectManager.withCreatureObject(pMovingObject, function(player)
		if (player:isAiAgent()) then
			return 0
		end
		
		if (player:isImperial() or player:isNeutral() or player:isRebel()) then
			player:broadcastToServer("\\#00E604" .. player:getFirstName() .. "\\#63C8F9 Has left the Rori PVP Zone!")
			player:sendSystemMessage("You have left the Rori PvP zone!")
		end
		return 0
	end)
end
