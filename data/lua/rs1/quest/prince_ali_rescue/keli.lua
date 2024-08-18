-- https://classic.runescape.wiki/w/Transcript:Lady_Keli

local function keli_pay(player, npc)
	say(player, "Are you sure they will pay?")
	npcsay(
		npc,
		"They will pay, or we will cut his hair off and send it to them"
	)
	say(
		player,
		"Don't you think that something tougher, maybe cut his finger off?"
	)
	npcsay(npc, "Thats a good idea. I could use talented people like you")
	npcsay(npc, "I may call on you if I need work doing")
end

local function keli_disturb(player, npc)
	say(player, "I should not disturb someone as tough as you")
	npcsay(npc, "I need to do a lot of work, goodbye")
	npcsay(npc, "When you get a little tougher, maybe I will give you a job")
end

local function keli_get_him_out(player, npc)
	local resp

	say(player, "Can you be sure they will not try to get him out?")
	npcsay(npc, "There is no way to release him")
	npcsay(npc, "The only key to the door is on a chain around my neck")
	npcsay(npc, "And the locksmith who made the lock,")
	npcsay(npc, "died suddenly when he had finished")
	npcsay(npc, "There is not another key like this in the world")

	resp = multi(
		player,
		"Could I see the key please",
		"That is a good way to keep secrets",
		"I should not disturb someone as tough as you"
	)

	if resp == 1 then
		say(player, "Could I see the key please, just for a moment")
		say(player, "It would be something I can tell my grandchildren")
		say(player, "When you are even more famous than you are now")
		npcsay(npc, "As you put it that way, I am sure you can see it")
		npcsay(npc, "You cannot steal the key, it is on an Adamantite chain")
		npcsay(npc, "I cannot see the harm")

		mes(player, "Keli shows you a small key on a stronglooking chain")

		local stage = getvar(player, "prince_ali_stage")

		-- TODO apparently this works without starting the quest too?
		if (stage == 2 or stage == 3) and held(player, "softclay", 1) then
			resp = multi(
				player,
				"Could I touch the key for a moment please",
				"I should not disturb someone as tough as you"
			)

			if resp == 1 then
				say(player, "Could I touch the key for a moment please")
				npcsay(npc, "Only for a moment then")

				mes(player, "You put a piece of your soft clay in your hand")
				mes(player, "As you touch the key, you take an imprint of it")

				remove(player, "softclay", 1)
				give(player, "keyprint", 1)

				say(player, "Thankyou so much, you are too kind, o great Keli")
				npcsay(npc, "There, run along now, I am very busy")
			elseif resp == 2 then
				keli_disturb(player, npc)
			end
		else
			npcsay(npc, "There, run along now, I am very busy")
		end
	elseif resp == 2 then
		say(player, "That is a good way to keep secrets")
		npcsay(npc, "It is the best way I know")
		npcsay(npc, "Dead men tell no tales")
		say(player, "I am glad I know none of your secrets, Keli")
	elseif resp == 3 then
		keli_disturb(player, npc)
	end
end

local function keli_plan(player, npc)
	local resp

	say(player, "What is your latest plan then?")
	say(player, "Of course you need not go into specific details")
	npcsay(
		npc,
		"Well, I can tell you, I have a valuable prisoner here in my cells"
	)
	npcsay(npc, "I can expect a high reward to be paid very soon for this guy")
	npcsay(npc, "I can't tell you who he is, but he is a lot colder now")

	resp = multi(
		player,
		"Ah, I see. You must have been very skilful",
		"Thats great, are you sure they will pay?",
		"Can you be sure they will not try to get him out?",
		"I should not disturb someone as tough as you"
	)

	if resp == 1 then
		say(player, "You must have been very skilful")
		npcsay(npc, "Yes, I did most of the work, we had to grab the Pr...")
		npcsay(
			npc,
			"er, we had to grab him under cover of ten of his bodyguards"
		)
		npcsay(npc, "It was a stronke of genius")

		resp = multi(
			player,
			"Are you sure they will pay?",
			"Can you be sure they will not try to get him out?",
			"I should not disturb someone as tough as you"
		)

		if resp == 1 then
			keli_pay(player, npc)
		elseif resp == 2 then
			keli_get_him_out(player, npc)
		elseif resp == 3 then
			keli_disturb(player, npc)
		end
	elseif resp == 2 then
		keli_pay(player, npc)
	elseif resp == 3 then
		keli_get_him_out(player, npc)
	elseif resp == 4 then
		keli_disturb(player, npc)
	end
end

local function keli_katrine(player, npc)
	say(player, "I think Katrine is still tougher")
	npcsay(npc, "Well you can think that all you like")
	npcsay(npc, "I know those blackarm cowards dare not leave the city")
	npcsay(npc, "Out here, I am toughest. You can tell them that!")
	npcsay(npc, "Now get out of my sight, before I call my guards")
end

local function keli_trained(player, npc)
	say(player, "You must have trained a lot for this work")
	npcsay(npc, "I have used a sword since I was a small girl")
	npcsay(npc, "stabbed three people before I was 6 years old")
end

local function keli_prompt(player, npc)
	local resp = multi(
		player,
		"I think Katrine is still tougher",
		"What is your latest plan then?",
		"You must have trained a lot for this work",
		"I should not disturb someone as tough as you"
	)

	if resp == 1 then
		keli_katrine(player, npc)
	elseif resp == 2 then
		keli_plan(player, npc)
	elseif resp == 3 then
		keli_trained(player, npc)
	elseif resp == 4 then
		keli_disturb(player, npc)
	end
end

local function keli_heard(player, npc)
	say(player, "The great Lady Keli, of course I have heard of you")
	say(player, "You are famous in Runescape!")
	npcsay(
		npc,
		"Thats very kind of you to say. Reputation are not easily earnt"
	)
	npcsay(npc, "I have managed to succeed where many fail")

	keli_prompt(player, npc)
end

function talknpc_lady_keli(player, npc)
	local stage = getvar(player, "prince_ali_stage")
	local resp

	if stage < 4 then
		say(player, "Are you the famous Lady Keli?")
		say(player, "Leader of the toughest gang of mercenary killers around?")
		npcsay(npc, "I am Keli, you have heard of me then")

		resp = multi(
			player,
			"Heard of you? you are famous in Runescape!",
			"I have heard a little, but I think Katrine is tougher",
			"I have heard rumours that you kill people",
			"No I have never really heard of you"
		)

		if resp == 1 then
			keli_heard(player, npc)
		elseif resp == 2 then
			keli_katrine(player, npc)
		elseif resp == 3 then
			say(player, "I have heard rumours that you kill people")
			npcsay(npc, "Theres always someone ready to spread rumours")
			npcsay(
				npc,
				"I heard a rumour the other day, that some men are wearing "
					.. "skirts"
			)
			npcsay(
				npc,
				"If one of my men wore a skirt, he would wish he hadn't"
			)

			keli_prompt(player, npc)
		elseif resp == 4 then
			say(player, "No I have never really heard of you")
			npcsay(npc, "You must be new to this land then")
			npcsay(
				npc,
				"EVERYONE knows of Lady Keli and her prowess with the sword"
			)

			resp = multi(
				player,
				"No, still doesn't ring a bell",
				"Yes, of course I have heard of you",
				"You must have trained a lot for this work",
				"I should not disturb someone as tough as you"
			)

			if resp == 1 then
				say(player, "No, still doesn't ring a bell")
				npcsay(npc, "Well, you know of me now")
				npcsay(npc, "I will ring your bell if you do not show respect")

				resp = multi(
					player,
					"I do not show respect to killers and hoodlums",
					"You must have trained a lot for this work",
					"I should not disturb someone as tough as you, great lady"
				)

				if resp == 1 then
					say(player, "I do not show respect to killers and hoodlums")
					npcsay(npc, "You should, you really should")
					npcsay(
						npc,
						"I am wealthy enough to place a bounty on your head"
					)
					npcsay(npc, "Or just remove your head myself")
					npcsay(
						npc,
						"Now go, I am busy, too busy to fight a would-be "
							.. "hoodlum"
					)
				elseif resp == 2 then
					keli_trained(player, npc)
				elseif resp == 3 then
					keli_disturb(player, npc)
				end
			elseif resp == 2 then
				keli_heard(player, npc)
			elseif resp == 3 then
				keli_trained(player, npc)
			elseif resp == 4 then
				keli_disturb(player, npc)
			end
		end
	else
		npcsay(npc, "You tricked me, and tied me up")
		npcsay(npc, "You should not stay here if you want to remain alive")
		npcsay(npc, "Guards! Guards! Kill this stranger")
	end
end

function usenpc_lady_keli_rope(player, npc)
	local stage = getvar(player, "prince_ali_stage")

	if stage < 3 then
		mes(
			player,
			"You cannot tie Keli up until you have all equipment and "
				.. "disabled the guard"
		)

		-- this is accurate!
		talknpc_lady_keli(player, npc)
	elseif stage == 3 and getvar(player, "prince_ali_joe_drunk") ~= 0 then
		if getvar(player, "prince_ali_keli_tied_once") ~= 0 then
			mes(
				player,
				"You overpower Keli again, tie her up, and put her in a "
					.. "cupboard"
			)
			mes(
				player,
				"You must open the door to rescue the prince before she escapes"
			)
		else
			mes(
				player,
				"You overpower Keli, tie her up, and put her in a cupboard"
			)

			setvar(player, "prince_ali_keli_tied_once", 1)
		end

		delnpc("lady keli")
		pause(100, 100)
		addnpc("lady keli", 195, 640)
	elseif stage >= 4 then
		mes(
			player,
			"You have rescued the prince already, you cannot use the same plan "
				.. "again"
		)
	end
end

register_usenpc("lady keli", "rope", usenpc_lady_keli_rope)
