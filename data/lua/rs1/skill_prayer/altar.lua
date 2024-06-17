-- ./RSC 2001/replays master archive/Scenery/scenery- altar- port sarim church- pray (from 5pp to 29-max-pp).pcap
-- ./RSC 2001/replays master archive/Scenery/scenery- altar- recharge while praying.pcap
function oploc1_altar(player, x, y)
	if statdown(player, STAT_PRAYER) then
		mes(player, "@que@You recharge your prayer points")
		healstat(player, STAT_PRAYER, 100, 0)
	else
		mes(player, "@que@You already have full prayer points")
	end
end

function oploc1_chaos_altar(player, x, y)
	oploc1_altar(player, x, y)
end

-- ./RSC 2001/replays master archive/Scenery/scenery- monastery- monk's altar (unique)- full pp- recharge- (+2 bonus pp).pcap
function oploc1_monks_altar(player, x, y)
	if statdown(player, STAT_PRAYER) then
		mes(player, "@que@You recharge your prayer points")
		healstat(player, STAT_PRAYER, 100, 0)
	end
	addstat(player, STAT_PRAYER, 2, 0)
end
