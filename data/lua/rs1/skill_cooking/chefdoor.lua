-- https://classic.runescape.wiki/w/Transcript:Head_chef
-- ./Logg/Tylerbeg/08-01-2018 19.32.34 varrock pt2.pcap

function opbound1_chefdoor(player, x, y, dir)
	if not statatleast(player, STAT_COOKING, 32) then
		local npc = nearnpc(player, "head chef")
		if npc then
			npcsay(npc, "Sorry. Only the finest chefs are allowed in here")
		end
	elseif not worn(player, "chef's hat") then
		local npc = nearnpc(player, "head chef")
		if npc then
			npcsay(npc, "Where's your chef's hat")
			npcsay(npc, "You can't come in here unless you're wearing a chef's hat")
		end
	else
		mes(player, "@que@")
		boundaryteleport(player, x, y, dir)
		changebound(x, y, dir, "openlockeddoor")
		pause(2, 2)
		changebound(x, y, dir, "chefdoor")
	end
end
