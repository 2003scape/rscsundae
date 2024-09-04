-- https://classic.runescape.wiki/w/Transcript:Oracle

local wise_knowledge = {
	"You must search from within to find your true destiny",
	"No crisps at the party",
	"It is cunning, almost foxlike",
	"Is it waking up time, I'm not quite sure",
	"When in Asgarnia do as the Asgarnians do",
	"The light at the end of the tunnel is the demon infested lava pit",
	"Watch out for cabbages they are green and leafy",
	"Too many cooks spoil the anchovie pizza",
}

function talknpc_oracle(player, npc)
	if getvar(player, "dragon_stage") == 2 then
		local resp = multi(
			player,
			"I seek a piece of the map of the isle of Crondor",
			"Can you impart your wise knowledge to me oh oracle"
		)

		if resp == 1 then
			say(player, "I seek a piece of the map of the isle of Crandor")
			npcsay(npc, "The map's behind a door below")
			npcsay(npc, "But entering is rather tough")
			npcsay(npc, "And this is what you need to know")
			npcsay(npc, "You must have the following stuff")
			npcsay(npc, "First a drink used by the mage")
			npcsay(npc, "Next some worm string, changed to sheet")
			npcsay(npc, "Then a small crustacean cage")
			npcsay(npc, "Last a bowl that's not seen heat")
		elseif resp == 2 then
			say(player, "Can you impart your wise knowledge to me oh oracle")
			npcsay(npc, wise_knowledge[1])
		end
	else
		say(player, "Can you impart your wise knowledge to me oh oracle")
		npcsay(npc, wise_knowledge[randomvar(#wise_knowledge - 1) + 1])
	end
end
