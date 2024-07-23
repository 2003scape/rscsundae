-- https://classic.runescape.wiki/w/Transcript:Weaponsmaster
-- TODO should not be possible to attack if gang member

function takeobj_phoenix_crossbow(player, x, y)
	local npc = nearvisnpc(player, "weaponsmaster")
	if npc then
		local stage = getvar(player, "phoenix_stage")
		if stage < 2 then
			npcsay(npc, "Hey thief!")
			npcattack(npc, player)
			return
		else
			npcsay(npc, "Hey, that's Straven's")
			npcsay(npc, "He won't like you messing with that")
			return
		end
	end
	takeobject(player, "phoenix crossbow", x, y)
end

function talknpc_weaponsmaster(player, npc)
	local stage = getvar(player, "phoenix_stage")
	if stage < 2 then
		say(player, "Hello")
		npcsay(npc, "Hey I don't know you")
		npcsay(npc, "You're not meant to be here")
	else
		npcsay(npc, "Hello Fellow phoenix")
		npcsay(npc, "What are you after?")
		local resp = multi(player,
			"I'm after a weapon or two",
			"I'm looking for treasure")
		if resp == 1 then
			say(player, "I'm after a weapon or two")
			npcsay(npc, "Sure have a look around")
		elseif resp == 2 then
			say(player, "I'm looking for treasure")
			npcsay(npc, "We've not got any up here")
			npcsay(npc, "Go mug someone somewhere")
			npcsay(npc, "If you want some treasure")
		end
	end
end
