-- https://www.youtube.com/watch?t=1246&v=KPJYewzuHI8

function wartface_wormbrain(player, npc)
	local resp = multi(
		player,
		"I've heard one of your goblins has got a hold of part of a map",
		"So how is life for the goblins?"
	)

	if resp == 1 then
		say(
			player,
			"I've heard one of your goblins has got a hold of part of a map"
		)
		npcsay(npc, "Aha that'd be Wormbrain")
		say(player, "Where would he be")
		npcsay(npc, "Wormbrain he steals to much")
		npcsay(npc, "He got caught")
		npcsay(npc, "Now he lives in Port Sarim town jail")
		return true
	elseif resp == 2 then
        say(player, "So how is life for the goblins?")
		return false
	end
end
