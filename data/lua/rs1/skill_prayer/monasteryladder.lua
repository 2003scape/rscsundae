-- RSC 2001/replays master archive/Scenery/scernery- monastery- ladder-dialogue- abbot langley- join guild- success (70 out of 31 pray)
-- RSC 2001/replays master archive/Scenery/scenery- monastery - ladder dialogue- abbot langley- join guild- fail (29 out of 31 pray)

function oploc1_monasteryladder(player, x, y)
	if getvar(player, "monastery_joined") ~= 0 then
		mes(player, "@que@You climb up the ladder")
		changelevelup(player)
		return
	end
	local npc = nearvisnpc(player, "abbot langley")
	if not npc then
		-- XXX unknown behaviour
		return
	end
	npcbusy(npc)
	npcsay(npc, "Only members of our order can go up there")
	local resp = multi(player,
		"Well can i join your order?",
		"Oh sorry")
	if resp == 1 then
		say(player, "Well can I join your order?")
		if statatleast(player, STAT_PRAYER, 31) then
			npcsay(npc, "Ok I see you are someone suitable for our order")
			npcsay(npc, "You may join")
			setvar(player, "monastery_joined", 1)
		else
			npcsay(npc, "No I feel you are not devout enough")
			mes(player, "You need a prayer level of 31")
		end
	elseif resp == 2 then
		say(player, "Oh Sorry")
	end
	npcunbusy(npc)
end
