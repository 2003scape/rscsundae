-- https://classic.runescape.wiki/w/Transcript:Customs_Officer
-- RSC 2001/replays master archive/NPC (dialogue, notes)/Cstms officer (to varrock)/scenery- ship- board- dialogue- cstms officer- search- rum inv- fail- no trav- confisc all rum- 100% pirate t.pcap
-- rsc-preservation.xyz/Quests/Pirates treasure Pt3.pcap

local function customs_pay(player, npc)
	local resp = multi(player, "Ok", "Oh, I'll not bother then")
	if resp == 1 then
		if not held(player, "coins", 30) then
			say(player, "Oh dear I don't seem to have enough money")
		else
			say(player, "Ok")
			mes(player, "You pay 30 gold")
			remove(player, "coins", 30)
			delay(3)
			mes(player, "You board the ship")
			delay(3)
			teleport(player, 268, 651)
			delay(3)
			-- dramatic pause
			mes(player, "The ship arrives at Port Sarim")
		end
	elseif resp == 2 then
		say(player, "Oh, I'll not bother then")
	end
end

local function customs_search(player, npc)
	npcsay(npc, "You need to be searched before you can board")
	local resp = multi(player,
		"Why?",
		"Search away I have nothing to hide",
		"You're not putting your hands on my things")
	if resp == 1 then
		say(player, "Why?")
		npcsay(npc, "Because Asgarnia has banned the import of intoxicating spirits")
	elseif resp == 2 then
		say(player, "Search away I have nothing to hide")

		if held(player, "karamja rum", 1) then
			npcsay(npc, "Aha trying to smuggle rum are we?")
			mes(player, "The customs officer confiscates your rum")
			remove(player, "karamja rum", 30)
		else
			npcsay(npc, "Well you've got some odd stuff, but it's all legal")
			npcsay(npc, "Now you need to pay a boarding charge of 30 gold")
			customs_pay(player, npc)
		end
	elseif resp == 3 then
		say(player, "You're not putting your hands on my things")
		npcsay(npc, "You're not getting on this ship then")
	end
end

function oploc1_sarimjaship(player, x, y)
	local npc = nearvisnpc(player, "customs officer")
	if npc then
		customs_search(player, npc)
	end
end

function talknpc_customs_officer(player, npc)
	local resp = multi(player,
		"Can I board this ship?",
		"Does Karamja have any unusual customs then?")

	if resp == 1 then
		say(player, "Can I board this ship?")
		customs_search(player, npc)
	elseif resp == 2 then
		say(player, "Does Karamja have any unusual customs then?")
		npcsay(npc, "I'm not that sort of customs officer")
	end
end
