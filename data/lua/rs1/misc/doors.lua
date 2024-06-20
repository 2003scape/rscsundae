function opbound1_normdoor(player, x, y, dir)
	mes(player, "@que@The door swings open")
	changebound(x, y, dir, "normdoorframe")
end

function opbound2_normdoorframe(player, x, y, dir)
	mes(player, "@que@The door creaks shut")
	changebound(x, y, dir, "normdoor")
	delay(3)
end

function opbound1_highdoor(player, x, y, dir)
	mes(player, "@que@The door swings open")
	changebound(x, y, dir, "highdoorframe")
end

function opbound2_highdoorframe(player, x, y, dir)
	mes(player, "@que@The door creaks shut")
	changebound(x, y, dir, "highdoor")
	delay(3)
end

-- ./RSC 2001/3/f2p (not 100% complete)/scenery- draynor manor- front- doors (double)
function oploc2_doubledoorsopen(player, x, y)
	mes(player, "@que@The doors creak shut")
	changeloc(x, y, "doubledoorsclosed")
	delay(3)
end

function oploc1_doubledoorsclosed(player, x, y)
	mes(player, "@que@The doors swing open")
	changeloc(x, y, "doubledoorsopen")
end

-- ./RSC 2001/3/f2p (not 100% complete)/walkaround- falador- south- chicken farm
function oploc2_woodengateopen(player, x, y)
	mes(player, "@que@The gate creaks shut")
	changeloc(x, y, "woodengateclosed")
	delay(3)
end

function oploc1_woodengateclosed(player, x, y)
	mes(player, "@que@The gate swings open")
	changeloc(x, y, "woodengateopen")
end

function oploc2_metalgateopen(player, x, y)
	mes(player, "@que@The gate creaks shut")
	changeloc(x, y, "metalgateclosed")
	delay(3)
end

function oploc1_metalgateclosed(player, x, y)
	mes(player, "@que@The gate swings open")
	changeloc(x, y, "metalgateopen")
end

-- Logg/Tylerbeg/08-01-2018 19.32.34 varrock pt2.pcap
function oploc1_manholeclosed(player, x, y)
	mes(player, "@que@You slide open the manhole cover")
	changeloc(x, y, "manholeopen")
end

function oploc2_manholeopen(player, x, y)
	mes(player, "@que@You slide the cover back over the manhole")
	changeloc(x, y, "manholeclosed")
	delay(3)
end
