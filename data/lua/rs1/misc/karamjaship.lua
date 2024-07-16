-- https://classic.runescape.wiki/w/Transcript:Seaman_Lorris
-- rsc-preservation.xyz/Quests/Pirates treasure.pcap

local function sail_karamja(player, npc)
	npcbusy(npc)
	npcsay(npc, "Do you want to go on a trip to Karamja?")
	npcsay(npc, "The trip will cost you 30 gold")
	local resp = multi(player,
		"I'd rather go to Crandor Isle",
		"Yes please",
		"No thankyou")
	if resp == 1 then
		say(player, "I'd rather go to Crandor Isle")
		npcsay(npc, "No I need to stay alive")
		npcsay(npc, "I have a wife and family to support")
	elseif resp == 2 then
		say(player, "Yes please")
		if not held(player, "coins", 30) then
			say(player, "Oh dear I don't seem to have enough money")
		else
			mes(player, "You pay 30 gold")
			remove(player, "coins", 30)
			delay(3)
			mes(player, "You board the ship")
			delay(3)
			-- dramatic pause
			teleport(player, 324, 713)
			delay(3)
			mes(player, "The ship arrives at Karamja")
		end
	elseif resp == 3 then
		say(player, "No thankyou")
	end
	npcunbusy(npc)
end

function talknpc_captain_tobias(player, npc)
	sail_karamja(player, npc)
end

function talknpc_seaman_lorris(player, npc)
	sail_karamja(player, npc)
end

function talknpc_seaman_thresnor(player, npc)
	sail_karamja(player, npc)
end

function oploc1_karamjaship(player, x, y)
	local npc

	npc = nearvisnpc(player, "captain tobias")
	if npc then
		sail_karamja(player, npc)
		return
	end

	npc = nearvisnpc(player, "seaman lorris")
	if npc then
		sail_karamja(player, npc)
		return
	end

	npc = nearvisnpc(player, "seaman thresnor")
	if npc then
		sail_karamja(player, npc)
		return
	end
end
