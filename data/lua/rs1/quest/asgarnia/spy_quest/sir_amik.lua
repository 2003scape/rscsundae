-- https://classic.runescape.wiki/w/Transcript:Sir_Amik_Varze

local function sir_amik_plan(player, npc)
	npcsay(npc, "You've come along just right actually")
	npcsay(npc, "All of my knights are known to the black knights already")
	npcsay(npc, "Subtlety isn't exactly our strong point")
	say(player, "So what needs doing?")
	npcsay(npc, "Well the black knights have started making strange threats to us")
	npcsay(npc, "Demanding large amounts of money and land")
	npcsay(npc, "And threataning to invade Falador if we don't pay")
	npcsay(npc, "Now normally this wouldn't be a problem")
	npcsay(npc, "But they claim to have a powerful new secret weapon")
	npcsay(npc, "What I want you to do is to get inside their fortress")
	npcsay(npc, "Find out what their secret weapon is")
	npcsay(npc, "And then sabotage it")
	npcsay(npc, "You will be well paid")
	say(player, "OK I'll give it a try")
	setvar(player, "fortress_stage", 1)
end

local function sir_amik_afraid(player, npc)
	say(player, "I go and cower in a corner at the first sign of danger")
	npcsay(npc, "Err")
	npcsay(npc, "Well")
	npcsay(npc, "spy work does involve a little hiding in corners I suppose")
	local resp = multi(player,
		"Oh I suppose I'll give it a go then",
		"No I'm not convinced")
	if resp == 1 then
		say(player, "Oh I suppose I'll give it a go then")
		sir_amik_plan(player, npc)
	elseif resp == 2 then
		say(player, "No I'm not convinced")
	end
end

local function sir_amik_givequest(player, npc)
	npcsay(npc, "Well I need some spy work doing")
	npcsay(npc, "It's quite dangerous")
	npcsay(npc, "You will need to go into the Black Knight's fortress")
	local resp = multi(player,
		"I laugh in the face of danger",
		"I go and cower in a corner at the first sign of danger")
	if resp == 1 then
		say(player, "I laugh in the face of danger")
		npcsay(npc, "Well that's good")
		npcsay(npc, "Don't get too overconfident though")
		sir_amik_plan(player, npc)
	elseif resp == 2 then
		sir_amik_afraid(player, npc)
	end
end

local function sir_amik_init(player, npc)
	npcsay(npc, "I am the leader of the white knights of Falador")
	npcsay(npc, "Why do you seek my audience?")
	local resp = multi(player,
		"I seek a quest",
		"I don't I'm just looking around")
	if resp == 1 then
		say(player, "I seek a quest")
		if qp(player) < 12 then
			npcsay(npc, "Well i do have a task, but it is very dangerous")
			npcsay(npc, "and it's critical to us that no mistakes are made")
			npcsay(npc, "I couldn't possibly let an unexperienced quester like yourself go")
			mes(player, "You need at least 12 quest points before you may attempt this quest")
		else
			sir_amik_givequest(player, npc)
		end
	elseif resp == 2 then
		say(player, "I don't I'm just looking around")
		npcsay(npc, "Ok, don't break anything")
	end
end

function talknpc_sir_amik_varze(player, npc)
	local stage = getvar(player, "fortress_stage")
	if stage == 0 then
		sir_amik_init(player, npc)
	elseif stage == 1 then
		npcsay(npc, "How's the mission going?")
		say(player, "I haven't managed to find what the secret weapon is yet.")
	elseif stage == 2 then
		npcsay(npc, "How's the mission going?")
		say(player, "I've found out what the black knight's secret weapon is.")
		say(player, "It's a potion of invincibility.")
		npcsay(npc, "That is bad news.")
	elseif stage == 3 then
		say(player, "I have ruined the black knight's invincibilty potion.")
		say(player, "That should put a stop to your problem.")
		npcsay(npc, "Yes we have just received a message from the black knights.")
		npcsay(npc, "Saying they withdraw their demands.")
		npcsay(npc, "Which confirms your story")
		say(player, "You said you were going to pay me")
		npcsay(npc, "Yes that's right")
		mes(player, "Sir Amik hands you 2500 coins")
		delay(3)
		mes(player, "Well done.You have completed the Black Knights fortress quest")
		delay(3)
		give(player, "coins", 2500)
		setvar(player, "fortress_stage", 4)
		giveqp(player, 3)
	elseif stage == 4 then
		say(player, "Hello Sir Amik")
		npcsay(npc, "Hello friend")
	end
end
