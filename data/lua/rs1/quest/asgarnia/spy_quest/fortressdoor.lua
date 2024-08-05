-- https://classic.runescape.wiki/w/Transcript:Guard

local function guard_workhere(player, npc)
	say(player, "Yes but I work here")
	npcsay(npc, "Well this is the guards entrance")
	npcsay(npc, "And I might be new here")
	npcsay(npc, "But I can tell you're not a guard")
	npcsay(npc, "You're not even wearing proper guards uniform")
	local resp = multi(player,
		"Pleaasse let me in",
		"So what is this uniform?")
	if resp == 1 then
		say(player, "Pleaasse let me in")
		npcsay(npc, "Go away, you're getting annoying")
	elseif resp == 2 then
		say(player, "So what is this uniform?")
		npcsay(npc, "Well you can see me wearing it")
		npcsay(npc, "It's iron chain mail and a medium bronze helmet")
	end
end

function opbound1_fortressdoor(player, x, y, dir)
	if worn(player, "iron chain mail body", 1) and
	    worn(player, "medium bronze helmet", 1) then
		-- RSC 2001/7/black knights fortress- exterior- door- quest entrance- success- 0% quest.pcap
		mes(player, "@que@The door opens")
		boundaryteleport(player, x, y, dir)
		mes(player, "@que@You go through the door")
		changebound(x, y, dir, "openlockeddoor")
		pause(2, 2)
		changebound(x, y, dir, "fortressdoor")
		return
	end
	local guard = nearnpc(player, "fortressguard")
	npcbusy(guard)
	npcsay(guard, "Heh you can't come in here")
	npcsay(guard, "This is a high security military installation")
	local resp = multi(player,
		"Yes but I work here",
		"Oh sorry",
		"So who does it belong to?")
	if resp == 1 then
		guard_workhere(player, guard)
	elseif resp == 2 then
		say(player, "Oh sorry")
		npcsay(guard, "Don't let it happen again")
	elseif resp == 3 then
		say(player, "So who does it belong to?")
		npcsay(guard, "This fortress belongs to the order of black knights known as the Kinshra")
	end
	npcunbusy(guard)
end

local function enter_meeting_room(player, x, y, dir)
	-- RSC 2001/7/black knights fortress- interior 1st- door- dialogue- enter- disguise equipped- 0% quest.pcap
	-- 1e_Luis/Quests/Black Knights Fortress
	boundaryteleport(player, x, y, dir)
	mes(player, "@que@You go through the door")
	changebound(x, y, dir, "openlockeddoor")
	pause(5, 5)
	changebound(x, y, dir, "fortressdoor2")
	local knight = nearvisnpc(player, "black knight")
	if knight then
		npcsay(knight, "Die intruder!!")
		npcattack(knight, player)
	end
end

function opbound1_fortressdoor2(player, x, y, dir)
	local guard = nearvisnpc(player, "fortressguard")
	if not guard then
		enter_meeting_room(player, x, y, dir)
		return
	end
	npcbusy(guard)
	npcsay(guard, "I wouldn't go in there if I woz you")
	npcsay(guard, "Those black knights are in an important meeting")
	npcsay(guard, "They said they'd kill anyone who went in there")
	npcunbusy(guard)
	local resp = multi(player,
		"Ok I won't",
		"I don't care I'm going in anyway")
	if resp == 1 then
		say(player, "Ok I won't")
	elseif resp == 2 then
		say(player, "I don't care I'm going in anyway")
		enter_meeting_room(player, x, y, dir)
	end
end

function opbound1_fortressdoor3(player, x, y, dir)
	-- 1e_Luis/Quests/Black Knights Fortress
	local knight = nearvisnpc(player, "black knight")
	if knight then
		npcsay(knight, "Die intruder!!")
		npcattack(knight, player)
	else
		boundaryteleport(player, x, y, dir)
		mes(player, "@que@You go through the door")
		changebound(x, y, dir, "openlockeddoor")
		pause(2, 2)
		changebound(x, y, dir, "fortressdoor3")
	end
end
