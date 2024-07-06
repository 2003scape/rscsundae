-- https://classic.runescape.wiki/w/Transcript:Sir_Vyvin

function talknpc_sir_vyvin(player, npc)
	say(player, "Hello")
	npcsay(npc, "Greetings traveller")

	local resp = multi(player,
		"Do you have anything to trade?",
		"Why are there so many knights in this city?")

	if resp == 1 then
		say(player, "Do you have anything to trade?")
		npcsay(npc, "No I'm sorry")
	elseif resp == 2 then
		say(player, "Why are there so many knights in this city?")
		npcsay(npc, "We are the White Knights of Falador")
		npcsay(npc, "We are the most powerfull order of knights in the land")
		npcsay(npc, "We are helping the king Vallance rule the kingdom")
		npcsay(npc, "As he is getting old and tired")
	end
end

function oploc1_vyvincupboardopen(player, x, y)
	local npc = nearvisnpc(player, "sir vyvin")
	if npc and not npcbusy(npc) then
		-- RSC 2001/replays master archive/NPC (dialogue, notes)/Sir Vyvin/Cupboard attempt 100%  + No distract Falador White Knights Castle- Sir Vyvin-
		npcsay(npc, "Hey what are you doing?")
		npcsay(npc, "That's my cupboard")
		mes(player, "Maybe you need to get someone to distract Sir Vyvin for you")
	else
		-- 1e_Luis/Quests/Faladian Knights Sword.pcap
		if getvar(player, "sword_stage") ~= 2 then
			mes(player, "There is just a load of junk in here")
		else
			mes(player, "You find a small portrait in here which you take")
			give(player, "portrait", 1)
		end
	end
end

function oploc2_vyvincupboardopen(player, x, y)
	mes(player, "@que@You close the cupboard")
	changeloc(x, y, "vyvincupboardshut")
end

function oploc1_vyvincupboardshut(player, x, y)
	mes(player, "@que@You open the cupboard")
	changeloc(x, y, "vyvincupboardopen")
end
