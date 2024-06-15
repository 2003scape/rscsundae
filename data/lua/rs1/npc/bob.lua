-- https://classic.runescape.wiki/w/Transcript:Bob

function talknpc_bob(player, npc)
	npcsay(npc, "Hello. How can I help you?")
	local resp = multi(player,
		"Give me a quest!",
		"Have you anything to sell?")
	if resp == 1 then
		say(player, "Give me a quest!")
		npcsay(npc, "Get yer own!")
	elseif resp == 2 then
		say(player, "Have you anything to sell?")
		npcsay(npc, "Yes, I buy and sell axes, take you pick! (or axe)")
		openshop(player, "axeshop")
	end
end
