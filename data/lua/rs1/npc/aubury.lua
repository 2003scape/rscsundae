-- ./rscplus-replays-master/RSC 2001/3/f2p (not 100% complete)/dialogue- aubury- all- shop

function talknpc_aubury(player, npc)
	npcsay(npc, "Do you want to buy some runes?")
	local resp = multi(player,
		"Yes please",
		"Oh it's a rune shop. No thank you, then")
	if resp == 1 then
		say(player, "Yes Please")
		openshop(player, "runeshop")
	elseif resp == 2 then
		say(player, "Oh it's a rune shop. No thank you, then")
		npcsay(npc, "Well if you find someone who does want runes,")
		npcsay(npc, "send them my way")
	end
end
