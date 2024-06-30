-- 1e_Luis/Quests/Witchs Potion.pcap

local function cook_meat(player, low, high)
	thinkbubble(player, "rawmeat")
	remove(player, "rawmeat", 1)
	delay(3)
	if statrandom(player, STAT_COOKING, low, high) then
		mes(player, "@que@The meat is now nicely cooked")
		give(player, "cookedmeat", 1)
		advancestat(player, STAT_COOKING, 120, 0)
	else
		mes(player, "@que@You accidentally burn the meat")
		give(player, "burntmeat", 1)
	end
end

-- https://oldschool.runescape.wiki/w/Cooked_meat?oldid=14686423&action=edit

register_useloc("fire", "rawmeat", function(player, x, y)
	mes(player, "@que@You cook the meat on the fire...")
	cook_meat(player, 128, 512)
end)

register_useloc("range", "rawmeat", function(player, x, y)
	mes(player, "@que@You cook the meat on the stove...")
	cook_meat(player, 128, 512)
end)

register_useloc("cookrange", "rawmeat", function(player, x, y)
	if check_cookrange(player) then
		mes(player, "@que@You cook the meat on the stove...")
		cook_meat(player, 138, 532)
	end
end)
