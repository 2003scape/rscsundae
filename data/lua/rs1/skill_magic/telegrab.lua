-- RSC 2001/replays master archive/Skilling/Magic/spell- law- telekinetic grab (basics, range tests).pcap

register_spellobj("ghostskull", "telekinetic grab", function(player, item, x, y)
	mes(player, "@que@I can't use telekinetic grab on this object")
end)

register_spellobj("_", "telekinetic grab", function(player, item, x, y)
	mes(player, "@que@Cast spell successfully")
	takeobject(player, item, x, y)
	advancestat(player, STAT_MAGIC, 344, 0)
	if not worn(player, "staff of air") then
		remove(player, "air-rune", 1)
	end
	remove(player, "law-rune", 1)
	showeffect(1, x, y)
end)
