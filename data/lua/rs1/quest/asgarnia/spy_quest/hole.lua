-- 1e_Luis/Quests/Black Knights Fortress

register_useloc("hole", "cabbage", function(player, x, y)
	if getvar(player, "fortress_stage") ~= 2 then
		say(player, "Why would I want to do that?")
		return
	end
	mes(player, "You drop a cabbage down the hole.")
	remove(player, "cabbage", 1)
	delay(3)
	mes(player, "The cabbage lands in the cauldron below.")
	delay(5)
	mes(player, "The mixture in the cauldron starts to froth and bubble.")
	delay(5)
	mes(player, "You hear the witch groan in dismay.")
	setvar(player, "fortress_stage", 3)
end)

register_useloc("hole", "magiccabbage", function(player, x, y)
	if getvar(player, "fortress_stage") ~= 2 then
		say(player, "Why would I want to do that?")
		return
	end
	mes(player, "This is the wrong sort of cabbage!")
	delay(5)
	mes(player, "You are meant to be hindering the witch.")
	delay(3)
	mes(player, "Not helping her.")
end)
