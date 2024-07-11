-- 1e_Luis/Skilling/Enchanting on unenchantable jewelry.pcap

register_spellinv("_", "enchant lvl-1 amulet", function(player, item)
	mes(player, "@que@This spell can only be used on unenchanted sapphire amulets")
end)

register_spellinv("_", "enchant lvl-2 amulet", function(player, item)
	mes(player, "@que@This spell can only be used on unenchanted emerald amulets")
end)

register_spellinv("_", "enchant lvl-3 amulet", function(player, item)
	mes(player, "@que@This spell can only be used on unenchanted ruby amulets")
end)

register_spellinv("_", "enchant lvl-4 amulet", function(player, item)
	mes(player, "@que@This spell can only be used on unenchanted diamond amulets")
end)

register_spellinv("stringsaph", "enchant lvl-1 amulet", function(player, item)
	-- RSC 2001/replays master archive/Skilling/Magic/spell- cosmic- enchant lvl 1.pcap
	mes(player, "@que@You succesfully enchant the amulet")
	remove(player, "stringsaph", 1)
	give(player, "enchantsaph", 1)
	advancestat(player, STAT_MAGIC, 136, 0)
	if not worn(player, "staff of water") then
		remove(player, "water-rune", 1)
	end
	remove(player, "cosmic-rune", 1)
end)

register_spellinv("stringem", "enchant lvl-2 amulet", function(player, item)
	-- RSC 2001/replays master archive/Skilling/Magic/spell- cosmic- enchant lvl 2.pcap
	mes(player, "@que@You succesfully enchant the amulet")
	remove(player, "stringem", 1)
	give(player, "enchantem", 1)
	advancestat(player, STAT_MAGIC, 296, 0)
	if not worn(player, "staff of air") then
		remove(player, "air-rune", 3)
	end
	remove(player, "cosmic-rune", 1)
end)

register_spellinv("stringrub", "enchant lvl-3 amulet", function(player, item)
	-- RSC 2001/replays master archive/Skilling/Magic/spell- cosmic- enchant lvl 3.pcap
	mes(player, "@que@You succesfully enchant the amulet")
	remove(player, "stringrub", 1)
	give(player, "enchantrub", 1)
	advancestat(player, STAT_MAGIC, 472, 0)
	if not worn(player, "staff of fire") then
		remove(player, "fire-rune", 5)
	end
	remove(player, "cosmic-rune", 1)
end)

register_spellinv("stringdia", "enchant lvl-4 amulet", function(player, item)
	-- RSC 2001/replays master archive/Skilling/Magic/spell- cosmic- enchant lvl 4.pcap
	mes(player, "@que@You succesfully enchant the amulet")
	remove(player, "stringdia", 1)
	give(player, "enchantdia", 1)
	advancestat(player, STAT_MAGIC, 536, 0)
	if not worn(player, "staff of earth") then
		remove(player, "earth-rune", 10)
	end
	remove(player, "cosmic-rune", 1)
end)
