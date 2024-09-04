-- 1e_Luis/Quests/Witchs Potion

function killnpc_giantrat(player, npc, x, y)
	local stage = getvar(player, "hetty_stage")
	if stage == 1 then
		addobject(player, "rat's tail", 1, x, y)
	end
end

function killnpc_giantrat1(player, npc, x, y)
	killnpc_giantrat(player, npc, x, y)
end

function killnpc_rat(player, npc, x, y)
	local stage = getvar(player, "hetty_stage")
	if stage == 1 then
		addobject(player, "rat's tail", 1, x, y)
	end
end
