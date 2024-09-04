-- https://classic.runescape.wiki/w/Transcript:Guildmaster
-- Revisionism/06-15-2018 18.17.00 try to enter champ's guild with 31 qp, then go to the mines for tin and copper.pcap
-- RSC 2001/3/f2p (not 100% complete)/scenery- varrock- champions guild- door- open- success.pcap

function opbound1_championdoor(player, x, y, dir)
	if qp(player) < 32 then
		local npc = nearnpc(player, "guildmaster")
		if npc then
			npcsay(npc, "You have not proven yourself worthy to enter here yet")
			mes(player, "The door won't open - you need at least 32 quest points")
		end
	else
		mes(player, "@que@")
		boundaryteleport(player, x, y, dir)
		changebound(x, y, dir, "openlockeddoor")
		pause(3, 3)
		changebound(x, y, dir, "championdoor")
	end
end

function talknpc_guildmaster(player, npc)
	local resp = multi(player,
		"What is this place?",
		"Do you know where I could get a rune plate mail body?")
	if resp == 1 then
		say(player, "What is this place?")
		npcsay(npc, "This is the champion's guild")
		npcsay(npc, "Only Adventurers who have proved themselves worthy")
		npcsay(npc, "by gaining influence from quests are allowed in here")
		npcsay(npc, "As the number of quests in the world rises")
		npcsay(npc, "So will the requirements to get in here")
		npcsay(npc, "But so will the rewards")
	elseif resp == 2 then
		say(player, "Do you know where I could get a rune plate mail body?")
		npcsay(npc, "I have a friend called Oziach who lives by the cliffs")
		npcsay(npc, "He has a supply of rune plate mail")
		npcsay(npc, "He may sell you some if you're lucky, he can be a little strange though")
		setvar(player, "dragon_stage", 1)
	end
end
