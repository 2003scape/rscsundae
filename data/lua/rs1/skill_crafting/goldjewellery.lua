-- stiched together from various replays
-- check the following directory:
-- RSC 2001/replays master archive/Skilling/Crafting/
-- and 1e_Luis/Skilling/Crafting requirements and Observatory quick.pcap
-- and Logg/Tylerbeg/06-13-2018 18.10.29 crafting.pcap

local function make_gem_amulet(player)
	mes(player, "@que@what sort of gem do you want to put in the amulet?")
	local resp = multi(player,
		"Sapphire", "Emerald", "Ruby", "Diamond")
	if resp == 1 then
		if not statatleast(player, STAT_CRAFTING, 13) then
			mes(player, "@que@You need a crafting skill of level 13 to make this")
			return
		end
		if not held(player, "sapphire", 1) then
			mes(player, "@que@You do not have a cut sapphire to make a sapphire amulet")
			return
		end
		thinkbubble(player, "gold bar")
		mes(player, "@que@You make a Sapphire amulet")
		remove(player, "gold bar", 1)
		remove(player, "sapphire", 1)
		give(player, "nostringsaph", 1)
		advancestat(player, STAT_CRAFTING, 52, 0)
	elseif resp == 2 then
		if not statatleast(player, STAT_CRAFTING, 30) then
			mes(player, "@que@You need a crafting skill of level 30 to make this")
			return
		end
		if not held(player, "emerald", 1) then
			mes(player, "@que@You do not have a cut Emerald to make a Emerald amulet")
			return
		end
		thinkbubble(player, "gold bar")
		mes(player, "@que@You make an Emerald amulet")
		remove(player, "gold bar", 1)
		remove(player, "emerald", 1)
		give(player, "nostringem", 1)
		advancestat(player, STAT_CRAFTING, 280, 0)
	elseif resp == 3 then
		if not statatleast(player, STAT_CRAFTING, 50) then
			mes(player, "@que@You need a crafting skill of level 50 to make this")
			return
		end
		if not held(player, "ruby", 1) then
			mes(player, "@que@You do not have a cut ruby to make a ruby amulet")
			return
		end
		thinkbubble(player, "gold bar")
		mes(player, "@que@You make a ruby amulet")
		remove(player, "gold bar", 1)
		remove(player, "ruby", 1)
		give(player, "nostringrub", 1)
		advancestat(player, STAT_CRAFTING, 340, 0)
	elseif resp == 4 then
		if not statatleast(player, STAT_CRAFTING, 70) then
			mes(player, "@que@You need a crafting skill of level 70 to make this")
			return
		end
		if not held(player, "diamond", 1) then
			mes(player, "@que@You do not have a cut diamond to make a diamond amulet")
			return
		end
		thinkbubble(player, "gold bar")
		mes(player, "@que@You make a diamond amulet")
		remove(player, "gold bar", 1)
		remove(player, "diamond", 1)
		give(player, "nostringdia", 1)
		advancestat(player, STAT_CRAFTING, 400, 0)
	end
end

local function make_gem_necklace(player)
	mes(player, "@que@what sort of gem do you want to put in the necklace?")
	local resp = multi(player,
		"Sapphire", "Emerald", "Ruby", "Diamond")
	if resp == 1 then
		if not statatleast(player, STAT_CRAFTING, 10) then
			mes(player, "@que@You need a crafting skill of level 10 to make this")
			return
		end
		if not held(player, "sapphire", 1) then
			mes(player, "@que@You do not have a cut sapphire to make a sapphire necklace")
			return
		end
		thinkbubble(player, "gold bar")
		mes(player, "@que@You make a Sapphire necklace")
		remove(player, "gold bar", 1)
		remove(player, "sapphire", 1)
		give(player, "sapphire necklace", 1)
		advancestat(player, STAT_CRAFTING, 220, 0)
	elseif resp == 2 then
		if not statatleast(player, STAT_CRAFTING, 24) then
			mes(player, "@que@You need a crafting skill of level 24 to make this")
			return
		end
		if not held(player, "emerald", 1) then
			mes(player, "@que@You do not have a cut Emerald to make a Emerald necklace")
			return
		end
		thinkbubble(player, "gold bar")
		mes(player, "@que@You make an Emerald necklace")
		remove(player, "gold bar", 1)
		remove(player, "emerald", 1)
		give(player, "emerald necklace", 1)
		advancestat(player, STAT_CRAFTING, 240, 0)
	elseif resp == 3 then
		if not statatleast(player, STAT_CRAFTING, 40) then
			mes(player, "@que@You need a crafting skill of level 40 to make this")
			return
		end
		if not held(player, "ruby", 1) then
			mes(player, "@que@You do not have a cut ruby to make a ruby necklace")
			return
		end
		thinkbubble(player, "gold bar")
		mes(player, "@que@You make a ruby necklace")
		remove(player, "gold bar", 1)
		remove(player, "ruby", 1)
		give(player, "ruby necklace", 1)
		advancestat(player, STAT_CRAFTING, 300, 0)
	elseif resp == 4 then
		if not statatleast(player, STAT_CRAFTING, 56) then
			mes(player, "@que@You need a crafting skill of level 56 to make this")
			return
		end
		if not held(player, "diamond", 1) then
			mes(player, "@que@You do not have a cut diamond to make a diamond necklace")
			return
		end
		thinkbubble(player, "gold bar")
		mes(player, "@que@You make a diamond necklace")
		remove(player, "gold bar", 1)
		remove(player, "diamond", 1)
		give(player, "diamond necklace", 1)
		advancestat(player, STAT_CRAFTING, 360, 0)
	end
end

local function make_gem_ring(player)
	mes(player, "@que@what sort of gem do you want to put in the ring?")
	local resp = multi(player,
		"Sapphire", "Emerald", "Ruby", "Diamond")
	if resp == 1 then
		if not statatleast(player, STAT_CRAFTING, 8) then
			mes(player, "@que@You need a crafting skill of level 8 to make this")
			return
		end
		if not held(player, "sapphire", 1) then
			mes(player, "@que@You do not have a cut sapphire to make a sapphire ring")
			return
		end
		thinkbubble(player, "gold bar")
		mes(player, "@que@You make a Sapphire ring")
		remove(player, "gold bar", 1)
		remove(player, "sapphire", 1)
		give(player, "sapphire ring", 1)
		advancestat(player, STAT_CRAFTING, 160, 0)
	elseif resp == 2 then
		if not statatleast(player, STAT_CRAFTING, 18) then
			mes(player, "@que@You need a crafting skill of level 18 to make this")
			return
		end
		if not held(player, "emerald", 1) then
			mes(player, "@que@You do not have a cut Emerald to make a Emerald ring")
			return
		end
		thinkbubble(player, "gold bar")
		mes(player, "@que@You make an Emerald ring")
		remove(player, "gold bar", 1)
		remove(player, "emerald", 1)
		give(player, "emerald ring", 1)
		advancestat(player, STAT_CRAFTING, 220, 0)
	elseif resp == 3 then
		if not statatleast(player, STAT_CRAFTING, 30) then
			mes(player, "@que@You need a crafting skill of level 30 to make this")
			return
		end
		if not held(player, "ruby", 1) then
			mes(player, "@que@You do not have a cut ruby to make a ruby ring")
			return
		end
		thinkbubble(player, "gold bar")
		mes(player, "@que@You make a ruby ring")
		remove(player, "gold bar", 1)
		remove(player, "ruby", 1)
		give(player, "ruby ring", 1)
		advancestat(player, STAT_CRAFTING, 280, 0)
	elseif resp == 4 then
		if not statatleast(player, STAT_CRAFTING, 42) then
			mes(player, "@que@You need a crafting skill of level 42 to make this")
			return
		end
		if not held(player, "diamond", 1) then
			mes(player, "@que@You do not have a cut diamond to make a diamond ring")
			return
		end
		thinkbubble(player, "gold bar")
		mes(player, "@que@You make a diamond ring")
		remove(player, "gold bar", 1)
		remove(player, "diamond", 1)
		give(player, "diamond ring", 1)
		advancestat(player, STAT_CRAFTING, 340, 0)
	end
end

local function make_amulet(player)
	if not held(player, "amulet mould", 1) then
		mes(player, "@que@You need an amulet mould to make a gold amulet")
		return
	end
	mes(player, "@que@Would you like to put a gem in the amulet?")
	local resp = multi(player, "Yes", "No")
	if resp == 1 then
		make_gem_amulet(player)
	elseif resp == 2 then
		if not statatleast(player, STAT_CRAFTING, 8) then
			mes(player, "@que@You need a crafting skill of level 8 to make this")
			return
		end
		thinkbubble(player, "gold bar")
		mes(player, "@que@You make a gold amulet")
		remove(player, "gold bar", 1)
		give(player, "gold amulet", 1)
		advancestat(player, STAT_CRAFTING, 120, 0)
	end
end

local function make_necklace(player)
	if not held(player, "necklace mould", 1) then
		mes(player, "@que@You need a necklace mould to make a gold necklace")
		return
	end
	mes(player, "@que@Would you like to put a gem in the necklace?")
	local resp = multi(player, "Yes", "No")
	if resp == 1 then
		make_gem_necklace(player)
	elseif resp == 2 then
		if not statatleast(player, STAT_CRAFTING, 6) then
			mes(player, "@que@You need a crafting skill of level 6 to make this")
			return
		end
		thinkbubble(player, "gold bar")
		mes(player, "@que@You make a gold necklace")
		remove(player, "gold bar", 1)
		give(player, "gold necklace", 1)
		advancestat(player, STAT_CRAFTING, 80, 0)
	end
end

local function make_ring(player)
	if not held(player, "ring mould", 1) then
		mes(player, "@que@You need a ring mould to make a gold ring")
		return
	end
	mes(player, "@que@Would you like to put a gem in the ring?")
	local resp = multi(player, "Yes", "No")
	if resp == 1 then
		make_gem_ring(player)
	elseif resp == 2 then
		if not statatleast(player, STAT_CRAFTING, 5) then
			mes(player, "@que@You need a crafting skill of level 5 to make this")
			return
		end
		thinkbubble(player, "gold bar")
		mes(player, "@que@You make a gold ring")
		remove(player, "gold bar", 1)
		give(player, "gold ring", 1)
		advancestat(player, STAT_CRAFTING, 60, 0)
	end
end

register_useloc("furnace", "gold bar", function(player, x, y)
	mes(player, "@que@what would you like to make")
	local resp = multi(player, "ring", "Necklace", "amulet")
	if resp == 1 then
		make_ring(player)
	elseif resp == 2 then
		make_necklace(player)
	elseif resp == 3 then
		make_amulet(player)
	end
end)
