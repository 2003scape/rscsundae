-- https://classic.runescape.wiki/w/Skeleton#Drops_(level_31_and_54)

local drops = {
	-- weapons and armour
	{ "medium iron helmet", 1, 6 },
	{ "iron short sword", 1, 4 },
	{ "iron axe", 1, 2 },
	{ "bronze chain mail body", 1, 1 },
	{ "iron scimitar", 1, 1 },

	-- runes
	{ "air-rune", 10, 3 },
	{ "chaos-rune", 3, 3 },
	{ "water-rune", 6, 3 },
	{ "law-rune", 1, 2 },
	{ "cosmic-rune", 1, 1 },

	-- coins
	{ "coins", 5, 25 },
	{ "coins", 10, 24 },
	{ "coins", 25, 8 },
	{ "coins", 45, 4 },
	{ "coins", 65, 3 },
	{ "coins", 1, 2 },

	-- other
	{ "randomherb", 1, 20 },
	{ "bronze bar", 1, 5 },
	{ "randomjewel", 1, 1 },
}

function killnpc_skeleton3(player, npc, x, y)
	local ran = randomvar(128)

	local weight = 0

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

-- https://classic.runescape.wiki/w/Talk:Zombie#Drops_%2832%29
function killnpc_zombie3(player, npc, x, y)
	killnpc_skeleton3(player, npc, x, y)
end
