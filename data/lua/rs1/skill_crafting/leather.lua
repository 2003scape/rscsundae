-- https://classic.runescape.wiki/w/Crafting#Leather_Working
--
-- pcaps/rsc-preservation.xyz/Quests/lost-city.pcap is mislabeled but shows
-- leather crafting.  One reel of thread is consumed after 5 items are made.
--
-- see also:
-- 1e_Luis/Skilling/Crafting distinct items
-- 1e_Luis/Skilling/Crafting requirements and Observatory quick
--
-- milestone: first script using stored variables.

local function subtract_thread(player)
	local amount = getvar(player, "thread_used")
	if amount > 4 then
		mes(player, "@que@You use up one of your reels of thread")
		remove(player, "thread", 1)
		amount = 0
	else 
		amount = amount + 1
	end
	setvar(player, "thread_used", amount)
end

function useinv_needle_leather(player)
	if not held(player, "thread", 1) then
		-- XXX: made up message, not found in replays
		-- adapted from the RS2 message
		mes(player, "@que@You need some thread to make anything out of leather")
		return
	end
	mes(player, "@que@What would you like to make?")
	local resp = multi(player, "Armour", "Gloves", "Boots")
	if resp == 1 then
		if not statatleast(player, STAT_CRAFTING, 14) then
			mes(player, "@que@You need to have a crafting of level 14 or higher to make Leather Armour")
			return
		end
		mes(player, "You make some leather armour")
		remove(player, "leather", 1)
		give(player, "leather armour", 1)
		advancestat(player, STAT_CRAFTING, 100, 0)
		subtract_thread(player)
	elseif resp == 2 then
		mes(player, "You make some leather gloves")
		remove(player, "leather", 1)
		give(player, "leather gloves", 1)
		advancestat(player, STAT_CRAFTING, 55, 0)
		subtract_thread(player)
	elseif resp == 3 then
		if not statatleast(player, STAT_CRAFTING, 7) then
			mes(player, "@que@You need to have a crafting of level 7 or higher to make boots")
			return
		end
		mes(player, "You make some boots")
		remove(player, "leather", 1)
		give(player, "boots", 1)
		advancestat(player, STAT_CRAFTING, 65, 0)
		subtract_thread(player)
	end
end

register_useinv("needle", "leather", useinv_needle_leather)
