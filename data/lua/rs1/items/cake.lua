-- https://web.archive.org/web/20020204224007/http://www.jagex.com/manual/cooking.html
-- messages came from grepping pcaps

function eat_cake(player)
	remove(player, "cake", 1)
	give(player, "partial cake", 1)
	mes(player, "@que@You eat part of the cake")
	if statdown(player, STAT_HITS) then
		healstat(player, STAT_HITS, 4, 0)
		mes(player, "@que@It heals some health")
	end
end

function eat_cake_partial(player)
	remove(player, "partial cake", 1)
	give(player, "slice of cake", 1)
	mes(player, "@que@You eat some more of the cake")
	if statdown(player, STAT_HITS) then
		healstat(player, STAT_HITS, 4, 0)
		mes(player, "@que@It heals some health")
	end
end

function eat_cake_slice(player)
	remove(player, "slice of cake", 1)
	mes(player, "@que@You eat the slice of cake")
	if statdown(player, STAT_HITS) then
		healstat(player, STAT_HITS, 4, 0)
		mes(player, "@que@It heals some health")
	end
end

function eat_choc_cake(player)
	remove(player, "chocolate cake", 1)
	give(player, "partial chocolate cake", 1)
	mes(player, "@que@You eat part of the chocolate cake")
	if statdown(player, STAT_HITS) then
		healstat(player, STAT_HITS, 5, 0)
		mes(player, "@que@It heals some health")
	end
end

function eat_choc_cake_partial(player)
	remove(player, "partial chocolate cake", 1)
	give(player, "chocolate slice", 1)
	mes(player, "@que@You eat some more of the chocolate cake")
	if statdown(player, STAT_HITS) then
		healstat(player, STAT_HITS, 5, 0)
		mes(player, "@que@It heals some health")
	end
end

function eat_choc_cake_slice(player)
	remove(player, "chocolate slice", 1)
	mes(player, "@que@You eat the chocolate slice")
	if statdown(player, STAT_HITS) then
		healstat(player, STAT_HITS, 5, 0)
		mes(player, "@que@It heals some health")
	end
end

register_useobj("cake", eat_cake)
register_useobj("partial cake", eat_cake_partial)
register_useobj("slice of cake", eat_cake_slice)
register_useobj("chocolate cake", eat_choc_cake)
register_useobj("partial chocolate cake", eat_choc_cake_partial)
register_useobj("chocolate slice", eat_choc_cake_slice)
