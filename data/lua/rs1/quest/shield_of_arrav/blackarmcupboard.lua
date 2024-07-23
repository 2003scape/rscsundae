-- pcaps/1e_Luis/Quests/Shield of Arrav/Black Arm Gang/Part 2.pcap

function oploc1_blackarmcupboardshut(player, x, y)
	mes(player, "You open the cupboard")
	changeloc(x, y, "blackarmcupboardopen")
end

function oploc1_blackarmcupboardopen(player, x, y)
	if held(player, "arravshield2", 1) then
		mes(player, "The cupboard is empty")
	else
		mes(player, "You search the cupboard")
		mes(player, "You find half a shield which you take")
		give(player, "arravshield2", 1)
		if getvar(player, "arrav_stage") < 4 then
			setvar(player, "arrav_stage", 4)
		end
	end
end

function oploc2_blackarmcupboardopen(player, x, y)
	mes(player, "You close the cupboard")
end
