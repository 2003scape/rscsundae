-- 1e_Luis/Quests/Witchs Potion.pcap

function useloc_range_cookedmeat(player, x, y)
	local stage = getvar(player, "hetty_stage")
	if stage == 1 then
		mes(player, "@que@You cook the meat on the stove...")
		remove(player, "cookedmeat", 1)
		give(player, "burntmeat", 1)
		mes(player, "@que@you burn the meat")
	else
		mes(player, "Nothing interesting happens")
	end
end

function useloc_fire_cookedmeat(player, x, y)
	local stage = getvar(player, "hetty_stage")
	if stage == 1 then
		mes(player, "@que@You cook the meat on the fire...")
		remove(player, "cookedmeat", 1)
		give(player, "burntmeat", 1)
		mes(player, "@que@you burn the meat")
	else
		mes(player, "Nothing interesting happens")
	end
end

register_useloc("fire", "cookedmeat", useloc_fire_cookedmeat)
register_useloc("range", "cookedmeat", useloc_range_cookedmeat)
