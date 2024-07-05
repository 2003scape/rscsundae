-- RSC 2001/replays master archive/Skilling/Crafting/uncut sapphire- cut to sapphire.pcap
register_useinv("chisel", "uncut sapphire", function(player)
	if not statatleast(player, STAT_CRAFTING, 20) then
		-- 1e_Luis/Skilling/Crafting requirements and Observatory quick.pcap
		mes(player, "@que@you need a crafting level of 20 to cut sapphires")
		return
	end
	mes(player, "You cut the sapphire")
	remove(player, "uncut sapphire", 1)
	give(player, "sapphire", 1)
	advancestat(player, STAT_CRAFTING, 200, 0)
end)

-- RSC 2001/replays master archive/Skilling/Crafting/uncut emerald- cut to emerald.pcap
register_useinv("chisel", "uncut emerald", function(player)
	if not statatleast(player, STAT_CRAFTING, 27) then
		-- 1e_Luis/Skilling/Crafting requirements and Observatory quick.pcap
		mes(player, "@que@you need a crafting level of 27 to cut emeralds")
		return
	end
	mes(player, "You cut the emerald")
	remove(player, "uncut emerald", 1)
	give(player, "emerald", 1)
	advancestat(player, STAT_CRAFTING, 270, 0)
end)

-- RSC 2001/replays master archive/Skilling/Crafting/uncut ruby- cut to ruby.pcap
register_useinv("chisel", "uncut ruby", function(player)
	if not statatleast(player, STAT_CRAFTING, 34) then
		-- 1e_Luis/Skilling/Crafting requirements and Observatory quick.pcap
		mes(player, "@que@you need a crafting level of 34 to cut rubies")
		return
	end
	mes(player, "You cut the ruby")
	remove(player, "uncut ruby", 1)
	give(player, "ruby", 1)
	advancestat(player, STAT_CRAFTING, 340, 0)
end)

-- RSC 2001/replays master archive/Skilling/Crafting/uncut diamond- cut to diamond.pcap
register_useinv("chisel", "uncut diamond", function(player)
	if not statatleast(player, STAT_CRAFTING, 43) then
		-- 1e_Luis/Skilling/Crafting requirements and Observatory quick.pcap
		mes(player, "@que@you need a crafting level of 43 to cut diamonds")
		return
	end
	mes(player, "You cut the diamond")
	remove(player, "uncut diamond", 1)
	give(player, "diamond", 1)
	advancestat(player, STAT_CRAFTING, 430, 0)
end)
