-- https://classic.runescape.wiki/w/Transcript:Wayne

function talknpc_wayne(player, npc)
	npcsay(npc, "Welcome to Wayne's chains")
	npcsay(npc, "Do you wanna buy or sell some chain mail?")
	local resp = multi(player,
		"Yes please",
		"No thanks")
	if resp == 1 then
		say(player, "Yes please")
		openshop(player, "chainmailshop")
	elseif resp == 2 then
		say(player, "No thanks")
	end
end
