-- https://classic.runescape.wiki/w/Transcript:Barbarian

function talknpc_barbarian(player, npc)
	say(player, "Hello")
	local rand = randomvar(11)
	if rand == 0 then
		npcsay(npc, "Hello")
	elseif rand == 1 then
		mes(player, "@que@The barbarian grunts")
	elseif rand == 2 then
		npcsay(npc, "Good day, my dear fellow")
	elseif rand == 3 then
		npcsay(npc, "ug")
	elseif rand == 4 then
		mes(player, "@que@The barbarian ignores you")
	elseif rand == 5 then
		npcsay(npc, "Grr")
	elseif rand == 6 then
		npcsay(npc, "Wanna fight?")
		npcattack(npc, player)
	elseif rand == 7 then
		npcsay(npc, "I'm a little busy right now")
		npcsay(npc, "We're getting ready for our next barbarian raid")
	elseif rand == 8 then
		npcsay(npc, "Go away")
		npcsay(npc, "This is our village")
	elseif rand == 9 then
		npcsay(npc, "Who are you?")
		say(player, "I'm a bold adventurer")
		npcsay(npc, "You don't look very strong")
	else
		npcsay(npc, "Beer?")
	end	
end
