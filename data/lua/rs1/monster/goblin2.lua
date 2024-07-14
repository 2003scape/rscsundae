-- https://classic.runescape.wiki/w/Goblin#Drops_(Level_13)

local drops = {
	-- weapons and armour
	{ "bronze spear", 1, 9 },
	{ "bronze axe", 1, 3 },
	{ "bronze scimitar", 1, 1 },

	-- ammunition
	{ "body-rune", 1, 3 },
	{ "earth-rune", 3, 3 },
	{ "mind-rune", 1, 3 },
	{ "chaos-rune", 1, 1 },
	{ "nature-rune", 1, 1 },
	{ "arrows", 7, 3 },

	-- coins
	{ "coins", 1, 34 },
	{ "coins", 3, 13 },
	{ "coins", 5, 8 },
	{ "coins", 16, 7 },
	{ "coins", 24, 3 },

	-- other
	{ "goblin armour", 1, 10 },
	{ "randomherb", 1, 2 },
	{ "redcape", 1, 1 },
	{ "grapes", 1, 1 },
	{ "tin ore", 1, 1 },
}

function killnpc_goblin2(player, npc, x, y)
	local ran = randomvar(128)

	local weight = 0

	addobject(player, "bones", 1, x, y)

	for _, drop in ipairs(drops) do
		weight = weight + drop[3]

		if ran < weight then
			-- https://github.com/2004Scape/Server/blob/3554f9ee5a4c229ca5dd7955a41110dc9bdecced/data/src/scripts/drop%20tables/scripts/goblin.rs2#L81
			if drop[1] == "bronze spear" then
				addobject(player, "hammer", 1, x, y)
				break
			else
				addobject(player, drop[1], drop[2], x, y)
				break
			end
		end
	end
end
