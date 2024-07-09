-- https://classic.runescape.wiki/w/Moss_Giant#Drops

local drops = {
	-- weapons and armour
	{ "black square shield", 1, 5 },
	{ "magic staff", 1, 2 },
	{ "medium steel helmet", 1, 2 },
	{ "mithril short sword", 1, 2 },
	{ "steel kite shield", 1, 1 },

	-- runes
	{ "law-rune", 2, 4 },
	{ "air-rune", 12, 3 },
	{ "chaos-rune", 5, 3 },
	{ "earth-rune", 18, 3 },
	{ "nature-rune", 4, 3 },
	{ "cosmic-rune", 2, 2 },
	{ "blood-rune", 1, 1 },
	{ "death-rune", 2, 1 },

	-- coins
	{ "coins", 82, 35 },
	{ "coins", 37, 19 },
	{ "coins", 15, 11 },
	{ "coins", 119, 10 },
	{ "coins", 5, 2 },
	{ "coins", 300, 2 },

	-- other
	{ "steel bar", 1, 6 },
	{ "randomherb", 1, 5 },
	{ "randomjewel", 1, 4 },
	{ "brass necklace", 1, 1 },
	{ "coal", 1, 1 },
	{ "spinach roll", 1, 1 },
}

function killnpc_moss_giant(player, npc, x, y)
	local ran = randomvar(128)

	local weight = 0

	addobject(player, "big bones", 1, x, y)

	for _, drop in ipairs(drops) do
		weight = weight + drop[3]

		if ran < weight then
			if drop[1] == "randomherb" then
				randomherb(player, x, y)
				break
			elseif drop[1] == "randomjewel" then
				randomjewel(player, x, y)
				break
			elseif drop[1] == "blood-rune" then
				-- https://github.com/2004Scape/Server/blob/b724dfcd9fe3b1f2a1de254394ccb64ad9a5a59c/data/src/scripts/drop%20tables/scripts/moss_giant.rs2#L4
				addobject(player, "coins", 2, x, y)
				break
			else
				addobject(player, drop[1], drop[2], x, y)
				break
			end
		end
	end
end
