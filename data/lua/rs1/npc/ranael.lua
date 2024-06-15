-- https://classic.runescape.wiki/w/Transcript:Ranael

function talknpc_ranael(player, npc)
	npcsay(npc, "Do you want to buy any armoured skirts?")
	npcsay(npc, "Designed especially for ladies who like to fight")
	local resp = multi(player,
		"Yes please",
		"No thank you that's not my scene")
	if resp == 1 then
		say(player, "Yes please")
		openshop(player, "skirtshop")
	elseif resp == 2 then
		say(player, "No thank you that's not my scene")
	end
end
