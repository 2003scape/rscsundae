-- https://classic.runescape.wiki/w/Fish_Food
-- https://classic.runescape.wiki/w/Fountain_(Draynor_Manor)
-- https://classic.runescape.wiki/w/Poisoned_fish_food

register_useinv("poison", "fish food", function(player)
	remove(player, "poison", 1)
	remove(player, "fish food", 1)
	give(player, "poisoned fish food", 1)
	mes(player, "You poison the fish food")
end)

local function have_killed_piranhas(player)
	return getvar(player, "ernest_killed_piranhas") ~= 0
		or getvar(player, "ernest_stage") == 3
end

register_useloc("hauntedfountain", "fish food", function(player, x, y)
	mes(player, "You pour the fish food into the fountain")
	remove(player, "fish food", 1)
	delay(3)
	mes(player, "You see the pirhanas eating the food")
	delay(3)
	mes(player, "The pirhanas seem hungrier than ever")
end)

register_useloc("hauntedfountain", "poisoned fish food", function(player, x, y)
	mes(player, "You pour the poisoned fish food into the fountain")
	remove(player, "poisoned fish food", 1)
	delay(3)
	mes(player, "You see the pirhanas eating the food")
	delay(3)
	mes(player, "The pirhanas drop dead and float to the surface")

	if not have_killed_piranhas(player) then
		setvar(player, "ernest_killed_piranhas", 1)
	end
end)

function oploc2_hauntedfountain(player, x, y)
	if not have_killed_piranhas(player) then
		say(player, "There seems to be a pressure gauge in here")
		say(player, "There are a lot of Pirhanas in there though")
		say(player, "I can't get the gauge out")
	elseif
		have_killed_piranhas(player)
		and getvar(player, "ernest_stage") ~= 3
		and not held(player, "pressure gauge", 1)
	then
		say(player, "There seems to be a pressure gauge in here")
		say(player, "There are also some dead fish")
		mes(player, "you get the pressure gauge from the fountain")
		give(player, "pressure gauge", 1)
	else
		mes(player, "It's full of dead fish")
	end
end
