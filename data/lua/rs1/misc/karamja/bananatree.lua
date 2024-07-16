-- RSC 2001/replays master archive/Scenery/scenery- banana tree- max banana from tree is 5.pcap
-- RSC 2001/replays master archive/Scenery/scenery- banana tree- perfect banana respawn rate (approx 8m 15s).pcap
-- RSC 2001/replays master archive/Scenery/scenery- banana tree- picking bananas.pcap

function oploc2_bananafull(player, x, y)
	mes(player, "@que@you pick a banana")
	give(player, "banana", 1)
	local picked = getvar(player, "banana_picked")
	picked = picked + 1
	if picked > 4 then
		setvar(player, "banana_picked", 0)
		changeloc(x, y, "bananaempty")
		pause(825, 825)
		changeloc(x, y, "bananafull")
	else
		setvar(player, "banana_picked", picked)
	end
end

function oploc2_bananaempty(player, x, y)
	mes(player, "@que@there are no bananas left on the tree")
end
