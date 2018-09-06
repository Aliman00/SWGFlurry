tps_cardas_guard = Creature:new {
	customName = "a Car'das guard",
	randomNameTag = false,
	socialGroup = "jabba",
	faction = "jabba",
	level = 15,
	chanceHit = 0.25,
	damageMin = 120,
	damageMax = 130,
	baseXp = 95,
	baseHAM = 1100,
	baseHAMmax = 1400,
	armor = 0,
	resists = {0,0,0,0,0,0,0,-1,-1},
	meatType = "",
	meatAmount = 0,
	hideType = "",
	hideAmount = 0,
	boneType = "",
	boneAmount = 0,
	milk = 0,
	tamingChance = 0,
	ferocity = 0,
	pvpBitmask = ATTACKABLE,
	creatureBitmask = PACK,
	optionsBitmask = AIENABLED,
	diet = HERBIVORE,

	templates = {
				"object/mobile/dressed_cardas_guard_f_01.iff",
				"object/mobile/dressed_cardas_guard_f_02.iff",
				"object/mobile/dressed_cardas_guard_f_03.iff",
				"object/mobile/dressed_cardas_guard_f_04.iff",
				"object/mobile/dressed_cardas_guard_f_05.iff",
				"object/mobile/dressed_cardas_guard_f_06.iff",
				"object/mobile/dressed_cardas_guard_m_01.iff",
				"object/mobile/dressed_cardas_guard_m_02.iff",
				"object/mobile/dressed_cardas_guard_m_03.iff",
				"object/mobile/dressed_cardas_guard_m_04.iff",
				"object/mobile/dressed_cardas_guard_m_05.iff",
				"object/mobile/dressed_cardas_guard_m_06.iff",
				"object/mobile/dressed_cardas_guard_m_07.iff",
				"object/mobile/dressed_cardas_guard_m_08.iff",
				"object/mobile/dressed_cardas_guard_m_09.iff",
				"object/mobile/dressed_cardas_guard_m_10.iff",
				},
	lootGroups = {
		{
			groups = {
				{group = "junk", chance = 2000000},
				{group = "wearables_common", chance = 1000000},
				{group = "weapons_all", chance = 3000000},
				{group = "tailor_components", chance = 1500000},
				{group = "loot_kit_parts", chance = 1500000},
				{group = "printer_parts", chance = 1000000},
			}
		}
	},
	weapons = {"corsec_police_weapons"},
	conversationTemplate = "",
	reactionStf = "@npc_reaction/military",
	attacks = merge(brawlermaster,marksmanmaster)
}

CreatureTemplates:addCreatureTemplate(tps_cardas_guard, "tps_cardas_guard")
