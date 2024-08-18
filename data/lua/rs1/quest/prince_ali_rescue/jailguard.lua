-- https://classic.runescape.wiki/w/Transcript:Jailguard

function talknpc_jailguard(player, npc)
	say(player, "Hi, who are you guarding here?")
	npcsay(npc, "Can't say, all very secret. you should get out of here")
	npcsay(npc, "I am not supposed to talk while I guard")

	local resp = multi(
		player,
		"Hey, chill out, I wont cause you trouble",
		"I had better leave, I don't want trouble"
	)

	if resp == 1 then
		say(player, "Hey, chill out, I wont cause you trouble")
		say(player, "I was just wondering what you do to relax")
		npcsay(
			npc,
			"You never relax with these people, but its a good career for a "
				.. "young man"
		)
	elseif resp == 2 then
		say(player, "I had better leave, I don't want trouble")
		npcsay(npc, "Thanks I appreciate that")
		npcsay(
			npc,
			"Talking on duty can be punishable by having your mouth stiched up"
		)
		npcsay(npc, "These are tough people, no mistake")
	end
end
