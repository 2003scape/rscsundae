-- RSC 2001/3/f2p (not 100% complete)/dialogue- grum- all- shop
-- https://classic.runescape.wiki/w/Transcript:Grum

function talknpc_grum(player, npc)
	npcsay(npc, "Would you like to buy or sell some gold jewellery?")
	local resp = multi(player,
		"Yes please",
		"No, I'm not that rich")
	if resp == 1 then
		say(player, "Yes Please")
		openshop(player, "goldshop")
	elseif resp == 2 then
		say(player, "No, I'm not that rich")
		npcsay(npc, "Get out then we don't want any riff-raff in here")
	end
end
