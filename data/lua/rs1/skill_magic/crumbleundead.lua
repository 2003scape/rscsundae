function skillnpc_crumble_undead(player, target)
	mes(player, "@que@Cast spell successfully")
	advancestat(player, STAT_MAGIC, 392, 0)
	if not worn(player, "staff of air") then
		remove(player, "air-rune", 2)
	end
	if not worn(player, "staff of earth") then
		remove(player, "earth-rune", 2)
	end
	remove(player, "chaos-rune", 1)
	shootnpc(player, target, "missile12")
end

register_skillnpc("skeleton", "crumble undead", skillnpc_crumble_undead)
register_skillnpc("zombie", "crumble undead", skillnpc_crumble_undead)
register_skillnpc("ghost", "crumble undead", skillnpc_crumble_undead)
register_skillnpc("_", "crumble undead", function(player, target)
	-- ./rscplus-replays-master/kRiStOf/Salarin The Twisted
	mes(player, "@que@This spell can only be used on skeletons, zombies and ghosts")
end)
