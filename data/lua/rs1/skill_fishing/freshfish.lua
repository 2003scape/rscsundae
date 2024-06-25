-- rsc-preservation.xyz/Skilling/Fishing_Cooking [Barbarian Village] [Fly Fishing] [Fishing Level 22-41] [Cooking Level 15-40] part 1.pcap
function useloc_freshfish_fly_fishing_rod(player, x, y)
	if not statatleast(player, STAT_FISHING, 20) then
		-- 1e_Luis/Quests/Fishing Contest.pcap
		mes(player, "@que@You need at least level 20 fishing to lure these fish")
		return
	end
	if not held(player, "feather", 1) then
		-- flying sno (redacted chat) replays/fsnom2@aol.com/07-30-2018 09.06.45.pcap
		mes(player, "@que@You don't have any feathers left to lure the fish")
		return
	end
	thinkbubble(player, "fly fishing rod")
	mes(player, "@que@You attempt to catch a fish")
	delay(4)
	-- https://oldschool.runescape.wiki/w/Raw_salmon?oldid=14536367&action=edit
	if statatleast(player, STAT_FISHING, 30) and
	    statrandom(player, STAT_FISHING, 16, 96) then
		mes(player, "@que@You catch a salmon")
		remove(player, "feather", 1)
		give(player, "raw salmon", 1)
		advancestat(player, STAT_FISHING, 360, 0)
	elseif statrandom(player, STAT_FISHING, 32, 192) then
		mes(player, "@que@You catch a trout")
		remove(player, "feather", 1)
		give(player, "raw trout", 1)
		advancestat(player, STAT_FISHING, 280, 0)
	else
		mes(player, "@que@You fail to catch anything")
	end
end

register_useloc("freshfish", "fly fishing rod", useloc_freshfish_fly_fishing_rod)

function oploc1_freshfish(player, x, y)
	if not held(player, "fly fishing rod", 1) then
		-- Logg/Tylerbeg/08-02-2018 03.50.02 barbarian village and edgeville.pcap
		mes(player, "@que@You need a fly fishing rod to lure these fish")
		return
	end
	useloc_freshfish_fly_fishing_rod(player, x, y)
end

-- 1e_Luis/Skilling/Fishing distinct fishes.pcap
function useloc_freshfish_fishing_rod(player, x, y)
	if not statatleast(player, STAT_FISHING, 25) then
		-- 1e_Luis/Quests/Heroes Quest/Heroes Quest Pt1.pcap
		mes(player, "@que@You need at least level 25 fishing to bait these fish")
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
	-- https://oldschool.runescape.wiki/w/Raw_pike?oldid=14645857&action=edit
	if statrandom(player, STAT_FISHING, 16, 96) then
		mes(player, "@que@You catch a pike")
		remove(player, "fishing bait", 1)
		give(player, "raw pike", 1)
		advancestat(player, STAT_FISHING, 240, 0)
	else
		mes(player, "@que@You fail to catch anything")
	end
end

register_useloc("freshfish", "fishing rod", useloc_freshfish_fishing_rod)

function oploc2_freshfish(player, x, y)
	if not held(player, "fishing rod", 1) then
		-- Logg/Tylerbeg/08-02-2018 03.50.02 barbarian village and edgeville.pcap
		mes(player, "@que@You need a fishing rod to bait these fish")
		return
	end
	useloc_freshfish_fishing_rod(player, x, y)
end
