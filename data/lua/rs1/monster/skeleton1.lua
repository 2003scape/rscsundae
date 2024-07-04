-- https://classic.runescape.wiki/w/Skeleton#Drops_(level_21)

local drops = {
	-- weapons and armour
	{ "medium bronze helmet", 1, 3 },
	{ "bronze chain mail body", 1, 1 },
	{ "iron mace", 1, 1 },
	{ "leather gloves", 1, 1 },

	-- runes and ammunition
	{ "mind-rune", 1, 3 },
	{ "earth-rune", 1, 2 },
	{ "nature-rune", 2, 2 },
	{ "water-rune", 2, 2 },
	{ "chaos-rune", 1, 1 },
	{ "arrows", 8, 4 },

	-- coins
	{ "coins", 2, 28 },
	{ "coins", 6, 21 },
	{ "coins", 1, 8 },
	{ "coins", 13, 7 },
	{ "coins", 20, 4 },
	{ "coins", 30, 3 },

	-- other
	{ "randomherb", 1, 21 },
	{ "bucket", 1, 1 },
}

function killnpc_skeleton1(player, npc, x, y)
	local ran = randomvar(128)

	local weight = 0

	addobject(player, "bones", 1, x, y)

	for _, drop in ipairs(drops) do
		weight = weight + drop[3]

		if ran < weight then
			if drop[1] == "randomherb" then
				randomherb(player, x, y)
				break
			else
				addobject(player, drop[1], drop[2], x, y)
				break
			end
		end
	end
end
