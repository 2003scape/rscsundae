-- see this folder:
-- RSC 2001/replays master archive/Skilling/Smithing

local function smelt_bronze(player)
	-- Logg/Loggykins/08-05-2018 15.05.32  stand in draynor for a while, then get lured out to kbd, turn back, and meet up with shasta, then bank to draynor.pcap
	if not held(player, "copper ore", 1) then
		mes(player, "@que@You also need some copper to make bronze")
		return
	end
	-- Logg/Tylerbeg/08-05-2018 12.40.40 reloaded client after luis reminded me that rsc+ has an item patch mode.pcap
	if not held(player, "tin ore", 1) then
		mes(player, "@que@You also need some tin to make bronze")
		return
	end
	mes(player, "@que@You smelt the copper and tin together in the furnace")
	delay(3)
	mes(player, "@que@You retrieve a bar of bronze")
	remove(player, "copper ore", 1)
	remove(player, "tin ore", 1)
	give(player, "bronze bar", 1)
	advancestat(player, STAT_SMITHING, 25, 0)
end

local function smelt_iron(player)
	thinkbubble(player, "iron ore")
	if not statatleast(player, STAT_SMITHING, 15) then
		-- pcaps/RSC 2001/7/smithing- smelt- iron ore- fail- insufficient level- special message.pcap
		mes(player, "@que@You need to be at least level-15 smithing to smelt iron")
		mes(player, "@que@Practice your smithing using tin and copper to make bronze")
		return
	end
	mes(player, "@que@You smelt the iron in the furnace")
	delay(3)
	if random(128) then
		mes(player, "@que@You retrieve a bar of iron")
		remove(player, "iron ore", 1)
		give(player, "iron bar", 1)
		advancestat(player, STAT_SMITHING, 50, 0)
	else
		mes(player, "@que@The ore is too impure and you fail to refine it")
		remove(player, "iron ore", 1)
	end
end

local function smelt_silver(player)
	thinkbubble(player, "silver")
	if not statatleast(player, STAT_SMITHING, 20) then
		mes(player, "@que@You need to be at least level-20 smithing to smelt silver")
		return
	end
	mes(player, "@que@You place a lump of silver in the furnace")
	delay(3)
	mes(player, "@que@You retrieve a bar of silver")
	remove(player, "silver", 1)
	give(player, "silver bar", 1)
	advancestat(player, STAT_SMITHING, 55, 0)
end

local function smelt_gold(player)
	thinkbubble(player, "gold")
	if not statatleast(player, STAT_SMITHING, 40) then
		mes(player, "@que@You need to be at least level-40 smithing to smelt gold")
		return
	end
	mes(player, "@que@You place a lump of gold in the furnace")
	delay(3)
	mes(player, "@que@You retrieve a bar of gold")
	remove(player, "gold", 1)
	give(player, "gold bar", 1)
	advancestat(player, STAT_SMITHING, 90, 0)
end

local function smelt_steel(player)
	if not statatleast(player, STAT_SMITHING, 30) then
		mes(player, "@que@You need to be at least level-30 smithing to smelt steel")
		return
	end
	if (not held(player, "coal", 2)) or
	    (not held(player, "iron ore", 1)) then
		mes(player, "@que@You need 1 iron-ore and 2 coal to make steel")
		return
	end
	mes(player, "@que@You place the iron and 2 heaps of coal into the furnace")
	delay(3)
	mes(player, "@que@You retrieve a bar of steel")
	remove(player, "iron ore", 1)
	remove(player, "coal", 2)
	give(player, "steel bar", 1)
	advancestat(player, STAT_SMITHING, 70, 0)
end

local function smelt_mithril(player)
	thinkbubble(player, "mithril ore")
	if not statatleast(player, STAT_SMITHING, 50) then
		mes(player, "@que@You need to be at least level-50 smithing to smelt mithril")
		return
	end
	if not held(player, "coal", 4) then
		-- pcaps/flying sno (redacted chat) replays/fsnom2@aol.com/07-11-2018 20.03.44.pcap
		mes(player, "@que@You need 4 heaps of coal to smelt mithril")
		return
	end
	mes(player, "@que@You place the mithril and 4 heaps of coal into the furnace")
	delay(3)
	mes(player, "@que@You retrieve a bar of mithril")
	remove(player, "mithril ore", 1)
	remove(player, "coal", 4)
	give(player, "mithril bar", 1)
	advancestat(player, STAT_SMITHING, 120, 0)
end

local function smelt_adamantite(player)
	thinkbubble(player, "adamantite ore")
	if not statatleast(player, STAT_SMITHING, 70) then
		mes(player, "@que@You need to be at least level-70 smithing to smelt adamantite")
		return
	end
	if not held(player, "coal", 6) then
		mes(player, "@que@You need 6 heaps of coal to smelt adamantite")
		return
	end
	mes(player, "@que@You place the adamantite and 6 heaps of coal into the furnace")
	delay(3)
	mes(player, "@que@You retrieve a bar of adamantite")
	remove(player, "adamantite ore", 1)
	remove(player, "coal", 6)
	give(player, "adamantite bar", 1)
	advancestat(player, STAT_SMITHING, 150, 0)
end

local function smelt_runite(player)
	thinkbubble(player, "runite ore")
	if not statatleast(player, STAT_SMITHING, 85) then
		mes(player, "@que@You need to be at least level-85 smithing to smelt runite")
		return
	end
	if not held(player, "coal", 8) then
		mes(player, "@que@You need 8 heaps of coal to smelt runite")
		return
	end
	mes(player, "@que@You place the runite and 8 heaps of coal into the furnace")
	delay(3)
	mes(player, "@que@You retrieve a bar of runite")
	remove(player, "runite ore", 1)
	remove(player, "coal", 8)
	give(player, "runite bar", 1)
	advancestat(player, STAT_SMITHING, 200, 0)
end

register_useloc("furnace", "copper ore", function(player)
	thinkbubble(player, "copper ore")
	smelt_bronze(player)
end)

register_useloc("furnace", "tin ore", function(player)
	thinkbubble(player, "tin ore")
	smelt_bronze(player)
end)

register_useloc("furnace", "iron ore", function(player)
	thinkbubble(player, "iron ore")
	if held(player, "coal", 1) then
		smelt_steel(player)
	else
		smelt_iron(player)
	end
end)

register_useloc("furnace", "coal", function(player)
	thinkbubble(player, "coal")
	smelt_steel(player)
end)

register_useloc("furnace", "silver", function(player)
	thinkbubble(player, "silver")
	smelt_silver(player)
end)

register_useloc("furnace", "gold", function(player)
	thinkbubble(player, "gold")
	smelt_gold(player)
end)

register_useloc("furnace", "mithril ore", function(player)
	thinkbubble(player, "mithril ore")
	smelt_mithril(player)
end)

register_useloc("furnace", "adamantite ore", function(player)
	thinkbubble(player, "adamantite ore")
	smelt_adamantite(player)
end)

register_useloc("furnace", "runite ore", function(player)
	thinkbubble(player, "runite ore")
	smelt_runite(player)
end)
