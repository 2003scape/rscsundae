-- ./rscplus-replays-master/RSC 2001/replays master archive/Shops/Zekes scimitar shop- al kharid/dialogue- zeke- all- shop

function talknpc_zeke(player, npc)
	npcsay(npc, "A thousand greetings sir")
	local resp = multi(player,
		"Do you want to trade?",
		"Nice cloak")
	if resp == 1 then
		say(player, "Do you want to trade?")
		npcsay(npc, "Yes, certainly")
		npcsay(npc, "I deal in scimitars")
		openshop(player, "scimitarshop")
	elseif resp == 2 then
		say(player, "Nice cloak")
		npcsay(npc, "Thank you")
	end
end
