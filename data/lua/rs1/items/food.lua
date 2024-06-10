-- https://classic.runescape.wiki/w/Food
-- ./rscplus-replays-master/1e_Luis/Effect testing/Fish food heals
-- ./rscplus-replays-master/Logg/Tylerbeg/06-17-2018 21.12.28 making prototype dart after finishing Tourist Trap, buy some rope, talking to Basu, eat tomato and cheese, thief 35 to 36

function eat_simple(player, type, amount)
	mes(player, string.format("@que@You eat the %s", type))
	remove(player, type, 1)
	if statdown(player, STAT_HITS) then
		mes(player, "@que@It heals some health")
		healstat(player, STAT_HITS, amount, 0)
	end
end

function eat_anchovies(player)
	eat_simple(player, "anchovies", 1)
end

register_useobj("anchovies", eat_anchovies)

function eat_shrimp(player)
	eat_simple(player, "shrimp", 3)
end

register_useobj("shrimp", eat_shrimp)

function eat_sardine(player)
	eat_simple(player, "sardine", 4)
end

register_useobj("sardine", eat_sardine)

function eat_herring(player)
	eat_simple(player, "herring", 5)
end

register_useobj("herring", eat_herring)

function eat_trout(player)
	eat_simple(player, "trout", 7)
end

register_useobj("trout", eat_trout)

function eat_pike(player)
	eat_simple(player, "pike", 8)
end

register_useobj("pike", eat_pike)

function eat_salmon(player)
	eat_simple(player, "salmon", 9)
end

register_useobj("salmon", eat_salmon)

function eat_tuna(player)
	eat_simple(player, "tuna", 10)
end

register_useobj("tuna", eat_tuna)

function eat_lobster(player)
	eat_simple(player, "lobster", 12)
end

register_useobj("lobster", eat_lobster)

function eat_swordfish(player)
	eat_simple(player, "swordfish", 14)
end

register_useobj("swordfish", eat_swordfish)

function eat_tomato(player)
	eat_simple(player, "tomato", 2)
end

register_useobj("tomato", eat_tomato)

function eat_cheese(player)
	eat_simple(player, "cheese", 2)
end

register_useobj("cheese", eat_cheese)

function eat_banana(player)
	eat_simple(player, "banana", 2)
end

register_useobj("banana", eat_banana)

function eat_chocolate(player)
	eat_simple(player, "chocolate bar", 3)
end

register_useobj("chocolate bar", eat_chocolate)

function eat_bread(player)
	eat_simple(player, "bread", 4)
end

register_useobj("bread", eat_bread)

function eat_stew(player)
	eat_simple(player, "stew", 9)
end

register_useobj("stew", eat_stew)
