-- ./rscplus-replays-master/rsc-preservation.xyz/Shops/Port Sarim Gerrant Fishing Shop
-- ./rscplus-replays-master/RSC 2001/3/f2p (not 100% complete)/dialogue- gerrant- all- shop

function talknpc_gerrant(player, npc)
	npcsay(npc, "Welcome you can buy fishing equipment at my store")
	npcsay(npc, "We'll also buy anything you catch off you")
	local resp = multi(player,
		"Let's see what you've got then",
		"Sorry, I'm not interested")
	if resp == 1 then
		say(player, "Let's see what you've got then")
		openshop(player, "fishingshop")
	elseif resp == 2 then
		say(player, "Sorry,I'm not interested")
	end
end
