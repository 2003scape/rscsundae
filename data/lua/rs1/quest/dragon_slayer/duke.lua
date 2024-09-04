-- https://classic.runescape.wiki/w/Transcript:Duke_of_Lumbridge

function talknpc_duke_of_lumbridge(player, npc)
	local stage = getvar(player, "dragon_stage")

	local seek_shield = stage >= 2
		and not held(player, "anti dragon breath shield", 1)

	npcsay(npc, "Greetings welcome to my castle")

	local choices = {
		"Have you any quests for me?",
		"Where can I find money?",
	}

	if seek_shield then
		table.insert(
			choices,
			1,
			"I seek a shield that will protect me from dragon breath"
		)
	end

	local resp = multi(player, table.unpack(choices))

	if not seek_shield then
		resp = resp + 1
	end

	if resp == 1 then
		say(player, "I seek a shield that will protect me from dragon breath")
		npcsay(npc, "A knight going on a dragon quest hmm?")
		npcsay(npc, "A most worthy cause")
		npcsay(npc, "Guard this well my friend")
		mes(player, "The duke hands you a shield")
		give(player, "anti dragon breath shield", 1)
	elseif resp == 2 then
		say(player, "Have you any quests for me?")
		npcsay(npc, "All is well for me")
	elseif resp == 3 then
		say(player, "Where can I find money?")
		npcsay(
			npc,
			"I've heard the blacksmiths are prosperous amoung the peasantry"
		)
		npcsay(npc, "Maybe you could try your hand at that")
	end
end
