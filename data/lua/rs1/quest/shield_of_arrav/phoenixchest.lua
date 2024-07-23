function oploc1_phoenixshutchest(player, x, y)
	mes(player, "You open the chest")
	changeloc(x, y, "phoenixopenchest")
end

function oploc1_phoenixopenchest(player, x, y)
	if held(player, "arravshield1", 1) then
		-- 1e_Luis/Quests/Shield of Arrav/Phoenix Gang/Part 2.pcap
		mes(player, "The chest is empty")
	else
		-- Revisionism/06-16-2018 22.27.41 shield of arrav (phoenix gang side) pt1.pcap
		mes(player, "You search the chest")
		mes(player, "You find half a shield which you take")
		give(player, "arravshield1", 1)
		if getvar(player, "arrav_stage") < 4 then
			setvar(player, "arrav_stage", 4)
		end
	end
end

function oploc2_phoenixopenchest(player, x, y)
	mes(player, "You close the chest")
	changeloc(x, y, "phoenixshutchest")
end
