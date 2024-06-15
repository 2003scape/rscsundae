-- RSC 2001/replays master archive/Unsorted junk/selling stuff to varrock general store
-- RSC 2001/replays master archive/Shops/General store- Al kharid/dialogue- general store (al kharid)
-- RSC 2001/replays master archive/Shops/General store- Falador/general store- sold item degradation (world 4 pop 16)
-- RSC 2001/replays master archive/Shops/General store- Karamja/dialogue- general store- karamja- shop

local function do_dialogue(player, npc, shop_name)
	npcsay(npc, "Can I help you at all?")
	local resp = multi(player,
		"Yes please. What are you selling?",
		"No thanks")
	if resp == 1 then
		say(player, "Yes please. What are you selling?")
		npcsay(npc, "Take a look")
		openshop(player, shop_name)
	elseif resp == 2 then
		say(player, "No thanks")
	end
end

function talknpc_genstore1(player, npc)
	do_dialogue(player, npc, "generalshop1")
end

function talknpc_genstore2(player, npc)
	do_dialogue(player, npc, "generalshop2")
end

function talknpc_genstore3(player, npc)
	do_dialogue(player, npc, "generalshop3")
end

function talknpc_genstore4(player, npc)
	do_dialogue(player, npc, "generalshop4")
end

function talknpc_genstore5(player, npc)
	do_dialogue(player, npc, "generalshop5")
end

function talknpc_genstore6(player, npc)
	do_dialogue(player, npc, "generalshop6")
end

function talknpc_genstore7(player, npc)
	do_dialogue(player, npc, "generalshop7")
end

function talknpc_genstore8(player, npc)
	do_dialogue(player, npc, "generalshop8")
end
