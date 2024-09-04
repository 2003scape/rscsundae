-- https://classic.runescape.wiki/w/White_Knight#Drops

local drops = {
	-- weapons and armour
	{ "longbow", 1, 3 },
	{ "iron long sword", 1, 2 },
	{ "medium steel helmet", 1, 2 },
	{ "steel short sword", 1, 2 },

	-- runes
	{ "chaos-rune", 2, 5 },
	{ "nature-rune", 3, 4 },
	{ "body-rune", 8, 3 },
	{ "water-rune", 20, 3 },
	{ "mind-rune", 3, 2 },
	{ "law-rune", 1, 1 },

	-- coins
	{ "coins", 48, 37 },
	{ "coins", 15, 18 },
	{ "coins", 70, 9 },
	{ "coins", 8, 7 },
	{ "coins", 2, 5 },
	{ "coins", 120, 3 },

	-- other
	{ "iron bar", 3, 8 },
	{ "iron bar", 2, 2 },
	{ "randomherb", 1, 5 },
	{ "randomjewel", 1, 1 },
	{ "flourpot", 1, 1 },
	{ "half an apple pie", 1, 1 },
	{ "iron ore", 1, 1 },
}

function killnpc_white_knight(player, npc, x, y)
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
