-- https://classic.runescape.wiki/w/Dwarf#Drops

local drops = {
	-- weapons and armour
	{ "pickaxe", 1, 13 },
	{ "medium bronze helmet", 1, 4 },
	{ "bronze battle axe", 1, 2 },
	{ "iron battle axe", 1, 1 },

	-- runes
	{ "chaos-rune", 1, 4 },
	{ "nature-rune", 1, 4 },

	-- coins
	{ "coins", 4, 23 },
	{ "coins", 10, 12 },
	{ "coins", 30, 2 },

	-- other
	{ "hammer", 1, 10 },
	{ "bronze bar", 1, 7 },
	{ "iron ore", 1, 4 },
	{ "copper ore", 1, 3 },
	{ "iron bar", 1, 3 },
	{ "tin ore", 1, 3 },
	{ "coal", 1, 2 },
	{ "randomjewel", 1, 1 },
}

function killnpc_dwarf(player, npc, x, y)
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
