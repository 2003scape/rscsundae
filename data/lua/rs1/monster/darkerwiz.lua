-- https://classic.runescape.wiki/w/Darkwizard#Drops_(Level_25)

local drops = {
	-- weapons and armour
	{ "staff", 1, 8 },
	{ "blackwizhat", 1, 4 },
	{ "black robe", 1, 3 },

	-- ammunition
	{ "earth-rune", 7, 3 },
	{ "earth-rune", 12, 2 },
	{ "earth-rune", 24, 4 },
	{ "nature-rune", 3, 6 },
	{ "nature-rune", 3, 6 },
	{ "air-rune", 7, 3 },
	{ "air-rune", 12, 2 },
	{ "body-rune", 7, 3 },
	{ "body-rune", 12, 2 },
	{ "fire-rune", 7, 3 },
	{ "fire-rune", 12, 2 },
	{ "mind-rune", 7, 3 },
	{ "mind-rune", 12, 2 },
	{ "water-rune", 7, 3 },
	{ "water-rune", 12, 2 },
	{ "blood-rune", 1, 2 },
	{ "law-rune", 2, 1 },
	{ "cosmic-rune", 1, 1 },

	-- coins
	{ "coins", 1, 17 },
	{ "coins", 2, 16 },
	{ "coins", 4, 9 },
	{ "coins", 29, 3 },
	{ "coins", 30, 1 },
}

function killnpc_darkerwiz(player, npc, x, y)
	local ran = randomvar(128)

	local weight = 0

	for _, drop in ipairs(drops) do
		weight = weight + drop[3]

		if ran < weight then
			-- https://github.com/2004Scape/Server/blob/3b31da7a1ee31d7fcd9f74f43eae7d5a1dbbd5a3/data/src/scripts/drop%20tables/scripts/dark_wizard.rs2#L47
			if drop[1] == "blood-rune" then
				addobject(player, "blackwizhat", 1, x, y)
				break
			else
				addobject(player, drop[1], drop[2], x, y)
				break
			end
		end
	end
end
