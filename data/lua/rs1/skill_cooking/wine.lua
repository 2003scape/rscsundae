-- Logg/Tylerbeg/08-01-2018 19.32.34 varrock pt2.pcap

function useinv_waterjug_grapes(player)
	if not statatleast(player, STAT_COOKING, 35) then
		-- XXX message is unverified, no replay available
		mes(player, "@que@You need a cooking level of 35 to make wine")
		return
	end
	mes(player, "@que@You squeeze the grapes into the jug")
	remove(player, "waterjug", 1)
	remove(player, "grapes", 1)
	delay(6)
	-- chance comes from 2004scape
	if statrandom(player, STAT_COOKING, 48, 352) then
		mes(player, "@que@You make some nice wine")
		give(player, "wine", 1)
		advancestat(player, STAT_COOKING, 440, 0)
	else
		-- XXX message is unverified, no replay available
		mes(player, "@que@You accidentally make some bad wine")
		give(player, "bad wine", 1)
	end
end

register_useinv("waterjug", "grapes", useinv_waterjug_grapes)
