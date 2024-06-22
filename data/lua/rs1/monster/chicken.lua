-- https://classic.runescape.wiki/w/Chicken#Drops
-- Milestone: first drop table implemented

function killnpc_chicken(player, npc, x, y)
	addobject(player, "bones", 1, x, y)
	addobject(player, "raw chicken", 1, x, y)
	local ran = randomvar(128)
	if ran < 20 then
		addobject(player, "feather", 10, x, y)
	elseif ran < 76 then
		addobject(player, "feather", 3, x, y)
	end
end
