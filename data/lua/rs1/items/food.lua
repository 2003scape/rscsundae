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

function opinv_anchovies(player)
	eat_simple(player, "anchovies", 1)
end

function opinv_shrimp(player)
	eat_simple(player, "shrimp", 3)
end

function opinv_sardine(player)
	eat_simple(player, "sardine", 4)
end

function opinv_herring(player)
	eat_simple(player, "herring", 5)
end

function opinv_trout(player)
	eat_simple(player, "trout", 7)
end

function opinv_pike(player)
	eat_simple(player, "pike", 8)
end

function opinv_salmon(player)
	eat_simple(player, "salmon", 9)
end

function opinv_tuna(player)
	eat_simple(player, "tuna", 10)
end

function opinv_lobster(player)
	eat_simple(player, "lobster", 12)
end

function opinv_swordfish(player)
	eat_simple(player, "swordfish", 14)
end

function opinv_tomato(player)
	eat_simple(player, "tomato", 2)
end

function opinv_cheese(player)
	eat_simple(player, "cheese", 2)
end

function opinv_banana(player)
	eat_simple(player, "banana", 2)
end

function opinv_chocolate_bar(player)
	eat_simple(player, "chocolate bar", 3)
end

function opinv_bread(player)
	eat_simple(player, "bread", 4)
end
