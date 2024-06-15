-- ./rscplus-replays-master/RSC 2001/3/f2p (not 100% complete)/dialogue- scavvo- all- shop
-- https://classic.runescape.wiki/w/Transcript:Scavvo

function talknpc_scavvo(player, npc)
	npcsay(npc, "Ello matey")
	npcsay(npc, "Want to buy some exciting new toys?")
	local resp = multi(player,
		"No, toys are for kids",
		"Lets have a look then",
		"Ooh goody goody toys")
	if resp == 1 then
		say(player, "No, toys are for kids")
	elseif resp == 2 then
		say(player, "Lets have a look then")
		openshop(player, "runiteshop")
	elseif resp == 3 then
		say(player, "Ooh goody goody toys")
		openshop(player, "runiteshop")
	end
end
