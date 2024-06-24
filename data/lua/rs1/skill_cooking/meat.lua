-- 1e_Luis/Quests/Witchs Potion.pcap
-- TODO: should be able to intentionally burn during witch's potion

local function cook_meat(player)
	thinkbubble(player, "rawmeat")
	remove(player, "rawmeat", 1)
	delay(3)
	-- https://oldschool.runescape.wiki/w/Cooked_meat?oldid=14686423
	if statrandom(player, STAT_COOKING, 128, 640) then
		mes(player, "@que@The meat is now nicely cooked")
		give(player, "cookedmeat", 1)
		advancestat(player, STAT_COOKING, 120, 0)
	else
		mes(player, "@que@You accidentally burn the meat")
		give(player, "burntmeat", 1)
	end
end

function useloc_range_rawmeat(player, x, y)
	mes(player, "@que@You cook the meat on the stove...")
	cook_meat(player)
end

function useloc_fire_rawmeat(player, x, y)
	mes(player, "@que@You cook the meat on the fire...")
	cook_meat(player)
end

register_useloc("fire", "rawmeat", useloc_fire_rawmeat)
register_useloc("range", "rawmeat", useloc_range_rawmeat)
