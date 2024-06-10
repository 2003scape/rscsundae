-- ./rscplus-replays-master/RSC 2001/replays master archive/Stat effects/Consumable Stat effects/Alcohol/beer- (level 99-99-99 base)

function drink_beer(player)
	thinkbubble(player, "beer")
	mes(player, "@que@You drink the beer")
	mes(player, "@que@You feel slightly reinvigorated")
	mes(player, "@que@And slightly dizzy too")
	remove(player, "beer", 1)
	substat(player, STAT_ATTACK, 1, 5)
	addstat(player, STAT_STRENGTH, 1, 2)
	healstat(player, STAT_HITS, 1, 0)
end

register_useobj("beer", drink_beer)
