function opbound1_closet_door(player, x, y, dir)
	mes(player, "The door is locked")
end

function usebound_closet_door_closet_key(player, x, y, dir)
	thinkbubble(player, "closet key")
	mes(player, "@que@You unlock the door")
	mes(player, "@que@You go through the door")
	boundaryteleport(player, x, y, dir)
	changebound(x, y, dir, "openlockeddoor")
	pause(3, 3)
	changebound(x, y, dir, "closet door")
end

register_usebound("closet door", "closet key", usebound_closet_door_closet_key)
