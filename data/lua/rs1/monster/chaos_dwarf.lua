-- https://classic.runescape.wiki/w/Chaos_Dwarf#Drops

local drops = {
	-- weapons and armour
	{ "large steel helmet", 1, 2 },
	{ "mithril long sword", 1, 1 },
	{ "mithril square shield", 1, 1 },

	-- runes
	{ "law-rune", 2, 4 },
	{ "air-rune", 16, 3 },
	{ "chaos-rune", 7, 3 },
	{ "mind-rune", 25, 3 },
	{ "nature-rune", 6, 3 },
	{ "cosmic-rune", 6, 2 },
	{ "death-rune", 2, 1 },
	{ "water-rune", 7, 1 },

	-- coins
	{ "coins", 92, 40 },
	{ "coins", 47, 18 },
	{ "coins", 25, 11 },
	{ "coins", 150, 10 },
	{ "coins", 15, 2 },
	{ "coins", 350, 2 },

	-- other
	{ "muddy key", 1, 7 },
	{ "mithril bar", 1, 6 },
	{ "randomjewel", 1, 3 },
	{ "cheese", 1, 1 },
	{ "coal", 1, 1 },
	{ "tomato", 1, 1 },
}

function killnpc_chaos_dwarf(player, npc, x, y)
	local ran = randomvar(128)

	local weight = 0

	addobject(player, "bones", 1, x, y)

	for _, drop in ipairs(drops) do
		weight = weight + drop[3]

		if ran < weight then
			if drop[1] == "randomjewel" then
				randomjewel(player, x, y)
				break
			else
				addobject(player, drop[1], drop[2], x, y)
				break
			end
		end
	end
end
