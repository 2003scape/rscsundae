-- Zephyr (redacted chat) replays/1m d4 r4ng3r/06-14-2018 10.32.00 testing muddy chest bug.pcap
-- Zephyr (redacted chat) replays/1m d4 r4ng3r/06-14-2018 10.54.23 two people can get muddy chest at the same time, kill a couple black dragons

function oploc1_shutgorkachest(player, x, y)
	mes(player, "@que@the chest is locked")
end

function useloc_shutgorkachest_gorkakey(player, x, y)
	-- no idea why the messages are so inconsistent
	mes(player, "@que@you unlock the chest with your key")
	delay(3)
	remove(player, "gorkakey", 1)
	mes(player, "You find some treasure in the chest")
	give(player, "uncut ruby", 1)
	give(player, "mithril bar", 1)
	give(player, "law-rune", 2)
	give(player, "anchovie pizza", 1)
	give(player, "mithril dagger", 1)
	give(player, "coins", 50)
	give(player, "death-rune", 2)
	give(player, "chaos-rune", 10)
	changeloc(x, y, "opengorkachest")
	-- might be a pause to release the player, but unlikely
	delay(5)
	changeloc(x, y, "shutgorkachest")
end

register_useloc("shutgorkachest", "gorkakey", useloc_shutgorkachest_gorkakey)
