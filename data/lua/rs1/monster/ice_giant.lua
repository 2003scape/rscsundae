-- https://classic.runescape.wiki/w/Ice_Giant#Drops

local drops = {
	-- weapons and armour
	{ "iron 2-handed sword", 1, 5 },
	{ "black kite shield", 1, 4 },
	{ "steel axe", 1, 4 },
	{ "steel short sword", 1, 4 },
	{ "iron plated skirt", 1, 1 },
	{ "mithril mace", 1, 1 },
	{ "mithril square shield", 1, 1 },

	-- runes
	{ "nature-rune", 4, 4 },
	{ "body-rune", 25, 3 },
	{ "mind-rune", 16, 3 },
	{ "law-rune", 2, 2 },
	{ "blood-rune", 1, 1 },
	{ "cosmic-rune", 3, 1 },
	{ "death-rune", 2, 1 },
	{ "water-rune", 8, 1 },

	-- coins
	{ "coins", 117, 40 },
	{ "coins", 53, 12 },
	{ "coins", 196, 10 },
	{ "coins", 8, 7 },
	{ "coins", 22, 6 },
	{ "coins", 400, 2 },

	-- other
	{ "randomjewel", 1, 4 },
	{ "wine", 1, 3 },
	{ "banana", 1, 1 },
	{ "mithril ore", 1, 1 },
}

function killnpc_ice_giant(player, npc, x, y)
	local ran = randomvar(128)

	local weight = 0

	for _, drop in ipairs(drops) do
		weight = weight + drop[3]

		if ran < weight then
			if drop[1] == "randomjewel" then
				randomjewel(player, x, y)
				break
			elseif drop[1] == "blood-rune" then
				-- https://github.com/2004Scape/Server/blob/ea12070508e42e6fac1092a77f18fe74200bc051/data/src/scripts/drop%20tables/scripts/ice_giant.rs2#L43
				addobject(player, "black kite shield", 1, x, y)
				break
			else
				addobject(player, drop[1], drop[2], x, y)
				break
			end
		end
	end
end
