-- https://classic.runescape.wiki/w/Compost_Heap

function oploc2_compost_heap(player, x, y)
	mes(player, "I'm not looking through that with my hands")
end

function useloc_compost_heap_spade(player, x, y)
	local stage = getvar(player, "ernest_stage")

	mes(player, "You dig through the compost heap")
	delay(3)

	if stage ~= 2 or held(player, "closet key", 1) then
		mes(player, "You find nothing of interest")
	else
		mes(player, "You find a small key")
		give(player, "closet key", 1)
	end
end

register_useloc("compost heap", "spade", useloc_compost_heap_spade)
