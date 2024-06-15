-- RSC 2001/3/f2p (not 100% complete)/dialogue- betty- all- shop

function talknpc_betty(player, npc)
	npcsay(npc, "Welcome to the magic emporium")
	local resp = multi(player,
		"Can I see your wares?",
		"Sorry I'm not into magic")
	if resp == 1 then
		say(player, "Can I see your wares?")
		npcsay(npc, "Yes")
		openshop(player, "magicshop")
	elseif resp == 2 then
		say(player, "Sorry I'm not into magic")
		npcsay(npc, "Send anyone my way who is")
	end
end
