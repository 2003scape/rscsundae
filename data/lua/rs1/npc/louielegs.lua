-- ./rscplus-replays-master/RSC 2001/replays master archive/NPC (dialogue, notes)/Louie legs/dialogue- louie legs- all- shop

function talknpc_louie_legs(player, npc)
	npcsay(npc, "Hey, wanna buy some armour?")
	local resp = multi(player,
		"What have you got?",
		"No, thank you")
	if resp == 1 then
		say(player, "What have you got?")
		npcsay(npc, "Take a look, see")
		openshop(player, "legsshop")
	elseif resp == 2 then
		say(player, "No, thank you")
	end
end
