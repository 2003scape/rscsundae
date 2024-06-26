-- RSC 2001/3/f2p (not 100% complete)/scenery- varrock- dummy- fail- there is nothing more you can learn.pcap
-- https://classic.runescape.wiki/w/Dummy

function oploc1_dummy(player)
	mes(player, "@que@You swing at the dummy")
	delay(3)
	mes(player, "@que@You hit the dummy")
	if not statatleast(player, STAT_ATTACK, 8) then
		-- https://oldschool.runescape.wiki/w/Dummy?oldid=14632042#Varrock
		advancestat(player, STAT_ATTACK, 20, 0)
	else
		mes(player, "@que@There is nothing more you can learn from hitting a dummy")
	end
end
