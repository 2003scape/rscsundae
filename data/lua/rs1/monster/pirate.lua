-- https://classic.runescape.wiki/w/Pirate#Drops

local drops = {
	-- weapons and armour
	{ "bronze scimitar", 1, 10 },
	{ "iron dagger", 1, 6 },
	{ "iron plate mail body", 1, 1 },

	-- ammunition
	{ "chaos-rune", 1, 5 },
	{ "nature-rune", 1, 5 },
	{ "air-rune", 7, 2 },
	{ "earth-rune", 3, 2 },
	{ "fire-rune", 3, 2 },
	{ "law-rune", 1, 1 },
	{ "arrows", 12, 3 },
	{ "arrows", 9, 2 },

	-- coins
	{ "coins", 4, 28 },
	{ "coins", 25, 10 },
	{ "coins", 12, 7 },
	{ "coins", 7, 6 },
	{ "coins", 35, 4 },
	{ "coins", 55, 1 },

	-- other
	{ "chef's hat", 1, 1 },
	{ "iron bar", 1, 1 },
	{ "randomjewel", 1, 1 },
}

function killnpc_pirate(player, npc, x, y)
	local ran = randomvar(128)

	local weight = 0

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
