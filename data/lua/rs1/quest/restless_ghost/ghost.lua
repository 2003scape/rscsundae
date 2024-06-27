-- https://classic.runescape.wiki/w/Transcript:Ghost_(The_restless_ghost)

local function ghost_fail_goodbye(player, npc)
	say(player, "Goodbye. Thanks for the chat")
	npcsay(npc, "Wooo wooo")
end

local function ghost_fail_dontspeak(player, npc)
	say(player, "Sorry I don't speak ghost")
	npcsay(npc, "Woo woo?")
	say(player, "Nope still don't understand you")
	npcsay(npc, "Woooooooo")
	say(player, "Never mind")
end

local function ghost_fail_treasure(player, npc)
	say(player, "Any hints where I can find some treasure?")
	npcsay(npc, "Wooooooo woo!")
	local resp = multi(player,
		"Sorry I don't speak ghost",
		"Thank you. You've been very helpful")
	if resp == 1 then
		ghost_dontspeak(player, npc)
	elseif resp == 2 then
		say(player, "Thank you. You've been very helpfull")
		npcsay(npc, "Wooooooo")
	end
end

local function ghost_fail_notsure(player, npc)
	local resp = multi(player,
		"Goodbye. Thanks for the chat",
		"Hmm I'm not sure about that")
	if resp == 1 then
		ghost_fail_goodbye(player, npc)
	elseif resp == 2 then
		say(player, "Hmm I'm not sure about that")
		npcsay(npc, "Wooo woo")
		say(player, "Well if you insist")
		npcsay(npc, "Wooooooooo")
		say(player, "Ah well, better be off now")
		npcsay(npc, "Woo")
		say(player, "Bye")
	end
end

local function ghost_fail_thought(player, npc)
	say(player, "Yeah that's what I thought")
	npcsay(npc, "Wooo woooooooooooooo")
	ghost_fail_notsure(player, npc)
end

local function ghost_fail_recipe(player, npc)
	say(player, "You'll have to give me the recipe some time")
	npcsay(npc, "Wooooooo woo woooooooo")
	ghost_fail_notsure(player, npc)
end

local function ghost_fail_brother(player, npc)
	say(player, "My brother had exactly the same problem")
	npcsay(npc, "Woo Wooooo")
	npcsay(npc, "Wooooo Woo woo woo")
	local resp = multi(player,
		"Goodbye. Thanks for the chat",
		"You'll have to give me the recipe some time")
	if resp == 1 then
		ghost_fail_goodbye(player, npc)
	elseif resp == 2 then
		ghost_fail_recipe(player, npc)
	end
end

local function ghost_fail_really(player, npc)
	say(player, "Did he really?")
	npcsay(npc, "Woo")
	local resp = multi(player,
		"My brother had exactly the same problem",
		"Goodbye. Thanks for the chat")
	if resp == 1 then
		ghost_fail_brother(player, npc)
	elseif resp == 2 then
		ghost_fail_goodbye(player, npc)
	end
end

local function ghost_fail_interesting(player, npc)
	say(player, "Ooh that's interesting")
	npcsay(npc, "Woo wooo")
	npcsay(npc, "Woooooooooooooooooo")
	local resp = multi(player,
		"Did he really?",
		"Yeah that's what I thought")
	if resp == 1 then
		ghost_fail_really(player, npc)
	elseif resp == 2 then
		ghost_fail_thought(player, npc)
	end
end

local function ghost_fail(player, npc)
	npcsay(npc, "Wooo wooo wooooo")
	local resp = multi(player,
		"Sorry I don't speak ghost",
		"Ooh that's interesting",
		"Any hints where I can find some treasure?")
	if resp == 1 then
		ghost_fail_dontspeak(player, npc)
	elseif resp == 2 then
		ghost_fail_interesting(player, npc)
	elseif resp == 3 then
		ghost_fail_treasure(player, npc)
	end
end

local function ghost_task(player, npc)
	say(player, "I have been told a certain task may need to be completed")
	say(player, "So you can rest in peace")
	npcsay(npc, "I should think it is probably because")
	npcsay(npc, "A warlock has come along and stolen my skull")
	npcsay(npc, "If you look inside my coffin there")
	npcsay(npc, "you'll find my corspse without a head on it")
	say(player, "Do you know where this warlock might be now?")
	npcsay(npc, "I think it was one of the warlocks who lives in a big tower")
	npcsay(npc, "In the sea southwest from here")
	say(player, "Ok I will try and get the skull back for you, so you can rest in peace.")
	npcsay(npc, "Ooh thank you. That would be such a great relief")
	npcsay(npc, "It is so dull being a ghost")
	setvar(player, "ghost_stage", 3)
end

local function ghost_why(player, npc)
	say(player, "Yes, Ok do you know why you're a ghost?")
	npcsay(npc, "No, I just know I can't do anything much like this")
	ghost_task(player, npc)
end

local function ghost_clever(player, npc)
	say(player, "Yep clever aren't I")
	npcsay(npc, "I'm impressed")
	npcsay(npc, "You must be very powerfull")
	npcsay(npc, "I don't suppose you can stop me being a ghost?")
	local resp = multi(player,
		"Yes, Ok. Do you know why you're a ghost?",
		"No, you're scary")
	if resp == 1 then
		ghost_why(player, npc)
	elseif resp == 2 then
		say(player, "No, you're scary")
	end
end

local function ghost_tease(player, npc)
	say(player, "No")
	npcsay(npc, "Oh that's a pity. You got my hopes up there")
	say(player, "Yeah, it is pity. Sorry")
	npcsay(npc, "Hang on a second. You can understand me")

	local resp = multi(player, "No I can't", "Yep clever aren't I")
	if resp == 1 then
		say(player, "No I can't")
		npcsay(npc, "I don't know, the first person I can speak to in ages is a moron")
	elseif resp == 2 then
		ghost_clever(player, npc)
	end
end

local function ghost_amulet(player, npc)
	say(player, "Wow, this amulet works")
	npcsay(npc, "Oh its your amulet that's doing it. I did wonder")
	npcsay(npc, "I don't suppose you can help me? I don't like being a ghost")
	local resp = multi(player,
		"Yes, Ok. Do you know why you're a ghost?",
		"No, you're scary")
	if resp == 1 then
		ghost_why(player, npc)
	elseif resp == 2 then
		say(player, "No, you're scary")
	end
end

local function ghost_intro(player, npc)
	npcsay(npc, "Not very good actually")
	say(player, "What's the problem then?")
	npcsay(npc, "Did you just understand what I said?")
	local resp = multi(player,
		"Yep, now tell me what the problem is",
		"No, you sound like you're speaking non-sense to me",
		"Wow, this amulet works")
	if resp == 1 then
		say(player, "Yep, now tell me what the problem is")
		npcsay(npc, "Wow this is incredible, I didn't expect any one to understand me again")
		say(player, "Yes, yes I can understand you")
		say(player, "But have you any idea why you are doomed to be a ghost?")
		npcsay(npc, "I'm not sure")
		ghost_task(player, npc)
	elseif resp == 2 then
		ghost_tease(player, npc)
	elseif resp == 3 then
		ghost_amulet(player, npc)
	end
end

function talknpc_ghostx(player, npc)
	if worn(player, "amulet of ghostspeak", 1) then
		local stage = getvar(player, "ghost_stage")
		if stage < 3 then
			ghost_intro(player, npc)
		elseif stage < 4 then
			npcsay(npc, "How are you doing finding my skull?")
			say(player, "Sorry, I can't find it at the moment")
			npcsay(npc, "Ah well keep on looking")
			npcsay(npc, "I'm pretty sure it's somewhere in the tower south west from here")
			npcsay(npc, "There's a lot of levels to the tower, though")
			npcsay(npc, "I suppose it might take a little while to find")
		else
			npcsay(npc, "How are you doing finding my skull?")
			say(player, "I have found it")
			npcsay(npc, "Hurrah now I can stop being a ghost")
			npcsay(npc, "You just need to put it in my coffin over there")
			npcsay(npc, "And I will be free")
		end
	else
		ghost_fail(player, npc)
	end
end
