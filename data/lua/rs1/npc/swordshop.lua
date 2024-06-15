-- ./rscplus-replays-master/RSC 2001/3/f2p (not 100% complete)/Varrock swords- shop

function talknpc_swordshop(player, npc)
	npcsay(npc, "Hello bold adventurer")
	npcsay(npc, "Can I interest you in some swords?")
	local resp = multi(player,
		"Yes please",
		"No, I'm OK for swords right now")
	if resp == 1 then
		say(player, "Yes please")
		openshop(player, "swordshop")
	elseif resp == 2 then
		say(player, "No, I'm OK for swords right now")
	end
end
