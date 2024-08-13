-- https://classic.runescape.wiki/w/Transcript:Veronica

function talknpc_veronica(player, npc)
	local stage = getvar(player, "ernest_stage")

	local resp

	if stage == 0 then
		npcsay(npc, "Can you please help me?")
		npcsay(npc, "I'm in a terrible spot of trouble")

		resp = multi(
			player,
			"Aha, sounds like a quest. I'll help",
			"No, I'm looking for something to kill"
		)

		if resp == 1 then
			say(player, "Aha, sounds like a quest")
			say(player, "I'll help")

			npcsay(npc, "Yes yes I suppose it is a quest")
			npcsay(npc, "My fiance Ernest and I came upon this house here")
			npcsay(npc, "Seeing as we were a little lost")
			npcsay(npc, "Ernest decided to go in and ask for directions")
			npcsay(npc, "That was an hour ago")
			npcsay(npc, "That house looks very spooky")
			npcsay(npc, "Can you go and see if you can find him for me?")

			say(player, "Ok, I'll see what I can do")

			npcsay(npc, "Thank you, thank you")
			npcsay(npc, "I'm very grateful")

			setvar(player, "ernest_stage", 1)
		elseif resp == 2 then
			say(player, "No, I'm looking for something to kill")
			npcsay(npc, "Oooh you violent person you")
		end
	elseif stage == 1 then
		npcsay(npc, "Have you found my sweetheart yet?")
		say(player, "No, not yet")
	elseif stage == 2 then
		npcsay(npc, "Have you found my sweetheart yet?")

		say(player, "Yes, he's a chicken")

		npcsay(npc, "I know he's not exactly brave")
		npcsay(npc, "But I think you're being a little harsh")

		say(player, "No no he's been turned into an actual chicken")
		say(player, "By a mad scientist")

		mes(player, "Veronica lets out an ear piercing shreek")
		delay(3)

		npcsay(npc, "Eeeeek")
		npcsay(npc, "My poor darling")
		npcsay(npc, "Why must these things happen to us?")

		say(player, "Well I'm doing my best to turn him back")

		npcsay(npc, "Well be quick")
		npcsay(npc, "I'm sure being a chicken can't be good for him")
	elseif stage == 3 then
		npcsay(npc, "Thank you for rescuing Ernest")
		say(player, "Where is he now?")
		npcsay(npc, "Oh he went off to talk to some green warty guy")
		npcsay(npc, "I'm sure he'll be back soon")
	end
end
