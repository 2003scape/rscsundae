-- https://classic.runescape.wiki/w/Dragon_(Dragon_slayer)

function killnpc_questdragon(player, npc, x, y)
	-- TODO not sure if elvarg dropped anything here; may be inaccurate
	addobject(player, "big bones", 1, x, y)

	if getvar(player, "dragon_stage") == 3 then
		teleport(player, 410, 3481)
		mes(player, "Well done you have completed the dragon slayer quest")
		setvar(player, "dragon_stage", 4)
		setvar(player, "dragon_funchest", 0)
		setvar(player, "dragon_oraclechest", 0)
		giveqp(player, 2)
		advancestat(player, STAT_STRENGTH, 2600, 1200)
		advancestat(player, STAT_DEFENSE, 2600, 1200)
	end
end
