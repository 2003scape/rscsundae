-- https://classic.runescape.wiki/w/Transcript:Oziach

local function oziach_get_everything(player, npc)
	say(player, "Ok I'll try and get everything together")
	npcsay(npc, "Fare ye well")
end

local oziach_dragon

local function oziach_shield(player, npc)
	say(player, "Where can I get an antidragon shield?")
	npcsay(
		npc,
		"I believe the Duke of Lumbrige Castle may have one in his armoury"
	)

	local resp = multi(player,
		"So where can I find this dragon?",
		"Ok, I'll try and get everything together")

	if resp == 1 then
		oziach_dragon(player, npc)
	elseif resp == 2 then
		oziach_get_everything(player, npc)
	end
end

local oziach_first_map_piece, oziach_second_map_piece

local function oziach_third_map_piece(player, npc)
	say(player, "Where is the third piece of map?")
	npcsay(npc, "That was stolen by one of the goblins from the goblin village")

	local resp = multi(player,
		"Where can I get an antidragon shield?",
		"Where is the first piece of map?",
		"Where is the second piece of map?",
		"Ok, I'll try and get everything together")

	if resp == 1 then
		oziach_sheld(player, npc)
	elseif resp == 2 then
		oziach_first_map_piece(player, npc)
	elseif resp == 3 then
		oziach_second_map_piece(player, npc)
	elseif resp == 4 then
		oziach_get_everything(player, npc)
	end
end

oziach_second_map_piece = function(player, npc)
	say(player, "Where is the second piece of map?")
	npcsay(npc, "You will need to talk to the oracle on the ice mountain")

	local resp = multi(
		player,
		"Where can I get an antidragon shield?",
		"Where is the first piece of map?",
		"Where is the third piece of map?",
		"Ok, I'll try and get everything together"
	)

	if resp == 1 then
		oziach_sheld(player, npc)
	elseif resp == 2 then
		oziach_first_map_piece(player, npc)
	elseif resp == 3 then
		oziach_third_map_piece(player, npc)
	elseif resp == 4 then
		oziach_get_everything(player, npc)
	end
end

oziach_first_map_piece = function(player, npc)
	say(player, "Where is the first piece of map?")
	npcsay(npc, "deep in a strange building known as Melzar's maze")
	npcsay(npc, "Located north west of Rimmington")

	if not held(player, "melzarkey", 1) then
		npcsay(npc, "You will need this to get in")
		npcsay(npc, "This is the key to the front entrance to the maze")
		mes(player, "Oziach hands you a key")
		give(player, "melzarkey", 1)
	end

	local resp = multi(player,
		"Where can I get an antidragon shield?",
		"Where is the second piece of map?",
		"Where is the third piece of map?",
		"Ok. I'll try and get everything together")

	if resp == 1 then
		oziach_shield(player, npc)
	elseif resp == 2 then
		oziach_second_map_piece(player, npc)
	elseif resp == 3 then
		oziach_third_map_piece(player, npc)
	elseif resp == 4 then
		oziach_get_everything(player, npc)
	end
end

oziach_dragon = function(player, npc)
	say(player, "So where can I find this dragon?")
	npcsay(npc, "That is a problem too yes")
	npcsay(npc, "No one knows where the Isle of Crandor is located")
	npcsay(npc, "There was a map")
	npcsay(npc, "But it was torn up into three pieces")
	npcsay(npc, "Which are now scattered across Asgarnia")
	npcsay(npc, "You'll also struggle to find someone bold enough to take a ship to Crandor Island")

	local resp = multi(
		player,
		"Where is the first piece of map?",
		"Where is the second piece of map?",
		"Where is the third piece of map?",
		"Where can I get an antidragon shield?"
	)

	if resp == 1 then
		oziach_first_map_piece(player, npc)
	elseif resp == 2 then
		oziach_second_map_piece(player, npc)
	elseif resp == 3 then
		oziach_third_map_piece(player, npc)
	elseif resp == 4 then
		oziach_shield(player, npc)
	end
end

local function oziach_find(player, npc)
	local resp = multi(player,
		"So where can I find this dragon?",
		"Where can I get an antidragon shield?")

	if resp == 1 then
		oziach_dragon(player, npc)
	elseif resp == 2 then
		oziach_shield(player, npc)
	end
end

local function oziach_equipment(player, npc)
	npcsay(npc, "Elvarg really is one of the most powerful dragons")
	npcsay(npc, "I really wouldn't recommend charging in without special equipment")
	npcsay(npc, "Her breath is the main thing to watch out for")
	npcsay(npc, "You can get fried very fast")
	npcsay(npc, "Unless you have a special flameproof antidragon shield")
	npcsay(npc, "It won't totally protect you")
	npcsay(npc, "but it should prevent some of the damage to you")
	setvar(player, "dragon_stage", 2)
	oziach_find(player, npc)
end

local function oziach_prove(player, npc)
	local resp

	npcsay(npc, "This is armour fit for a hero to be sure")
	npcsay(npc, "So you'll need to prove to me that you're a hero before you can buy some")

	resp = multi(player,
		"So how am I meant to prove that?",
		"That's a pity, I'm not a hero")

	if resp == 1 then
		say(player, "So how am I meant to prove that?")
		npcsay(npc, "Well if you want to prove yourself")
		npcsay(npc, "You could try and defeat Elvarg the dragon of the Isle of Crandor")

		resp = multi(player,
			"A dragon, that sounds like fun",
			"And will i need anything to defeat this dragon",
			"I may be a champion, but I don't think I'm up to dragon killing yet")

		if resp == 1 then
			say(player, "A dragon, that sounds like fun")
			oziach_equipment(player, npc)
		elseif resp == 2 then
			say(player, "And will I need anything to defeat this dragon?")
			npcsay(npc, "It's funny you shoud say that")
			oziach_equipment(player, npc)
		elseif resp == 3 then
			say(player, "I may be a champion, but I don't think I'm up to dragon killing yet")
			npcsay(npc, "Yes I can understand that")
		end
	elseif resp == 2 then
		say(player, "That's a pity, I'm not a hero")
	end
end

function talknpc_oziach(player, npc)
	local stage = getvar(player, "dragon_stage")
	local resp

	if stage == 0 or stage == 1 then
		npcsay(npc, "Aye tiz a fair day my friend")

		local choices = { "I'm not your friend", "Yes it's a very nice day" }

		if stage == 1 then
			table.insert(choices, 1, "Can you sell me some rune plate mail?")
			resp = multi(player, table.unpack(choices))
		else
			resp = multi(player, table.unpack(choices)) + 1
		end

		if resp == 1 then
			say(player, "Can you sell me some rune plate mail?")
			npcsay(npc, "Soo how does thee know I'ave some?")

			resp = multi(player,
				"The guildmaster of the champion guild told me",
				"I am a master detective")

			if resp == 1 then
				say(player, "The guildmaster of the champion guild told me")
				npcsay(npc, "Well if you're worthy of his advise")
				npcsay(npc, "You must have something going for you")
				npcsay(npc, "He has been known to let some weeklin's into his guild though")
				npcsay(npc, "I don't want just any old pumpkinmush to have this armour")
				npcsay(npc, "jus cos they have a large amount of cash")
				oziach_prove(player, npc)
			elseif resp == 2 then
				say(player, "I am a master detective")
				npcsay(npc, "well however you found out about it")
				oziach_prove(player, npc)
			end
		elseif resp == 2 then
			say(player, "I'm not your friend")
			npcsay(npc,
			    "I'd be surprised if your anyone's friend with that sort of manners")
		elseif resp == 3 then
			say(player, "Yes it's a very nice day")
			npcsay(npc, "Aye may the Gods walk by your side")
		end
	elseif stage == 2 or stage == 3 then
		npcsay(npc, "So how is thy quest going?")
		oziach_find(player, npc)
	elseif stage == 4 then
		say(player, "I have slain the dragon")
		npcsay(npc, "Well done")

		resp = multi(player,
			"Can I buy a rune mail body now please?",
			"Thank you")

		if resp == 1 then
			say(player, "Can I buy a rune mail body now please?")
			openshop(player, "runeplateshop")
		elseif resp == 2 then
			say(player, "Thank you")
		end
	end
end
