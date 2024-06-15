-- https://classic.runescape.wiki/w/Transcript:Banker

function talknpc_banker(player, npc)
	npcsay(npc, "Good day, how may I help you?")
	local resp = multi(player,
		"I'd like to access my bank account please",
		"What is this place?")
	if resp == 1 then
		say(player, "I'd like to access my bank account please")
		if male(player) then
			npcsay(npc, "Certainly Sir")
		else
			npcsay(npc, "Certainly Miss")
		end
		displaybalance(player)
	elseif resp == 2 then
		-- assumed from later "bank of runescape" dialogue
		say(player, "What is this place?")
		npcsay(npc, "This is a branch of the bank of Varrock")
		npcsay(npc, "We have branches in many towns")
		say(player, "And what do you do?")
		npcsay(npc, "We will look after your items and money for you")
		npcsay(npc, "So leave your valuables with us if you want to keep them safe")
	end
end
