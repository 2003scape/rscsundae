-- https://classic.runescape.wiki/w/Transcript:Tanner

-- pcaps/1e_Luis/Skilling/Crafting distinct items.pcap
local function do_buy_leather(player, npc)
	if not held(player, "cow hide", 1) then
		say(player, "I don't have any cow hides left now")
		return
	end
	if not held(player, "coins", 1) then
		-- XXX: this message isn't in a replay, made up
		say(player, "I don't have any money left now")
		return
	end
	mes(player, "You swap a cow hide for a piece of leather")
	remove(player, "coins", 1)
	remove(player, "cow hide", 1)
	give(player, "leather", 1)
	delay(1)
	do_buy_leather(player, npc)
end

-- RSC 2001/replays master archive/NPC (dialogue, notes)/Tanner/dialogue- tanner
function talknpc_tanner(player, npc)
	npcsay(npc, "Greetings friend I'm a manufacturer of leather")
	local resp = multi(player,
		"Can I buy some leather then?",
		"Here's some cow hides, can I buy some leather now?",
		"Leather is rather weak stuff")
	if resp == 1 then
		say(player, "Can I buy some leather then?")
		npcsay(npc, "I make leather from cow hides")
		npcsay(npc, "Bring me some of them and a gold coin per hide")
	elseif resp == 2 then
		say(player, "Here's some cow hides, Can I buy some leather")
		npcsay(npc, "Ok")
		do_buy_leather(player, npc)
	elseif resp == 3 then
		say(player, "Leather is rather weak stuff")
		npcsay(npc, "Well yes if all you're concerned with how much it will protect you in a fight")
	end
end
