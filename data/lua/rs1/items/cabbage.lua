-- ./rscplus-replays-master/RSC 2001/3/f2p (not 100% complete)/cabbage- draynor manor- consume
-- ./rscplus-replays-master/RSC 2001/replays master archive/Stat effects/Consumable Stat effects/Healing/cabbage- consume- no health restored shown

function opinv_normalcabbage(player)
	mes(player, "@que@You eat the cabbage. Yuck!")
	remove(player, "cabbage", 1)
	if statdown(player, STAT_HITS) then
		healstat(player, STAT_HITS, 1, 0)
		mes(player, "@que@It heals some health anyway")
	end
end

function opinv_magic_cabbage(player)
	mes(player, "@que@You eat the cabbage")
	mes(player, "@que@It seems to taste nicer than normal")
	remove(player, "magic cabbage", 1)
	addstat(player, STAT_DEFENSE, 1, 2)
	healstat(player, STAT_HITS, 1, 0)
end
