-- RSC 2001/3/f2p (not 100% complete)/walkaround- draynor manor- top floor.pcap
function oploc1_openchest(player, x, y)
	mes(player, "@que@You search the chest, but find nothing")
end

-- RSC 2001/3/f2p (not 100% complete)/walkaround- draynor manor- top floor.pcap
function oploc2_openchest(player, x, y)
	mes(player, "@que@You close the chest")
	changeloc(x, y, "shutchest")
end

-- RSC 2001/3/f2p (not 100% complete)/walkaround- draynor manor- top floor.pcap
function oploc1_shutchest(player, x, y)
	mes(player, "@que@You open the chest")
	changeloc(x, y, "openchest")
end

-- RSC 2001/7/walkaround- varrock- jolly boar inn- all floors
function oploc1_cupboardopen(player, x, y)
	mes(player, "@que@You search the cupboard, but find nothing")
end

function oploc2_cupboardopen(player, x, y)
	mes(player, "@que@You close the cupboard")
	changeloc(x, y, "cupboardshut")
end

function oploc1_cupboardshut(player, x, y)
	mes(player, "@que@You open the cupboard")
	changeloc(x, y, "cupboardopen")
end
