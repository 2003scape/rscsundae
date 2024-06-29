register_useinv("pie dish", "pastry dough", function(player)
	-- RSC 2001/7/cooking- put pie shell in dish.pcap
	mes(player, "@que@You put the dough in the pie dish to make a pie shell")
	remove(player, "pie dish", 1)
	remove(player, "pastry dough", 1)
	give(player, "pie shell", 1)
end)

register_useinv("pie shell", "rawmeat", function(player)
	-- XXX not in replay, assumed from stew
	mes(player, "@que@you need to precook the meat")
end)

register_useinv("pie shell", "redberries", function(player)
	if not statatleast(player, STAT_COOKING, 10) then
		-- XXX message assumed, not in replay
		mes(player, "@que@You need a cooking level of 10 to make redberry pie")
		return
	end
	-- RSC 2001/7/cooking- put redberries in pie.pcap
	mes(player, "@que@You fill your pie with redberries")
	remove(player, "redberries", 1)
	remove(player, "pie shell", 1)
	give(player, "uncooked redberry pie", 1)
end)

register_useinv("pie shell", "cookedmeat", function(player)
	if not statatleast(player, STAT_COOKING, 20) then
		-- XXX message assumed, not in replay
		mes(player, "@que@You need a cooking level of 20 to make meat pie")
		return
	end
	-- XXX not in replay, assumed from other pies
	mes(player, "@que@You fill your pie with meat")
	remove(player, "cookedmeat", 1)
	remove(player, "pie shell", 1)
	give(player, "uncooked meat pie", 1)
end)

register_useinv("pie shell", "cooking apple", function(player)
	if not statatleast(player, STAT_COOKING, 30) then
		-- XXX message assumed, not in replay
		mes(player, "@que@You need a cooking level of 30 to make apple pie")
		return
	end
	-- Logg/Tylerbeg/08-01-2018 19.32.34 varrock pt2
	mes(player, "@que@you fill your pie with cooking apple")
	remove(player, "cooking apple", 1)
	remove(player, "pie shell", 1)
	give(player, "uncooked apple pie", 1)
end)

-- from here on:
-- 1e_Luis/Quests/Faladian Knights Sword.pcap

function opinv_burntpie(player)
	mes(player, "you remove the burnt pie from the pie dish")
	remove(player, "burntpie", 1)
	give(player, "pie dish", 1)
end

local function cook_pie(player, raw, cooked, xp, low, high)
	thinkbubble(player, raw)
	mes(player, "@que@You cook the pie in the oven...")
	delay(5)
	remove(player, raw, 1)
	if statrandom(player, STAT_COOKING, low, high) then
		mes(player, "@que@You remove the pie from the oven")
		give(player, cooked, 1)
		advancestat(player, STAT_COOKING, xp, 0)
	else
		mes(player, "@que@You accidentally burn the pie")
		give(player, "burntpie", 1)
	end
end

local function cook_redberry_pie(player, low, high)
	if not statatleast(player, STAT_COOKING, 10) then
		-- XXX message assumed, not in replay
		mes(player, "@que@You need a cooking level of 10 to make redberry pie")
		return
	end
	cook_pie(player,
		"uncooked redberry pie", "redberry pie",
		288, low, high)
end

local function cook_meat_pie(player, low, high)
	if not statatleast(player, STAT_COOKING, 20) then
		-- XXX message assumed, not in replay
		mes(player, "@que@You need a cooking level of 20 to make meat pie")
		return
	end
	cook_pie(player,
		"uncooked meat pie", "meat pie",
		320, low, high)
end

local function cook_apple_pie(player, low, high)
	if not statatleast(player, STAT_COOKING, 30) then
		-- XXX message assumed, not in replay
		mes(player, "@que@You need a cooking level of 30 to make apple pie")
		return
	end
	cook_pie(player,
		"uncooked apple pie", "apple pie",
		400, low, high)
end

-- rates from OSRS

register_useloc("fire", "uncooked redberry pie", function(player)
	mes(player, "@que@You need a proper oven to cook this")
end)

register_useloc("range", "uncooked redberry pie", function(player)
	cook_redberry_pie(player, 98, 452)
end)

register_useloc("cookrange", "uncooked redberry pie", function(player)
	cook_redberry_pie(player, 108, 462)
end)

register_useloc("fire", "uncooked meat pie", function(player)
	mes(player, "@que@You need a proper oven to cook this")
end)

register_useloc("range", "uncooked meat pie", function(player)
	cook_meat_pie(player, 78, 412)
end)

register_useloc("cookrange", "uncooked meat pie", function(player)
	cook_meat_pie(player, 88, 432)
end)

register_useloc("fire", "uncooked apple pie", function(player)
	mes(player, "@que@You need a proper oven to cook this")
end)

register_useloc("range", "uncooked apple pie", function(player)
	cook_apple_pie(player, 58, 372)
end)

register_useloc("cookrange", "uncooked apple pie", function(player)
	cook_apple_pie(player, 58, 372)
end)
