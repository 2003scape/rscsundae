-- https://classic.runescape.wiki/w/Transcript:Curator
-- 1e_Luis/Quests/Shield of Arrav/Black Arm Gang/Part 2.pcap

function talknpc_curator(player, npc)
	npcsay(npc, "Welcome to the museum of Varrock")
	if held(player, "arravshield1", 1) and
	    held(player, "arravshield2", 1) then
		say(player, "I have retrieved the shield of Arrav and I would like to claim my reward")
		npcsay(npc, "The shield of Arrav?")
		npcsay(npc, "Let me see that")
		mes(player, "The curator peers at the shield")
		delay(5)
		npcsay(npc, "This is incredible")
		npcsay(npc, "That shield has been missing for about twenty five years")
		npcsay(npc, "Well give me the shield")
		npcsay(npc, "And I'll write you out a certificate")
		npcsay(npc, "Saying you have returned the shield")
		npcsay(npc, "So you can claim your reward from the king")
		say(player, "Can I have two certificates?")
		say(player, "I needed significant help from a friend to get the shield")
		say(player, "We'll split the reward")
		npcsay(npc, "Oh ok")
		mes(player, "You hand over the shield parts")
		remove(player, "arravshield1", 1)
		remove(player, "arravshield2", 1)
		delay(3)
		mes(player, "The curator writes out two certificates")
		give(player, "certificate", 2)
		npcsay(npc, "Take these to the king")
		npcsay(npc, "And he'll pay you both handsomely")
	elseif held(player, "arravshield1", 1) or
		    held(player, "arravshield2", 1) then
		say(player, "I have half the shield of Arrav here")
		say(player, "Can I get a reward")
		npcsay(npc, "Well it might be worth a small reward")
		npcsay(npc, "The entire shield would me worth much much more")
		say(player, "Ok I'll hang onto it")
		say(player, "And see if I can find the other half")
	else
		local resp = multi(player,
			"Have you any interesting news?",
			"Do you know where I could find any treasure?")
		if resp == 1 then
			say(player, "Have you any interesting news?")
			npcsay(npc, "No, I'm only interested in old stuff")
		elseif resp == 2 then
			say(player, "Do you know where I could find any treasure?")
			npcsay(npc, "This museum is full of treasures")
			say(player, "No, I meant treasures for me")
			npcsay(npc, "Any treasures this museum knows about")
			npcsay(npc, "It aquires")
		end
	end
end
