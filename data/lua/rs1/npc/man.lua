-- https://classic.runescape.wiki/w/Transcript:Man
-- https://github.com/2004Scape/Server/blob/main/data/src/scripts/npc/scripts/man.rs2#L29
-- frequencies from https://chisel.weirdgloop.org/dialogue/npcs/Man
-- This was the first script ever written for RSC Sundae.

local function man1_trade(player, npc)
	say(player, "Do you wish to trade?")
	npcsay(npc, "No, I have nothing I wish to get rid of")
	npcsay(npc, "If you want to do some trading,")
	npcsay(npc, "there are plenty of shops and market stalls around though")
end

local function man1_quest(player, npc)
	say(player, "I'm in search of a quest")
	npcsay(npc, "I'm sorry I can't help you there")
end


local function man1_enemies(player, npc)
	say(player, "I'm in search of enemies to kill")
	npcsay(npc, "I've heard there are many fearsome creatures under the ground")
end

function talknpc_man1(player, npc)
	say(player, "Hello")
	say(player, "How's it going?")

	local rand = randomvar(256)

	if rand < 46 then
		npcsay(npc, "How can I help you?")
		local resp = multi(player,
			"Do you wish to trade?",
			"I'm in search of a quest",
			"I'm in search of enemies to kill")
		if resp == 1 then
			man1_trade(player, npc)
		elseif resp == 2 then
			man1_quest(player, npc)
		elseif resp == 3 then
			man1_enemies(player, npc)
		end
	elseif rand < 83 then
		npcsay(npc, "I'm fine")
		npcsay(npc, "How are you?")
		say(player, "Very well, thank you")
	elseif rand < 103 then
		npcsay(npc, "Not too bad")
	elseif rand < 119 then
		npcsay(npc, "Who are you?")
		say(player, "I am a bold adventurer")
		npcsay(npc, "A very noble profession")
	elseif rand < 134 then
		npcsay(npc, "Get out of my way")
		npcsay(npc, "I'm in a hurry")
	elseif rand < 149 then
		npcsay(npc, "I'm fine")
		npcsay(npc, "How are you?")
		say(player, "Very well, thank you")
	elseif rand < 164 then
		man1_trade(player, npc)
	elseif rand < 178 then
		man1_quest(player, npc)
	elseif rand < 192 then
		npcsay(npc, "I'm a little worried about the increase in Goblins these days")
		say(player, "Don't worry. I'll kill them")
	elseif rand < 202 then
		npcsay(npc, "Hello")
		npcsay(npc, "Nice weather we've been having")
	elseif rand < 211 then
		man1_enemies(player, npc)
	elseif rand < 220 then
		mes(player, "@que@The man ignores you")
		-- TODO npcretreat()?
	elseif rand < 228 then
		npcsay(npc, "I'm a little worried")
		npcsay(npc, "I've heard there's lots of people going about,")
		npcsay(npc, "killing citizens at random")
	elseif rand < 233 then
		npcsay(npc, "Hello")
	elseif rand < 237 then
		npcsay(npc, "No, I don't want to buy anything")
	elseif rand < 241 then
		npcsay(npc, "Get out of my way")
		npcsay(npc, "I'm in a hurry")
	elseif rand < 244 then
		npcsay(npc, "I think we need a new king")
		npcsay(npc, "The one we've got isn't very good")
	elseif rand < 247 then
		npcsay(npc, "None of your business")
	elseif rand < 249 then
		npcsay(npc, "No, I don't have any spare change")
	elseif rand < 251 then
		npcsay(npc, "Yo wassup!")
	elseif rand < 253 then
		npcsay(npc, "Are you asking for a fight?")
		npcattack(npc, player)
	elseif rand < 255 then
		npcsay(npc, "That is classified information")
	else
		npcsay(npc, "Have this flier")
		give(player, "flier", 1)
	end
end
