-- Logg/Tylerbeg/07-19-2018 10.15.33 more underground pass.pcap
-- https://web.archive.org/web/20020204224007/http://www.jagex.com/manual/cooking.html

function eat_pizza(player)
	remove(player, "plain pizza", 1)
	mes(player, "@que@You eat the pizza")
	if statdown(player, STAT_HITS) then
		healstat(player, STAT_HITS, 11, 0)
		mes(player, "@que@It heals some health")
	end
end

function eat_meat_pizza(player)
	remove(player, "meat pizza", 1)
	give(player, "half meat pizza", 1)
	mes(player, "@que@You eat half of the pizza")
	if statdown(player, STAT_HITS) then
		healstat(player, STAT_HITS, 7, 0)
		mes(player, "@que@It heals some health")
	end
end

function eat_half_meat_pizza(player)
	remove(player, "half meat pizza", 1)
	mes(player, "@que@You eat half of the pizza")
	if statdown(player, STAT_HITS) then
		healstat(player, STAT_HITS, 7, 0)
		mes(player, "@que@It heals some health")
	end
end

function eat_anchovie_pizza(player)
	remove(player, "anchovie pizza", 1)
	give(player, "half anchovie pizza", 1)
	mes(player, "@que@You eat half of the pizza")
	if statdown(player, STAT_HITS) then
		healstat(player, STAT_HITS, 8, 0)
		mes(player, "@que@It heals some health")
	end
end

function eat_half_anchovie_pizza(player)
	remove(player, "half anchovie pizza", 1)
	mes(player, "@que@You eat half of the pizza")
	if statdown(player, STAT_HITS) then
		healstat(player, STAT_HITS, 8, 0)
		mes(player, "@que@It heals some health")
	end
end

register_useobj("plain pizza", eat_pizza)
register_useobj("meat pizza", eat_meat_pizza)
register_useobj("half meat pizza", eat_half_meat_pizza)
register_useobj("anchovie pizza", eat_anchovie_pizza)
register_useobj("half anchovie pizza", eat_half_anchovie_pizza)
