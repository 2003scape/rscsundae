-- https://classic.runescape.wiki/w/Giant#Drops

local drops = {
	-- weapons and armour
	{ "large iron helmet", 1, 5 },
	{ "iron dagger", 1, 4 },
	{ "iron kite shield", 1, 3 },
	{ "bronze mace", 1, 2 },
	{ "steel long sword", 1, 2 },

	-- runes
	{ "fire-rune", 10, 3 },
	{ "law-rune", 1, 3 },
	{ "water-rune", 5, 3 },
	{ "cosmic-rune", 1, 2 },
	{ "mind-rune", 2, 2 },
	{ "nature-rune", 4, 2 },
	{ "chaos-rune", 1, 1 },
	{ "death-rune", 1, 1 },

	-- coins
	{ "coins", 38, 26 },
	{ "coins", 52, 10 },
	{ "coins", 15, 8 },
	{ "coins", 3, 6 },
	{ "coins", 8, 6 },
	{ "coins", 88, 2 },

	-- other
	{ "randomherb", 1, 7 },
	{ "randomjewel", 1, 3 },
	{ "limpwurt root", 1, 11 },
	{ "beer", 1, 6 },
}

function killnpc_giant(player, npc, x, y)
	local ran = randomvar(128)

	local weight = 0

	addobject(player, "big bones", 1, x, y)

	for _, drop in ipairs(drops) do
		weight = weight + drop[3]

		if ran < weight then
			if drop[1] == "randomherb" then
				randomherb(player, x, y)
			elseif drop[1] == "randomjewel" then
				randomjewel(player, x, y)
			else
				addobject(player, drop[1], drop[2], x, y)
			end
			break
		end
	end
end
