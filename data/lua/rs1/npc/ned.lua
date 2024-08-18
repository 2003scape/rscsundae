-- https://classic.runescape.wiki/w/Transcript:Ned

function talknpc_ned(player, npc)
	local resp

	npcsay(npc, "Why hello there, me friends call me Ned")
	npcsay(npc, "I was a man of the sea, but its past me now")
	npcsay(npc, "Could I be making or selling you some Rope?")

	local prince_ali_stage = getvar(player, "prince_ali_stage")

	if prince_ali_stage == 2 or prince_ali_stage == 3 then
		resp = multi(
			player,
			"Yes, I would like some Rope",
			"No thanks Ned, I don't need any",
			"Ned, could you make other things from wool?"
		)
	else
		resp = multi(
			player,
			"Yes, I would like some Rope",
			"No thanks Ned, I don't need any"
		)
	end

	if resp == 1 then
		say(player, "Yes, I would like some Rope")
		npcsay(npc, "Well, I can sell you some rope for 15 coins")
		npcsay(
			npc,
			"Or I can be making you some if you gets me 4 balls of wool"
		)
		npcsay(npc, "I strands them together I does, makes em strong")

		local has_wool = held(player, "ball of wool", 4)
		local wool_choice = "I will go and get some wool"

		if has_wool then
			wool_choice =
				"I have some balls of wool. could you make me some Rope?"
		end

		resp = multi(
			player,
			"Okay, please sell me some Rope",
			"Thats a little more than I want to pay",
			wool_choice
		)

		if resp == 1 then
			if held(player, "coins", 15) then
				say(player, "Okay, please sell me some Rope")
				remove(player, "coins", 15)
				mes(player, "You hand Ned 15 coins")
				npcsay(npc, "There you go, finest rope in Runescape")
				mes(player, "Ned gives you a coil of rope")
				give(player, "rope", 1)
			else
				mes(player, "You Don't have enough coins to buy any rope!")
			end
		elseif resp == 2 then
			say(player, "Thats a little more than I want to pay")
			npcsay(npc, "Well, if you ever need rope. thats the price. sorry")
			npcsay(npc, "An old sailor needs money for a little drop o rum.")
		elseif resp == 3 then
			say(player, wool_choice)

			if has_wool then
				npcsay(npc, "Sure I can.")
				remove(player, "ball of wool", 4)
				give(player, "rope", 1)
			else
				npcsay(npc, "Aye, you do that")
				npcsay(
					npc,
					"Remember, it takes 4 balls of wool to make strong rope"
				)
			end
		end
	elseif resp == 2 then
		say(player, "No thanks Ned, I don't need any")
		npcsay(npc, "Well, old Neddy is always here if you do")
		npcsay(npc, "Tell your friends, I can always be using the business")
	elseif resp == 3 then
		-- defined in ../quest/prince_ali_rescue/ned.lua
		ned_other_things(player, npc)
	end
end
