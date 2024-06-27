-- https://classic.runescape.wiki/w/Barbarian#Drops_2

local drops = {
	-- weapons and armour
	{ "iron mace", 1, 1 },
	{ "staff", 1, 4 },
	{ "bronze axe", 1, 6 },

	-- ammunition
	{ "bronze arrows", 15, 3 },
	{ "law-rune", 1, 1 },
	{ "mind-rune", 3, 2 },
	{ "fire-rune", 3, 2 },
	{ "earth-rune", 1, 3 },
	{ "chaos-rune", 1, 4 },

	-- coins 
	{ "coins", 27, 3 },
	{ "coins", 17, 5 },
	{ "coins", 8, 9 },
	{ "coins", 5, 42 },

	-- other
	{ "beer", 1, 1 },
	{ "cookedmeat", 1, 1 },
	{ "flier", 1, 1 },
	{ "fur", 1, 1 },
	{ "ring mould", 1, 1 },
	{ "tin ore", 1, 1 },
	{ "randomjewel", 1, 1 },
}

function killnpc_barbarian(player, npc, x, y)
	local ran = randomvar(128)

	local weight = 0

	addobject(player, "bones", 1, x, y)

	for _, drop in ipairs(drops) do
		weight = weight + drop[3]

		if ran < weight then
			if drop[1] == "randomjewel" then
				randomjewel(player, x, y)
			else
				addobject(player, drop[1], drop[2], x, y)
			end
			break
		end
	end
end
