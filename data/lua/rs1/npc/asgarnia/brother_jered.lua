-- https://classic.runescape.wiki/w/Transcript:Brother_Jered

local function bless_star(player, npc)
	npcsay(npc, "Well I can bless that star of Saradomin you have")
	local resp = multi(player,
		"Yes Please",
		"No thankyou")
	if resp == 1 then
		-- RSC 2001/replays master archive/NPC (dialogue, notes)/Brother Jered/dialogue- Brother Jered- symbol in inv- blessing success.pcap
		say(player, "Yes Please")
		remove(player, "stringstar", 1)
		give(player, "blessedstar", 1)
		mes(player, "You give Jered the symbol")
		delay(2)
		mes(player, "Jered closes his eyes and places his hand on the symbol")
		delay(2)
		mes(player, "He softly chants")
		delay(2)
		mes(player, "Jered passes you the holy symbol")
	elseif resp == 2 then
		say(player, "No Thankyou")
	end
end

function talknpc_brother_jered(player, npc)
	local resp = multi(player,
		"What can you do to help a bold adventurer such as myself?",
		"Praise be to Saradomin")
	if resp == 1 then
		say(player, "What can you do to help a bold adventurer such as myself?")
		if held(player, "stringstar", 1) then
			bless_star(player, npc)
		elseif held(player, "nostringstar", 1) then
			npcsay(npc, "Well if you put a string on that holy symbol")
			npcsay(npc, "I can bless it for you")
		else
			npcsay(npc, "If you have a silver star")
			npcsay(npc, "Which is the holy symbol of Saradomin")
			npcsay(npc, "Then I can bless it")
			npcsay(npc, "Then if you are wearing it")
			npcsay(npc, "It will help you when you are praying")
		end
	elseif resp == 2 then
		say(player, "Praise be to Saradomin")
		npcsay(npc, "Yes praise he who brings life to this world")
	end
end
