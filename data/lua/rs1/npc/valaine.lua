-- ./rscplus-replays-master/RSC 2001/3/f2p (not 100% complete)/dialogue- valaine- all- shop

function talknpc_valaine(player, npc)
	npcsay(npc, "Hello there.")
	npcsay(npc, "Want to have a look at what we're selling today?")
	local resp = multi(player,
		"Yes please",
		"No thank you")
	if resp == 1 then
		say(player, "Yes please.")
		openshop(player, "championshop")
	elseif resp == 2 then
		say(player, "No thank you")
	end
end
