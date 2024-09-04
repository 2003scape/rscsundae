-- https://classic.runescape.wiki/w/Transcript:Osman

local osman_second_thing, osman_final_thing

local function osman_find_things(player, npc)
	say(player, "Okay, I better go find some things")
	npcsay(npc, "May good luck travel with you")
	npcsay(npc, "Don't forget to find Leela. It can't be done without her help")
	setvar(player, "prince_ali_stage", 2)
end

local function osman_first_thing(player, npc)
	say(player, "What is the first thing I must do?")
	npcsay(npc, "The prince is guarded by some stupid guards, and a clever woman")
	npcsay(npc, "The woman is our only way to get the prince out")
	npcsay(npc, "Only she can walk freely about the area")
	npcsay(npc, "I think you will need to tie her up")
	npcsay(npc, "one coil of rope should do for that")
	npcsay(npc, "And then disguise the prince as her to get him out without suspicion")
	say(player, "How good must the disguise must be?")
	npcsay(npc, "Only good enough to fool the guards at a distance")
	npcsay(npc, "Get a skirt like hers. Same colour, same style")
	npcsay(npc, "We will only have a short time")
	npcsay(npc, "A blonde wig too. That is up to you to make or find")
	npcsay(npc, "Something to colour the skin of the prince")
	npcsay(npc, "My daughter and top spy, leela, can help you there")

	local resp = multi(player,
		"Explain the first thing again",
		"What is needed second?",
		"And the final thing you need?",
		"Okay, I better go find some things")

	if resp == 1 then
		osman_first_thing(player, npc)
	elseif resp == 2 then
		osman_second_thing(player, npc)
	elseif resp == 3 then
		osman_final_thing(player, npc)
	elseif resp == 4 then
		osman_find_things(player, npc)
	end
end

osman_second_thing = function(player, npc)
	say(player, "What is needed second?")
	npcsay(npc, "We need the key, or a copy made")
	npcsay(npc, "If you can get some soft clay, then you can copy the key")
	npcsay(npc, "If you can convince Lady Keli to show it to you for a moment")
	npcsay(npc, "She is very boastful. It should not be too hard")
	npcsay(npc, "Bring the imprint to me, with a bar of bronze.")

	local resp = multi(player,
		"What is the first thing I must do?",
		"What exactly is needed second?",
		"And the final thing you need?",
		"Okay, I better go find some things")

	if resp == 1 then
		osman_first_thing(player, npc)
	elseif resp == 2 then
		osman_second_thing(player, npc)
	elseif resp == 3 then
		osman_final_thing(player, npc)
	elseif resp == 4 then
		osman_final_thing(player, npc)
	end
end

osman_final_thing = function(player, npc)
	say(player, "And the final things you need?")
	npcsay(npc, "You will need to stop the guard at the door")
	npcsay(npc, "Find out if he has any weaknesses, and use them")

	local resp = multi(player,
		"What is the first thing I must do?",
		"What exactly is needed second?",
		"Okay, I better go find some things")

	if resp == 1 then
		osman_first_thing(player, npc)
	elseif resp == 2 then
		osman_second_thing(player, npc)
	elseif resp == 3 then
		osman_find_things(player, npc)
	end
end

local function osman_still_need(player, npc)
	say(player, "Can you tell me what I still need to get?")
	npcsay(npc, "Let me check. You need:")

	-- i think this is right
	if getvar(player, "prince_ali_key_received") ~= 0 then
		npcsay(npc, "You have the key, good")
	else
		npcsay(npc, "A print of the key in soft clay, and a bronze bar")
		npcsay(npc, "Then collect the key from Leela")
	end

	if held(player, "blondwig", 1) then
		npcsay(npc, "The wig you have got, well done")
	else
		npcsay(npc, "You need to make a Blonde Wig somehow. Leela may help")
	end

	if held(player, "pinkskirt", 1) then
		npcsay(npc, "You have the skirt, good")
	else
		npcsay(npc, "A skirt the same as Keli's,")
	end

	if held(player, "skinpaste", 1) then
		npcsay(npc, "You have the skin paint, well done")
		npcsay(npc, "I thought you would struggle to make that")
	else
		npcsay(npc, "Something to colour the Princes skin lighter")
	end

	if held(player, "rope", 1) then
		npcsay(npc, "Yes, you have the rope.")
	else
		npcsay(npc, "Rope to tie Keli up with")
	end

	-- TODO dialogue is missing if you'v discussed 3 beer plan with Leela
	npcsay(npc, "You still need some way to stop the guard from interfering")
	npcsay(npc, "Once you have everything, Go to Leela")
	npcsay(npc, "she must be ready to get the prince away")
end

function talknpc_osman(player, npc)
	local stage = getvar(player, "prince_ali_stage")
	local resp

	if stage == 0 then
		npcsay(npc, "Hello, I am Osman")
		npcsay(npc, "What can I assist you with")

		resp = multi(player,
			"You don't seem very tough. Who are you?",
			"I hear wild rumours about a Prince",
			"I am just being nosy.")

		if resp == 1 then
			say(player, "You don't seem very tough. Who are you?")
			npcsay(npc, "I am in the employ of the Emir")
			npcsay(npc, "That is all you need to know")
		elseif resp == 2 then
			say(player, "I hear wild rumours about a Prince")
			npcsay(npc, "The prince is not here. He is... away")
			npcsay(npc, "If you can be trusted, speak to the chancellor, Hassan")
		elseif resp == 3 then
			say(player, "I am just being nosy") -- no period
			npcsay(npc, "That bothers me not")
			npcsay(npc, "The secrets of Al Kharid protect themselves")
		end
	elseif stage == 1 then
		say(player, "The chancellor trusts me. I have come for instructions")
		npcsay(npc, "Our Prince is captive by the Lady Keli")
		npcsay(npc, "We just need to make the rescue")
		npcsay(npc, "There are three things we need you to do")

		resp = multi(player,
			"What is the first thing I must do?",
			"What is needed second?",
			"And the final things you need?")

		if resp == 1 then
			osman_first_thing(player, npc)
		elseif resp == 2 then
			osman_second_thing(player, npc)
		elseif resp == 3 then
			osman_final_thing(player, npc)
		end
	elseif stage == 2 or stage == 3 then
		local key_made = getvar(player, "prince_ali_key_made")
		local key_received = getvar(player, "prince_ali_key_received")
		local key_paid = getvar(player, "prince_ali_key_paid")

		if held(player, "princeskey", 1) and key_paid == 0 then
			npcsay(npc, "You can get your part payment from the chancellor if you need to")

			resp = multi(player,
				"I will get it now. I have some things to buy",
				"Can you tell me what I still need to get?")

			if resp == 1 then
				say(player, "I will get it now. I have some things to buy")
				-- TODO missing, probably nothing
			elseif resp == 2 then
				osman_still_need(player, npc)
			end
		elseif
			key_made == 0
			and key_received ~= 0
			and not held(player, "keyprint", 1)
		then
			npcsay(npc, "You have lost the key for the Princes cell")
			npcsay(npc, "Get me the imprint and some more bronze, and I can get another made")
			say(player, "I will go and get they key imprint again.")
		elseif
			key_made == 0
			and held(player, "keyprint", 1)
			and held(player, "bronze bar", 1)
		then
			if key_received ~= 0 then
				npcsay(npc, "Well done, we can remake the key now.")
			else
				npcsay(npc, "Well done, we can make the key now.")
			end

			remove(player, "keyprint", 1)
			remove(player, "bronze bar", 1)
			mes(player, "Osman takes the Key imprint and the bronze bar")
			setvar(player, "prince_ali_key_made", 1)

			npcsay(npc, "Pick the key up from Leela.")

			if key_paid == 0 then
				npcsay(npc, "I will let you get 80 coins from the chancellor for getting this key")
			end
		elseif
			key_made == 0
			and held(player, "keyprint", 1)
			and not held(player, "bronze bar", 1)
		then
			npcsay(npc, "Good, you have the print of the key")
			npcsay(npc, "Get a bar of Bronze too, and I can get the key made")

			resp = multi(player,
				"I will get one, and come back",
				"Can you tell me what I still need to get?")

			if resp == 1 then
				say(player, "I will get one, and come back")
			elseif resp == 2 then
				osman_still_need(player, npc)
			end
		else
			osman_still_need(player, npc)
		end
	elseif stage == 4 then
		npcsay(npc, "The prince is safe, and on his way home with Leela")
		npcsay(npc, "You can pick up your payment from the chancellor")
	elseif stage == 5 then
		npcsay(npc, "Well done. A great rescue")
		npcsay(npc, "I will remember you if I have anything dangerous to do")
	end
end
