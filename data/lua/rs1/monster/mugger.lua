-- https://classic.runescape.wiki/w/Mugger#Drops

local drops = {
	-- ammunition
	{ "mind-rune", 6, 3 },
	{ "earth-rune", 3, 2 },
	{ "water-rune", 4, 2 },

	-- coins
	{ "coins", 5, 12 },
	{ "coins", 10, 3 },
	{ "coins", 25, 1 },

	-- other
	{ "rope", 1, 40 },
	{ "randomherb", 1, 13 },
	{ "fishing bait", 1, 6 },
	{ "copper ore", 1, 2 },
	{ "medium bronze helmet", 1, 2 },
	{ "cabbage", 1, 1 },
	{ "knife", 1, 1 },
}

function killnpc_mugger(player, npc, x, y)
	local ran = randomvar(128)

	local weight = 0

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
