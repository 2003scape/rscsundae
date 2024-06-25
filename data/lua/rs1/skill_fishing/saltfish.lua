-- rsc-preservation.xyz/Skilling/Fishing [Draynor] [Small Fishing Net] [Level 15-20].pcap
function useloc_saltfish_net(player, x, y)
	thinkbubble(player, "net")
	mes(player, "@que@You attempt to catch some fish")
	delay(4)
	-- https://oldschool.runescape.wiki/w/Raw_shrimps?oldid=14679146
	if statatleast(player, STAT_FISHING, 15) and
	    statrandom(player, STAT_FISHING, 24, 128) then
		mes(player, "@que@You catch some anchovies")
		give(player, "raw anchovies", 1)
		advancestat(player, STAT_FISHING, 40, 0)
	elseif statrandom(player, STAT_FISHING, 48, 256) then
		mes(player, "@que@You catch some shrimps")
		give(player, "raw shrimp", 1)
		advancestat(player, STAT_FISHING, 40, 0)
	else
		mes(player, "@que@You fail to catch anything")
	end
end

register_useloc("saltfish", "net", useloc_saltfish_net)

-- rsc-preservation.xyz/Skilling/Draynor fishing rod with bait level 5 to 10.pcap
function useloc_saltfish_fishing_rod(player, x, y)
	if not statatleast(player, STAT_FISHING, 5) then
		-- guessed message, but probably correct (same as pike except the number)
		mes(player, "@que@You need at least level 5 fishing to bait these fish")
		return
	end
	if not held(player, "fishing bait", 1) then
		-- 1e_Luis/Quests/Heroes Quest/Heroes Quest Pt1.pcap
		mes(player, "@que@You don't have any fishing bait left")
		return
	end
	thinkbubble(player, "fishing rod")
	mes(player, "@que@You attempt to catch a fish")
	delay(4)
	-- https://oldschool.runescape.wiki/w/Raw_sardine?oldid=14552532
	if statatleast(player, STAT_FISHING, 10) and
	    statrandom(player, STAT_FISHING, 24, 128) then
		mes(player, "@que@You catch a herring")
		remove(player, "fishing bait", 1)
		give(player, "raw herring", 1)
		advancestat(player, STAT_FISHING, 120, 0)
	elseif statrandom(player, STAT_FISHING, 32, 192) then
		mes(player, "@que@You catch a sardine")
		remove(player, "fishing bait", 1)
		give(player, "raw sardine", 1)
		advancestat(player, STAT_FISHING, 80, 0)
	else
		mes(player, "@que@You fail to catch anything")
	end
end

register_useloc("saltfish", "fishing rod", useloc_saltfish_fishing_rod)

function oploc1_saltfish(player, x, y)
	-- rsc-preservation.xyz/Skilling/Draynor fishing rod with bait level 5 to 10.pcap
	if not held(player, "net", 1) then
		mes(player, "@que@You need a net to catch these fish")
		return
	end
	useloc_saltfish_net(player, x, y)
end

function oploc2_saltfish(player, x, y)
	-- Logg/Tylerbeg/08-02-2018 03.50.02 barbarian village and edgeville.pcap
	if not held(player, "fishing rod", 1) then
		mes(player, "@que@You need a fishing rod to bait these fish")
		return
	end
	useloc_saltfish_fishing_rod(player, x, y)
end
