-- https://classic.runescape.wiki/w/Hobgoblin#Drops

local drops = {
	-- weapons and armour
	{ "bronze spear", 1, 3 },
	{ "iron short sword", 1, 3 },
	{ "steel dagger", 1, 3 },
	{ "iron spear", 1, 2 },
	{ "steel spear", 1, 2 },
	{ "crossbow", 1, 1 },
	{ "steel long sword", 1, 1 },

	-- ammunition
	{ "law-rune", 1, 3 },
	{ "body-rune", 4, 2 },
	{ "chaos-rune", 2, 2 },
	{ "fire-rune", 5, 2 },
	{ "nature-rune", 3, 2 },
	{ "water-rune", 1, 2 },
	{ "cosmic-rune", 1, 1 },

	-- coins
	{ "coins", 15, 36 },
	{ "coins", 5, 12 },
	{ "coins", 28, 10 },
	{ "coins", 62, 4 },
	{ "coins", 42, 3 },

	-- other
	{ "limpwurt root", 1, 22 },
	{ "randomherb", 1, 7 },
	{ "randomjewel", 1, 2 },
	{ "goblin armour", 1, 2 },
}

function killnpc_hobgoblin(player, npc, x, y)
	local ran = randomvar(128)

	local weight = 0

	addobject(player, "bones", 1, x, y)

	for _, drop in ipairs(drops) do
		weight = weight + drop[3]

		if ran < weight then
			-- for f2p drops see:
			-- https://github.com/2004Scape/Server/blob/3b31da7a1ee31d7fcd9f74f43eae7d5a1dbbd5a3/data/src/scripts/drop%20tables/scripts/hobgoblin.rs2#L2
			if drop[1] == "randomherb" then
				randomherb(player, x, y)
			elseif drop[1] == "randomjewel" then
				randomjewel(player, x, y)
			elseif drop[1] == "bronze spear" then
				addobject(player, "goblin armour", 1, x, y)
			elseif drop[1] == "iron spear" then
				addobject(player, "coins", 28, x, y)
			elseif drop[1] == "steel spear" then
				addobject(player, "coins", 28, x, y)
			else
				addobject(player, drop[1], drop[2], x, y)
			end
			break
		end
	end
end
