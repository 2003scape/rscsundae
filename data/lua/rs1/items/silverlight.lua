function attacknpc_demon(player, npc)
	local applied = false

	if not worn(player, "silverlight") then
		return
	end

	-- claims max hit lowered by 2:
	-- https://classic.runescape.wiki/w/Lesser_Demon

	if not npcstatdown(npc, STAT_ATTACK) then
		npcsubstat(npc, STAT_ATTACK, 0, 15)
		applied = true
	end
	if not npcstatdown(npc, STAT_DEFENSE) then
		npcsubstat(npc, STAT_DEFENSE, 0, 15)
		applied = true
	end
	if not npcstatdown(npc, STAT_STRENGTH) then
		npcsubstat(npc, STAT_STRENGTH, 0, 15)
		applied = true
	end
	if applied then
		-- flying sno (redacted chat) replays/fsnom2@aol.com/07-30-2018 12.37.33
		mes(player, "As you strike the demon with silverlight he appears to weaken a lot")
	end
end
