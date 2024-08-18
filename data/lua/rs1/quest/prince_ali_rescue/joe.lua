-- https://classic.runescape.wiki/w/Transcript:Joe

local joe_life_of_guard, joe_boy

local function joe_leave(player, npc)
	say(player, "I had better leave, I don't want trouble")
	npcsay(npc, "Thanks I appreciate that")
	npcsay(
		npc,
		"Talking on duty can be punishable by having your mouth stitched up"
	)
	npcsay(npc, "These are tough people, no mistake")
end

local function joe_beer(player, npc)
	say(player, "I have some beer here, fancy one?")
	npcsay(npc, "Ah, that would be lovely, just one now, just to wet my throat")
	say(player, "Of course, it must be tough being here without a drink")

	remove(player, "beer", 1)
	mes(player, "You hand a beer to the guard, he drinks it in seconds")

	npcsay(npc, "Thas was perfect, i cant thank you enough")

	if held(player, "beer", 2) then
		say(player, "Would you care for another, my friend?")
		npcsay(npc, "I better not, I don't want to be drunk on duty")
		say(
			player,
			"Here, just keep these for later, I hate to see a thirsty guard"
		)

		remove(player, "beer", 2)

		mes(player, "You hand two more beers to the guard")
		mes(player, "he takes a sip of one, and then he drinks them both")

		npcsay(npc, "Franksh, that wash just what I need to shtay on guard")
		npcsay(npc, "No more beersh, i don't want to get drunk")

		mes(player, "The guard is drunk, and no longer a problem")

		setvar(player, "prince_ali_joe_drunk", 1)
	else
		say(player, "How are you? still ok. Not too drunk?")
		npcsay(npc, "No, I don't get drunk with only one drink")
		npcsay(npc, "You would need a few to do that, but thanks for the beer")
	end
end

local function joe_chill_out(player, npc)
	local resp

	say(player, "Hey, chill out, I wont cause you trouble")
	say(player, "I was just wondering what you do to relax")
	npcsay(
		npc,
		"You never relax with these people, but its a good career for a "
			.. "young man"
	)
	npcsay(npc, "And some of the shouting I rather like")
	npcsay(npc, "RESISTANCE IS USELESS!")

	resp = multi(
		player,
		"So what do you buy with these great wages?",
		"Tell me about the life of a guard",
		"Would you be interested in making a little more money?",
		"I had better leave, I don't want trouble"
	)

	if resp == 1 then
		say(player, "So what do you buy with these great wages?")
		npcsay(
			npc,
			"Really, after working here, theres only time for a drink or three"
		)
		npcsay(
			npc,
			"All of us guards go to the same bar, And drink ourselves stupid"
		)
		npcsay(
			npc,
			"Its what I enjoy these days, that fade into unconsciousness"
		)
		npcsay(npc, "I can't resist the sight of a really cold beer")

		local stage = getvar(player, "prince_ali_stage")

		if stage >= 3 and held(player, "beer", 1) then
			resp = multi(
				player,
				"I have some beer here, fancy one?",
				"Tell me about the life of a guard",
				"What did you want to be when you were a boy",
				"I had better leave, I don't want trouble"
			)
		else
			resp = multi(
				player,
				"Tell me about the life of a guard",
				"What did you want to be when you were a boy",
				"I had better leave, I don't want trouble"
			)

			resp = resp + 1
		end

		if resp == 1 then
			joe_beer(player, npc)
		elseif resp == 2 then
			joe_life_of_guard(player, npc)
		elseif resp == 3 then
			joe_boy(player, npc)
		elseif resp == 4 then
			joe_leave(player, npc)
		end
	elseif resp == 2 then
		joe_life_of_guard(player, npc)
	elseif resp == 3 then
		say(player, "Would you be interested in making a little more money?")
		npcsay(npc, "WHAT! are you trying to bribe me?")
		npcsay(npc, "I may not be a great guard, but I am loyal")
		npcsay(npc, "How DARE you try to bribe me!")
		say(player, "No,no, you got the wrong idea, totally")
		say(
			player,
			"I just wondered if you wanted some part-time bodyguard work"
		)
		npcsay(npc, "Oh. sorry. no, I don't need money")
		npcsay(npc, "As long as you were not offering me a bribe")

		resp = multi(
			player,
			"Tell me about the life of a guard",
			"What did you want to be when you were a boy",
			"I had better leave, I don't want trouble"
		)

		if resp == 1 then
			joe_life_of_guard(player, npc)
		elseif resp == 2 then
			joe_boy(player, npc)
		elseif resp == 3 then
			joe_leave(player.npc)
		end
	elseif resp == 4 then
		joe_leave(player, npc)
	end
end

joe_life_of_guard = function(player, npc)
	say(player, "Tell me about the life of a guard")
	npcsay(npc, "Well, the hours are good.....")
	npcsay(npc, ".... But most of those hours are a drag")
	npcsay(
		npc,
		"If only I had spent more time in Knight school when I was a young boy"
	)
	npcsay(npc, "Maybe I wouldn't be here now, scared of Keli")

	local resp = multi(
		player,
		"Hey, chill out, I won't cause you trouble",
		"What did you want to be when you were a boy",
		"I had better leave, I don't want trouble"
	)

	if resp == 1 then
		joe_chill_out(player, npc)
	elseif resp == 2 then
		joe_boy(player, npc)
	elseif resp == 3 then
		joe_leave(player, npc)
	end
end

joe_boy = function(player, npc)
	say(player, "What did you want to be when you were a boy")
	npcsay(npc, "Well, I loved to sit by the lake, with my toes in the water")
	npcsay(npc, "And shoot the fish with my bow and arrow")
	say(player, "That was a strange hobby for a little boy")
	npcsay(npc, "It kept us from goblin hunting, which was what most boys did")
	npcsay(npc, "What are you here for?")

	local resp = multi(
		player,
		"Chill out, I wont cause you trouble",
		"Tell me about the life of a guard",
		"I had better leave, I don't want trouble"
	)

	if resp == 1 then
		joe_chill_out(player, npc)
	elseif resp == 2 then
		joe_life_of_guard(player, npc)
	elseif resp == 3 then
		joe_leave(player, npc)
	end
end

function talknpc_joe(player, npc)
	local stage = getvar(player, "prince_ali_stage")

	if stage == 4 or stage == 5 then
		npcsay(npc, "Halt, who goes there? Friend or foe?")
		say(player, "Hi friend, I am just checking out things here")
		npcsay(npc, "The Prince got away, I am in trouble")
		npcsay(npc, "I better not talk to you, they are not sure I was drunk")
		say(player, "I won't say anything, your secret is safe with me")
	elseif getvar(player, "prince_ali_joe_drunk") ~= 0 then
		npcsay(npc, "Halt. Who goes there?")
		say(player, "Hello friend, I am just rescuing the prince, is that ok?")
		npcsay(npc, "Thatsh a funny joke. you are lucky I am shober")
		npcsay(npc, "Go in peace, friend")
	else
		local first_choice = "Hey, chill out, I wont cause you trouble"
		local offer_beer = stage == 3 and held(player, "beer", 1)

		if offer_beer then
			first_choice = "I have some beer here, fancy one?"
		else
			npcsay(npc, "Hi, I'm Joe, door guard for Lady Keli")
			say(player, "Hi, who are you guarding here?")
			npcsay(
				npc,
				"Can't say, all very secret. you should get out of here"
			)
			npcsay(npc, "I am not supposed to talk while I guard")
		end

		local resp = multi(
			player,
			first_choice,
			"Tell me about the life of a guard",
			"What did you want to be when you were a boy",
			"I had better leave, I don't want trouble"
		)

		if resp == 1 then
			if offer_beer then
				joe_beer(player, npc)
			else
				joe_chill_out(player, npc)
			end
		elseif resp == 2 then
			joe_life_of_guard(player, npc)
		elseif resp == 3 then
			joe_boy(player, npc)
		elseif resp == 4 then
			joe_leave(player, npc)
		end
	end
end
