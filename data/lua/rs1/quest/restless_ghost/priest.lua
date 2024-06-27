-- https://classic.runescape.wiki/w/Transcript:Priest

local function priest_world(player, npc)
	say(player, "Oh sorry I'm not from this world")
	npcsay(npc, "That's strange")
	npcsay(npc, "I thought things not from this world were all slime and tenticles")
	local resp = multi(player,
		"You don't understand. This is a computer game",
		"I am - do you like my disguise?")
	if resp == 1 then
		say(player, "You don't understand. This is a computer game")
		npcsay(npc, "I beg your pardon?")
		say(player, "Never mind")
	elseif resp == 2 then
		say(player, "I am - do you like my disguise?")
		npcsay(npc, "Aargh begone foul creature from another dimension")
		say(player, "Ok, Ok, It was a joke")
	end
end

local function priest_saradomin(player, npc)
	say(player, "Who's Saradomin?")
	npcsay(npc, "Surely you have heard of the God, Saradomin?")
	npcsay(npc, "He who creates the forces of goodness and purity in this world?")
	npcsay(npc, "I cannot believe your ignorance!")
	npcsay(npc, "This is the God with more followers than any other!")
	npcsay(npc, "At least in these parts!")
	npcsay(npc, "He who along with his brothers Guthix and Zamorak created this world")
	local resp = multi(player,
		"Oh that Saradomin",
		"Oh sorry I'm not from this world")
	if resp == 1 then
		say(player, "Oh that Saradomin")
		npcsay(npc, "There is only one Saradomin")
	elseif resp == 2 then
		priest_world(player, npc)
	end
end

local function priest_progress(player, npc)
	local stage = getvar(player, "ghost_stage")
	npcsay(npc, "Have you got rid of the ghost yet?")
	if stage == 1 then
		say(player, "I can't find father Urhney at the moment"
		npcsay(npc, "Well to get to the swamp he is in")
		npcsay(npc, "you need to go round the back of the castle")
		npcsay(npc, "The swamp is on the otherside of the fence to the south")
		npcsay(npc, "You'll have to go through the wood to the west to get round the fence")
		npcsay(npc, "Then you'll have to go right into the eastern depths of the swamp")
	elseif stage == 2 then
		say(player, "I had a talk with father Urhney")
		say(player, "He has given me this funny amulet to talk to the ghost with")
		npcsay(npc, "I always wondered what that amulet was")
		npcsay(npc, "Well I hope it's useful. Tell me if you get rid of the ghost")
	elseif stage == 3 then
		say(player, "I've found out that the ghost's corpse has lost its skull")
		say(player, "If I can find the skull the ghost will go")
		npcsay(npc, "That would explain it")
		npcsay(npc, "Well I haven't seen any skulls")
		say(player, "Yes I think a warlock has stolen it")
		npcsay(npc, "I hate warlocks")
		npcsay(npc, "Ah well good luck")
	elseif stage == 4 then
		say(player, "I've finally found the ghost's skull")
		npcsay(npc, "Great. Put it in the ghost's coffin and see what happens!")
	end
end

function talknpc_priest(player, npc)
	local stage = getvar(player, "ghost_stage")
	if stage >= 1 and stage <= 4 then
		priest_progress(player, npc)
		return
	end
	npcsay(npc, "Welcome to the church of holy Saradomin")
	local resp = multi(player,
		"Who's Saradomin?",
		"Nice place you've got here",
		"I'm looking for a quest")
	if resp == 1 then
		priest_saradomin(player, npc)
	elseif resp == 2 then
		say(player, "Nice place you've got here")
		npcsay(npc, "It is, isn't it?")
		npcsay(npc, "It was built 230 years ago")
	elseif resp == 3 then
		say(player, "I'm looking for a quest")
		if stage == 0 then
			npcsay(npc, "That's lucky, I need someone to do a quest for me")
			say(player, "Ok I'll help")
			npcsay(npc, "Ok the problem is, there is a ghost in the church graveyard")
			npcsay(npc, "I would like you to get rid of it")
			npcsay(npc, "If you need any help")
			npcsay(npc, "My friend father Urhney is an expert on ghosts")
			npcsay(npc, "I believe he is currently living as a hermit")
			npcsay(npc, "He has a little shack somewhere in the swamps south of here")
			npcsay(npc, "I'm sure if you told him that I sent you he'd be willing to help")
			npcsay(npc, "My name is father Aereck by the way")
			npcsay(npc, "Be careful going through the swamps")
			npcsay(npc, "I have heard they can be quite dangerous")
			setvar(player, "ghost_stage", 1)
		else
			npcsay(npc, "Sorry I only had the one quest")
		end
	end
end
