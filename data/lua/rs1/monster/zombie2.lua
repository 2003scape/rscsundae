-- https://classic.runescape.wiki/w/Zombie#Drops_%28Level_24%29

local drops = {
	-- weapons and armour
	{ "iron mace", 1, 3 },
	{ "crossbow", 1, 2 },
	{ "iron dagger", 1, 2 },
	{ "bronze kite shield", 1, 1 },

	-- runes
	{ "air-rune", 2, 3 },
	{ "body-rune", 2, 2 },
	{ "cosmic-rune", 1, 1 },
	{ "fire-rune", 5, 1 },

	-- coins
	{ "coins", 18, 21 },
	{ "coins", 10, 10 },
	{ "coins", 26, 8 },
	{ "coins", 35, 6 },

	-- other
	{ "tinderbox", 1, 2 },
	{ "eye of newt", 1, 1 },
	{ "tin ore", 1, 1 },
}

function killnpc_zombie2(player, npc, x, y)
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
