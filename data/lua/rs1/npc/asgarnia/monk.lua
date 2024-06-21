-- https://classic.runescape.wiki/w/Transcript:Monk
-- RSC 2001/replays master archive/NPC (dialogue, notes)/Monk/dialogue- monk- healing- restores hitpoints.pcap
-- Logg/Tylerbeg/08-02-2018 02.58.30 arena around ice mountain.pcap (39:00 onwards)
function talknpc_monk(player, npc)
	npcsay(npc, "Greetings traveller")
	local resp = multi(player,
		"Can you heal me? I'm injured",
		"Isn't this place built a bit out the way?")
	if resp == 1 then
		say(player, "Can you heal me?")
		say(player, "I'm injured")
		npcsay(npc, "Ok")
		mes(player, "The monk places his hands on your head")
		delay(3)
		mes(player, "You feel a little better")
		-- in rs2 it heals for 20% + 2, in rs1:
		-- heal for 15 at level 99
		healstat(player, STAT_HITS, 1, 15)
	elseif resp == 2 then
		say(player, "Isn't this place built a bit out the way?")
		npcsay(npc, "We like it that way")
		npcsay(npc, "We get disturbed less")
		npcsay(npc, "We still get a rather large amount of travellers")
		npcsay(npc, "looking for sanctuary and healing here as it is")
	end
end
