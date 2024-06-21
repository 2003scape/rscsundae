-- https://classic.runescape.wiki/w/Crafting#Pottery
--
-- No level requirement for firing, per:
-- https://github.com/2004Scape/Server/blob/main/data/src/scripts/skill_crafting/scripts/pottery/pottery.rs2
--
-- XXX: don't have recordings of failure to bake
--

-- RSC 2001/replays master archive/Skilling/Crafting/pottery- potter's wheel- create unfired pie dish
-- RSC 2001/replays master archive/Skilling/Crafting/pottery- potter's wheel- create unfired pot
-- RSC 2001/replays master archive/Skilling/Crafting/pottery- potter's wheel- create unfired bowl
-- 1e_Luis/Skilling/Crafting requirements and Observatory quick
function useloc_potters_wheel_softclay(player, x, y)
	thinkbubble(player, "soft clay")
	mes(player, "@que@What would you like to make?")
	local res = multi(player,
		"Pie dish",
		"Pot",
		"Bowl")
	if res == 1 then
		if not statatleast(player, STAT_CRAFTING, 4) then
			mes(player, "@que@you need to have a crafting of level 4 or higher to make pie dishes")
			return
		end
		mes(player, "@que@you make the clay into a pie dish")
		advancestat(player, STAT_CRAFTING, 60, 0)
		remove(player, "soft clay", 1)
		give(player, "unfired pie dish", 1)
	elseif res == 2 then
		mes(player, "@que@you make the clay into a pot")
		advancestat(player, STAT_CRAFTING, 25, 0)
		remove(player, "soft clay", 1)
		give(player, "unfired pot", 1)
	elseif res == 3 then
		if not statatleast(player, STAT_CRAFTING, 7) then
			mes(player, "@que@You need to have a crafting of level 7 or higher to make a bowl")
			return
		end
		mes(player, "@que@You make the clay into a bowl")
		advancestat(player, STAT_CRAFTING, 40, 0)
		remove(player, "soft clay", 1)
		give(player, "unfired bowl", 1)
	end
end

--SC 2001/replays master archive/Skilling/Crafting/pottery- pottery oven- create pot
function useloc_pottery_oven_unfired_pot(player, x, y)
	thinkbubble(player, "unfired pot")
	mes(player, "@que@You put the pot in the oven")
	remove(player, "unfired pot", 1)
	delay(3)
	mes(player, "@que@the pot hardens in the oven")
	delay(3)
	mes(player, "@que@You remove a pot from the oven")
	advancestat(player, STAT_CRAFTING, 15, 0)
	give(player, "pot", 1)
end

-- RSC 2001/replays master archive/Skilling/Crafting/pottery- pottery oven- create pie dish
function useloc_pottery_oven_unfired_pie_dish(player, x, y)
	thinkbubble(player, "unfired pie dish")
	mes(player, "@que@You put the pie dish in the oven")
	remove(player, "unfired pie dish", 1)
	delay(3)
	mes(player, "@que@the dish hardens in the oven")
	delay(3)
	mes(player, "@que@You remove a dish from the oven")
	advancestat(player, STAT_CRAFTING, 40, 0)
	give(player, "pie dish", 1)
end

-- RSC 2001/replays master archive/Skilling/Crafting/pottery- pottery oven- create bowl
function useloc_pottery_oven_unfired_bowl(player, x, y)
	thinkbubble(player, "unfired bowl")
	mes(player, "@que@You put the bowl in the oven")
	remove(player, "unfired bowl", 1)
	delay(3)
	mes(player, "@que@the bowl hardens in the oven")
	delay(3)
	mes(player, "@que@You remove a bowl from the oven")
	advancestat(player, STAT_CRAFTING, 60, 0)
	give(player, "bowl", 1)
end

-- RSC 2001/7/crafting- create soft clay
function useinv_water_clay(player)
	mes(player, "@que@You mix the clay and water")
	remove(player, "water", 1)
	remove(player, "clay", 1)
	give(player, "soft clay", 1)
	delay(2)
	mes(player, "@que@You now have some soft workable clay")
end

register_useinv("water", "clay", useinv_water_clay)
register_useloc("pottery oven", "unfired pot", useloc_pottery_oven_unfired_pot)
register_useloc("pottery oven", "unfired pie dish", useloc_pottery_oven_unfired_pie_dish)
register_useloc("pottery oven", "unfired bowl", useloc_pottery_oven_unfired_bowl)
register_useloc("potter's wheel", "soft clay", useloc_potters_wheel_softclay)
