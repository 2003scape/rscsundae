-- https://classic.runescape.wiki/w/Lesser_Demon#Drops

local drops = {
	-- weapons and armour
	{ "large steel helmet", 1, 4 },
	{ "steel axe", 1, 4 },
	{ "steel scimitar", 1, 3 },
	{ "medium rune helmet", 1, 1 },
	{ "mithril chain mail body", 1, 1 },
	{ "mithril square shield", 1, 1 },

	-- runes
	{ "fire-rune", 40, 8 },
	{ "fire-rune", 20, 1 },
	{ "chaos-rune", 8, 5 },
	{ "death-rune", 2, 3 },

	-- coins
	{ "coins", 120, 40 },
	{ "coins", 40, 29 },
	{ "coins", 200, 10 },
	{ "coins", 10, 7 },

	-- other
	{ "randomjewel", 1, 4 },
	{ "wine", 2, 3 },
	{ "gold", 1, 2 },
	{ "randomherb", 1, 1 },
}

function killnpc_lesser_demon(player, npc, x, y)
	local ran = randomvar(128)

	local weight = 0

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
