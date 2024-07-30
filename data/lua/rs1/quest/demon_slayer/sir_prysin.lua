-- https://classic.runescape.wiki/w/Transcript:Sir_Prysin
-- rsc-preservation.xyz/Quests/demon-slayer.pcap

local function prysin_who(player, npc)
	say(player, "I am a mighty adventurer. Who are you?")
	npcsay(npc, "I am")
	npcsay(npc, "A bold and famous knight of the realm")
end

local function prysin_confuse(player, npc)
	say(player, "I'm not sure. I was hoping you could tell me")
	npcsay(npc, "Well I've never met you before")
end

local function prysin_go(player, npc)
	say(player, "Well I'd better go key hunting")
	npcsay(npc, "Ok goodbye")
end

local function prysin_drain(player, npc)
	say(player, "So what does the drain connect to?")
	npcsay(npc, "It is the drain")
	npcsay(npc, "For the drainpipe running from the sink in the kitchen")
	npcsay(npc, "Down to the palace sewers")
	local resp = multi(player,
		"Where can I find Captain Rovin?",
		"Where does the wizard live?",
		"Well I'd better go key hunting")
	if resp == 1 then
		prysin_captain(player, npc)
	elseif resp == 2 then
		prysin_wizard(player, npc)
	elseif resp == 3 then
		prysin_go(player, npc)
	end
end

local function prysin_own_key(player, npc)
	say(player, "Can you give me your key?")
	npcsay(npc, "Um")
	npcsay(npc, "Ah")
	npcsay(npc, "Well there's a problem there as well")
	npcsay(npc, "I managed to drop the key in the drain")
	npcsay(npc, "Just outside the palace kitchen")
	npcsay(npc, "It is just inside and I can't reach it")
	local resp = multi(player,
		"So what does the drain lead to?",
		"Where can I find Captain Rovin?",
		"Where does the wizard live?",
		"Well I'd better go key hunting")
	if resp == 1 then
		prysin_drain(player, npc)
	elseif resp == 2 then
		prysin_captain(player, npc)
	elseif resp == 3 then
		prysin_wizard(player, npc)
	elseif resp == 4 then
		prysin_go(player, npc)
	end
end

local function prysin_captain(player, npc)
	say(player, "Where can I find Captain Rovin?")
	npcsay(npc, "Captain Rovin lives at the top of the guards quarters")
	npcsay(npc, "in the northwest wing of this palace")
	local resp = multi(player,
		"Can you give me your key?",
		"Where does the wizard live?",
		"Well I'd better go key hunting")
	if resp == 1 then
		prysin_own_key(player, npc)
	elseif resp == 2 then
		prysin_wizard(player, npc)
	elseif resp == 3 then
		prysin_go(player, npc)
	end
end

function prysin_wizard(player, npc)
	say(player, "Where does the wizard live?")
	npcsay(npc, "Wizard Traiborn?")
	npcsay(npc, "He is one of the wizards who lives in the tower")
	npcsay(npc, "On the little island just off the south coast")
	npcsay(npc, "I believe his quarters are on the first floor of the tower")
	local resp = multi(player,
		"Can you give me your key?",
		"Where can I find Captain Rovin?",
		"Well I'd better go key hunting")
	if resp == 1 then
		prysin_own_key(player, npc)
	elseif resp == 2 then
		prysin_captain(player, npc)
	elseif resp == 3 then
		prysin_go(player, npc)
	end
end

local function prysin_keys(player, npc)
	npcsay(npc, "I kept one of the keys")
	npcsay(npc, "I gave the other two")
	npcsay(npc, "To other people for safe keeping")
	npcsay(npc, "One I gave to Rovin")
	npcsay(npc, "who is captain of the palace guard")
	npcsay(npc, "I gave the other to the wizard Traiborn")
	setvar(player, "silverlight_stage", 1)
	local resp = multi(player,
		"Can you give me your key?",
		"Where can I find Captain Rovin?",
		"Where does the wizard live?")
	if resp == 1 then
		prysin_own_key(player, npc)
	elseif resp == 2 then
		prysin_captain(player, npc)
	elseif resp == 3 then
		prysin_wizard(player, npc)
	end
end

local function prysin_get_silverlight(player, npc)
	npcsay(npc, "The problem is getting silverlight")
	say(player, "You mean you don't have it?")
	npcsay(npc, "Oh I do have it")
	npcsay(npc, "But it is so powerful")
	npcsay(npc, "That I have put it in a special box")
	npcsay(npc, "Which needs three different keys to open it")
	npcsay(npc, "That way, it won't fall into the wrong hands")
	local resp = multi(player,
		"So give me the keys",
		"And why is this a problem?")
	if resp == 1 then
		say(player, "So give me the keys")
		npcsay(npc, "Um")
		npcsay(npc, "Well, It's not so easy")
		prysin_keys(player, npc)
	elseif resp == 2 then
		say(player, "And why is this a problem?")
		prysin_keys(player, npc)
	end
end

local function prysin_silverlight(player, npc)
	say(player, "I need to find Silverlight")
	npcsay(npc, "What do you need to find that for?")
	say(player, "I need it to fight Delrith")
	npcsay(npc, "Delrith?")
	npcsay(npc, "I thought the world was rid of him")
	local resp = multi(player,
		"Well, the gypsy's crystal ball seems to think otherwise",
		"He's back and unfortunatly I've got to deal with him")
	if resp == 1 then
		say(player, "Well, the gypsy's crystal ball seems to think otherwise")
		npcsay(npc, "Well if the ball says so, I'd better help you")
		prysin_get_silverlight(player, npc)
	elseif resp == 2 then
		say(player, "He's back and unfortunatly I've got to deal with him")
		npcsay(npc, "You don't look up to much")
		npcsay(npc, "I suppose Silverlight may be good enough to carry you through though")
		prysin_get_silverlight(player, npc)
	end
end

local function prysin_aris(player, npc)
	say(player, "Gypsy Aris said I should come and talk to you")
	npcsay(npc, "Gypsy Aris? Is she still alive?")
	npcsay(npc, "I remember her from when I was pretty young")
	npcsay(npc, "Well what do you need to talk to me about?")
	local resp = multi(player,
		"I need to find Silverlight",
		"Yes, she is still alive")
	if resp == 1 then
		prysin_silverlight(player, npc)
	elseif resp == 2 then
		say(player, "Yes, she is still alive")
		npcsay(npc, "I would have thought she would have died by now")
		npcsay(npc, "She was pretty old, when I was a lad")
		npcsay(npc, "Anyway, what can I do for you?")
		prysin_silverlight(player, npc)
	end
end

local function prysin_init(player, npc)
	local resp
	npcsay(npc, "Hello, who are you")

	if getvar(player, "delrith_stage") == 0 then
		resp = multi(player,
			"I am a mighty adventurer. Who are you?",
			"I'm not sure, I was hoping you could tell me")
	else
		resp = multi(player,
			"I am a mighty adventurer. Who are you?",
			"I'm not sure, I was hoping you could tell me",
			"Gypsy Aris said I should come and talk to you")
	end
	if resp == 1 then
		prysin_who(player, npc)
	elseif resp == 2 then
		prysin_confuse(player, npc)
	elseif resp == 3 then
		prysin_aris(player, npc)
	end
end

local function prysin_progress(player, npc)
	npcsay(npc, "So how are you doing with getting the keys?")

	if held(player, "silverlight key 1") and
	    held(player, "silverlight key 2") and
	    held(player, "silverlight key 3") then
		say(player, "I've got them all")
		npcsay(npc, "Excellent. Now I can give you Silverlight")
		mes(player, "You give all three keys to Sir Prysin")
		delay(5)
		mes(player, "Sir Prysin unlocks a long thin box")
		delay(4)
		mes(player, "Prysin hands you an impressive looking sword")
		give(player, "silverlight", 1)
		remove(player, "silverlight key 1", 1)
		remove(player, "silverlight key 2", 1)
		remove(player, "silverlight key 3", 1)
		return
	end

	if held(player, "silverlight key 1") or
	    held(player, "silverlight key 2") or 
	    held(player, "silverlight key 3") then
		if held(player, "silverlight key 2") then
			say(player, "I've got the key off Captain Rovin")
		end
		if held(player, "silverlight key 3") then
			say(player, "I've got the key You dropped down the drain")
		end
		if held(player, "silverlight key 1") then
			say(player, "I've got the key off Wizard Traiborn")
		end
	else
		say(player, "I've not found any of them yet")
	end

	local resp = multi(player,
		"Can you remind me where all the keys were again",
		"I'm still looking")
	if resp == 1 then
		say(player, "Can you remind me where all the keys were again")
		prysin_keys(player, npc)
	elseif resp == 2 then
		say(player, "I'm still looking")
		npcsay(npc, "Ok, tell me when you've got them all")
	end
end

function talknpc_sir_prysin(player, npc)
	local delrith_stage = getvar(player, "delrith_stage")
	if delrith_stage >= 2 then
		npcsay(npc, "Hello, I've heard you stopped that demon well done")
		say(player, "Yes, that's right")
		npcsay(npc, "A good job well done then")
		say(player, "Thank you")
	else
		if held(player, "silverlight", 1) then
			npcsay(npc, "You sorted that demon yet?")
			say(player, "No, not yet")
			npcsay(npc, "Well get on with it")
			npcsay(npc, "He'll be pretty powerful when he gets to full strength")
			return
		end
		local silverlight_stage = getvar(player, "silverlight_stage")
		if silverlight_stage == 0 then
			prysin_init(player, npc)
		else
			prysin_progress(player, npc)
		end
	end
end
