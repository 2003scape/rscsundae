-- ./rscplus-replays-master/RSC 2001/replays master archive/Stat effects/Consumable Stat effects/Alcohol/beer- (level 99-99-99 base)

function opinv_beer(player)
	thinkbubble(player, "beer")
	remove(player, "beer", 1)
	mes(player, "@que@You drink the beer")
	mes(player, "@que@You feel slightly reinvigorated")
	healstat(player, STAT_HITS, 1, 0)
	mes(player, "@que@And slightly dizzy too")
	substat(player, STAT_ATTACK, 1, 5)
	addstat(player, STAT_STRENGTH, 1, 2)
end
