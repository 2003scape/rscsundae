-- rscplus-replays-master/RSC 2001/replays master archive/Stat effects/Consumable Stat effects/Healing/spinach roll- consume- heal hp shown- (+2)

function eat_spinach_roll(player)
	mes(player, "@que@You eat the spinach roll")
	remove(player, "spinach roll", 1)
	if statdown(player, STAT_HITS) then
		healstat(player, STAT_HITS, 2, 0)
		mes(player, "@que@It tastes a bit weird, but fills you up")
	end
end

register_useobj("spinach roll", eat_spinach_roll)
