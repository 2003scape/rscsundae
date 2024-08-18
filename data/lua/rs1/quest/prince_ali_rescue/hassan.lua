-- https://classic.runescape.wiki/w/Transcript:Hassan

function talknpc_hassan(player, npc)
	local stage = getvar(player, "prince_ali_stage")
	local key_paid = getvar(player, "prince_ali_key_paid")
	local resp

	if stage == 0 then
		npcsay(
			npc,
			"Greetings. I am Hassan, Chancellor to the Emir of Al Kharid"
		)

		resp = multi(
			player,
			"Can I help you? You must need some help here in the desert.",
			"Its just too hot here. How can you stand it?",
			"Do you mind if I just kill your Warriors?"
		)

		if resp == 1 then
			say(
				player,
				"Can I help you? You must need some help here in the desert"
			)
			npcsay(npc, "I need the services of someone, yes.")
			npcsay(npc, "If you are interested, see the spymaster, Osman")
			npcsay(
				npc,
				"I manage the finances here. come to me when you need payment"
			)
			setvar(player, "prince_ali_stage", 1)
		elseif resp == 2 then
			say(player, "Its just too hot here. How can you stand it?")
			npcsay(npc, "We manage, in our humble way. We are a wealthy town")
			npcsay(npc, "And we have water. It cures many thirsts")
			mes(player, "The chancellor hands you some water")
			give(player, "waterbucket", 1)
		elseif resp == 3 then
			say(player, "Do you mind if I just kill your warriors?")
			npcsay(npc, "You are welcome. They are not expensive.")
			npcsay(
				npc,
				"We have them here to stop the elite guard being bothered"
			)
			npcsay(npc, "They are a little harder to kill.")
		end
	elseif stage == 1 then
		npcsay(npc, "Have you found the spymaster, Osman, Yet?")
		npcsay(npc, "You cannot proceed in your task without reporting to him")
	elseif stage == 2 or stage == 3 then
		local key_made = getvar(player, "prince_ali_key_made")
		local key_received = getvar(player, "prince_ali_key_received")

		if key_made ~= 0 or key_received ~= 0 then
			if key_paid ~= 0 then
				npcsay(npc, "Hello again adventurer")
				npcsay(npc, "You have received payment for your tasks so far")
				npcsay(npc, "No more will be paid until the Prince is rescued")
			else
				npcsay(npc, "You have proved your services useful to us")
				npcsay(
					npc,
					"Here is 80 coins for the work you have already done"
				)
				give(player, "coins", 80)
				setvar(player, "prince_ali_key_paid", 1)
			end
		else
			npcsay(npc, "I understand the Spymaster has hired you")
			npcsay(npc, "I will pay the reward only when the Prince is rescued")
			npcsay(
				npc,
				"I can pay some expenses once the spymaster approves it"
			)
		end
	elseif stage == 4 then
		npcsay(
			npc,
			"You have the eternal gratitude of the Emir for rescuing his son"
		)
		npcsay(npc, "I am authorised to pay you 700 coins")

		if key_paid ~= 0 then
			npcsay(npc, "80 was put aside for the key. that leaves 620")
			give(player, "coins", 620)
			mes(player, "The chancellor pays you 620 coins")
		else
			give(player, "coins", 700)
			mes(player, "The chancellor pays you 700 coins")
		end

		mes(player, "You have completed the quest of the Prince of Al Kharid")

		setvar(player, "prince_ali_key_paid", 0)
		setvar(player, "prince_ali_key_made", 0)
		setvar(player, "prince_ali_key_received", 0)
		setvar(player, "prince_ali_keli_tied_once", 0)
		setvar(player, "prince_ali_joe_drunk", 0)

		setvar(player, "prince_ali_stage", 5)
		giveqp(player, 3)
	elseif stage == 5 then
		npcsay(npc, "You are a friend of the town of Al Kharid")
		npcsay(
			npc,
			"Please, keep in contact. Good employees are not easy to find"
		)
	end
end
