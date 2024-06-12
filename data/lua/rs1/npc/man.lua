-- https://classic.runescape.wiki/w/Transcript:Man
-- https://github.com/2004Scape/Server/blob/main/data/src/scripts/npc/scripts/man.rs2#L29
-- This was the first script ever written for RSC Sundae.

function man1_trade(player, npc)
	say(player, "Do you wish to trade?")
	npcsay(npc, "No, I have nothing I wish to get rid of")
	npcsay(npc, "If you want to do some trading,")
	npcsay(npc, "there are plenty of shops and market stalls around though")
end

function man1_quest(player, npc)
	say(player, "I'm in search of a quest")
	npcsay(npc, "I'm sorry I can't help you there")
end


function man1_enemies(player, npc)
	say(player, "I'm in search of enemies to kill")
	npcsay(npc, "I've heard there are many fearsome creatures under the ground")
end

function talknpc_man1(player, npc)
	say(player, "Hello")
	say(player, "How's it going?")

	if random(12) then
		npcsay(npc, "How can I help you?")
		local opt = multi(player,
			"Do you wish to trade?",
			"I'm in search of a quest",
			"I'm in search of enemies to kill")
		if opt == 1 then
			man1_trade(player, npc)
		elseif opt == 2 then
			man1_quest(player, npc)
		elseif opt == 3 then
			man1_enemies(player, npc)
		end
	elseif random(12) then
		npcsay(npc, "How can I help you?")
		man1_trade(player, npc)
	elseif random(12) then
		npcsay(npc, "How can I help you?")
		man1_quest(player, npc)
	elseif random(12) then
		npcsay(npc, "How can I help you?")
		man1_enemies(player, npc)
	elseif random(12) then
		npcsay(npc, "I'm a little worried")
		npcsay(npc, "I've heard there's lots of people going about,")
		npcsay(npc, "killing citizens at random")
	elseif random(12) then
		npcsay(npc, "I'm a little worried about the increase in Goblins these days")
		say(player, "Don't worry. I'll kill them")
	elseif random(12) then
		npcsay(npc, "Not too bad")
	elseif random(12) then
		mes(player, "@que@The man ignores you")
	elseif random(12) then
		npcsay(npc, "None of your business")
	elseif random(12) then
		npcsay(npc, "Get out of my way")
		npcsay(npc, "I'm in a hurry")
	elseif random(12) then
		npcsay(npc, "I'm fine")
		npcsay(npc, "How are you?")
		say(player, "Very well, thank you")
	elseif random(12) then
		npcsay(npc, "Are you asking for a fight?")
		npcattack(npc, player)
	elseif random(12) then
		npcsay(npc, "Hello")
		npcsay(npc, "Nice weather we've been having")
	elseif random(12) then
		npcsay(npc, "That is classified information")
	elseif random(12) then
		npcsay(npc, "Who are you?")
		say(player, "I am a bold adventurer")
		npcsay(npc, "A very noble profession")
	elseif random(12) then
		npcsay(npc, "Do I know you?")
		say(player, "No, I was just wondering if you had anything interesting to say")
	elseif random(12) then
		npcsay(npc, "Have this flier")
		give(player, "flier", 1)
	elseif random(12) then
		npcsay(npc, "I think we need a new king")
		npcsay(npc, "The one we've got isn't very good")
	elseif random(12) then
		npcsay(npc, "No, I don't have any spare change")
	elseif random(12) then
		npcsay(npc, "No, I don't want to buy anything")
	else
		npcsay(npc, "Hello")
	end
end
