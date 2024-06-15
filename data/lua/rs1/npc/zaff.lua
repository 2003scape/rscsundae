-- ./rscplus-replays-master/RSC 2001/3/f2p (not 100% complete)/dialogue- zaff- all- shop

function talknpc_zaff(player, npc)
	npcsay(npc, "Would you like to buy or sell some staffs?")
	local resp = multi(player,
		"Yes please",
		"No, thank you")
	if resp == 1 then
		say(player, "Yes please")
		openshop(player, "staffshop")
	elseif resp == 2 then
		say(player, "No, thank you")
	end
end
