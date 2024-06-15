-- https://classic.runescape.wiki/w/Transcript:Rommik
-- https://classic.runescape.wiki/w/Transcript:Dommik

local function do_craftingshop(player, npc, shop)
	npcsay(npc, "Would you like to buy some crafting equipment")
	local resp = multi(player,
		"No I've got all the crafting equipment I need",
		"Let's see what you've got then")
	if resp == 1 then
		say(player, "No I've got all the crafting equipment I need")
		npcsay(npc, "Ok fair well on your travels")
	elseif resp == 2 then
		say(player, "Let's see what you've got then")
		openshop(player, shop)
	end
end

function talknpc_rommik(player, npc)
	do_craftingshop(player, npc, "craftingshop")
end

function talknpc_dommik(player, npc)
	do_craftingshop(player, npc, "craftingshop2")
end
