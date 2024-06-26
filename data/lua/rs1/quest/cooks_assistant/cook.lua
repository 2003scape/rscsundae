-- https://classic.runescape.wiki/w/Transcript:Cook
-- 1e_Luis/Quests/Cooks Assistant

local function cook_smalltalk(player, npc)
	say(player, "Nice hat")
	npcsay(npc, "Err thank you -it's a pretty ordinary cooks hat really")
end

local function cook_whatswrong(player, npc)
	say(player, "What's wrong?")
	npcsay(npc, "Ooh dear I'm in a terrible mess")
	npcsay(npc, "It's the duke's birthday today")
	npcsay(npc, "I'm meant to be making him a big cake for this evening")
	npcsay(npc, "Unfortunately, I've forgotten to buy some of the ingredients")
	npcsay(npc, "I'll never get them in time now")
	npcsay(npc, "I don't suppose you could help me?")

	local resp = multi(player,
		"Yes, I'll help you",
		"No, I don't feel like it. Maybe later")
	if resp == 1 then
		say(player, "Yes, I'll help you")
		npcsay(npc, "Oh thank you, thank you")
		npcsay(npc, "I need milk, eggs and flour")
		npcsay(npc, "I'd be very grateful if you can get them to me")
		setvar(player, "cook_stage", 1)
	elseif resp == 2 then
		say(player, "No, I don't feel like it. Maybe later")
		npcsay(npc, "OK, suit yourself")
	end
end

local function cook_unhappy(player, npc)
	say(player, "You don't look very happy")
	npcsay(npc, "No, I'm not")
	local resp = multi(player,
		"What's wrong?",
		"I'd take the rest of the day off if I were you")
	if resp == 1 then
		cook_whatswrong(player, npc)
	elseif resp == 2 then
		say(player, "I'd take the rest of the day off if I were you")
		npcsay(npc, "No, that's the worst thing I could do - I'd get in terrible trouble")
		cook_whatswrong(player, npc)
	end
end

local function cook_intro(player, npc)
	npcsay(npc, "What am I to do?")
	local resp = multi(player,
		"What's wrong?",
		"Well you could give me all your money",
		"You don't look very happy")
	if resp == 1 then
		cook_whatswrong(player, npc)
	elseif resp == 2 then
		say(player, "Well you could give me all your money")
		npcsay(npc, "HaHa very funny")
	elseif resp == 3 then
		cook_unhappy(player, npc)
	end
end

local function cook_progress(player, npc)
	npcsay(npc, "How are you getting on with finding the ingredients?")

	if held(player, "milk", 1) and
	    held(player, "flourpot", 1) and
	    held(player, "egg", 1) then
		say(player, "I now have everything you need for your cake")
		say(player, "Milk, flour, and an egg!")
		npcsay(npc, "I am saved thankyou!")
		mes(player, "You give some milk, an egg and some flour to the cook")
		delay(3)
		remove(player, "milk", 1)
		remove(player, "flourpot", 1)
		remove(player, "egg", 1)
		mes(player, "Well done. You have completed the cook's assistant quest")
		advancestat(player, STAT_COOKING, 1000, 200)
		setvar(player, "cook_stage", 2)
		giveqp(player, 1)
	elseif not held(player, "milk", 1) and
	    not held(player, "flourpot", 1) and
	    not held(player, "egg", 1) then
		say(player, "I'm afraid I don't have any yet!")
		npcsay(npc, "Oh dear oh dear!")
		npcsay(npc, "I need flour, eggs, and milk")
		npcsay(npc, "Without them I am doomed!")
	else
		say(player, "I have found some of the things you asked for:")

		if held(player, "milk", 1) then
			say(player, "I have some milk")
		end

		if held(player, "flourpot", 1) then
			say(player, "I have some flour")
		end

		if held(player, "egg", 1) then
			say(player, "I have an egg")
		end

		npcsay(npc, "Great, but can you get the other ingredients as well?")
		npcsay(npc, "You still need to find")

		if not held(player, "milk", 1) then
			npcsay(npc, "Some milk")
		end

		if not held(player, "flourpot", 1) then
			npcsay(npc, "Some flour")
		end

		if not held(player, "egg", 1) then
			npcsay(npc, "An egg")
		end
	end
end

local function cook_complete(player, npc)
	npcsay(npc, "Hello friend, how is the adventuring going?")
	local resp = multi(player,
		"I am getting strong and mighty",
		"I keep on dying",
		"Nice hat",
		"Can I use your range?")
	if resp == 1 then
		say(player, "I am getting strong and mighty")
		npcsay(npc, "Glad to hear it")
	elseif resp == 2 then
		say(player, "I keep on dying")
		npcsay(npc, "Ah well at least you keep coming back to life!")
	elseif resp == 3 then
		cook_smalltalk(player, npc)
	elseif resp == 4 then
		say(player, "Can I use your range?")
		npcsay(npc, "Go ahead")
		npcsay(npc, "It's a very good range")
		npcsay(npc, "It's easier to use than most other ranges")
	end
end


function talknpc_cook(player, npc)
	local stage = getvar(player, "cook_stage")

	if stage == 0 then
		cook_intro(player, npc)
	elseif stage == 1 then
		cook_progress(player, npc)
	elseif stage == 2 then
		cook_complete(player, npc)
	end
end
