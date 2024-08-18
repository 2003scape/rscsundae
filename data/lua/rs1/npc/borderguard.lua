-- https://classic.runescape.wiki/w/Transcript:Border_Guard

local function enter_kharidgate(player, entry)
	mes(player, "@que@The gate swings open")

	if entry then
		teleport(player, 91, 649)
	else
		teleport(player, 92, 649)
	end
end

local function talknpc_borderguard(player, npc, entry)
	say(player, "Can I come through this gate?")

	if getvar(player, "prince_ali_stage") < 4 then
		npcsay(npc, "You must pay a toll of 10 gold coins to pass")

		local resp = multi(
			player,
			"No thankyou, I'll walk round",
			"Who does my money go to?",
			"yes ok"
		)

		if resp == 1 then
			say(player, "No thankyou, I'll walk round")
			npcsay(npc, "Ok suit yourself")
		elseif resp == 2 then
			say(player, "Who does my money go to?")
			npcsay(npc, "The money goes to the city of Al Kharid")
		elseif resp == 3 then
			say(player, "Yes ok")

			if held(player, "coins", 10) then
				mes(player, "@que@You pay the guard")
				remove(player, "coins", 10)
				delay(2)
				npcsay(npc, "You may pass")
				enter_kharidgate(player, entry)
			else
				say(
					player,
					"Oh dear I don't actually seem to have enough money"
				)
			end
		end
	else
		npcsay(npc, "You may pass for free, you are a friend of Al Kharid")
		enter_kharidgate(player, entry)
	end
end

function talknpc_borderguard1(player, npc)
	talknpc_borderguard(player, npc, true)
end

function talknpc_borderguard2(player, npc)
	talknpc_borderguard(player, npc, false)
end

function oploc1_kharidgateclosed(player, x, y)
	mes(player, "You need to talk to the border guard")
end
