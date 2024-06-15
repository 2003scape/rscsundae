-- ./rscplus-replays-master/RSC 2001/replays master archive/NPC (dialogue, notes)/Herquin/dialogue- herquin- all- shop
-- https://classic.runescape.wiki/w/Transcript:Herquin

function talknpc_herquin(player, npc)
	local resp = multi(player,
		"Do you wish to trade?",
		"Sorry i don't want to talk to you actually")
	if resp == 1 then
		say(player, "Do you wish to trade?")
		npcsay(npc, "Why yes this a jewel shop after all")
		openshop(player, "gemshop")
	elseif resp == 2 then
		say(player, "Sorry I don't want to talk to you actually")
	end
end
