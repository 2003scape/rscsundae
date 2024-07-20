-- https://classic.runescape.wiki/w/Greater_Demon#Drops

local drops = {
	-- weapons and armour
	{ "steel 2-handed sword", 1, 4 },
	{ "steel axe", 1, 3 },
	{ "steel battle axe", 1, 3 },
	{ "adamantite plate mail legs", 1, 1 },
	{ "large rune helmet", 1, 1 },
	{ "mithril kite shield", 1, 1 },

	-- runes
	{ "fire-rune", 50, 8 },
	{ "fire-rune", 25, 1 },
	{ "chaos-rune", 10, 3 },
	{ "death-rune", 3, 3 },

	-- coins
	{ "coins", 132, 40 },
	{ "coins", 44, 29 },
	{ "coins", 220, 10 },
	{ "coins", 11, 7 },
	{ "coins", 460, 1 },

	-- other
	{ "randomjewel", 1, 5 },
	{ "tuna", 2, 3 },
	{ "gold bar", 1, 2 },
	{ "thread", 10, 1 },
}

function killnpc_greater_demon(player, npc, x, y)
	local ran = randomvar(128)

	local weight = 0

	addobject(player, "ashes", 1, x, y)

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
