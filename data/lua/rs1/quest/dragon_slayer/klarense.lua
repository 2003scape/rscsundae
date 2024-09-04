-- https://classic.runescape.wiki/w/Transcript:Klarense

function talknpc_klarense(player, npc)
	local stage = getvar(player, "dragon_stage")
	local owns_ship = getvar(player, "dragon_owns_ship") ~= 0
	local resp

	if owns_ship then
		resp = multi(
			player,
			"So would you like to sail this ship to Crandor Isle for me?",
			"So what needs fixing on this ship?",
			"What are you going to do now you don't have a ship?"
		)

		if resp == 1 then
			say(
				player,
				"So would you like to sail this ship to Crandor Isle for me?"
			)
			npcsay(npc, "No not me, I'm frightened of dragons")
		elseif resp == 2 then
			say(player, "So what needs fixing on this ship?")
			npcsay(
				npc,
				"Well the big gaping hole in the hold is the main problem"
			)
			npcsay(npc, "you'll need a few planks")
			npcsay(npc, "Hammered in with steel nails")
		elseif resp == 3 then
			say(player, "What are you going to do now you don't have a ship?")
			npcsay(npc, "Oh I'll be fine")
			npcsay(npc, "I've got work as Port Sarim's first life guard")
		end
	elseif stage <= 2 then
		npcsay(
			npc,
			"You're interested in a trip on the Lumbridge Lady are you?"
		)
		npcsay(npc, "I admit she looks fine, but she isn't seaworthy right now")

		local choices = {
			"Do you know when she will be seaworthy",
			"Ah well, nevermind",
		}

		if stage == 2 and not owns_ship then
			table.insert(
				choices,
				2,
				"Would you take me to Crandor Isle when it's ready?"
			)
			table.insert(choices, 3, "I don't suppose I could buy it")

			resp = multi(player, table.unpack(choices))
		else
			resp = multi(player, table.unpack(choices))

			if resp > 1 then
				resp = 4
			end
		end

		if resp == 1 then
			say(player, "Do you know when she will be seaworthy")
			npcsay(npc, "No not really")
			npcsay(npc, "Port Sarim's shipbuilders aren't very efficient")
			npcsay(npc, "So it could be quite a while")
		elseif resp == 2 then
			say(player, "Would you take me to Crandor Isle when it's ready?")
			npcsay(npc, "Well even if I knew how to get there")
			npcsay(npc, "I wouldn't like to risk it")
			npcsay(
				npc,
				"Especially after to goin to all the effort of fixing the "
					.. "old girl up"
			)
		elseif resp == 3 then
			say(player, "I don't suppose I could buy it")
			npcsay(
				npc,
				"I'm sure the work needed to do on it wouldn't be too expensive"
			)
			npcsay(npc, "How does 2000 gold sound for a price?")

			resp = multi(
				player,
				"Yep sounds good",
				"I'm not paying that much for a broken boat"
			)

			if resp == 1 then
				say(player, "Yep sounds good")

				if held(player, "coins", 2000) then
					remove(player, "coins", 2000)
					setvar(player, "dragon_owns_ship", 1)
					npcsay(npc, "Ok she's all yours")
				else
					say(player, "Except I don't have that much money on me")
				end
			elseif resp == 2 then
				say(player, "I'm not paying that much for a broken boat")
				npcsay(
					npc,
					"That's Ok, I didn't particularly want to sell anyway"
				)
			end
		elseif resp == 4 then
			say(player, "Ah well never mind")
		end
	end
end
