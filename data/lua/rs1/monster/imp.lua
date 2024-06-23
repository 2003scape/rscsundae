-- https://classic.runescape.wiki/w/Imp#Drops

local drops = {
	-- beads
	{ "black bead", 1, 5 },
	{ "red bead", 1, 5 },
	{ "white bead", 1, 5 },
	{ "yellow bead", 1, 5 },

	-- ammunition
	{ "crossbow bolts", 1, 8 },

	-- foodstuffs
	{ "grain", 1, 10 },
	{ "egg", 1, 5 },
	{ "raw chicken", 1, 5 },
	{ "burntbread", 1, 4 },
	{ "burntmeat", 1, 4 },
	{ "bread dough", 1, 2 },
	{ "cabbage", 1, 2 },
	{ "flour", 1, 2 },
	{ "bread", 1, 1 },
	{ "cookedmeat", 1, 1 },

	-- other
	{ "ball of wool", 1, 8 },
	{ "hammer", 1, 8 },
	{ "bluewizhat", 1, 8 },
	{ "ashes", 1, 6 },
	{ "tinderbox", 1, 5 },
	{ "bucket", 1, 4 },
	{ "cadavaberries", 1, 4 },
	{ "clay", 1, 4 },
	{ "shears", 1, 4 },
	{ "chefshat", 1, 2 },
	{ "flier", 1, 2 },
	{ "jug", 1, 2 },
	{ "pot", 1, 2 },
	{ "waterbucket", 1, 2 },
	{ "waterjug", 1, 2 },
	{ "acnepotion", 1, 1 }
}

function killnpc_imp(player, npc, x, y)
	local ran = randomvar(128)

	local weight = 0

	for _, drop in ipairs(drops) do
		weight = weight + drop[3]

		if ran < weight then
			addobject(player, drop[1], drop[2], x, y)
			break
		end
	end
end
