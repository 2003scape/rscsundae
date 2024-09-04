-- https://classic.runescape.wiki/w/Transcript:Ned

function ned_take_me_to_crandor(player, npc)
	say(player, "You're a sailor? Could you take me to the Isle of Crandor")
	npcsay(npc, "Well I was a sailor")
	npcsay(npc, "I've not been able to get work at sea these days though")
	npcsay(npc, "They say I am too old")
	mes(player, "There is a wistfull look in Ned's eyes")
	delay(3)
	npcsay(npc, "I miss those days")
	npcsay(npc, "If you could get me a ship I would take you anywhere")

	if getvar(player, "dragon_ship_fix_stage") == 3 then
		say(player, "As it happens I do have a ship ready to sail")
		npcsay(npc, "That'd be grand, where is it")
		say(
			player,
			"It's called the Lumbrige Lady and it's docked in Port Sarim"
		)
		npcsay(npc, "I'll go right over there and check her out then")
		npcsay(npc, "See you over there")
		setvar(player, "dragon_ned_in_ship", 1)
	else
		say(player, "I will work on finding a sea worthy ship then")
	end
end

local function crandor_voyage(player, npc)
	mes(player, "You feel the ship begin to move")
	delay(3)
	mes(player, "You are out at sea")
	delay(3)
	mes(player, "The ship is sailing")
	delay(3)
	mes(player, "The ship is sailing")
	delay(3)
	mes(player, "You feel a crunch")
	delay(3)
	npcsay(npc, "Aha we've arrived")
	teleport(player, 281, 3472)
	setvar(player, "dragon_ship_crandor", 1)
	setvar(player, "dragon_ship_fix_stage", 0)
end

-- ned in lumbridge lady
function talknpc_ned2(player, npc)
	local resp

	if getvar(player, "dragon_ship_crandor") ~= 0 then
		resp = multi(
			player,
			"Is the ship ready to sail back?",
			"So are you enjoying this exotic island vacation?"
		)

		if resp == 1 then
			say(player, "Is the ship ready to sail back?")
			npcsay(
				npc,
				"Well when we arrived the ship took a nasty jar from those "
					.. "rocks"
			)
			npcsay(npc, "We may be stranded")
		elseif resp == 2 then
			say(player, "So are you enjoying this exotic island vacation?")
			npcsay(
				npc,
				"Well it would have been better if I'd brought my sun lotion"
			)
			npcsay(
				npc,
				"Oh and the skeletons which won't let me leave the ship"
			)
			npcsay(npc, "Probably aren't helping either")
		end
	else
		npcsay(npc, "Hello again " .. (male(player) and "lad" or "lass"))

		if getvar(player, "dragon_stage") == 2 then
			resp = multi(
				player,
				"So are you going to take me to Crandor Island now then?",
				"So are you still up to sailing this ship?"
			)

			if resp == 1 then
				say(
					player,
					"So are you going to take me to Crandor Island now then?"
				)
				npcsay(npc, "Okay show me the map and we'll set sail now")

				local has_map = false

				if held(player, "map", 1) then
					has_map = true

					mes(player, "You give the map to ned")
					remove(player, "map", 1)
				elseif
					held(player, "mappart1", 1)
					and held(player, "mappart2", 1)
					and held(player, "mappart3", 1)
				then
					has_map = true

					mes(player, "You give the parts of the map to ned")
					remove(player, "mappart1", 1)
					remove(player, "mappart2", 1)
					remove(player, "mappart3", 1)
				end

				if has_map then
					say(player, "Here it is")
					crandor_voyage(player, npc)
					setvar(player, "dragon_stage", 3)
				end
			elseif resp == 2 then
				say(player, "So are you still up to sailing this ship?")
				npcsay(npc, "Well I am a tad rusty")
				npcsay(
					npc,
					"I'm sure it'll all come back to me, once I get into action"
				)
				npcsay(npc, "I hope...")
			end
		else
			resp = multi(
				player,
				"Can you take me back to Crandor again",
				"How did you get back?"
			)

			if resp == 1 then
				say(player, "Can you take me back to Crandor again?")

				if getvar(player, "dragon_ship_fix_stage") == 3 then
					npcsay(npc, "Okie Dokie")
					crandor_voyage(player, npc)
				else
					npcsay(npc, "Well I would, but the last adventure")
					npcsay(npc, "Hasn't left this tub in the best of shapes")
					npcsay(npc, "You'll have to fix it again")
				end
			elseif resp == 2 then
				say(player, "How did you get back?")
				npcsay(npc, "I got towed back by a passing friendly whale")
			end
		end
	end
end
