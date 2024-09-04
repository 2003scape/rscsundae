-- https://classic.runescape.wiki/w/Darkwizard#Drops_(Level_13)

local drops = {
	-- weapons and armour
	{ "black robe", 1, 10 },
	{ "staff", 1, 8 },
	{ "blackwizhat", 1, 6 },

	-- ammunition
	{ "chaos-rune", 1, 8 },
	{ "nature-rune", 1, 8 },
	{ "air-rune", 3, 3 },
	{ "air-rune", 8, 2 },
	{ "body-rune", 3, 3 },
	{ "body-rune", 8, 2 },
	{ "earth-rune", 3, 3 },
	{ "earth-rune", 8, 2 },
	{ "fire-rune", 3, 3 },
	{ "fire-rune", 8, 2 },
	{ "mind-rune", 3, 3 },
	{ "mind-rune", 8, 2 },
	{ "water-rune", 3, 3 },
	{ "water-rune", 8, 2 },
	{ "law-rune", 1, 1 },
	{ "blood-rune", 1, 1 },

	-- coins
	{ "coins", 1, 24 },
	{ "coins", 2, 9 },
	{ "coins", 18, 7 },
	{ "coins", 30, 1 },
}

function killnpc_wiz1(player, npc, x, y)
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
