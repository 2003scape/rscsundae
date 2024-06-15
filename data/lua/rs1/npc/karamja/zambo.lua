-- ./rscplus-replays-master/RSC 2001/replays master archive/NPC (dialogue, notes)/Zambo/dialogue- zambos spirits and wines- karamja
-- https://classic.runescape.wiki/w/Transcript:Zambo

function talknpc_zambo(player, npc)
	npcsay(npc, "Hey are you wanting to try some of my fine wines and spirits?")
	npcsay(npc, "All brewed locally on Karamja island")
	local resp = multi(player,
		"Yes please",
		"No thankyou")
	if resp == 1 then
		say(player, "Yes please")
		openshop(player, "boozeshop")
	elseif resp == 2 then
		say(player, "No thankyou")
	end
end
