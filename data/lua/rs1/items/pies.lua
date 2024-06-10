-- https://web.archive.org/web/20020204224007/http://www.jagex.com/manual/cooking.html
-- messages came from grepping pcaps

function eat_redberry_pie(player)
	remove(player, "redberry pie", 1)
	give(player, "half a redberry pie", 1)
	mes(player, "@que@You eat half of a redberry pie")
	if statdown(player, STAT_HITS) then
		healstat(player, STAT_HITS, 3, 0)
		mes(player, "@que@It heals some health")
	end
end

function eat_redberry_pie_half(player)
	remove(player, "half a redberry pie", 1)
	mes(player, "@que@You eat half of a redberry pie")
	if statdown(player, STAT_HITS) then
		healstat(player, STAT_HITS, 3, 0)
		mes(player, "@que@It heals some health")
	end
end

function eat_meat_pie(player)
	remove(player, "meat pie", 1)
	give(player, "half a meat pie", 1)
	mes(player, "@que@You eat half of a meat pie")
	if statdown(player, STAT_HITS) then
		healstat(player, STAT_HITS, 4, 0)
		mes(player, "@que@It heals some health")
	end
end

function eat_meat_pie_half(player)
	remove(player, "half a meat pie", 1)
	mes(player, "@que@You eat half of a meat pie")
	if statdown(player, STAT_HITS) then
		healstat(player, STAT_HITS, 4, 0)
		mes(player, "@que@It heals some health")
	end
end

function eat_apple_pie(player)
	remove(player, "apple pie", 1)
	give(player, "half an apple pie", 1)
	mes(player, "@que@You eat half of an apple pie")
	if statdown(player, STAT_HITS) then
		healstat(player, STAT_HITS, 5, 0)
		mes(player, "@que@It heals some health")
	end
end

function eat_apple_pie_half(player)
	remove(player, "half an apple pie", 1)
	mes(player, "@que@You eat half of an apple pie")
	if statdown(player, STAT_HITS) then
		healstat(player, STAT_HITS, 5, 0)
		mes(player, "@que@It heals some health")
	end
end

register_useobj("redberry pie", eat_redberry_pie)
register_useobj("meat pie", eat_meat_pie)
register_useobj("apple pie", eat_apple_pie)
register_useobj("half a redberry pie", eat_redberry_pie_half)
register_useobj("half a meat pie", eat_meat_pie_half)
register_useobj("half an apple pie", eat_apple_pie_half)
