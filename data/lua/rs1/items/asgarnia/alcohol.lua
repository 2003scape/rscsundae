-- ./rscplus-replays-master/RSC 2001/replays master archive/Stat effects/Consumable Stat effects/Alcohol/wizards mindbomb- (level 99-99-99)magic99
-- ./rscplus-replays-master/RSC 2001/replays master archive/Stat effects/Consumable Stat effects/Alcohol/dwarven stout- (level 99-97-99 base)99mining
-- ./rscplus-replays-master/RSC 2001/replays master archive/Stat effects/Consumable Stat effects/Alcohol/asgarnian ale- (level 99-97-99 base)
-- https://classic.runescape.wiki/w/Wizard's%20Mind_Bomb
-- https://classic.runescape.wiki/w/Dwarven_Stout
-- https://classic.runescape.wiki/w/Asgarnian_Ale

function opinv_wizards_mind_bomb(player)
	thinkbubble(player, "wizard's mind bomb")
	remove(player, "wizard's mind bomb", 1)
	mes(player, "@que@you drink the Wizard's Mind Bomb")
	mes(player, "@que@You feel very strange")
	substat(player, STAT_ATTACK, 1, 5)
	substat(player, STAT_DEFENSE, 1, 5)
	substat(player, STAT_STRENGTH, 1, 5)
	addstat(player, STAT_MAGIC, 1, 2)
	healstat(player, STAT_HITS, 1, 0)
end

function opinv_dwarven_stout(player)
	thinkbubble(player, "dwarven stout")
	remove(player, "dwarven stout", 1)
	mes(player, "@que@You drink the Dwarven Stout")
	mes(player, "@que@It tastes foul")
	substat(player, STAT_ATTACK, 1, 5)
	substat(player, STAT_DEFENSE, 1, 5)
	substat(player, STAT_STRENGTH, 1, 5)
	addstat(player, STAT_MINING, 1, 0)
	addstat(player, STAT_SMITHING, 1, 0)
	healstat(player, STAT_HITS, 1, 0)
	delay(1)
	mes(player, "@que@It tastes pretty strong too")
end

function opinv_asgarnian_ale(player)
	thinkbubble(player, "asgarnian ale")
	remove(player, "asgarnian ale", 1)
	mes(player, "@que@You drink the Ale")
	mes(player, "@que@You feel slightly reinvigorated")
	healstat(player, STAT_HITS, 1, 0)
	mes(player, "@que@And slightly dizzy too")
	substat(player, STAT_ATTACK, 1, 5)
	addstat(player, STAT_STRENGTH, 2, 0)
end

register_opinv("wizard's mind bomb", drink_mindbomb)
