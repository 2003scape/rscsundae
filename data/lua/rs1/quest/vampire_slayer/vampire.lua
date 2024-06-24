-- TODO
function npcattack_vampire(player, npc, x, y)
	if held(player, "garlic", 1) then
		mes(player, "The vampire appears to weaken")
		npcsubstat(npc, STAT_ATTACK, 1, 50)
		npcsubstat(npc, STAT_DEFENSE, 1, 50)
		npcsubstat(npc, STAT_STRENGTH, 1, 50)
	end
end

function killnpc_vampire(player, npc, x, y)
	local stage = getvar(player, "vampire_stage")

	if worn(player, "stake", 1) and held(player, "hammer", 1) then
		remove(player, "stake", 1)
		mes(player, "You hammer the stake in to the vampires chest!")

		if stage == 1 then
			mes(player, "Well done you have completed the vampire slayer quest")
			advancestat(player, STAT_ATTACK, 1300, 600)
			setvar(player, "vampire_stage", 2)
			giveqp(player, 3)
		end
	else
		mes(player, "The vampire seems to regenerate")
		npchealstat(npc, STAT_HITS, 0, 100)
	end
end
