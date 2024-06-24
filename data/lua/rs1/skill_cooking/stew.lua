-- Logg/Tylerbeg/08-01-2018 21.50.15 cooking pizza and cake
-- Logg/Tylerbeg/08-02-2018 21.05.11 east ardougne, chaos tower, clocktower dungeon, hazeel cult dungeon.pcap

function useinv_bowl_of_water_rawmeat(player)
	mes(player, "@que@you need to precook the meat")
end

function useinv_bowl_of_water_potato(player)
	mes(player, "@que@You cut up the potato and put it into the bowl")
	remove(player, "potato", 1)
	remove(player, "bowl of water", 1)
	give(player, "stew1", 1)
end

function useinv_stew1_cookedmeat(player)
	mes(player, "@que@You cut up the meat and put it into the stew")
	remove(player, "cookedmeat", 1)
	remove(player, "stew1", 1)
	give(player, "uncooked stew", 1)
end

function useinv_bowl_of_water_cookedmeat(player)
	mes(player, "@que@You cut up the meat and put it into the bowl")
	remove(player, "cookedmeat", 1)
	remove(player, "bowl of water", 1)
	give(player, "stew2", 1)
end

function useinv_stew2_potato(player)
	mes(player, "@que@You cut up the potato and put it into the stew")
	remove(player, "potato", 1)
	remove(player, "stew2", 1)
	give(player, "uncooked stew", 1)
end

function useloc_range_uncooked_stew(player, x, y)
	if not statatleast(player, STAT_COOKING, 25) then
		-- XXX message assumed, not in replay
		mes(player, "@que@You need a cooking level of 25 to cook stew")
		return
	end
	thinkbubble(player, "uncooked stew")
	mes(player, "@que@You cook the stew on the range...")
	remove(player, "uncooked stew", 1)
	delay(3)
	-- chance from 2004scape
	if statrandom(player, STAT_COOKING, 68, 392) then
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

register_useinv("bowl of water", "rawmeat", useinv_bowl_of_water_rawmeat)
register_useinv("bowl of water", "cookedmeat", useinv_bowl_of_water_cookedmeat)
register_useinv("bowl of water", "potato", useinv_bowl_of_water_potato)
register_useinv("stew1", "cookedmeat", useinv_stew1_cookedmeat)
register_useinv("stew2", "potato", useinv_stew2_potato)
register_useloc("range", "uncooked stew", useloc_range_uncooked_stew)
