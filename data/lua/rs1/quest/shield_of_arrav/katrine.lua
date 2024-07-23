-- https://classic.runescape.wiki/w/Transcript:Katrine
-- RSC 2001/3/f2p (not 100% complete)/dialogue- katrine- phoenix gang member.pcap

local function katrine_riches(player, npc)
	say(player, "I'm looking for fame and riches")
	npcsay(npc, "And you expect to find it up the backstreets of Varrock?")
end

local function katrine_business(player, npc)
	say(player, "What sort of business")
	npcsay(npc, "A small family business")
	npcsay(npc, "We give financial advice to other companies")
end

local function katrine_mission(player, npc)
	npcsay(npc, "I think I may have a solution actually")
	npcsay(npc, "Our rival gang - the phoenix gang")
	npcsay(npc, "Has a weapons stash a little east of here")
	npcsay(npc, "We're fresh out of crossbows")
	npcsay(npc, "So if you could steal a couple of crossbows for us")
	npcsay(npc, "That would be very much appreciated")
	npcsay(npc, "Then I'll be happy to call you a black arm")
	local resp = multi(player,
		"Ok no problem",
		"Sounds a little tricky got anything easier?")
	if resp == 1 then
		say(player, "Ok no problem")
		setvar(player, "blackarm_stage", 1)
	elseif resp == 2 then
		say(player, "Sounds a little tricky got anything easier?")
		npcsay(npc, "If you're not up for a little bit of danger")
		npcsay(npc, "I don't think you've got anything to offer our gang")
	end
end

local function katrine_join(player, npc)
	say(player, "I want to become a member of your gang")
	npcsay(npc, "How unusual")
	npcsay(npc, "Normally we recruit for our gang")
	npcsay(npc, "By watching local thugs and thieves in action")
	npcsay(npc, "People don't normally waltz in here")
	npcsay(npc, "Saying 'hello can I play'")
	npcsay(npc, "How can I be sure you can be trusted?")
	local resp = multi(player,
		"Well you can give me a try, can't you?",
		"Well people tell me I have an honest face")
	if resp == 1 then
		say(player, "Well you can give me a try, can't you?")
		npcsay(npc, "I'm not so sure")
		katrine_mission(player, npc)
	elseif resp == 2 then
		say(player, "Well people tell me I have an honest face")
		npcsay(npc, "How unusual someone honest wanting to join a gang of thieves")
		npcsay(npc, "Excuse me if i remain unconvinced")
		katrine_mission(player, npc)
	end
end

local function katrine_want(player, npc)
	local resp = multi(player,
		"I want to become a member of your gang",
		"I want some hints for becoming a thief",
		"I'm looking for the door out of here")
	if resp == 1 then
		katrine_join(player, npc)
	elseif resp == 2 then
		say(player, "I want some hints for becomming a thief")
		npcsay(npc, "Well I'm sorry luv")
		npcsay(npc, "I'm not giving away my secrets")
		npcsay(npc, "Not to none black arm members anyway")
	elseif resp == 3 then
		say(player, "I'm looking for the door out of here")
		mes(player, "Katrine groans")
		delay(3)
		npcsay(npc, "Try the one you just came in")
	end
end

local function katrine_known(player, npc)
	say(player, "I've heard you're the blackarm gang")
	npcsay(npc, "Who told you that?")
	local resp = multi(player,
		"I'd rather not reveal my sources",
		"It was the tramp outside",
		"Everyone knows - its no great secret")
	if resp == 1 then
		say(player, "I'd rather not reveal my sources")
		npcsay(npc, "Yes, I can understand that")
		npcsay(npc, "So what do you want with us?")
		katrine_want(player, npc)
	elseif resp == 2 then
		say(player, "It was the tramp outside")
		npcsay(npc, "Is that guy still out there?")
		npcsay(npc, "He's getting to be a nuisance")
		npcsay(npc, "Remind me to send someone to kill him")
		npcsay(npc, "So now you've found us")
		npcsay(npc, "What do you want?")
		katrine_want(player, npc)
	elseif resp == 3 then
		say(player, "Everyone knows")
		say(player, "It's no great secret")
		npcsay(npc, "I thought we were safe back here")
		say(player, "Oh no, not at all")
		say(player, "It's so obvious")
		say(player, "Even the town guard have caught on")
		npcsay(npc, "Wow we must be obvious")
		npcsay(npc, "I guess they'll be expecting bribes again soon in that case")
		npcsay(npc, "Thanks for the information")
		npcsay(npc, "Is there anything else you want to tell me?")
		katrine_want(player, npc)
	end
end

function talknpc_katrine(player, npc)
	local stage = getvar(player, "phoenix_stage")
	if stage >= 2 then
		npcsay(npc, "You've got some guts coming here")
		npcsay(npc, "Phoenix guy")
		mes(player, "Katrine Spits")
		delay(3)
		npcsay(npc, "Now go away")
		npcsay(npc, "Or I'll make sure you 'aven't got those guts anymore")
		return
	end
	stage = getvar(player, "blackarm_stage")
	if stage == 0 then
		say(player, "What is this place?")
		npcsay(npc, "It's a private business")
		npcsay(npc, "Can I help you at all?")
		stage = getvar(player, "blackarm_known")
		if known == 0 then
			local resp = multi(player,
				"What sort of business",
				"I'm looking for fame and riches")
			if resp == 2 then
				katrine_business(player, npc)
			elseif resp == 3 then
				katrine_riches(player, npc)
			end
		else
			local resp = multi(player,
				"I've heard you're the blackarm gang",
				"What sort of business",
				"I'm looking for fame and riches")
			if resp == 1 then
				katrine_known(player, npc)
			elseif resp == 2 then
				katrine_business(player, npc)
			elseif resp == 3 then
				katrine_riches(player, npc)
			end
		end
	elseif stage == 1 then
		npcsay(npc, "Have you got those crossbows for me yet?")
		if held(player, "phoenix crossbow", 2) then
			say(player, "Yes I have")
			npcsay(npc, "Ok you can join our gang now")
			npcsay(npc, "Feel free to enter any the rooms of the ganghouse")
			setvar(player, "blackarm_stage", 2)
		elseif held(player, "phoenix crossbow", 1) then
			say(player, "I have one")
			npcsay(npc, "I need two")
			npcsay(npc, "Come back when you have them")
		else
			say(player, "No I haven't found them yet")
			npcsay(npc, "I need two crossbows")
			npcsay(npc, "Stolen from the phoenix gang weapons stash")
			npcsay(npc, "which if you head east for a bit")
			npcsay(npc, "Is a building on the south side of of the road")
		end
	else
		say(player, "Hey")
		npcsay(npc, "Hey")
		local resp = multi(player,
			"Who are all those people in there?",
			"Teach me to be a top class criminal")
		if resp == 1 then
			say(player, "Who are all those people in there?")
			npcsay(npc, "They're just various rogues and thieves")
			say(player, "They don't say a lot")
			npcsay(npc, "Nope")
		elseif resp == 2 then
			say(player, "Teach me to be a top class criminal")
			npcsay(npc, "Teach yourself")
		end
	end
end

function opbound1_blackarmdoor(player, x, y, dir)
	if getvar(player, "blackarm_stage") < 2 then
		-- RSC 2001/3/f2p (not 100% complete)/scenery- varrock- black arms hideout- door- open- phoenix gang member- door wont open.pcap
		delay(3)
		mes(player, "The door won't open")
	else
		-- 1e_Luis/Quests/Shield of Arrav/Black Arm Gang/Part 2.pcap
		mes(player, "You hear the door being unbarred")
		boundaryteleport(player, x, y, dir)
		mes(player, "You go through the door")
		changebound(x, y, dir, "openlockeddoor")
		pause(5, 5)
		changebound(x, y, dir, "blackarmdoor")
	end
end
