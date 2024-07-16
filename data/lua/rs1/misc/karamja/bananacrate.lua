-- RSC 2001/replays master archive/NPC (dialogue, notes)/Luthas/scenery- karamja- crate- stash banana- try to stash rum- fail- 100% pirates treasure- stash all banana full.pcap

register_useloc("bananacrate", "banana", function(player, x, y)
	local num = getvar(player, "crate_bananas")
	if num > 9 then
		mes(player, "@que@the crate is already full")
	else
		remove(player, "banana", 1)
		mes(player, "@que@you put a banana in the crate")
		num = num + 1
		setvar(player, "crate_bananas", num)
	end
end)
