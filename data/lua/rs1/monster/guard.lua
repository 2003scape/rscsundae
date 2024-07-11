-- https://classic.runescape.wiki/w/Guard#Drops

local drops = {
	-- weapons and armour
	{ "iron dagger", 1, 6 },
	{ "medium bronze helmet", 1, 4 },
	{ "bronze long sword", 1, 1 },

	-- ammunition
	{ "air-rune", 4, 2 },
	{ "earth-rune", 1, 2 },
	{ "fire-rune", 1, 2 },
	{ "chaos-rune", 1, 1 },
	{ "nature-rune", 1, 1 },
	{ "arrows", 1, 3 },
	{ "arrows", 2, 2 },

	-- coins
	{ "coins", 1, 38 },
	{ "coins", 7, 16 },
	{ "coins", 4, 10 },
	{ "coins", 9, 12 },
	{ "coins", 17, 4 },
	{ "coins", 25, 4 },
	{ "coins", 30, 2 },

	-- other
	{ "grain", 1, 1 },
	{ "iron ore", 1, 1 },
}

function killnpc_guard(player, npc, x, y)
	local ran = randomvar(128)

	local weight = 0

	addobject(player, "bones", 1, x, y)

	for _, drop in ipairs(drops) do
		weight = weight + drop[3]

		if ran < weight then
			addobject(player, drop[1], drop[2], x, y)
			break
		end
	end
end
