-- Logg/Tylerbeg/08-01-2018 19.32.34 varrock pt2.pcap

function make_dough(player, watertype, watervessel)
	mes(player, "@que@What sort of dough do you wish to make?")
	local choices = {
		"Bread dough", "Pastry dough", "Pizza dough"
	}
	local resp = multi(player, table.unpack(choices))
	mes(player, "@que@You mix the water and flour to make some "
		.. string.lower(choices[resp]))
	remove(player, "flourpot", 1)
	remove(player, watertype, 1)
	give(player, "pot", 1)
	give(player, watervessel, 1)
	if resp == 1 then
		give(player, "bread dough", 1)
	elseif resp == 2 then
		give(player, "pastry dough", 1)
	elseif resp == 3 then
		give(player, "pizza base", 1)
	end
end

function useloc_range_bread_dough(player)
	thinkbubble(player, "bread dough")
	mes(player, "@que@You cook the dough in the oven...")
	remove(player, "bread dough", 1)
	delay(3)
	-- chance comes from 2004scape
	if statrandom(player, STAT_COOKING, 118, 492) then
		mes(player, "@que@You remove the bread from the oven")
		give(player, "bread", 1)
		advancestat(player, STAT_COOKING, 160, 0)
	else
		-- XXX message not in replay, adapted from RS2
		mes(player, "@que@You accidentally burn the bread")
		give(player, "burntbread", 1)
	end
end


-- this message can be found in various replays but I'm not sure
-- bread dough specifically was ever tested :(
register_useloc("fire", "bread dough", function(player, x, y)
	mes(player, "@que@You need a proper oven to cook this")
end)

register_useinv("waterjug", "flourpot", function(player)
	make_dough(player, "waterjug", "jug")
end)

register_useinv("waterbucket", "flourpot", function(player)
	make_dough(player, "waterbucket", "bucket")
end)

register_useinv("bowl of water", "flourpot", function(player)
	make_dough(player, "bowl of water", "bowl")
end)

register_useloc("range", "bread dough", useloc_range_bread_dough)
register_useloc("cookrange", "bread dough", useloc_range_bread_dough)
