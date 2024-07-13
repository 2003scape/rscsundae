-- Revisionism/06-15-2018 04.23.22 goblin diplomacy.pcap

function talknpc_redgoblin(player, npc)
	npcsay(npc, "Red Armour best")
	local resp = multi(player, "Err Ok", "Why is red best?")
	if resp == 1 then
		say(player, "Err Ok")
	elseif resp == 2 then
		say(player, "Why is red best?")
		npcsay(npc, "Cos General Bentnoze says so")
		npcsay(npc, "And he bigger than me")
	end
end

function talknpc_greengoblin(player, npc)
	npcsay(npc, "green Armour best")
	local resp = multi(player, "Err Ok", "Why is green best?")
	if resp == 1 then
		say(player, "Err Ok")
	elseif resp == 2 then
		say(player, "Why is green best?")
		npcsay(npc, "I forgot now")
		npcsay(npc, "but General Wartface says it is")
		npcsay(npc, "So it must be")
	end
end
