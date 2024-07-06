-- https://classic.runescape.wiki/w/Ice_warrior#Drops

local drops = {
	-- weapons and armour
	{ "iron battle axe", 1, 3 },
	{ "mithril mace", 1, 1 },

	-- ammunition
	{ "nature-rune", 3, 10 },
	{ "chaos-rune", 2, 8 },
	{ "law-rune", 1, 7 },
	{ "cosmic-rune", 1, 5 },
	{ "death-rune", 1, 3 },

	-- coins
	{ "coins", 15, 57 },

	-- other
	{ "randomherb", 1, 10 },
	{ "randomjewel", 1, 3 },
}

function killnpc_ice_warrior(player, npc, x, y)
	local ran = randomvar(128)

	local weight = 0

	addobject(player, "bones", 1, x, y)

	for _, drop in ipairs(drops) do
		weight = weight + drop[3]

		if ran < weight then
			if drop[1] == "randomjewel" then
				randomjewel(player, x, y)
				break
			elseif drop[1] == "randomherb" then
				randomherb(player, x, y)
				break
			else
				addobject(player, drop[1], drop[2], x, y)
				break
			end
		end
	end
end
