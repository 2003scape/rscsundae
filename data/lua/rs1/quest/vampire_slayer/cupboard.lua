-- rsc-preservation.xyz/Quests/Vampire Slayer

function oploc1_garliccupboardshut(player, x, y)
	mes(player, "You open the cupboard")
	changeloc(x, y, "garliccupboardopen")
end

function oploc1_garliccupboardopen(player, x, y)
	mes(player, "You search the cupboard")

	if held(player, "garlic", 1) then
		mes(player, "The cupboard is empty")
	else
		mes(player, "You find a clove of garlic that you take")
		give(player, "garlic", 1)
	end
end

function oploc2_garliccupboardopen(player, x, y)
	changeloc(x, y, "garliccupboardshut")
	mes(player, "You close the cupboard")
end
