-- https://classic.runescape.wiki/w/Transcript:Wizard_Mizgog
-- Revisionism/06-05-2018 17.36.08-imp-catcher

local function mizgog_smalltalk(player, npc)
	say(player, "Most of your friends are pretty quiet aren't they?")
	npcsay(npc, "Yes they've mostly got their heads in the clouds")
	npcsay(npc, "Thinking about magic")
end

local function mizgog_progress(player, npc)
	npcsay(npc, "So how are you doing finding my beads?")
	if held(player, "red bead", 1) and
	    held(player, "yellow bead", 1) and
	    held(player, "black bead", 1) and
	    held(player, "white bead", 1) then
		say(player, "I've got all four beads")
		say(player, "It was hard work I can tell you")
		npcsay(npc, "Give them here and I'll sort out a reward")
		mes(player, "You give four coloured beads to Wizard Mizgog")
		npcsay(npc, "Here's you're reward then")
		npcsay(npc, "An Amulet of accuracy")
		mes(player, "The Wizard hands you an amulet")
		mes(player, "@gre@Well done. You have completed the Imp catcher quest")
		remove(player, "red bead", 1)
		remove(player, "yellow bead", 1)
		remove(player, "black bead", 1)
		remove(player, "white bead", 1)
		give(player, "amulet of accuracy", 1)
		advancestat(player, STAT_MAGIC, 1500, 400)
		giveqp(player, 1)
		setvar(player, "mizgog_stage", 2)
	elseif held(player, "red bead", 1) or
	    held(player, "yellow bead", 1) or
	    held(player, "black bead", 1) or
	    held(player, "white bead", 1) then
		say(player, "I have found some of your beads")
		npcsay(npc, "Come back when you have them all")
		npcsay(npc, "The four colours of beads I need")
		npcsay(npc, "Are red,yellow,black and white")
		npcsay(npc, "Go chase some imps")
	else
		say(player, "I've not found any yet")
		npcsay(npc, "Well get on with it")
		npcsay(npc, "I've lost a white bead, a red bead, a black bead and a yellow bead")
		npcsay(npc, "Go kill some imps")
	end
end

local function mizgog_givequest(player, npc)
	say(player, "Give me a quest!")
	npcsay(npc, "Give me a quest what?")
	local resp = multi(player,
		"Give me a quest please",
		"Give me a quest or else",
		"Just stop messing around and give me a quest")
	if resp == 1 then
		say(player, "Give me a quest please")
		npcsay(npc, "Well seeing as you asked nicely")
		npcsay(npc, "I could do with some help")
		npcsay(npc, "The wizard Grayzag next door decided he didn't like me")
		npcsay(npc, "So he cast of spell of summoning")
		npcsay(npc, "And summoned hundred of little imps")
		npcsay(npc, "These imps stole all sorts of my things")
		npcsay(npc, "Most of these things I don't really care about")
		npcsay(npc, "They're just eggs and balls of string and things")
		npcsay(npc, "But they stole my 4 magical beads")
		npcsay(npc, "There was a red one, a yellow one, a black one and a white one")
		npcsay(npc, "These imps have now spread out all over the kingdom")
		npcsay(npc, "Could you get my beads back for me")
		say(player, "I'll try")
		setvar(player, "mizgog_stage", 1)
	elseif resp == 2 then
		say(player, "Give me a quest or else")
		npcsay(npc, "Or else what? You'll attack me?")
		npcsay(npc, "Hahaha")
	elseif resp == 3 then
		say(player, "Just stop messing around and give me a quest")
		npcsay(npc, "Ah now you're assuming I have one to give")
	end
end

local function mizgog_intro(player, npc)
	npcsay(npc, "Hello there")
	local say = multi(player,
		"Give me a quest!",
		"Most of your friends are pretty quiet aren't they?")
	if say == 1 then
		mizgog_givequest(player, npc)
	elseif say == 2 then
		mizgog_smalltalk(player, npc)
	end
end

local function mizgog_after(player, npc)
	local say = multi(player,
		"Got any more quests?",
		"Most of your friends are pretty quiet aren't they?")
	if say == 1 then
		say(player, "Got any more quests?")
		npcsay(npc, "No Everything is good with the world today")
	elseif say == 2 then
		mizgog_smalltalk(player, npc)
	end
end

function talknpc_wizard_mizgog(player, npc)
	local stage = getvar(player, "mizgog_stage")
	if stage == 0 then
		mizgog_intro(player, npc)
	elseif stage == 1 then
		mizgog_progress(player, npc)
	else
		mizgog_after(player, npc)
	end
end
