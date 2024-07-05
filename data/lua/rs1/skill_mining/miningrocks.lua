-- https://classic.runescape.wiki/w/Mining#Rocks
-- https://github.com/2004Scape/Server/blob/014bb62b7ccc483ddb7412dee67c158ca92a90b5/data/src/scripts/skill_mining/configs/mine.dbrow

-- for gem finding behaviour see
-- RSC 2001/replays master archive/Skilling/Mining/mining- runite (with respawn time w2 pop37).pcap
-- it seems to replace the path for finding ore
-- ratios are from the OSRS wiki
local function mine_random_jewel(player)
	if not random(1) then
		return false
	end

	local ran = randomvar(16)
	if ran < 1 then
		mes(player, "@que@You just found a diamond!")
		give(player, "uncut diamond", 1)
	elseif ran < 4 then
		mes(player, "@que@You just found a ruby!")
		give(player, "uncut ruby", 1)
	elseif ran < 8 then
		mes(player, "@que@You just found an emerald!")
		give(player, "uncut emerald", 1)
	else
		mes(player, "@que@You just found a sapphire!")
		give(player, "uncut sapphire", 1)
	end
	return true
end

local function do_initial_mining(player)
	-- RSC 2001/replays master archive/Skilling/Mining/mining- fail- you need a pickaxe to mine this rock.pcap
	if not held(player, "pickaxe", 1) then
		mes(player, "You need a pickaxe to mine this rock")
		return false
	end
	thinkbubble(player, "pickaxe")
	mes(player, "@que@You swing your pick at the rock...")
	delay(3)
	return true
end

local function do_mine_empty(player, x, y)
	do_initial_mining(player)
	mes(player, "@que@There is currently no ore available in this rock")
end

-- https://classic.runescape.wiki/w/Transcript:Rock_(uninteresting)
local function do_mine_plain(player, x, y)
	if not do_initial_mining(player) then
		return
	end
	if statrandom(player, STAT_MINING, 64, 375) then
		if not mine_random_jewel(player) then
			mes(player, "@que@A few lumps of uninteresting rock break off")
			changeloc(x, y, "plainrock3")
			restoreloc(x, y, 2)
		end
	else
		mes(player, "@que@You only succeed in scratching the rock")
	end
end

local function do_mine_clay(player, x, y)
	if not do_initial_mining(player) then
		return
	end
	-- https://classic.runescape.wiki/w/User:Stormykins/Mining_rates
	if statrandom(player, STAT_MINING, 64, 375) then
		if not mine_random_jewel(player) then
			mes(player, "@que@You manage to obtain some clay")
			give(player, "clay", 1)
			advancestat(player, STAT_MINING, 20, 0)
			changeloc(x, y, "clayrock3")
			restoreloc(x, y, 2)
		end
	else
		mes(player, "@que@You only succeed in scratching the rock")
	end
end

local function do_mine_tin(player, x, y)
	if not do_initial_mining(player) then
		return
	end
	-- https://classic.runescape.wiki/w/User:Stormykins/Mining_rates
	if statrandom(player, STAT_MINING, 64, 375) then
		if not mine_random_jewel(player) then
			mes(player, "@que@You manage to obtain some tin ore")
			give(player, "tin ore", 1)
			advancestat(player, STAT_MINING, 70, 0)
			changeloc(x, y, "tinrock3")
			restoreloc(x, y, 5)
		end
	else
		mes(player, "@que@You only succeed in scratching the rock")
	end
end

local function do_mine_copper(player, x, y)
	if not do_initial_mining(player) then
		return
	end
	-- https://classic.runescape.wiki/w/User:Stormykins/Mining_rates
	if statrandom(player, STAT_MINING, 64, 375) then
		if not mine_random_jewel(player) then
			mes(player, "@que@You manage to obtain some copper ore")
			give(player, "copper ore", 1)
			advancestat(player, STAT_MINING, 70, 0)
			changeloc(x, y, "copperrock3")
			restoreloc(x, y, 5)
		end
	else
		mes(player, "@que@You only succeed in scratching the rock")
	end
end

local function do_mine_iron(player, x, y)
	if not do_initial_mining(player) then
		return
	end
	if statatleast(player, STAT_MINING, 15) and
	    statrandom(player, STAT_MINING, 96, 350) then
		if not mine_random_jewel(player) then
			mes(player, "@que@You manage to obtain some iron ore")
			give(player, "iron ore", 1)
			advancestat(player, STAT_MINING, 140, 0)
			changeloc(x, y, "ironrock3")
			restoreloc(x, y, 10)
		end
	else
		mes(player, "@que@You only succeed in scratching the rock")
	end
end

local function do_mine_silver(player, x, y)
	if not do_initial_mining(player) then
		return
	end
	if statatleast(player, STAT_MINING, 20) and
	    statrandom(player, STAT_MINING, 25, 200) then
		if not mine_random_jewel(player) then
			mes(player, "@que@You manage to obtain some silver")
			give(player, "silver", 1)
			advancestat(player, STAT_MINING, 160, 0)
			changeloc(x, y, "silverrock3")
			restoreloc(x, y, 100)
		end
	else
		mes(player, "@que@You only succeed in scratching the rock")
	end
end

local function do_mine_coal(player, x, y)
	if not do_initial_mining(player) then
		return
	end
	if statatleast(player, STAT_MINING, 30) and
	    statrandom(player, STAT_MINING, 16, 100) then
		if not mine_random_jewel(player) then
			mes(player, "@que@You manage to obtain some coal")
			give(player, "coal", 1)
			advancestat(player, STAT_MINING, 200, 0)
			changeloc(x, y, "coalrock3")
			restoreloc(x, y, 40)
		end
	else
		mes(player, "@que@You only succeed in scratching the rock")
	end
end

local function do_mine_gold(player, x, y)
	if not do_initial_mining(player) then
		return
	end
	if statatleast(player, STAT_MINING, 40) and
	    statrandom(player, STAT_MINING, 7, 75) then
		if not mine_random_jewel(player) then
			mes(player, "@que@You manage to obtain some gold")
			give(player, "gold", 1)
			advancestat(player, STAT_MINING, 260, 0)
			changeloc(x, y, "goldrock3")
			restoreloc(x, y, 100)
		end
	else
		mes(player, "@que@You only succeed in scratching the rock")
	end
end

local function do_mine_mithril(player, x, y)
	if not do_initial_mining(player) then
		return
	end
	if statatleast(player, STAT_MINING, 55) and
	    statrandom(player, STAT_MINING, 5, 50) then
		if not mine_random_jewel(player) then
			mes(player, "@que@You manage to obtain some mithril ore")
			give(player, "mithril ore", 1)
			advancestat(player, STAT_MINING, 320, 0)
			changeloc(x, y, "mithrilrock3")
			restoreloc(x, y, 170)
		end
	else
		mes(player, "@que@You only succeed in scratching the rock")
	end
end

local function do_mine_adamantite(player, x, y)
	if not do_initial_mining(player) then
		return
	end
	if statatleast(player, STAT_MINING, 70) and
	    statrandom(player, STAT_MINING, 2, 25) then
		if not mine_random_jewel(player) then
			mes(player, "@que@You manage to obtain some adamantite ore")
			give(player, "adamantite ore", 1)
			advancestat(player, STAT_MINING, 380, 0)
			changeloc(x, y, "adamantiterock3")
			restoreloc(x, y, 300)
		end
	else
		mes(player, "@que@You only succeed in scratching the rock")
	end
end

local function do_mine_runite(player, x, y)
	if not do_initial_mining(player) then
		return
	end
	if statatleast(player, STAT_MINING, 85) and
	    statrandom(player, STAT_MINING, 1, 18) then
		if not mine_random_jewel(player) then
			mes(player, "@que@You manage to obtain some runite ore")
			give(player, "runite ore", 1)
			advancestat(player, STAT_MINING, 500, 0)
			changeloc(x, y, "runiterock3")
			restoreloc(x, y, 1400)
		end
	else
		mes(player, "@que@You only succeed in scratching the rock")
	end
end

function useloc_plainrock1_pickaxe(player, x, y)
	do_mine_plain(player, x, y)
end

function useloc_clayrock1_pickaxe(player, x, y)
	do_mine_clay(player, x, y)
end

function useloc_tinrock1_pickaxe(player, x, y)
	do_mine_tin(player, x, y)
end

function useloc_copperrock1_pickaxe(player, x, y)
	do_mine_copper(player, x, y)
end

function useloc_ironrock1_pickaxe(player, x, y)
	do_mine_copper(player, x, y)
end

function oploc1_plainrock1(player, x, y)
	do_mine_plain(player, x, y)
end

function oploc1_plainrock2(player, x, y)
	do_mine_plain(player, x, y)
end

function oploc1_plainrock3(player, x, y)
	do_mine_empty(player, x, y)
end

function oploc1_clayrock1(player, x, y)
	do_mine_clay(player, x, y)
end

function oploc1_clayrock2(player, x, y)
	do_mine_clay(player, x, y)
end

function oploc1_clayrock3(player, x, y)
	do_mine_empty(player, x, y)
end

function oploc1_tinrock1(player, x, y)
	do_mine_tin(player, x, y)
end

function oploc1_tinrock2(player, x, y)
	do_mine_tin(player, x, y)
end

function oploc1_tinrock3(player, x, y)
	do_mine_empty(player, x, y)
end

function oploc1_copperrock1(player, x, y)
	do_mine_copper(player, x, y)
end

function oploc1_copperrock2(player, x, y)
	do_mine_copper(player, x, y)
end

function oploc1_copperrock3(player, x, y)
	do_mine_empty(player, x, y)
end

function oploc1_ironrock1(player, x, y)
	do_mine_iron(player, x, y)
end

function oploc1_ironrock2(player, x, y)
	do_mine_iron(player, x, y)
end

function oploc1_ironrock3(player, x, y)
	do_mine_empty(player, x, y)
end

function oploc1_silverrock1(player, x, y)
	do_mine_silver(player, x, y)
end

function oploc1_silverrock2(player, x, y)
	do_mine_silver(player, x, y)
end

function oploc1_silverrock3(player, x, y)
	do_mine_empty(player, x, y)
end

function oploc1_coalrock1(player, x, y)
	do_mine_coal(player, x, y)
end

function oploc1_coalrock2(player, x, y)
	do_mine_coal(player, x, y)
end

function oploc1_coalrock3(player, x, y)
	do_mine_empty(player, x, y)
end

function oploc1_mithrilrock1(player, x, y)
	do_mine_mithril(player, x, y)
end

function oploc1_mithrilrock2(player, x, y)
	do_mine_mithril(player, x, y)
end

function oploc1_mithrilrock3(player, x, y)
	do_mine_empty(player, x, y)
end

function oploc1_adamantiterock1(player, x, y)
	do_mine_adamantite(player, x, y)
end

function oploc1_adamantiterock2(player, x, y)
	do_mine_adamantite(player, x, y)
end

function oploc1_adamantiterock3(player, x, y)
	do_mine_empty(player, x, y)
end

function oploc1_runiterock1(player, x, y)
	do_mine_runite(player, x, y)
end

function oploc1_runiterock2(player, x, y)
	do_mine_runite(player, x, y)
end

function oploc1_runiterock3(player, x, y)
	do_mine_empty(player, x, y)
end

register_useloc("plainrock1", "pickaxe", useloc_plainrock1_pickaxe)
register_useloc("plainrock2", "pickaxe", useloc_plainrock1_pickaxe)
register_useloc("tinrock3", "pickaxe", do_mine_empty)
register_useloc("tinrock1", "pickaxe", useloc_tinrock1_pickaxe)
register_useloc("tinrock2", "pickaxe", useloc_tinrock1_pickaxe)
register_useloc("tinrock3", "pickaxe", do_mine_empty)
register_useloc("copperrock1", "pickaxe", useloc_copperrock1_pickaxe)
register_useloc("copperrock2", "pickaxe", useloc_copperrock1_pickaxe)
register_useloc("copperrock3", "pickaxe", do_mine_empty)
register_useloc("ironrock1", "pickaxe", useloc_ironrock1_pickaxe)
register_useloc("ironrock2", "pickaxe", useloc_ironrock1_pickaxe)
register_useloc("ironrock3", "pickaxe", do_mine_empty)
register_useloc("mithrilrock1", "pickaxe", useloc_mithrilrock1_pickaxe)
register_useloc("mithrilrock2", "pickaxe", useloc_mithrilrock1_pickaxe)
register_useloc("mithrilrock3", "pickaxe", do_mine_empty)
register_useloc("adamantiterock1", "pickaxe", useloc_adamantiterock1_pickaxe)
register_useloc("adamantiterock2", "pickaxe", useloc_adamantiterock1_pickaxe)
register_useloc("adamantiterock3", "pickaxe", do_mine_empty)
register_useloc("coalrock1", "pickaxe", useloc_coalrock1_pickaxe)
register_useloc("coalrock2", "pickaxe", useloc_coalrock1_pickaxe)
register_useloc("coalrock3", "pickaxe", do_mine_empty)
register_useloc("goldrock1", "pickaxe", useloc_goldrock1_pickaxe)
register_useloc("goldrock2", "pickaxe", useloc_goldrock1_pickaxe)
register_useloc("goldrock3", "pickaxe", do_mine_empty)
register_useloc("clayrock1", "pickaxe", useloc_clayrock1_pickaxe)
register_useloc("clayrock2", "pickaxe", useloc_clayrock1_pickaxe)
register_useloc("clayrock3", "pickaxe", do_mine_empty)
register_useloc("silverrock1", "pickaxe", useloc_silverrock1_pickaxe)
register_useloc("silverrock2", "pickaxe", useloc_solverrock1_pickaxe)
register_useloc("silverrock3", "pickaxe", do_mine_empty)
register_useloc("runiterock1", "pickaxe", useloc_runiterock1_pickaxe)
register_useloc("runiterock2", "pickaxe", useloc_runiterock1_pickaxe)
register_useloc("runiterock3", "pickaxe", do_mine_empty)
