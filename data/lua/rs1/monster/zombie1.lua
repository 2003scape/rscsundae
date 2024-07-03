-- https://classic.runescape.wiki/w/Zombie#Drops_(Level_19)

local drops = {
	-- weapons and armour
	{ "medium bronze helmet", 1, 4 },
	{ "bronze long sword", 1, 1 },
	{ "iron axe", 1, 1 },

	-- ammunition
	{ "body-rune", 4, 5 },
	{ "mind-rune", 3, 5 },
	{ "air-rune", 9, 4 },
	{ "nature-rune", 4, 1 },
	{ "arrows", 8, 4 },

	-- coins
	{ "coins", 10, 11 },
	{ "coins", 4, 4 },
	{ "coins", 18, 3 },
	{ "coins", 13, 2 },
	{ "coins", 28, 2 },

	-- other
	{ "randomherb", 1, 25 },
	{ "fishing bait", 5, 46 },
	{ "copper ore", 1, 2 },
}

function killnpc_zombie1(player, npc, x, y)
	local ran = randomvar(128)

	local weight = 0

	addobject(player, "bones", 1, x, y)

	for _, drop in ipairs(drops) do
		weight = weight + drop[3]

		if ran < weight then
			if drop[1] == "randomherb" then
				randomherb(player, x, y)
				break
			else
				addobject(player, drop[1], drop[2], x, y)
				break
			end
		end
	end
end
