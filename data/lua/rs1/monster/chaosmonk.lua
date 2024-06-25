-- RSC 2001/replays master archive/NPC (monsters, combat)/Monk of Zamorak (level 19)/dialogue- monk of zamorak (lvl 19)- talk to- attack- attempt to take zamorak wine
-- RSC 2001/replays master archive/NPC (monsters, combat)/Monk of Zamorak (level 29)/dialogue- monk of zamorak (lvl 29)- talk to-
-- RSC 2001/10/monk of zamorak- curse

function talknpc_chaosmonk1(player, npc)
	npcsay(npc, "Who are you to dare speak to the servants of Zamorak ?")
	npcattack(npc, player)
end

function talknpc_chaosmonk2(player, npc)
	npcsay(npc, "Save your speech for the altar")
	npcattack(npc, player)
end

function attacknpc_chaosmonk1(player, npc)
	npcsay(npc, "A curse be upon you")
	mes(player, "You feel slightly weakened")
	substat(player, STAT_ATTACK, 2, 5)
	substat(player, STAT_STRENGTH, 2, 5)
	substat(player, STAT_DEFENSE, 2, 5)
	npcattack(npc, player)
end

function attacknpc_chaosmonk2(player, npc)
	attacknpc_chaosmonk1(player, npc)
end

function takeobj_half_full_wine_jug(player, x, y)
	local npc = nearnpc(player, "chaosmonk1")
	if npc then
		attacknpc_chaosmonk1(player, npc)
		return
	end

	npc = nearnpc(player, "chaosmonk2")
	if npc then
		attacknpc_chaosmonk2(player, npc)
		return
	end
end
