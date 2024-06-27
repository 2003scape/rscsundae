-- rsc-preservation.xyz/Quests/The restless ghost - Part 1

function oploc1_shutghostcoffin(player, x, y)
	mes(player, "You open the coffin")
	changeloc(x, y, "openghostcoffin")
end

function oploc1_openghostcoffin(player, x, y)
	mes(player, "There's a skeleton without a skull in here")
	if not nearnpc(player, "ghostx") then
		addnpc("ghostx", 103, 674) 
	end
end

function oploc2_openghostcoffin(player, x, y)
	mes(player, "You close the coffin")
	changeloc(x, y, "shutghostcoffin")
end

-- rsc-preservation.xyz/Quests/The restless ghost - Part 2

function useloc_openghostcoffin_ghostskull(player, x, y)
	if getvar(player, "ghost_stage") ~= 4 then
		mes(player, "Nothing interesting happens")
		return
	end
	mes(player, "You put the skull in the coffin")
	delay(3)
	remove(player, "ghostskull", 1)
	--TODO: implement delnpc
	--delnpc("ghostx")
	mes(player, "The ghost has vanished")
	delay(3)
	mes(player, "You think you hear a faint voice in the air")
	delay(3)
	mes(player, "Thank you")
	delay(3)
	mes(player, "You have completed the restless ghost quest")
	advancestat(player, STAT_PRAYER, 2000, 250)
	giveqp(player, 1)
	setvar(player, "ghost_stage", 5)
end

register_useloc("openghostcoffin", "ghostskull", useloc_openghostcoffin_ghostskull)
