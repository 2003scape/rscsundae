-- https://classic.runescape.wiki/w/Transcript:Straven
-- Revisionism/06-16-2018 22.27.41 shield of arrav (phoenix gang side) pt1.pcap

local function straven_jobs(player, npc)
	say(player, "How do I get a job with the VTAM corporation?")
	npcsay(npc, "Get a copy of the Varrock Herald")
	npcsay(npc, "If we have any positions right now")
	npcsay(npc, "They'll be advertised in there")
end

local function straven_whynot(player, npc)
	say(player, "Why not?")
	npcsay(npc, "Sorry that is classified information")
end

local function straven_know(player, npc)
	say(player, "I know who you are")
	npcsay(npc, "I see")
	npcsay(npc, "Carry on")
	say(player, "This is the headquarters of the Phoenix Gang")
	say(player, "The most powerful crime gang this city has seen")
	npcsay(npc, "And supposing we were this crime gang")
	npcsay(npc, "What would you want with us?")
	local resp = multi(player,
		"I'd like to offer you my services",
		"I want nothing. I was just making sure you were them")
	if resp == 1 then
		say(player, "I'd like to offer you my services")
		npcsay(npc, "You mean you'd like to join the phoenix gang?")
		npcsay(npc, "Well the phoenix gang doesn't let people join just like that")
		npcsay(npc, "You can't be too careful, you understand")
		npcsay(npc, "Generally someone has to prove their loyalty before they can join")
		say(player, "How would I go about this?")
		npcsay(npc, "Let me think")
		npcsay(npc, "I have an idea")
		npcsay(npc, "A rival gang of ours")
		npcsay(npc, "Called the black arm gang")
		npcsay(npc, "Is meant to be meeting their contact from Port Sarim today")
		npcsay(npc, "In the blue moon inn")
		npcsay(npc, "the south entrance to this city")
		npcsay(npc, "The name of this contact is Jonny the beard")
		npcsay(npc, "Kill him and bring back his intelligence report")
		say(player, "Ok, I'll get on it")
		setvar(player, "phoenix_stage", 1)
	elseif resp == 2 then
		say(player, "I want nothing. I was just making sure you were them")
		npcsay(npc, "Well stop wasting my time")
	end
end

local function straven_reject(player, npc)
	npcsay(npc, "Heh you can't go in there")
	npcsay(npc, "Only authorised personnel of the VTAM corporation are allowed beyond this point")
	if getvar(player, "phoenix_known") ~= 0 then
		local resp = multi(player,
			"I know who you are",
			"How do I get a job with the VTAM corporation?",
			"Why not?")
		if resp == 1 then
			straven_know(player, npc)
		elseif resp == 2 then
			straven_jobs(player, npc)
		elseif resp == 3 then
			straven_whynot(player, npc)
		end
	else
		local resp = multi(player,
			"How do I get a job with the VTAM corporation?",
			"Why not?")
		if resp == 1 then
			straven_jobs(player, npc)
		elseif resp == 2 then
			straven_whynot(player, npc)
		end
	end
end

function talknpc_straven(player, npc)
	local stage = getvar(player, "phoenix_stage")
	if stage < 1 then
		say(player, "What's through that door?")
		straven_reject(player, npc)
	elseif stage < 2 then
		npcsay(npc, "Hows your little mission going?")
		if not held(player, "report", 1) then
			say(player, "I haven't managed to find the report yet")
			npcsay(npc, "You need to kill Jonny the beard")
			npcsay(npc, "Who should be in the blue moon inn")
		else
			say(player, "I have the intelligence report")
			npcsay(npc, "Lets see it then")
			mes(player, "You hand over the report")
			delay(5)
			mes(player, "The man reads the report")
			remove(player, "report", 1)
			delay(3)
			npcsay(npc, "Yes this is very good")
			npcsay(npc, "Ok you can join the phoenix gang")
			npcsay(npc, "I am Straven, one of the gang leaders")
			say(player, "Nice to meet you")
			npcsay(npc, "Here is a key")
			mes(player, "Straven hands you a key")
			give(player, "phoenixkey2", 1)
			delay(3)
			npcsay(npc, "It will let you enter our weapon supply area")
			npcsay(npc, "Round the front of this building")
			setvar(player, "phoenix_stage", 2)
		end
	else
		npcsay(npc, "Greetings fellow gang member")
		if not held(player, "phoenixkey2", 1) then
			say(player, "I have lost the key you gave me")
			npcsay(npc, "You need to be more careful")
			npcsay(npc, "We don't want that falling into the wrong hands")
			npcsay(npc, "Ah well")
			npcsay(npc, "Have this spare")
			return
		end
		local resp = multi(player,
			"I've heard you've got some cool treasure in this place",
			"Any suggestions for where I can go thieving?",
			"Where's the Blackarm gang hideout?")
		if resp == 1 then
			say(player, "I've heard you've got some cool treasures in this place")
			npcsay(npc, "Oh yeah, we've all stolen some stuff in our time")
			npcsay(npc, "The candlesticks down here")
			npcsay(npc, "Were quite a challenge to get out the palace")
			say(player, "And the shield of Arrav")
			say(player, "I heard you got that")
			npcsay(npc, "hmm")
			npcsay(npc, "That was a while ago")
			npcsay(npc, "We don't even have all the shield anymore")
			npcsay(npc, "About 5 years ago")
			npcsay(npc, "We had a massive fight in our gang")
			npcsay(npc, "The shield got broken in half during the fight")
			npcsay(npc, "Shortly after the fight")
			npcsay(npc, "Some gang members decided")
			npcsay(npc, "They didn't want to be part of our gang anymore")
			npcsay(npc, "So they split off to form their own gang")
			npcsay(npc, "The black arm gang")
			npcsay(npc, "On their way out")
			npcsay(npc, "They looted what treasures they could from us")
			npcsay(npc, "Which included one of the halves of the shield")
			npcsay(npc, "We've been rivals with the black arms ever since")
		elseif resp == 2 then
			say(player, "Any suggestions for where I can go thieving?")
			npcsay(npc, "You can always try the market")
			npcsay(npc, "Lots of opportunity there")
		elseif resp == 3 then
			say(player, "Where's the Blackarm gang hideout?")
			say(player, "I wanna go sabotage em")
			npcsay(npc, "That would be a little tricky")
			npcsay(npc, "Their security is pretty good")
			npcsay(npc, "Not as good as ours obviously")
			npcsay(npc, "But still good")
			npcsay(npc, "If you really want to go there")
			npcsay(npc, "It is in the alleyway")
			npcsay(npc, "To the west as you come in the south gate")
			npcsay(npc, "One of our operatives is often near the alley")
			npcsay(npc, "A red haired tramp")
			npcsay(npc, "He may be able to give you some ideas")
			say(player, "Thanks for the help")
		end
	end
end

function opbound1_phoenixdoor(player, x, y, dir)
	local stage = getvar(player, "phoenix_stage")
	if stage < 1 then
		local npc = nearvisnpc(player, "straven")
		if npc then
			straven_reject(player, npc)
		else
			mes(player, "The door is locked")
		end
	elseif stage < 2 then
		mes(player, "The door is locked")
	else
		mes(player, "The door is opened for you")
		boundaryteleport(player, x, y, dir)
		mes(player, "You go through the door")
		changebound(x, y, dir, "openlockeddoor")
		pause(5, 5)
		changebound(x, y, dir, "phoenixdoor")
	end
end
