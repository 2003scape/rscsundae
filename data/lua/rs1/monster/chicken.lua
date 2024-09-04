-- https://classic.runescape.wiki/w/Chicken#Drops
-- Milestone: first drop table implemented

function killnpc_chicken(player, npc, x, y)
	local ran = randomvar(128)
	if ran < 20 then
		addobject(player, "feather", 10, x, y)
	elseif ran < 96 then
		addobject(player, "feather", 3, x, y)
	end
end
