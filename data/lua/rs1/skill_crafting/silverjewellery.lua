register_useloc("furnace", "silver bar", function(player, x, y)
	if not statatleast(player, STAT_CRAFTING, 16) then
		-- 1e_Luis/Skilling/Crafting requirements and Observatory quick.pcap
		mes(player, "@que@You need a crafting skill of level 16 to make this")
		return
	end
	if not held(player, "holy symbol mould", 1) then
		-- Logg/Tylerbeg/06-06-2018 07.15.52 some crafting of unholy symbols.pcap
		mes(player, "@que@You need a Holy symbol mould to make a holy symbol!")
		return
	end
	-- RSC 2001/replays master archive/Skilling/Crafting/smelt silver- unstrung holy symbol- success.pcap
	thinkbubble(player, "silver bar")
	mes(player, "@que@You make a Holy symbol of Saradomin")
	remove(player, "silver bar", 1)
	give(player, "nostringstar", 1)
	advancestat(player, STAT_CRAFTING, 200, 0)
end)

register_useinv("nostringstar", "ball of wool", function(player)
	-- adapted from:
	-- RSC 2001/replays master archive/Skilling/Crafting/stringing- unblessed holy symbol.pcap
	-- (which actually shows an unholy symbol, stringing the holy symbol
	-- is lost information)
	mes(player, "You put some string on your holy symbol")
	remove(player, "ball of wool", 1)
	remove(player, "nostringstar", 1)
	give(player, "stringstar", 1)
end)
