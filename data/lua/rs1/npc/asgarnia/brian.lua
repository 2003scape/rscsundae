-- https://classic.runescape.wiki/w/Transcript:Brian

function talknpc_brian(player, npc)
	npcsay(npc, "ello")
	local resp = multi(player,
		"So are you selling something?",
		"ello")
	if resp == 1 then
		say(player, "So are you selling something?")
		npcsay(npc, "Yep take a look at these great axes")
		openshop(player, "battleaxeshop")
	elseif resp == 2 then
		say(player, "Ello")
	end
end
