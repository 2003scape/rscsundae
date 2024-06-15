-- https://classic.runescape.wiki/w/Transcript:Cassie

function talknpc_cassie(player, npc)
	say(player, "What wares are you selling?")
	npcsay(npc, "I buy and sell shields")
	npcsay(npc, "Do you want to trade?")
	local resp = multi(player,
		"Yes please",
		"No thank you")
	if resp == 1 then
		say(player, "Yes please")
		openshop(player, "shieldshop")
	elseif resp == 2 then
		say(player, "No thank you")
	end
end
