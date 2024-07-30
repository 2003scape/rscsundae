-- https://classic.runescape.wiki/w/Transcript:Gypsy
-- rsc-preservation.xyz/Quests/demon-slayer.pcap

local function gypsy_thanks(player, npc)
	say(player, "Ok thanks I'll do my best to stop the Demon")
	npcsay(npc, "Good luck, may Guthix be with you")
end

local function gypsy_incantation(player, npc)
	say(player, "What is the magical incantion?")
	npcsay(npc, "Oh yes let me think a second")
	mes(player, "The gypsy is thinking")
	delay(18)
	npcsay(npc, "Alright I've got it now I think")
	npcsay(npc, "It goes")
	npcsay(npc, "Carlem")
	npcsay(npc, "Aber")
	npcsay(npc, "Camerinthum")
	npcsay(npc, "Purchai")
	npcsay(npc, "Gabindo")
	npcsay(npc, "Have you got that?")
	say(player, "I think so, yes")
	local resp = multi(player,
		"Ok thanks I'll do my best to stop the Demon",
		"Where can I find silverlight?")
	if resp == 1 then
		gypsy_thanks(player, npc)
	elseif resp == 2 then
		gypsy_silverlight(player, npc)
	end
end

function gypsy_silverlight(player, npc)
	say(player, "Where can I find Silverlight?")
	npcsay(npc, "Silverlight has been passed down through Wally's descendents")
	npcsay(npc, "I believe it is currently in the care of one of the king's knights")
	npcsay(npc, "called Sir Prysin")
	npcsay(npc, "He shouldn't be to hard to find the he lives in the royal palace in this city")
	npcsay(npc, "Tell him Gypsy Aris sent you")
	setvar(player, "delrith_stage", 1)
	local resp = multi(player,
		"Ok thanks I'll do my best to stop the Demon",
		"What is the magical incantation?")
	if resp == 1 then
		gypsy_thanks(player, npc)
	elseif resp == 2 then
		gypsy_incantation(player, npc)
	end
end

local function gypsy_how_delrith(player, npc)
	npcsay(npc, "Wally managed to arrive at the stone circle")
	npcsay(npc, "Just as Delrith was summoned by a cult of chaos druids")
	npcsay(npc, "By reciting the correct magical incantation")
	npcsay(npc, "and thrusting Silverlight into Delright, while he was newly summoned")
	npcsay(npc, "Wally was able to imprison Delrith")
	npcsay(npc, "in the stone block in the centre of the circle")
	npcsay(npc, "Delrith will come forth from the stone circle again")
	npcsay(npc, "I would imagine an evil sorcerer is already starting on the rituals")
	npcsay(npc, "To summon Delrith as we speak")
	local resp = multi(player,
		"What is the magical incantion?",
		"Where can I find silverlight?")
	if resp == 1 then
		gypsy_incantation(player, npc)
	elseif resp == 2 then
		gypsy_silverlight(player, npc)
	end
end

local function gypsy_where_delrith(player, npc)
	say(player, "Ok where is he? I'll kill him for you")
	npcsay(npc, "Well you can't just go and fight")
	npcsay(npc, "He can't be harmed by ordinary weapons")
	gypsy_how_delrith(player, npc)
end

local function gypsy_cities(player, npc)
	say(player, "How am I meant to fight a demon who can destroy cities?")
	npcsay(npc, "I admit it won't be easy")
	gypsy_how_delrith(player, npc)
end

local function gypsy_wally(player, npc)
	say(player, "Wally doesn't sound like a very heroic name")
	npcsay(npc, "Yes I know. Maybe that is why history doesn't remember him")
	npcsay(npc, "However he was a very great hero.")
	npcsay(npc, "Who knows how much pain and suffering")
	npcsay(npc, "Delrith would have brought forth without Wally to stop him")
	npcsay(npc, "It looks like you are going to need to perform similar heroics")
	local resp = multi(player,
		"How am I meant to fight a demon who can destroy cities?",
		"Ok where is he? I'll kill him for you")
	if resp == 1 then
		gypsy_cities(player, npc)
	elseif resp == 2 then
		gypsy_where_delrith(player, npc)
	end
end

local function gypsy_explain_delrith(player, npc)
	say(player, "Who's Delrith?")
	npcsay(npc, "Delrith")
	npcsay(npc, "Delrtih is a powerfull demon")
	npcsay(npc, "Oh I really hope he didn't see me")
	npcsay(npc, "Looking at him through my crystal ball")
	npcsay(npc, "He tried to destroy this city 150 years ago")
	npcsay(npc, "He was stopped just in time, by the great hero Wally")
	npcsay(npc, "Wally managed to trap the demon")
	npcsay(npc, "In the stone circle just south of this city")
	npcsay(npc, "Using his magic sword silverlight")
	npcsay(npc, "Ye Gods")
	npcsay(npc, "Silverlight was the sword you were holding the ball vision")
	npcsay(npc, "You are the one destined to try and stop the demon this time")
	local resp = multi(player,
		"How am I meant to fight a demon who can destroy cities?",
		"Ok where is he? I'll kill him for you",
		"Wally doesn't sound like a very heroic name")
	if resp == 1 then
		gypsy_cities(player, npc)
	elseif resp == 2 then
		gypsy_where_delrith(player, npc)
	elseif resp == 3 then
		gypsy_wally(player, npc)
	end
end

local function gypsy_delrith(player, npc)
	local resp = multi(player,
		"Who's Delrith",
		"Get a grip!")
	if resp == 1 then
		gypsy_explain_delrith(player, npc)
	elseif resp == 2 then
		say(player, "Get a grip!")
		npcsay(npc, "Sorry. I didn't expect to see Delrith")
		npcsay(npc, "I had to break away quickly in case he detected me")
		gypsy_explain_delrith(player, npc)
	end
end

local function gypsy_fortune(player, npc)
	if not held(player, "coins", 1) then
		say(player, "Oh dear. I don't have any money")
		return
	end
	remove(player, "coins", 1)
	say(player, "Ok, here you go")
	npcsay(npc, "Come closer")
	npcsay(npc, "And listen carefully to what the future holds for you")
	npcsay(npc, "As I peer into the swirling mists of the crystal ball")
	npcsay(npc, "I can see images forming")
	npcsay(npc, "I can see you")
	npcsay(npc, "You are holding a very impressive looking sword")
	npcsay(npc, "I'm sure I recognise that sword")
	npcsay(npc, "There is a big dark shadow appearing now")
	npcsay(npc, "Aaargh")
	local resp = multi(player,
		"Very interesting what does the Aaargh bit mean?",
		"Are you alright?",
		"Aaargh?")
	if resp == 1 then
		say(player, "Very interesting what does the Aaargh bit mean?")
		npcsay(npc, "Aaargh its Delrith")
		npcsay(npc, "Delrith is coming")
		gypsy_delrith(player, npc)
	elseif resp == 2 then
		say(player, "Are you alright?")
		npcsay(npc, "Aaargh its Delrith")
		gypsy_delrith(player, npc)
	elseif resp == 3 then
		say(player, "Aaargh?")
		npcsay(npc, "Aaargh its Delrith")
		gypsy_delrith(player, npc)
	end
end

-- shuuuuuun
local function gypsy_nonbeliever(player, npc)
	say(player, "No, I don't believe in that stuff")
	npcsay(npc, "Ok suit yourself")
end

local function gypsy_imagination(player, npc)
	say(player, "Believe me, I have a good imagination")
	npcsay(npc, "You seem like just the sort of person")
	npcsay(npc, "Who would want their fortune told then")
	local resp = multi(player,
		"No, I don't believe in that stuff",
		"Yes please")
	if resp == 1 then
		gypsy_nonbeliever(player, npc)
	elseif resp == 2 then
		gypsy_fortune(player, npc)
	end
end

local function gypsy_future(player, npc)
	say(player, "Go on then what's my future")
	npcsay(npc, "Cross my palm with silver and I'll tell you")
	local resp = multi(player,
		"Ok here you go",
		"Oh you want me to pay. No thanks")
	if resp == 1 then
		gypsy_fortune(player, npc)
	elseif resp == 2 then
		say(player, "Oh you want me to pay. No thanks")
		npcsay(npc, "Go away then")
	end
end

local function gypsy_powers(player, npc)
	say(player, "How do you know how old I think you are")
	npcsay(npc, "I have the power to know")
	npcsay(npc, "Just as I have the power to foresee the future")
	local resp = multi(player,
		"Ok what am I thinking now?",
		"Ok but how old are you?",
		"Go on then, what's my future?")
	if resp == 1 then
		say(player, "Ok what am I thinking now?")
		npcsay(npc, "You are thinking that I'll never guess what you are thinking")
	elseif resp == 2 then
		-- XXX verify
		gypsy_age(player, npc)
	elseif resp == 3 then
		gypsy_future(player, npc)
	end
end

local function gypsy_age(player, npc)
	say(player, "Ooh how old are you then?")
	npcsay(npc, "Older than you imagine")
	local resp = multi(player,
		"Believe me, I have a good imagination",
		"How do you know how old I think you are?",
		"Oh pretty old then")
	if resp == 1 then
		gypsy_imagination(player, npc)
	elseif resp == 2 then
		gypsy_powers(player, npc)
	elseif resp == 3 then
		say(player, "Oh pretty old then")
		npcsay(npc, "Yes I'm old")
		npcsay(npc, "Don't rub it in")
	end
end

local function gypsy_young(player, npc)
	say(player, "Who are you calling young one?!")
	npcsay(npc, "You have been on this world")
	npcsay(npc, "A relatively short time")
	npcsay(npc, "At least compared to me")
	npcsay(npc, "So do you want your fortune told or not?")
	local resp = multi(player,
		"Yes please",
		"No, I don't believe in that stuff",
		"Ooh how old are you then?")
	if resp == 1 then
		gypsy_fortune(player, npc)
	elseif resp == 2 then
		gypsy_nonbeliever(player, npc)
	elseif resp == 3 then
		gypsy_age(player, npc)
	end
end

local function gypsy_stop(player, npc)
	say(player, "Stop calling me that")
	npcsay(npc, "In the scheme of things you are very young")
	local resp = multi(player,
		"Ok but how old are you",
		"Oh if its in the scheme of things that's ok")
	if resp == 1 then
		say(player, "Ok but how old are you")
		npcsay(npc, "Count the number of legs of the chairs in the blue moon inn")
		npcsay(npc, "And multiply that number by seven")
		say(player, "Errr yeah whatever")
	elseif resp == 2 then
		say(player, "Oh if its in the scheme of things that's ok")
		npcsay(npc, "You show wisdom for one so young")
	end
end

function talknpc_gypsy(player, npc)
	local stage = getvar(player, "delrith_stage")
	if stage == 0 then
		npcsay(npc, "Hello, young one")
		npcsay(npc, "Cross my palm with silver and the future will be revealed to you")
		local resp = multi(player,
			"Ok, here you go",
			"Who are you calling young one?!",
			"No, I don't believe in that stuff")
		if resp == 1 then
			gypsy_fortune(player, npc)
		elseif resp == 2 then
			gypsy_young(player, npc)
		elseif resp == 3 then
			gypsy_nonbeliever(player, npc)
		end
	elseif stage == 1 then
		npcsay(npc, "Greetings how goes thy quest?")
		say(player, "I'm still working on it")
		npcsay(npc, "Well if you need my advice I'm always here young one")
		local resp = multi(player,
			"What is the magical incanation?",
			"Where can I find Silverlight?",
			"Well I'd better press on with it",
			"Stop calling me that")
		if resp == 1 then
			gypsy_incantation(player, npc)
		elseif resp == 2 then
			gypsy_silverlight(player, npc)
		elseif resp == 3 then
			say(player, "Well I'd better press on with it")
			npcsay(npc, "See you anon")
		elseif resp == 4 then
			gypsy_stop(player, npc)
		end
	else
		npcsay(npc, "Greetings young one")
		npcsay(npc, "You're a hero now")
		npcsay(npc, "That was a good bit of demonslaying")
		local resp = multi(player,
			"How do you know I killed it?",
			"Thanks",
			"Stop calling me that")
		if resp == 1 then
			say(player, "How do you know I killed it?")
			npcsay(npc, "You forget")
			npcsay(npc, "I'm good at knowing these things")
		elseif resp == 2 then
			say(player, "Thanks")
		elseif resp == 3 then
			gypsy_stop(player, npc)
		end
	end
end
