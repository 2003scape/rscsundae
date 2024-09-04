-- https://classic.runescape.wiki/w/Transcript:Leela

local leela_key, leela_guards

local function leela_equipment(player, npc)
	say(player, "I will go and get the rest of the escape equipment")
	npcsay(npc, "I shall await your return with everything")
end

local function leela_disguise(player, npc)
	local resp

	say(player, "I must make a disguise. What do you suggest?")
	npcsay(npc, "Only the lady Keli, can wander about outside the jail")
	npcsay(npc, "The guards will shoot to kill if they see the prince out")
	npcsay(npc, "so we need a disguise well enough to fool them at a distance")

	local disguise_items = 0

	if held(player, "blondwig", 1) then
		npcsay(npc, "The wig you have got, well done")
		disguise_items = disguise_items + 1
	else
		npcsay(npc, "You need a wig, maybe made from wool")
		npcsay(npc, "If you find someone who can work with wool, ask them about it")
		npcsay(npc, "Then the old witch may be able to help you dye it")
	end

	if held(player, "pinkskirt", 1) then
		npcsay(npc, "You have got the skirt, good")
		disguise_items = disguise_items + 1
	else
		npcsay(npc, "You will need to get a pink skirt, same as Keli's")
	end

	if held(player, "skinpaste", 1) then
		npcsay(npc, "You have the skin paint, well done")
		npcsay(npc, "I thought you would struggle to make that")
		disguise_items = disguise_items + 1
	else
		npcsay(npc, "we still need something to colour the Princes skin lighter")
		npcsay(npc, "There's an old witch close to here, she knows about many things")
		npcsay(npc, "She may know some way to make the skin lighter")
	end

	if disguise_items == 3 then
		npcsay(npc, "You do have everything for the disguise")
	end

	if held(player, "rope", 1) then
		npcsay(npc, "You have rope I see, tie up Keli")
		npcsay(npc, "that will be the most dangerous part of the plan")
	else
		npcsay(npc, "You will still need some rope to tie up Keli, of course")
		npcsay(npc, "I heard that there was a good ropemaker around here")
	end

	if held(player, "princeskey", 1) then
		resp = multi(player,
			"What can i do with the guards?",
			"I will go and get the rest of the escape equipment")

		resp = resp + 1
	else
		resp = multi(player,
			"I need to get the key made",
			"What can i do with the guards?",
			"I will go and get the rest of the escape equipment")
	end

	if resp == 1 then
		leela_key(player, npc)
	elseif resp == 2 then
		leela_guards(player, npc)
	elseif resp == 3 then
		leela_equipment(player, npc)
	end
end

leela_key = function(player, npc)
	say(player, "I need to get the key made")
	npcsay(npc, "Yes, that is most important")
	npcsay(npc, "There is no way you can get the real key.")
	npcsay(npc, "It is on a chain around Keli's neck. almost impossible to steal")
	npcsay(npc, "get some soft clay, and get her to show you the key somehow")
	npcsay(npc, "then take the print, with bronze, to my father")

	local resp = multi(player,
		"I must make a disguise. What do you suggest?",
		"What can i do with the guards?",
		"I will go and get the rest of the escape equipment")

	if resp == 1 then
		leela_disguise(player, npc)
	elseif resp == 2 then
		leela_guards(player, npc)
	elseif resp == 3 then
		leela_equipment(player, npc)
	end
end

leela_guards = function(player, npc)
	local resp

	say(player, "What can i do with the guards?")
	npcsay(npc, "Most of the guards will be easy")
	npcsay(npc, "The disguise will get past them")
	npcsay(npc, "The only guard who will be a problem will be the one at the door")
	npcsay(npc, "He is talkative, try to find a weakness in him")
	npcsay(npc, "We can discuss this more when you have the rest of the escape kit")

	if held(player, "princeskey", 1) then
		resp = multi(player,
			"I must make a disguise. What do you suggest?",
			"I will go and get the rest of the escape equipment")

		if resp == 2 then
			resp = 3
		end
	else
		resp = multi(player,
			"I must make a disguise. What do you suggest?",
			"I need to get the key made",
			"I will go and get the rest of the escape equipment")
	end

	if resp == 1 then
		leela_disguise(player, npc)
	elseif resp == 2 then
		leela_key(player, npc)
	elseif resp == 3 then
		leela_equipment(player, npc)
	end
end

function talknpc_leela(player, npc)
	local stage = getvar(player, "prince_ali_stage")
	local resp

	if getvar(player, "prince_ali_key_made") ~= 0 then
		npcsay(npc, "My father sent this key for you, be careful not to lose it")
		mes(player, "Leela gives you a copy of the key to the princes door")
		give(player, "princeskey", 1)
		setvar(player, "prince_ali_key_made", 0)
		setvar(player, "prince_ali_key_received", 1)
	end

	if stage < 2 then
		say(player, "What are you waiting here for")
		npcsay(npc, "That is no concern of yours, adventurer")
	elseif stage == 2 then
		local has_equipment = getvar(player, "prince_ali_key_received") ~= 0
			and held(player, "rope", 1)
			and held(player, "blondwig", 1)
			and held(player, "pinkskirt", 1)
			and held(player, "skinpaste", 1)

		if has_equipment then
			setvar(player, "prince_ali_stage", 3)
			talknpc_leela(player, npc)
		else
			say(player, "I am here to help you free the prince")
			npcsay(npc, "Your employment is known to me.")
			npcsay(npc, "Now, do you know all that we need to make the break?")

			if held(player, "princeskey", 1) then
				resp = multi(player,
					"I must make a disguise. What do you suggest?",
					"What can i do with the guards?",
					"I will go and get the rest of the escape equipment")

				if resp > 1 then
					resp = resp + 1
				end
			else
				resp = multi(player,
					"I must make a disguise. What do you suggest?",
					"I need to get the key made",
					"What can i do with the guards?",
					"I will go and get the rest of the escape equipment")
			end

			if resp == 1 then
				leela_disguise(player, npc)
			elseif resp == 2 then
				leela_key(player, npc)
			elseif resp == 3 then
				leela_guards(player, npc)
			elseif resp == 4 then
				leela_equipment(player, npc)
			end
		end
	elseif stage == 3 then
		npcsay(npc, "Good, you have all the basic equipment")
		npcsay(npc, "What are your plans to stop the guard interfering?")

		resp = multi(player,
			"I haven't spoken to him yet",
			"I was going to attack him",
			"I hoped to get him drunk",
			"Maybe I could bribe him to leave")

		if resp == 1 then
			say(player, "I haven't spoken to him yet")
			npcsay(npc, "Well, speaking to him may find a weakness he has")
			npcsay(npc, "See if theres something that could stop him bothering us")
		elseif resp == 2 then
			say(player, "I was going to attack him")
			npcsay(npc, "I don't think you should")
			npcsay(npc, "If you do the rest of the gang and Keli would attack you")
			npcsay(npc, "The door guard should be removed first, to make it easy")
		elseif resp == 3 then
			say(player, "I hoped to get him drunk")
			npcsay(npc, "Well, thats possible. These guards do like a drink")
			npcsay(npc, "I would think that it will take at least 3 beers to do it well")
			npcsay(npc, "You would probably have to do it all at the same time too")
			npcsay(npc, "The effects of the local beer wear of quickly")
		elseif resp == 4 then
			say(player, "Maybe I could bribe him to leave")
			npcsay(npc, "You could try. I don't think the emir will pay anything towards it")
			npcsay(npc, "And we did bribe one of their guards once")
			npcsay(npc, "Keli killed him in front of the other guards, as a deterrent")
			npcsay(npc, "It would probably take a lot of gold")
		end

		npcsay(npc, "Good luck with the guard. When the guard is out you can tie up Keli")
	elseif stage > 3 then
		npcsay(npc, "Thank you, Al Kharid will forever owe you for your help")
		npcsay(npc, "I think that if there is ever anything that needs to be done,")
		npcsay(npc, "you will be someone they can rely on")
	end
end
