-- https://classic.runescape.wiki/w/Transcript:Apothecary

local function apothecary_strength(player, npc)
	say(player, "Can you make a strength potion?")
	if not held(player, "red spiders eggs", 1) or
	    not held(player, "limpwurt root", 1) or
	    not held(player, "coins", 5) then
		npcsay(npc, "Yes. But the ingredients are a little hard to find")
		npcsay(npc, "If you ever get them I will make it for you. For a cost")
		say(player, "So what are the ingredients?")
		npcsay(npc, "You'll need to find the eggs of the deadly red spider")
		npcsay(npc, "And a limpwurt root")
		npcsay(npc, "Oh and you'll have to pay me 5 coins")
		say(player, "Ok, I'll look out for them")
		return
	end
	say(player, "I have the root and spiders eggs needed to make it")
	npcsay(npc, "Well give me them and 5 gold and I'll make you your potion")
	local resp = multi(player, "Yes ok", "No thanks")
	if resp == 1 then
		say(player, "Yes ok")
		-- RSC 2001/replays master archive/NPC (dialogue, notes)/Apothecary/dialogue- apothecary- can you make a potion- success.pcap
		mes(player, "You give a limpwurt root some red spiders eggs and 5 coins to the apothecary")
		remove(player, "coins", 5)
		remove(player, "red spiders eggs", 1)
		remove(player, "limpwurt root", 1)
		delay(3)
		mes(player, "The Apothecary brews up a potion")
		delay(5)
		mes(player, "The Apothecary gives you a strength potion")
		give(player, "strength4", 1)
	elseif resp == 2 then
		say(player, "No thanks")
	end
end

local function apothecary_normal(player, npc)
	npcsay(npc, "I am the apothecary")
	npcsay(npc, "I have potions to brew. Do you need anything specific?")
	local resp = multi(player,
		"Can you make a strength potion?",
		"Do you know a potion to make hair fall out?",
		"Have you got any good potions to give way?")
	if resp == 1 then
		apothecary_strength(player, npc)
	elseif resp == 2 then
		say(player, "Do you know a potion to make hair fall out?")
		npcsay(npc, "I do indeed. I gave it to my mother. That's why I now live alone")
	elseif resp == 3 then
		say(player, "Have you got any good potions to give way?")
		if held(player, "acnepotion", 1) then
			npcsay(npc, "Only that spot cream. Hope you enjoy it")
			-- in reality there's fallthrough here, see:
			-- RSC 2001/replays master archive/NPC (dialogue, notes)/Apothecary/dialogue- apothecary- alrdy have potion- do you have a free potion
			-- but might as well fix the bug...
		elseif random(128) then
			npcsay(npc, "Yes, ok. Try this potion")
			give(player, "acnepotion", 1)
		else
			npcsay(npc, "Sorry, charity is not my strong point")
		end
	end
end

function talknpc_apothecary(player, npc)
	local stage = getvar(player, "rj_stage")
	if stage == 3 then
		say(player, "Apothecary. Father Lawrence sent me")
		say(player, "I need some Cadava potion to help Romeo and Juliet")
		npcsay(npc, "Cadava potion. Its pretty nasty. And hard to make")
		npcsay(npc, "Wing of Rat, Tail of frog. Ear of snake and horn of dog")
		npcsay(npc, "I have all that, but I need some cadavaberries")
		npcsay(npc, "You will have to find them while I get the rest ready")
		npcsay(npc, "Bring them here when you have them. But be careful. They are nasty")
		setvar(player, "rj_stage", 4)
	elseif stage == 4 then
		if not held(player, "cadavaberries", 1) then
			npcsay(npc, "Keep searching for the berries")
			npcsay(npc, "They are needed for the potion")
			return
		end
		-- 1e_Luis/Quests/Romeo & Juliet.pcap
		npcsay(npc, "Well done. You have the berries")
		remove(player, "cadavaberries", 1)
		mes(player, "You hand over the berries")
		delay(3)
		mes(player, "Which the apothecary shakes up in vial of strange liquid")
		npcsay(npc, "Here is what you need")
		mes(player, "The apothecary gives you a Cadava potion")
		give(player, "cadava", 1)
	else
		apothecary_normal(player, npc)
	end
end
