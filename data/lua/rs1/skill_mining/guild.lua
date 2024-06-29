-- Revisionism/06-14-2018 01.08.40 can't enter the mining guild, thinking about doing pirate's treasure
-- Logg/RSC 2018/08-03-2018 02.21.27 fishing guild, mining guild

local function reject_mining_guild(player)
	local npc = nearnpc(player, "guilddwarf")
	if npc then
		npcsay(npc, "Sorry only the top miners are allowed in there")
	end
end

function oploc1_mineladder(player, x, y)
	if statatleast(player, STAT_MINING, 60) then
		-- no messaage here unlike normal ladders
		changeleveldown(player)
	else
		reject_mining_guild(player)
		mes(player, "You need a mining of level 60 to enter")
	end
end

function opbound1_minedoor(player, x, y, dir)
	if statatleast(player, STAT_MINING, 60) then
		boundaryteleport(player, x, y, dir)
		mes(player, "@que@you go through the door")
		changebound(x, y, dir, "openlockeddoor")
		delay(5)
		changebound(x, y, dir, "minedoor")
	else
		reject_mining_guild(player)
		mes(player, "The door won't open - you need a mining of level 60 to enter")
	end
end


