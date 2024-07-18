-- rsc-preservation.xyz/Quests/Pirates treasure Pt4.pcap

function oploc1_piratechest(player, x, y)
	mes(player, "The chest is locked")
end

register_useloc("piratechest", "chest key", function(player, x, y)
	changeloc(x, y, "openlockedchest")
	mes(player, "You unlock the chest")
	mes(player, "All that is in the chest is a message")
	delay(3)
	mes(player, "You take the message from the chest")
	delay(3)
	mes(player, "It says dig just behind the south bench in the park")
	remove(player, "chest key", 1)
	changeloc(x, y, "piratechest")
	setvar(player, "treasure_stage", 3)
end)
