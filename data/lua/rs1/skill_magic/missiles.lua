-- 07-11-2018 16.12.51 autocast magic on some guards for 2 hours.pcap

function finalize_wind_strike(player)
	mes(player, "@que@Cast spell successfully")
	advancestat(player, STAT_MAGIC, 88, 0)
	if not worn(player, "staff of air") then
		remove(player, "air-rune", 1)
	end
	remove(player, "mind-rune", 1)
end

function finalize_water_strike(player)
	mes(player, "@que@Cast spell successfully")
	advancestat(player, STAT_MAGIC, 120, 0)
	if not worn(player, "staff of air") then
		remove(player, "air-rune", 1)
	end
	if not worn(player, "staff of water") then
		remove(player, "water-rune", 1)
	end
	remove(player, "mind-rune", 1)
end

function finalize_earth_strike(player)
	mes(player, "@que@Cast spell successfully")
	advancestat(player, STAT_MAGIC, 152, 0)
	if not worn(player, "staff of air") then
		remove(player, "air-rune", 1)
	end
	if not worn(player, "staff of earth") then
		remove(player, "earth-rune", 2)
	end
	remove(player, "mind-rune", 1)
end

function finalize_fire_strike(player)
	mes(player, "@que@Cast spell successfully")
	advancestat(player, STAT_MAGIC, 184, 0)
	if not worn(player, "staff of air") then
		remove(player, "air-rune", 2)
	end
	if not worn(player, "staff of fire") then
		remove(player, "fire-rune", 3)
	end
	remove(player, "mind-rune", 1)
end

function finalize_wind_bolt(player)
	mes(player, "@que@Cast spell successfully")
	advancestat(player, STAT_MAGIC, 216, 0)
	if not worn(player, "staff of air") then
		remove(player, "air-rune", 2)
	end
	remove(player, "chaos-rune", 1)
end

function finalize_water_bolt(player)
	mes(player, "@que@Cast spell successfully")
	advancestat(player, STAT_MAGIC, 264, 0)
	if not worn(player, "staff of air") then
		remove(player, "air-rune", 2)
	end
	if not worn(player, "staff of water") then
		remove(player, "water-rune", 2)
	end
	remove(player, "chaos-rune", 1)
end

function finalize_earth_bolt(player)
	mes(player, "@que@Cast spell successfully")
	advancestat(player, STAT_MAGIC, 312, 0)
	if not worn(player, "staff of air") then
		remove(player, "air-rune", 2)
	end
	if not worn(player, "staff of earth") then
		remove(player, "earth-rune", 3)
	end
	remove(player, "chaos-rune", 1)
end

function finalize_fire_bolt(player)
	mes(player, "@que@Cast spell successfully")
	advancestat(player, STAT_MAGIC, 360, 0)
	if not worn(player, "staff of air") then
		remove(player, "air-rune", 3)
	end
	if not worn(player, "staff of fire") then
		remove(player, "fire-rune", 4)
	end
	remove(player, "chaos-rune", 1)
end

function finalize_wind_blast(player)
	mes(player, "@que@Cast spell successfully")
	advancestat(player, STAT_MAGIC, 408, 0)
	if not worn(player, "staff of air") then
		remove(player, "air-rune", 3)
	end
	remove(player, "death-rune", 1)
end

function finalize_water_blast(player)
	mes(player, "@que@Cast spell successfully")
	advancestat(player, STAT_MAGIC, 456, 0)
	if not worn(player, "staff of air") then
		remove(player, "air-rune", 3)
	end
	if not worn(player, "staff of water") then
		remove(player, "water-rune", 3)
	end
	remove(player, "death-rune", 1)
end

function finalize_earth_blast(player)
	mes(player, "@que@Cast spell successfully")
	advancestat(player, STAT_MAGIC, 504, 0)
	if not worn(player, "staff of air") then
		remove(player, "air-rune", 3)
	end
	if not worn(player, "staff of earth") then
		remove(player, "earth-rune", 4)
	end
	remove(player, "death-rune", 1)
end

function finalize_fire_blast(player)
	mes(player, "@que@Cast spell successfully")
	advancestat(player, STAT_MAGIC, 552, 0)
	if not worn(player, "staff of air") then
		remove(player, "air-rune", 4)
	end
	if not worn(player, "staff of fire") then
		remove(player, "fire-rune", 5)
	end
	remove(player, "death-rune", 1)
end

function skillplayer_wind_strike(player, target)
	shootplayer(player, target, "missile01")
	finalize_wind_strike(player)
end

function skillnpc_wind_strike(player, target)
	shootnpc(player, target, "missile01")
	finalize_wind_strike(player)
end

function skillplayer_water_strike(player, target)
	shootplayer(player, target, "missile02")
	finalize_water_strike(player)
end

function skillnpc_water_strike(player, target)
	shootnpc(player, target, "missile02")
	finalize_water_strike(player)
end

function skillplayer_earth_strike(player, target)
	shootplayer(player, target, "missile03")
	finalize_earth_strike(player)
end

function skillnpc_earth_strike(player, target)
	shootnpc(player, target, "missile03")
	finalize_earth_strike(player)
end

function skillplayer_fire_strike(player, target)
	shootplayer(player, target, "missile04")
	finalize_fire_strike(player)
end

function skillnpc_fire_strike(player, target)
	shootnpc(player, target, "missile04")
	finalize_fire_strike(player)
end

function skillplayer_wind_bolt(player, target)
	shootplayer(player, target, "missile05")
	finalize_wind_bolt(player)
end

function skillnpc_wind_bolt(player, target)
	shootnpc(player, target, "missile05")
	finalize_wind_bolt(player)
end

function skillplayer_water_bolt(player, target)
	shootplayer(player, target, "missile06")
	finalize_water_bolt(player)
end

function skillnpc_water_bolt(player, target)
	shootnpc(player, target, "missile06")
	finalize_water_bolt(player)
end

function skillplayer_earth_bolt(player, target)
	shootplayer(player, target, "missile07")
	finalize_earth_bolt(player)
end

function skillnpc_earth_bolt(player, target)
	shootnpc(player, target, "missile07")
	finalize_earth_bolt(player)
end

function skillplayer_fire_bolt(player, target)
	shootplayer(player, target, "missile08")
	finalize_fire_bolt(player)
end

function skillnpc_fire_bolt(player, target)
	shootnpc(player, target, "missile08")
	finalize_fire_bolt(player)
end

function skillplayer_wind_blast(player, target)
	shootplayer(player, target, "missile09")
	finalize_wind_blast(player)
end

function skillnpc_wind_blast(player, target)
	shootnpc(player, target, "missile09")
	finalize_wind_blast(player)
end

function skillplayer_water_blast(player, target)
	shootplayer(player, target, "missile10")
	finalize_water_blast(player)
end

function skillnpc_water_blast(player, target)
	shootnpc(player, target, "missile10")
	finalize_water_blast(player)
end

function skillplayer_earth_blast(player, target)
	shootplayer(player, target, "missile11")
	finalize_earth_blast(player)
end

function skillnpc_earth_blast(player, target)
	shootnpc(player, target, "missile11")
	finalize_earth_blast(player)
end

function skillplayer_fire_blast(player, target)
	shootplayer(player, target, "missile12")
	finalize_fire_blast(player)
end

function skillnpc_fire_blast(player, target)
	shootnpc(player, target, "missile12")
	finalize_fire_blast(player)
end

register_skillnpc("_", "wind strike", skillnpc_wind_strike)
register_skillnpc("_", "water strike", skillnpc_water_strike)
register_skillnpc("_", "earth strike", skillnpc_earth_strike)
register_skillnpc("_", "fire strike", skillnpc_fire_strike)
register_skillnpc("_", "wind bolt", skillnpc_wind_bolt)
register_skillnpc("_", "water bolt", skillnpc_water_bolt)
register_skillnpc("_", "earth bolt", skillnpc_earth_bolt)
register_skillnpc("_", "fire bolt", skillnpc_fire_bolt)
register_skillnpc("_", "wind blast", skillnpc_wind_blast)
register_skillnpc("_", "water blast", skillnpc_water_blast)
register_skillnpc("_", "earth blast", skillnpc_earth_blast)
register_skillnpc("_", "fire blast", skillnpc_fire_blast)
