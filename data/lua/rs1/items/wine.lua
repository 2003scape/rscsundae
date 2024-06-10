-- ./rscplus-replays-master/RSC 2001/replays master archive/Stat effects/Consumable Stat effects/Alcohol/jug of wine- (level 1-1-1 base)

function drink_wine(player)
	thinkbubble("wine")
	mes(player, "@que@You drink the wine")
	mes(player, "@que@It makes you feel a bit dizzy")
	remove(player, "wine", 1)
	substat(player, STAT_ATTACK, 3, 0)
	healstat(player, STAT_HITS, 11, 0)
end

register_useobj("wine", drink_wine)
