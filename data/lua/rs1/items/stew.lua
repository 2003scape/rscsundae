-- pcaps/1e_Luis/Quests/Underground Pass/Underground Pass Part2.5.pcap
-- https://web.archive.org/web/20020204224007/http://www.jagex.com/manual/cooking.html

function opinv_stew(player)
	remove(player, "stew", 1)
	give(player, "bowl", 1)
	mes(player, "@que@You eat the stew")
	if statdown(player, STAT_HITS) then
		healstat(player, STAT_HITS, 9, 0)
		mes(player, "@que@It heals some health")
	end
end
