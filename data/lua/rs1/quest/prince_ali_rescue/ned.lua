-- https://classic.runescape.wiki/w/Transcript:Ned#During_Prince_Ali_Rescue

function ned_other_things(player, npc)
	local resp

	say(player, "Ned, could you make other things from wool?")
	npcsay(npc, "I am sure I can. What are you thinking of?")

	resp = multi(
		player,
		"Could you knit me a sweater?",
		"How about some sort of a wig?",
		"Could you repair the arrow holes in the back of my shirt?"
	)

	if resp == 1 then
		say(player, "Could you knit me a sweater?")
		npcsay(npc, "Do I look like a member of a sewing circle?")
		npcsay(
			npc,
			"Be off wi' you, I have fought monsters that would turn your "
				.. "hair blue"
		)
		npcsay(
			npc,
			"I don't need to be laughed at just 'cos I am getting a bit old"
		)
	elseif resp == 2 then
		say(player, "How about some sort of a wig?")
		npcsay(npc, "Well... Thats an interesting thought")
		npcsay(npc, "yes, I think I could do something")
		npcsay(npc, "Give me 3 balls of wool and I might be able to do it")

		if held(player, "ball of wool", 3) then
			resp = multi(
				player,
				"I have that now. Please, make me a wig",
				"I will come back when I need you to make me one"
			)

			if resp == 1 then
				say(player, "I have that now. Please, make me a wig")
				npcsay(npc, "Okay, I will have a go.")
				mes(player, "You hand Ned 3 balls of wool")
				remove(player, "ball of wool", 3)
				mes(
					player,
					"Ned works with the wool. His hands move with a speed "
						.. "you couldn't imagine"
				)
				delay(3)
				npcsay(
					npc,
					"Here you go, hows that for a quick effort? Not bad I "
						.. "think!"
				)
				mes(player, "Ned gives you a pretty good wig")
				give(player, "plainwig", 1)
				say(player, "Thanks Ned, theres more to you than meets the eye")
			elseif resp == 2 then
				say(player, "I will come back when I need you to make me one")
				npcsay(npc, "Well, it sounds like a challenge")
				npcsay(npc, "come to me if you need one")
			end
		else
			say(player, "great, I will get some. I think a wig would be useful")
		end
	elseif resp == 3 then
		say(player, "Could you repair the arrow holes in the back of my shirt?")
		npcsay(npc, "Ah yes, its a tough world these days")
		npcsay(npc, "Theres a few brave enough to attack from 10 metres away")
		mes(player, "Ned pulls out a needle and attacks your shirt")
		delay(2) -- seemed faster than 3 but not instant either
		npcsay(npc, "There you go, good as new")
		say(
			player,
			"Thanks Ned, maybe next time they will attack me face to face"
		)
	end
end
