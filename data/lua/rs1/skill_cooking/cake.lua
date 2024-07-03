-- Logg/Tylerbeg/08-01-2018 21.50.15 cooking pizza and cake.pcap

function mix_cake(player)
	if not held(player, "milk", 1) then
		mes(player, "@que@I also need some milk to make a cake")
		return
	end
	if not held(player, "egg", 1) then
		mes(player, "@que@I also need an egg to make a cake")
		return
	end
	if not held(player, "flourpot", 1) then
		mes(player, "@que@I also need some flour to make a cake")
		return
	end
	mes(player, "@que@You mix some milk, flour and egg together into a cake mixture")
	remove(player, "cake tin", 1)
	remove(player, "egg", 1)
	remove(player, "milk", 1)
	remove(player, "flourpot", 1)
	give(player, "bucket", 1)
	give(player, "pot", 1)
	give(player, "uncooked cake", 1)
end

function cook_cake(player)
	if not statatleast(player, STAT_COOKING, 40) then
		-- XXX message unverified
		mes(player, "@que@You need a cooking level of 40 to bake a cake")
		return
	end
	thinkbubble(player, "uncooked cake", 1)
	mes(player, "@que@You cook the cake in the oven...")
	remove(player, "uncooked cake", 1)
	give(player, "cake tin", 1)
	delay(5)
	-- chance from OSRS
	if statrandom(player, STAT_COOKING, 38, 332) then
		-- XXX nobody successfully cooks a cake in a replay,
		-- so this message is assumed
		mes(player, "@que@You remove the cake from the oven")
		give(player, "cake", 1)
		advancestat(player, STAT_COOKING, 480, 0)
	else
		mes(player, "@que@You accidentally burn the cake")
		give(player, "burnt cake", 1)
	end
end

register_useinv("cake tin", "milk", mix_cake)
register_useinv("cake tin", "egg", mix_cake)
register_useinv("cake tin", "flourpot", mix_cake)

register_useinv("cake", "chocolate bar", function(player)
	if not statatleast(player, STAT_COOKING, 50) then
		-- XXX message unverified
		mes(player, "@que@You need a cooking level of 50 to make chocolate cake")
		return
	end
	mes(player, "@que@You make a chocolate cake!")
	remove(player, "cake", 1)
	remove(player, "chocolate bar", 1)
	give(player, "chocolate cake", 1)
end)

register_useloc("fire", "uncooked cake", function(player, x, y)
	mes(player, "@que@You need a proper oven to cook this")
end)

register_useloc("range", "uncooked cake", cook_cake)
register_useloc("cookrange", "uncooked cake", cook_cake)
