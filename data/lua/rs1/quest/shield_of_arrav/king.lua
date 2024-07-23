-- https://classic.runescape.wiki/w/Transcript:King
-- 1e_Luis/Quests/Shield of Arrav/Black Arm Gang/Part 2.pcap

function talknpc_king(player, npc)
	if held(player, "certificate", 1) then
		local stage = getvar(player, "arrav_stage")
		say(player, "Your majesty")
		say(player, "I have come to claim the reward")
		say(player, "For the return of the shield of Arrav")
		if stage > 4 then
			npcsay(npc, "You have already claimed the reward")
			npcsay(npc, "You can't claim it twice")
			mes(player, "Why don't you give this certificate")
			delay(3)
			mes(player, "To whoever helped you get the shield")
		elseif stage < 4 then
			npcsay(npc, "The name on this certificate isn't yours!")
			npcsay(npc, "I can't give you the reward")
			npcsay(npc, "Unless you do the quest yourself")
		else
			mes(player, "You show the certificate to the king")
			delay(3)
			npcsay(npc, "My goodness")
			npcsay(npc, "This is the claim for a reward put out by my father")
			npcsay(npc, "I never thought I'd see anyone claim this reward")
			npcsay(npc, "I see you are claiming half the reward")
			npcsay(npc, "So that would come to 600 gold coins")
			mes(player, "You hand over a certificate")
			delay(3)
			mes(player, "The king gives you 600 coins")
			delay(3)
			mes(player, "Well done, you have completed the shield of Arrav quest")
			remove(player, "certificate", 1)
			give(player, "coins", 600)
			setvar(player, "arrav_stage", 5)
			giveqp(player, 1)
		end
	elseif held(player, "arravshield1", 1) or
	    held(player, "arravshield2", 1) then
		say(player, "Your majesty")
		say(player, "I have recovered the shield of Arrav")
		say(player, "I would like to claim the reward")
		npcsay(npc, "The shield of Arrav, eh?")
		npcsay(npc, "Yes, I do recall my father putting a reward out for that")
		npcsay(npc, "Very well")
		npcsay(npc, "Go get the authenticity of the shield verified")
		npcsay(npc, "By the curator at the museum")
		npcsay(npc, "And I will grant you your reward")
	else
		say(player, "Greetings, your majesty")
		npcsay(npc, "Do you have anything of import to say?")
		say(player, "Not really")
		npcsay(npc, "You will have to excuse me then")
		npcsay(npc, "I am very busy")
		npcsay(npc, "I have a kingdom to run")
	end
end
