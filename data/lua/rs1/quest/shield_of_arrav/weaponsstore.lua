-- RSC 2001/3/f2p (not 100% complete)/scenery- door- varrock- south east- door- phoenix gang weapons store- fail- its locked.pcap
-- RSC 2001/3/f2p (not 100% complete)/scenery- door- varrock- south east- door- phoenix gang weapons store- fail- must use key.pcap
-- RSC 2001/3/f2p (not 100% complete)/scenery- door- varrock- south east- door- phoenix gang weapons store- success- use key on door.pcap

function opbound1_phoenixdoor2(player, x, y, dir)
	mes(player, "The door is locked")
	if held(player, "phoenixkey2", 1) then
		mes(player, "You need to use your key to open it")
	end
end

register_usebound("phoenixdoor2", "phoenixkey2", function(player, x, y, dir)
	thinkbubble(player, "phoenixkey2")
	mes(player, "You unlock the door")
	boundaryteleport(player, x, y, dir)
	mes(player, "You go through the door")
	changebound(x, y, dir, "openlockeddoor")
	pause(5, 5)
	changebound(x, y, dir, "phoenixdoor2")
end)
