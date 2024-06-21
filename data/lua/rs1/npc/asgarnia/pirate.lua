-- https://classic.runescape.wiki/w/Transcript:Pirate

function talknpc_pirate(player, npc)
	say(player, "Hello")
	local rand = randomvar(25)
	if rand == 0 then
		npcsay(npc, "avast behind")
		say(player, "I'm not that fat")
	elseif rand == 1 then
		npcsay(npc, "I'm the scourge of the seven seas")
	elseif rand == 2 then
		npcsay(npc, "Arrh, I be in search of buried treasure")
	elseif rand == 3 then
		npcsay(npc, "Arrrh ye lily livered landlubber")
	elseif rand == 4 then
		npcsay(npc, "Arrh")
	elseif rand == 5 then
		npcsay(npc, "Yo ho ho me hearties")
	elseif rand == 6 then
		npcsay(npc, "Splice the mainbrace")
	elseif rand == 7 then
		npcsay(npc, "Avast me hearties")
	elseif rand == 8 then
		npcsay(npc, "Arrh I'll keel haul ye")
	elseif rand == 9 then
		npcsay(npc, "shiver me timbers")
	elseif rand == 10 then
		npcsay(npc, "Arrh be off with ye")
	elseif rand == 11 then
		npcsay(npc, "Yo ho ho and bottle of alchopop")
	elseif rand == 12 then
		npcsay(npc, "Arrh ye scury sea dog")
	elseif rand == 13 then
		npcsay(npc, "Batton down the hatches there's a storm a brewin")
	elseif rand == 14 then
		npcsay(npc, "A pox on ye")
	elseif rand == 15 then
		npcsay(npc, "Yo ho ho and a bottle of a rum")
	elseif rand == 16 then
		npcsay(npc, "3 days at port for resupply then out on the high sea")
	elseif rand == 17 then
		npcsay(npc, "Keel haul them I say")
	elseif rand == 18 then
		npcsay(npc, "Avast behind")
	elseif rand == 19 then
		npcsay(npc, "I think ye'll be taking a long walk off a short plank")
		npcattack(npc, player)
	elseif rand == 20 then
		npcsay(npc, "Good day to you my dear sir")
	elseif rand == 21 then
		npcsay(npc, "Great blackbeard's beard")
	elseif rand == 22 then
		npcsay(npc, "Arrh arrh")
	elseif rand == 23 then
		npcsay(npc, "Man overboard")
	else
		npcsay(npc, "Ahoy there")
	end
end
