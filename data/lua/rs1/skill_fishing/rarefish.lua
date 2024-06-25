-- 1e_Luis/Skilling/Fishing distinct fishes.pcap
function useloc_rarefish_harpoon(player, x, y)
	if not statatleast(player, STAT_FISHING, 35) then
		mes(player, "@que@You need at least level 35 fishing to harpoon these fish")
		return
	end
	thinkbubble(player, "harpoon")
	mes(player, "@que@You attempt to catch some fish")
	delay(4)
	-- https://oldschool.runescape.wiki/w/Raw_swordfish?oldid=14628791&action=edit
	if statatleast(player, STAT_FISHING, 50) and
	    statrandom(player, STAT_FISHING, 4, 48) then
		mes(player, "@que@You catch a swordfish")
		give(player, "raw swordfish", 1)
		advancestat(player, STAT_FISHING, 400, 0)
	elseif statrandom(player, STAT_FISHING, 8, 64) then
		mes(player, "@que@You catch a tuna")
		give(player, "raw tuna", 1)
		advancestat(player, STAT_FISHING, 320, 0)
	else
		mes(player, "@que@You fail to catch anything")
	end
end

register_useloc("rarefish", "harpoon", useloc_rarefish_harpoon)

-- 1e_Luis/Skilling/Fishing distinct fishes.pcap
function useloc_rarefish_lobster_pot(player, x, y)
	if not statatleast(player, STAT_FISHING, 40) then
		-- Logg/Loggykins/08-02-2018 15.18.06 cleaning tylerbegs bank, helping document catherby, start training combat in seers, document aronud fishing contest.pcap
		mes(player, "@que@You need at least level 40 fishing to catch lobsters")
		return
	end
	thinkbubble(player, "lobster pot")
	mes(player, "@que@You attempt to catch a lobster")
	delay(4)
	-- https://oldschool.runescape.wiki/w/Raw_lobster?oldid=14513944&action=edit
	if statrandom(player, STAT_FISHING, 6, 95) then
		mes(player, "@que@You catch a lobster")
		give(player, "raw lobster", 1)
		advancestat(player, STAT_FISHING, 360, 0)
	else
		mes(player, "@que@You fail to catch anything")
	end
end

register_useloc("rarefish", "lobster pot", useloc_rarefish_lobster_pot)

function oploc1_rarefish(player, x, y)
	if not held(player, "harpoon", 1) then
		-- Logg/Tylerbeg/08-04-2018 01.17.18 karamja, banana crate, karamja f2p dungeon, crandor, brimhaven, north karamja, fisher king rhelm, spam whistle.pcap
		mes(player, "@que@You need a harpoon to catch these fish")
		return
	end
	useloc_rarefish_harpoon(player, x, y)
end

function oploc2_rarefish(player, x, y)
	if not held(player, "lobster pot", 1) then
		-- Logg/Tylerbeg/08-04-2018 01.17.18 karamja, banana crate, karamja f2p dungeon, crandor, brimhaven, north karamja, fisher king rhelm, spam whistle.pcap
		mes(player, "@que@You need a lobster pot to catch lobsters")
		return
	end
	useloc_rarefish_lobster_pot(player, x, y)
end
