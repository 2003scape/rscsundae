-- ./rscplus-replays-master/RSC 2001/replays master archive/Stat effects/Consumable Stat effects/Alcohol/jug of wine- (level 1-1-1 base)

function opinv_wine(player)
	thinkbubble(player, "wine")
	mes(player, "@que@You drink the wine")
	mes(player, "@que@It makes you feel a bit dizzy")
	remove(player, "wine", 1)
	substat(player, STAT_ATTACK, 3, 0)
	if statdown(player, STAT_HITS) then
		-- Logg/Tylerbeg/07-26-2018 11.44.01 mapping the kharazi jungle.pcap
		healstat(player, STAT_HITS, 11, 0)
		mes(player, "@que@But it heals some health")
	end
end
