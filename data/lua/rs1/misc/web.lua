-- https://classic.runescape.wiki/w/Web
-- RSC 2001/7/scenery- web- wilderness- mage bank- success and fail- respawn rate

function usebound_web_knife(player, x, y, dir)
	mes(player, "You try to destroy the web...")
	delay(3)
	if random(128) then
		mes(player, "You slice through the web")
		changebound(x, y, dir, "blank")
		pause(50, 50)
		changebound(x, y, dir, "web")
	else
		mes(player, "You fail to cut through it")
	end
end

register_usebound("web", "knife", usebound_web_knife)
register_usebound("web", "dagger", usebound_web_knife)
register_usebound("web", "sword", usebound_web_knife)
register_usebound("web", "mace", usebound_web_knife)
register_usebound("web", "axe", usebound_web_knife)

