-- https://classic.runescape.wiki/w/Transcript:Professor_Oddenstein
-- https://classic.runescape.wiki/w/Transcript:Ernest

local function oddenstein_change_him_back(player, npc)
	npcsay(npc, "Um it's not so easy")
	npcsay(npc, "My machine is broken")
	npcsay(npc, "And the house gremlins")
	npcsay(npc, "Have run off with some vital bits")

	say(player, "Well I can look out for them")

	npcsay(npc, "That would be a help")
	npcsay(npc, "They'll be somewhere in the manor house or its grounds")
	npcsay(npc, "The gremlins never go further than the entrance gate")
	npcsay(npc, "I'm missing the pressure gauge and a rubber tube")
	npcsay(npc, "They've also taken my oil can")
	npcsay(npc, "Which I'm going to need to get this thing started again")

	setvar(player, "ernest_stage", 2)
end

function talknpc_professor_oddenstein(player, npc)
	local stage = getvar(player, "ernest_stage")
	local resp

	if stage == 2 then
		npcsay(npc, "Have you found anything yet?")

		local found_items = 0

		local has_oil = false
		local has_rubber_tube = false
		local has_pressure_gauge = false

		if held(player, "oil can", 1) then
			found_items = found_items + 1
			has_oil = true
		end

		if held(player, "rubber tube", 1) then
			found_items = found_items + 1
			has_rubber_tube = true
		end

		if held(player, "pressure gauge", 1) then
			found_items = found_items + 1
			has_pressure_gauge = true
		end

		if found_items == 3 then
			say(player, "I have everything")

			npcsay(npc, "Give em here then")

			mes(player, "You give a rubber tube, a pressure gauge and a can of oil to the Professer")

			delay(3)

			remove(player, "rubber tube", 1)
			remove(player, "pressure gauge", 1)
			remove(player, "oil can", 1)

			mes(player, "Oddenstein starts up the machine")
			delay(3)
			mes(player, "The machine hums and shakes")
			delay(3)
			mes(player, "Suddenly a ray shoots out of the machine at the chicken")
			delay(3)

			npcunbusy(npc)

			local ernest = nearnpc(player, "questchicken")

			if ernest == nil then
				ernest = nearnpc(player, "ernest")
			else
				ernest = changenpc(ernest, "ernest")
			end

			npcbusy(ernest)

			npcsay(ernest, "Thank you sir")
			npcsay(ernest, "It was dreadfully irritating being a chicken")
			npcsay(ernest, "How can I ever thank you?")

			say(player, "Well a cash reward is always nice")

			npcsay(ernest, "Of course, of course")
			mes(player, "Ernest hands you 300 coins")

			delay(3)

			give(player, "coins", 300)
			mes(player, "Well done. You have completed the Ernest the chicken quest")
			setvar(player, "ernest_killed_piranhas", 0)
			setvar(player, "ernest_stage", 3)
			giveqp(player, 4)

			npcunbusy(ernest)

			pause(85, 85)

			changenpc(ernest, "questchicken")
		elseif found_items > 0 then
			say(player, "I have found some of the things you need:")

			if has_oil then
				say(player, "I have a can of oil")
			end

			if has_pressure_gauge then
				say(player, "I have a pressure gauge")
			end

			if has_rubber_tube then
				say(player, "I have a rubber tube")
			end

			npcsay(npc, "Well that's a start")
			npcsay(npc, "You still need to find")

			if not has_oil then
				npcsay(npc, "A can of oil")
			end

			if not has_rubber_tube then
				npcsay(npc, "A rubber tube")
			end

			if not has_pressure_gauge then
				npcsay(npc, "A Pressure Gauge")
			end

			say(player, "Ok I'll try and find them")
		else
			say(player, "I'm afraid I don't have any yet!")

			npcsay(npc, "I need a rubber tube, a pressure gauge and a can of oil")
			npcsay(npc, "Then your friend can stop being a chicken")
		end
	else
		npcsay(npc, "Be careful in here")
		npcsay(npc, "Lots of dangerous equipment in here")

		if stage == 1 then
			resp = multi(player,
				"I'm looking for a guy called Ernest",
				"What do this machine do?", -- sic - only for this stage
				"Is this your house?")
		else
			resp = multi(player,
				"What does this machine do?",
				"Is this your house?")

			resp = resp + 1
		end

		if resp == 1 then
			say(player, "I'm looking for a guy called Ernest")

			npcsay(npc, "Ah Ernest, top notch bloke")
			npcsay(npc, "He's helping me with my experiments")

			say(player, "So you know where he is then?")

			npcsay(npc, "He's that chicken over there")

			say(player, "Ernest is a chicken?")
			say(player, "Are you sure?")

			npcsay(npc, "Oh he isn't normally a chicken")
			npcsay(npc, "Or at least he wasn't")
			npcsay(npc, "Until he helped me test my pouletmorph machine")
			npcsay(npc, "It was originally going to be called a transmutation machine")
			npcsay(npc, "But after testing Pouletmorph seems more appropriate")

			resp = multi(player,
				"I'm glad Veronica didn't actually get engaged to a chicken",
				"Change him back this instant")

			if resp == 1 then
				say(player, "I'm glad Veronica didn't actually get engaged to a chicken")

				npcsay(npc, "Who's Veronica?")

				say(player, "Ernest's fiancee")
				say(player, "She probably doesn't want to marry a chicken")

				npcsay(npc, "Ooh I dunno")
				npcsay(npc, "She could have free eggs for breakfast")

				say(player, "I think you'd better change him back")

				oddenstein_change_him_back(player, npc)
			elseif resp == 2 then
				say(player, "Change him back this instant")

				oddenstein_change_him_back(player, npc)
			end
		elseif resp == 2 then
			say(player, "What does this machine do?")

			npcsay(npc, "Nothing at the moment")
			npcsay(npc, "As it's broken")
			npcsay(npc, "It's meant to be a transmutation machine")
			npcsay(npc, "It has also spent time as a time travel machine")
			npcsay(npc, "And a dramatic lightning generator")
			npcsay(npc, "And a thing for generating monsters")
		elseif resp == 3 then
			say(player, "Is this your house?")

			npcsay(npc, "No, I'm just one of the tenants")
			npcsay(npc, "It belongs to the count")
			npcsay(npc, "Who lives in the basement")
		end
	end
end
