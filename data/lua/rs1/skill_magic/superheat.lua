-- Logg/Tylerbeg/06-13-2018 18.44.52 start superheat magic training.pcap

local function finalize_superheat(player)
	advancestat(player, STAT_MAGIC, 424, 0)
	if not worn(player, "staff of fire") then
		remove(player, "fire-rune", 4)
	end
	remove(player, "nature-rune", 1)
end

local function superheat_bronze(player)
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
	mes(player, "@que@You make a bar of bronze")
	remove(player, "copper ore", 1)
	remove(player, "tin ore", 1)
	give(player, "bronze bar", 1)
	advancestat(player, STAT_SMITHING, 25, 0)
	finalize_superheat(player)
end

register_spellinv("copper ore", "superheat item", superheat_bronze)
register_spellinv("tin ore", "superheat item", superheat_bronze)

local function superheat_iron(player)
	if not statatleast(player, STAT_SMITHING, 15) then
		-- pcaps/RSC 2001/7/smithing- smelt- iron ore- fail- insufficient level- special message.pcap
		mes(player, "@que@You need to be at least level-15 smithing to smelt iron")
		mes(player, "@que@Practice your smithing using tin and copper to make bronze")
		return
	end
	mes(player, "@que@You make a bar of iron")
	remove(player, "iron ore", 1)
	give(player, "iron bar", 1)
	advancestat(player, STAT_SMITHING, 50, 0)
	finalize_superheat(player)
end

local function superheat_steel(player)
	if not statatleast(player, STAT_SMITHING, 30) then
		mes(player, "@que@You need to be at least level-30 smithing to smelt steel")
		return
	end
	if (not held(player, "coal", 2)) or
	    (not held(player, "iron ore", 1)) then
		mes(player, "@que@You need 1 iron-ore and 2 coal to make steel")
		return
	end
	mes(player, "@que@You make a bar of steel")
	remove(player, "iron ore", 1)
	remove(player, "coal", 2)
	give(player, "steel bar", 1)
	advancestat(player, STAT_SMITHING, 70, 0)
	finalize_superheat(player)
end

register_spellinv("coal", "superheat item", function(player)
	superheat_steel(player)
end)

register_spellinv("iron ore", "superheat item", function(player)
	if held(player, "coal", 2) then
		superheat_steel(player)
	else
		superheat_iron(player)
	end
end)

register_spellinv("silver", "superheat item", function(player)
	if not statatleast(player, STAT_SMITHING, 20) then
		mes(player, "@que@You need to be at least level-20 smithing to smelt silver")
		return
	end
	mes(player, "@que@You make a bar of silver")
	remove(player, "silver", 1)
	give(player, "silver bar", 1)
	advancestat(player, STAT_SMITHING, 55, 0)
	finalize_superheat(player)
end)

register_spellinv("gold", "superheat item", function(player)
	if not statatleast(player, STAT_SMITHING, 40) then
		mes(player, "@que@You need to be at least level-40 smithing to smelt gold")
		return
	end
	mes(player, "@que@You make a bar of gold")
	remove(player, "gold", 1)
	give(player, "gold bar", 1)
	advancestat(player, STAT_SMITHING, 90, 0)
	finalize_superheat(player)
end)

register_spellinv("mithril ore", "superheat item", function(player)
	if not statatleast(player, STAT_SMITHING, 50) then
		mes(player, "@que@You need to be at least level-50 smithing to smelt mithril")
		return
	end
	if not held(player, "coal", 4) then
		-- pcaps/flying sno (redacted chat) replays/fsnom2@aol.com/07-11-2018 20.03.44.pcap
		mes(player, "@que@You need 4 heaps of coal to smelt mithril")
		return
	end
	mes(player, "@que@You make a bar of mithril")
	remove(player, "mithril ore", 1)
	remove(player, "coal", 4)
	give(player, "mithril bar", 1)
	advancestat(player, STAT_SMITHING, 120, 0)
	finalize_superheat(player)
end)

register_spellinv("adamantite ore", "superheat item", function(player)
	if not statatleast(player, STAT_SMITHING, 70) then
		mes(player, "@que@You need to be at least level-70 smithing to smelt adamantite")
		return
	end
	if not held(player, "coal", 6) then
		mes(player, "@que@You need 6 heaps of coal to smelt adamantite")
		return
	end
	mes(player, "@que@You make a bar of adamantite")
	remove(player, "adamantite ore", 1)
	remove(player, "coal", 6)
	give(player, "adamantite bar", 1)
	advancestat(player, STAT_SMITHING, 150, 0)
	finalize_superheat(player)
end)

register_spellinv("runite ore", "superheat item", function(player)
	if not statatleast(player, STAT_SMITHING, 85) then
		mes(player, "@que@You need to be at least level-85 smithing to smelt runite")
		return
	end
	if not held(player, "coal", 8) then
		mes(player, "@que@You need 8 heaps of coal to smelt runite")
		return
	end
	mes(player, "@que@You make a bar of runite")
	remove(player, "runite ore", 1)
	remove(player, "coal", 8)
	give(player, "runite bar", 1)
	advancestat(player, STAT_SMITHING, 200, 0)
	finalize_superheat(player)
end)
