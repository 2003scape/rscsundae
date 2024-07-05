-- https://classic.runescape.wiki/w/Skeleton#Drops_(level_25)

local drops = {
	-- weapons and armour
	{ "iron dagger", 1, 6 },
	{ "bronze short sword", 1, 4 },
	{ "bronze long sword", 1, 1 },

	-- runes and ammunition
	{ "arrows", 2, 7 },
	{ "arrows", 5, 4 },
	{ "air-rune", 8, 2 },
	{ "chaos-rune", 2, 2 },
	{ "earth-rune", 2, 2 },
	{ "fire-rune", 1, 2 },
	{ "nature-rune", 2, 1 },

	-- coins
	{ "coins", 2, 18 },
	{ "coins", 12, 15 },
	{ "coins", 4, 7 },
	{ "coins", 16, 4 },
	{ "coins", 25, 4 },
	{ "coins", 33, 4 },
	{ "coins", 48, 1 },

	-- other
	{ "randomherb", 1, 21 },
	{ "randomjewel", 1, 1 },
	{ "grain", 1, 1 },
	{ "iron ore", 1, 1 },
}

function killnpc_skeleton2(player, npc, x, y)
	local ran = randomvar(128)

	local weight = 0

	addobject(player, "bones", 1, x, y)

	for _, drop in ipairs(drops) do
		weight = weight + drop[3]

		if ran < weight then
			if drop[1] == "randomherb" then
				randomherb(player, x, y)
				break
			elseif drop[1] == "randomjewel" then
				randomjewel(player, x, y)
				break
			else
				addobject(player, drop[1], drop[2], x, y)
				break
			end
		end
	end
end
