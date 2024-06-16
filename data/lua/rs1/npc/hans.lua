-- RSC 2001/replays master archive/NPC (dialogue, notes)/Hans/dialogue- hans- all

function talknpc_hans(player, npc)
	npcsay(npc, "Hello what are you doing here?")
	local resp = multi(player,
		"I'm looking for whoever is in charge of this place",
		"I have come to kill everyone in this castle",
		"I don't know. I'm lost. Where am I?")
	if resp == 1 then
		say(player, "I'm looking for whoever is in charge of this place")
		npcsay(npc, "Sorry, I don't know where he is right now")
	elseif resp == 2 then
		say(player, "I have come to kill everyone in this castle")
		npcsay(npc, "HELP HELP!")
	elseif resp == 3 then
		say(player, "I don't know. I'm lost. Where am I?")
		npcsay(npc, "You are in Lumbridge Castle")
	end
end
