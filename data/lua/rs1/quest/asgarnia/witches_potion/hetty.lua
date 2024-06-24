-- https://classic.runescape.wiki/w/Transcript:Hetty
-- 1e_Luis/Quests/Witchs Potion

local function hetty_givequest(player, npc)
	npcsay(npc, "Ok I'm going to make a potion to help bring out your darker self")
	npcsay(npc, "So that you can perform acts of  dark magic with greater ease")
	npcsay(npc, "You will need certain ingredients")
	say(player, "What do I need")
	npcsay(npc, "You need an eye of newt, a rat's tail, an onion and a piece of burnt meat")
	setvar(player, "hetty_stage", 1)
end

local function hetty_drama(player, npc)
	say(player, "What you mean improve my magic?")
	npcsay(npc, "Yes improve your magic")
	npcsay(npc, "Do you have no sense of drama?")
	local resp = multi(player,
		"Yes I'd like to improve my magic",
		"No I'm not interested",
		"Show me the mysteries of the dark arts")
	if resp == 1 then
		say(player, "Yes I'd like to improve my magic")
		mes(player, "The witch sighs")
		hetty_givequest(player, npc)
	elseif resp == 2 then
		say(player, "No I'm not interested")
		npcsay(npc, "Many aren't to start off with")
		npcsay(npc, "But I think you'll be drawn back to this place")
	elseif resp == 3 then
		say(player, "Show me the mysteries of the dark arts")
		hetty_givequest(player, npc)
	end
end

local function hetty_askquest(player, npc)
	say(player, "I'm in search of a quest")
	npcsay(npc, "Hmm maybe I can think of something for you")
	npcsay(npc, "Would you like to become more proficient in the dark arts?")
	local resp = multi(player,
		"Yes help me become one with my darker side",
		"No I have my principles and honour",
		"What you mean improve my magic?")
	if resp == 1 then
		say(player, "Yes help me become one with my darker side")
		hetty_givequest(player, npc)
	elseif resp == 2 then
		say(player, "No, I have my principles and honour")
		npcsay(npc, "Suit yourself, but you're missing out")
	elseif resp == 3 then
		hetty_drama(player, npc)
	end
end

local function hetty_intro(player, npc)
	npcsay(npc, "What could you want with an old woman like me?")
	local resp = multi(player,
		"I am in search of a quest",
		"I've heard that you're a witch")
	if resp == 1 then
		hetty_askquest(player, npc)
	elseif resp == 2 then
		say(player, "I've heard that you are a witch")
		npcsay(npc, "Yes it does seem to be getting fairly common knowledge")
		npcsay(npc, "I fear I may get a visit from the witch hunters of Falador before long")
	end
end

local function hetty_progress(player, npc)
	npcsay(npc, "So have you found the things for the potion")
	if held(player, "onion", 1) and held(player, "rat's tail", 1) and
	    held(player, "burntmeat", 1) and held(player, "eye of newt", 1) then
		say(player, "Yes I have everything")
		npcsay(npc, "Excellent, can I have them then?")
		mes(player, "You pass the ingredients to Hetty")
		remove(player, "eye of newt", 1)
		remove(player, "rat's tail", 1)
		remove(player, "burntmeat", 1)
		remove(player, "onion", 1)
		mes(player, "Hetty put's all the ingredients in her cauldron")
		delay(3)
		mes(player, "Hetty closes her eyes and begins to chant")
		delay(3)
		npcsay(npc, "Ok drink from the cauldron")
		setvar(player, "hetty_stage", 2)
	else
		say(player, "No not yet")
		npcsay(npc, "Well remember you need to get")
		npcsay(npc, "An eye of newt, a rat's tail,some burnt meat and an onion")
	end
end

function talknpc_hetty(player, npc)
	npcsay(npc, "Greetings Traveller")

	local stage = getvar(player, "hetty_stage")
	if stage == 0 then
		hetty_intro(player, npc)
	elseif stage == 1 then
		hetty_progress(player, npc)
	elseif stage == 2 then
		npcsay(npc, "Well are you going to drink the potion or not?")
	else
		npcsay(npc, "How's your magic coming along?")
		say(player, "I'm practicing and slowly getting better")
		npcsay(npc, "good good")
	end
end
