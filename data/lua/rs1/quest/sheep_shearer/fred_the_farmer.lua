-- https://classic.runescape.wiki/w/Transcript:Fred_the_farmer

local function fred_start(player, npc)
	say(player, "Yes okay. I can do that")
	npcsay(npc, "Ok I'll see you when you have some wool")
	setvar(player, "fred_stage", 1)
end

local function fred_thing(player, npc)
	say(player, "What do you mean, the thing?")
	npcsay(npc, "I wouldn't worry about it")
	npcsay(npc, "Something ate all the previous shearers")
	npcsay(npc, "They probably got unlucky")
	npcsay(npc, "So are you going to help me?")
	local resp = multi(player,
		"Yes okay. I can do that",
		"Erm I'm a bit worried about this thing")
	if resp == 1 then
		fred_start(player, npc)
	elseif resp == 2 then
		-- you can tell whoever wrote this has a non-rhotic english dialect
		say(player, "Erm I'm a bit worried about this thing")
		npcsay(npc, "I'm sure it's nothing to worry about")
		npcsay(npc, "It's possible the other shearers aren't dead at all")
		npcsay(npc, "And are just hiding in the woods or something")
		say(player, "I'm not convinced")
	end
end

local function fred_exciting(player, npc)
	say(player, "That doesn't sound a very exciting quest")
	npcsay(npc, "Well what do you expect if you ask a farmer for a quest?")
	npcsay(npc, "Now are you going to help me or not?")
	local resp = multi(player,
		"Yes okay. I can do that",
		"No I'll give it a miss")
	if resp == 1 then
		fred_start(player, npc)
	elseif resp == 2 then
		say(player, "No I'll give it a miss")
	end
end

local function fred_quest(player, npc)
	say(player, "I'm looking for a quest")
	npcsay(npc, "You're after a quest, you say?")
	npcsay(npc, "Actually I could do with a bit of help")
	npcsay(npc, "My sheep are getting mighty woolly")
	npcsay(npc, "If you could shear them")
	npcsay(npc, "And while your at it spin the wool for me too")
	npcsay(npc, "Yes that's it. Bring me 20 balls of wool")
	npcsay(npc, "And I'm sure I could sort out some sort of payment")
	npcsay(npc, "Of course, there's the small matter of the thing")
	local resp = multi(player,
		"Yes okay. I can do that",
		"That doesn't sound a very exciting quest",
		"What do you mean, the thing?")
	if resp == 1 then
		fred_start(player, npc)
	elseif resp == 2 then
		fred_exciting(player, npc)
	elseif resp == 3 then
		fred_thing(player, npc)
	end
end

local function fred_kill(player, npc)
	say(player, "I'm looking for something to kill")
	npcsay(npc, "What on my land?")
	npcsay(npc, "Leave my livestock alone you scoundrel")
end

local function fred_lost(player, npc)
	say(player, "I'm lost")
	npcsay(npc, "How can you be lost?")
	npcsay(npc, "Just follow the road east and south")
	npcsay(npc, "You'll end up in Lumbridge fairly quickly")
end

-- rsc-preservation.xyz/Quests/sheep-shearer-zezima
local function fred_give(player, npc)
	local amount = getvar(player, "fred_wool")
	if amount == 20 then
		say(player, "Thats all of them")
		npcsay(npc, "I guess I'd better pay you then")
		mes(player, "The farmer hands you some coins")
		mes(player, "@gre@Well done you have completed the sheep shearer quest")
		give(player, "coins", 60)
		advancestat(player, SKILL_CRAFTING, 500, 100)
		giveqp(player, 1)
		return
	end
	if not held(player, "ball of wool", 1) then
		say(player, "That's all I've got so far")
		npcsay(npc, "I need more before I can pay you")
		say(player, "Ok I'll work on it")
		return
	end
	remove(player, "ball of wool", 1)
	mes(player, "You give Fred a ball of wool")
	delay(4)
	amount = amount + 1
	setvar(player, "fred_wool", amount)
	fred_give(player, npc)
end

local function fred_progress(player, npc)
	npcsay(npc, "How are you doing getting those balls of wool?")
	if held(player, "ball of wool", 1) then
		say(player, "I have some")
		npcsay(npc, "Give em here then")
		fred_give(player, npc)
	else
		if held(player, "wool", 1) then
			say(player, "Well I've got some wool")
			say(player, "I've not managed to make it into a ball though")
			npcsay(npc, "Well go find a spinning wheel then")
			npcsay(npc, "And get spinning")
		else
			say(player, "I haven't got any at the moment")
			npcsay(npc, "Ah well at least you haven't been eaten")
		end
	end
end

function talknpc_fred_the_farmer(player, npc)
	local stage = getvar(player, "fred_stage")
	if stage == 0 then
		npcsay(npc, "What are you doing on my land?")
		npcsay(npc, "You're not the one who keeps leaving all my gates open?")
		npcsay(npc, "And letting out all my sheep?")
		local resp = multi(player,
			"I'm looking for a quest",
			"I'm looking for something to kill",
			"I'm lost")
		if resp == 1 then
			fred_quest(player, npc)
		elseif resp == 2 then
			fred_kill(player, npc)
		elseif resp == 3 then
			fred_lost(player, npc)
		end
	elseif stage == 1 then
		fred_progress(player, npc)
	elseif stage == 2 then
		npcsay(npc, "What are you doing on my land?")
		local resp = multi(player,
			"I'm looking for something to kill",
			"I'm lost")
		if resp == 1 then
			fred_kill(player, npc)
		elseif resp == 2 then
			fred_lost(player, npc)
		end
	end
end
