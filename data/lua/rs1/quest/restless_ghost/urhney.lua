-- https://classic.runescape.wiki/w/Transcript:Urhney
-- rsc-preservation.xyz/Quests/The restless ghost - Part 1

local function urhney_friendly(player, npc)
	say(player, "Well that's friendly")
	npcsay(npc, "I said go away!")
	say(player, "Ok, ok")
end

local function urhney_house(player, npc)
	say(player, "I've come to repossess your house")
	npcsay(npc, "Under what grounds?")
	local resp = multi(player,
		"Repeated failure on mortgage payments",
		"I don't know, I just wanted this house")
	if resp == 1 then
		say(player, "Repeated failure on mortgage payments")
		npcsay(npc, "I don't have a mortgage")
		npcsay(npc, "I built this house myself")
		say(player, "Sorry I must have got the wrong address")
		say(player, "All the houses look the same around here")
	elseif resp == 2 then
		say(player, "I don't know, I just wanted this house")
		npcsay(npc, "Oh go away and stop wasting my time")
	end
end

local function urhney_haunting(player, npc)
	say(player, "He's got a ghost haunting his graveyard")
	npcsay(npc, "Oh the silly fool")
	npcsay(npc, "I leave town for just five months")
	npcsay(npc, "and already he can't manage")
	npcsay(npc, "Sigh")
	npcsay(npc, "Well I can't go back and exorcise it")
	npcsay(npc, "I vowed not to leave this place")
	npcsay(npc, "Until I had done a full two years of prayer and meditation")
	npcsay(npc, "Tell you what I can do though")
	npcsay(npc, "Take this amulet")
	give(player, "amulet of ghostspeak", 1)
	mes(player, "Father Urhney hands you an amulet")
	delay(3)
	npcsay(npc, "It is an amulet of Ghostspeak")
	npcsay(npc, "So called because when you wear it you can speak to ghosts")
	npcsay(npc, "A lot of ghosts are doomed to be ghosts")
	npcsay(npc, "Because they have left some task uncompleted")
	npcsay(npc, "Maybe if you know what this task is")
	npcsay(npc, "You can get rid of the ghost")
	npcsay(npc, "I'm not making any guarantees mind you")
	npcsay(npc, "But it is the best I can do right now")
	say(player, "Thank you, I'll give it a try")
	setvar(player, "ghost_stage", 2)
end

local function urhney_aereck(player, npc)
	say(player, "Father Aereck sent me to talk to you")
	npcsay(npc, "I suppose I'd better talk to you then")
	npcsay(npc, "What problems has he got himself into this time?")
	local resp = multi(player,
		"He's got a ghost haunting his graveyard",
		"You mean he gets himself into lots of problems?")
	if resp == 1 then
		urhney_haunting(player, npc)
	elseif resp == 2 then
		say(player, "You mean he gets himself into lots of problems?")
		npcsay(npc, "Yeah. For example when we were trainee priests")
		npcsay(npc, "He kept on getting stuck up bell ropes")
		npcsay(npc, "Anyway I don't have time for chitchat")
		npcsay(npc, "What's his problem this time?")
		urhney_haunting(player, npc)
	end
end

function talknpc_urhney(player, npc)
	npcsay(npc, "Go away, I'm meditating")

	local choices = {}
	local stage = getvar(player, "ghost_stage")
	if stage == 1 then
		table.insert(choices, "Father Aereck sent me to talk to you")
	end
	if stage > 1 and not held(player, "amulet of ghostspeak", 1) then
		table.insert(choices, "I've lost the amulet")
	end

	table.insert(choices, "Well that's friendly")
	table.insert(choices, "I've come to repossess your house")

	local resp = multi(player, table.unpack(choices))
	resp = choices[resp]
	if resp == "Father Aereck sent me to talk to you" then
		urhney_aereck(player, npc)
	elseif resp == "I've lost the amulet" then
		say(player, "I've lost the amulet")
		npcsay(npc, "How careless can you get")
		npcsay(npc, "Those things aren't easy to come by you know")
		npcsay(npc, "It's a good job I've got a spare")
		give(player, "amulet of ghostspeak", 1)
		mes(player, "Father Urhney hands you an amulet")
		delay(3)
		npcsay(npc, "Be more careful this time")
		say(player, "Ok I'll try to be")
	elseif resp == "Well that's friendly" then
		urhney_friendly(player, npc)
	elseif resp == "I've come to repossess your house" then
		urhney_house(player, npc)
	end
end
