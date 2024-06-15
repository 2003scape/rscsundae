-- https://classic.runescape.wiki/w/Transcript:Flynn

function talknpc_flynn(player, npc)
	npcsay(npc, "Hello do you want to buy or sell any maces?")
	local resp = multi(player,
		"No thanks",
		"Well I'll have a look anyway")
	if resp == 1 then
		say(player, "no thanks")
	elseif resp == 2 then
		say(player, "Well I'll have a look anyway")
		openshop(player, "maceshop")
	end
end
