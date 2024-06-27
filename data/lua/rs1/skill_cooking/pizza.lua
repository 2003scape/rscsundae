-- Logg/Tylerbeg/08-01-2018 21.50.15 cooking pizza and cake
-- Logg/Tylerbeg/08-05-2018 15.49.52 draynor bankstanding, make anchovie pizza, not selling my cat.pcap

register_useinv("pizza base", "cheese", function(player)
	mes(player, "@que@I should add the tomato first")
end)


register_useinv("pizza base", "tomato", function(player)
	mes(player, "@que@You add the tomato to the pizza")
	remove(player, "pizza base", 1)
	remove(player, "tomato", 1)
	give(player, "incomplete pizza", 1)
end)


register_useinv("incomplete pizza", "cheese", function(player)
	mes(player, "@que@You add the cheese to the pizza")
	remove(player, "pizza base", 1)
	remove(player, "cheese", 1)
	give(player, "uncooked pizza", 1)
end)

register_useinv("plain pizza", "rawmeat", function(player)
	-- XXX message assumed from anchovie message, not in replay
	mes(player, "@que@You need to cook the meat")
end)

register_useinv("plain pizza", "cookedmeat", function(player)
	if statatleast(player, STAT_COOKING, 45) then
		mes(player, "@que@You add the meat to the pizza")
		remove(player, "plain pizza", 1)
		remove(player, "cookedmeat", 1)
		give(player, "meat pizza", 1)
	else
		-- XXX message assumed, not in replay
		mes(player, "@que@You need a cooking level of 45 to make meat pizza")
	end
end)

register_useinv("plain pizza", "raw anchovies", function(player)
	mes(player, "@que@You need to cook the anchovies")
end)

register_useinv("plain pizza", "anchovies", function(player)
	if statatleast(player, STAT_COOKING, 55) then
		mes(player, "@que@You add the anchovies to the pizza")
 		remove(player, "plain pizza", 1)
		remove(player, "anchovies", 1)
		give(player, "anchovie pizza", 1)
	else
		-- XXX message assumed, not in replay
		mes(player, "@que@You need a cooking level of 55 to make anchovie pizza")
	end
end)

register_useloc("fire", "uncooked pizza", function(player, x, y)
	mes(player, "@que@You need a proper oven to cook this")
end)

register_useloc("range", "uncooked pizza", function(player, x, y)
	if not statatleast(player, STAT_COOKING, 35) then
		mes(player, "@que@You need a cooking level of 35 to make pizza")
		return
	end
	thinkbubble(player, "uncooked pizza")
	mes(player, "@que@You cook the pizza in the oven...")
	delay(5)
	remove(player, "uncooked pizza", 1)
	-- https://oldschool.runescape.wiki/w/Plain_pizza?oldid=14516457&action=edit
	if statrandom(player, STAT_COOKING, 8, 352) then
		mes(player, "@que@You remove the pizza from the oven")
		give(player, "plain pizza", 1)
		advancestat(player, STAT_COOKING, 440, 0)
	else
		mes(player, "@que@You accidentally burn the pizza")
		give(player, "burnt pizza", 1)
	end
end)
