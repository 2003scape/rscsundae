-- Logg/Tylerbeg/08-01-2018 21.50.15 cooking pizza and cake
-- Logg/Tylerbeg/08-02-2018 21.05.11 east ardougne, chaos tower, clocktower dungeon, hazeel cult dungeon.pcap

register_useinv("bowl of water", "rawmeat", function(player)
	mes(player, "@que@you need to precook the meat")
end)

register_useinv("bowl of water", "potato", function(player)
	mes(player, "@que@You cut up the potato and put it into the bowl")
	remove(player, "potato", 1)
	remove(player, "bowl of water", 1)
	give(player, "stew1", 1)
end)

register_useinv("stew1", "cookedmeat", function(player)
	mes(player, "@que@You cut up the meat and put it into the stew")
	remove(player, "cookedmeat", 1)
	remove(player, "stew1", 1)
	give(player, "uncooked stew", 1)
end)

register_useinv("bowl of water", "cookedmeat", function(player)
	mes(player, "@que@You cut up the meat and put it into the bowl")
	remove(player, "cookedmeat", 1)
	remove(player, "bowl of water", 1)
	give(player, "stew2", 1)
end)

register_useinv("stew2", "potato", function(player)
	mes(player, "@que@You cut up the potato and put it into the stew")
	remove(player, "potato", 1)
	remove(player, "stew2", 1)
	give(player, "uncooked stew", 1)
end)

function cook_stew(player, device, low, high)
	if not statatleast(player, STAT_COOKING, 25) then
		-- XXX message assumed, not in replay
		mes(player, "@que@You need a cooking level of 25 to cook stew")
		return
	end
	thinkbubble(player, "uncooked stew")
	mes(player, string.format("@que@You cook the stew on the %s...", device))
	remove(player, "uncooked stew", 1)
	delay(3)
	if statrandom(player, STAT_COOKING, low, high) then
		mes(player, "@que@The stew is now nicely cooked")
		give(player, "stew", 1)
		advancestat(player, STAT_COOKING, 360, 0)
	else
		-- XXX message assumed, not in replay
		mes(player, "@que@You accidentally burn the stew")
		give(player, "burnt stew", 1)
	end
end

function opinv_burnt_stew(player)
	-- not in replay, message from 2004scape
	remove(player, "burnt stew", 1)
	give(player, "bowl", 1)
	mes(player, "@que@You empty the burnt stew from the bowl")
end

-- chances from 2004scape:
-- https://github.com/2004Scape/Server/blob/79cedb0c2fa7cf2360d17393be085239c8059ee7/data/src/scripts/skill_cooking/configs/cooking_source/cooking_generic.dbrow#L17

register_useloc("fire", "uncooked stew", function(player, x, y)
	cook_stew(player, "fire", 68, 392)
end)

register_useloc("range", "uncooked stew", function(player, x, y)
	cook_stew(player, "range", 68, 392)
end)

register_useloc("cookrange", "uncooked stew", function(player, x, y)
	cook_stew(player, "range", 78, 412)
end)
