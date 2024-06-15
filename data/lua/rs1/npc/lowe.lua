-- ./rscplus-replays-master/RSC 2001/3/f2p (not 100% complete)/dialogue- lowe- all- shop

function talknpc_lowe(player, npc)
	npcsay(npc, "Welcome to Lowe's Archery Store")
	npcsay(npc, "Do you want to see my wares?")
	local resp = multi(player,
		"Yes please",
		"No, I prefer to bash things close up")
	if resp == 1 then
		say(player, "Yes Please")
		openshop(player, "archeryshop")
	elseif resp == 2 then
		say(player, "No, I prefer to bash things close up")
	end
end
