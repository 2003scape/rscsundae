-- ./rscplus-replays-master/rsc-preservation.xyz/Tutorial Island/Tutorial Island 03 06 2018

function useobj_cookedmeat(player)
	mes(player, "@que@You eat the meat")
	remove(player, "cookedmeat", 1)
	if statdown(player, STAT_HITS) then
		healstat(player, STAT_HITS, 3, 0)
		mes(player, "@que@It heals some health")
	end
end
