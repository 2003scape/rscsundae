-- https://classic.runescape.wiki/w/Man#Drops

local drops = {
	-- weapons and armour
	{ "iron dagger", 1, 1 },
	{ "medium bronze helmet", 1, 2 },

	-- ammunition
	{ "chaos-rune", 1, 1 },
	{ "earth-rune", 3, 2 },
	{ "fire-rune", 4, 2 },
	{ "mind-rune", 6, 2 },
	{ "arrows", 7, 3 },

	-- coins 
	{ "coins", 3, 38 },
	{ "coins", 5, 9 },
	{ "randomherb", 1, 23 },
	{ "coins", 15, 4 },
	{ "coins", 25, 1},

	-- other
	{ "fishing bait", 1, 5 },
	{ "copper ore", 1, 2 },
	{ "cabbage", 1, 1 },
}

function killnpc_man1(player, npc, x, y)
	local ran = randomvar(128)

	local weight = 0

	addobject(player, "bones", 1, x, y)

	for _, drop in ipairs(drops) do
		weight = weight + drop[3]

		if ran < weight then
			if drop[1] == "randomherb" then
				randomherb(player, x, y)
			else
				addobject(player, drop[1], drop[2], x, y)
			end
			break
		end
	end
end
