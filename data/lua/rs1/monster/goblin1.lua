-- https://classic.runescape.wiki/w/Goblin#Drops_(Level_7)

local drops = {
	-- weapons and armour
	{ "shortbow", 1, 1 },
	{ "bronze square shield", 1, 3 },
	{ "goblin armour", 1, 5 },

	-- ammunition
	{ "crossbow bolts", 8, 3 },
	{ "earth-rune", 3, 3 },
	{ "body-rune", 7, 5 },
	{ "water-rune", 4, 6 },

	-- coins 
	{ "coins", 20, 2 },
	{ "coins", 15, 3 },
	{ "coins", 9, 3 },
	{ "coins", 5, 28 },

	-- other
	{ "beer", 1, 2 },
	{ "brass necklace", 1, 1 },
	{ "chef's hat", 1, 1 },
}

function killnpc_goblin1(player, npc, x, y)
	local ran = randomvar(128)

	local weight = 0

	for _, drop in ipairs(drops) do
		weight = weight + drop[3]

		if ran < weight then
			addobject(player, drop[1], drop[2], x, y)
			break
		end
	end
end
