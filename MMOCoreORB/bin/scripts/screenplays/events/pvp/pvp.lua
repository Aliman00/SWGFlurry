--------------------------------------
--   Creator : TOXIC
--   Date : 01/1/2019
--------------------------------------
pvpScreenPlay = ScreenPlay:new {
	numberOfActs = 1,

	screenplayName = "pvpScreenPlay",
}

registerScreenPlay("pvpScreenPlay", true)

function pvpScreenPlay:start()
	if (isZoneEnabled("tatooine")) then
		self:spawnSceneObjects()
	end
end

function pvpScreenPlay:spawnSceneObjects()
--Spawn PvP Terminal for 2v2 Imperial Vs Rebel
	spawnSceneObject("tatooine", "object/tangible/terminal/systems_control_terminal.iff", 3373.25, 10.998, -5128.88, 0, -0.34202, 0, 0.939693, 0)
	spawnSceneObject("tatooine", "object/tangible/terminal/systems_control_terminal.iff", 3415.67, 5, -5087.69, 0, 0.332035, 0, -0.943267, 0)
end
