-- https://classic.runescape.wiki/w/Transcript:Traiborn_the_wizard
-- rsc-preservation.xyz/Quests/demon-slayer.pcap
-- RSC 2001/7/06-28-2018 03.16.25.pcap

local function traiborn_goodbye(player, npc)
	say(player, "Err I'd better be off really")
	npcsay(npc, "Oh ok have a good time")
	npcsay(npc, "and watch out for sheep!")
	npcsay(npc, "They're more cunning than they look")
end

local function traiborn_called(player, npc)
	say(player, "Err you just called me thingummywut")
	npcsay(npc, "You're a thingummywut?")
	npcsay(npc, "I've never seen one up close before")
	npcsay(npc, "They said I was mad")
	npcsay(npc, "Now you are my proof")
	npcsay(npc, "There ARE thingummywuts in this tower")
	npcsay(npc, "Now where can I find a cage big enough to keep you?")
	local resp = multi(player,
		"Err I'd better be off really",
		"They're right, you are mad")
	if resp == 1 then
		traiborn_goodbye(player, npc)
	elseif resp == 2 then
		say(player, "They're right, you are mad")
		npcsay(npc, "That's a pity")
		npcsay(npc, "I thought maybe they were winding me up")
	end
end

local function traiborn_thingy(player, npc)
	say(player, "Whats a thingummywut?")
	npcsay(npc, "A thingummywut?")
	npcsay(npc, "Where? , Where?")
	npcsay(npc, "Those pesky thingummywuts")
	npcsay(npc, "They get everywhere")
	npcsay(npc, "They leave a terrible mess too")
	local resp = multi(player,
		"Err you just called me thingummywut",
		"Tell me what they look like and I'll mash 'em")
	if resp == 1 then
		traiborn_called(player, npc)
	elseif resp == 2 then
		say(player, "Tell me what they look like and I'll mash 'em")
		npcsay(npc, "Don't be ridiculous")
		npcsay(npc, "No-one has ever seen one")
		npcsay(npc, "They're invisible")
		npcsay(npc, "Or a myth")
		npcsay(npc, "Or a figment of my imagination")
		npcsay(npc, "Can't remember which right now")
	end
end

local function traiborn_powerful(player, npc)
	say(player, "Teach me to be a mighty and powerful wizard")
	npcsay(npc, "Wizard, Eh?")
	npcsay(npc, "You don't want any truck with that sort")
	npcsay(npc, "They're not to be trusted")
	npcsay(npc, "That's what I've heard anyways")
	local resp = multi(player,
		"So aren't you a wizard?",
		"Oh I'd better stop talking to you then")
	if resp == 1 then
		say(player, "So aren't you a wizard")
		npcsay(npc, "How dare you?")
		npcsay(npc, "Of course I'm a wizard")
		npcsay(npc, "Now don't be so cheeky or I'll turn you into a frog")
	elseif resp == 2 then
		say(player, "Oh I'd better stop talking to you then")
		npcsay(npc, "Cheerio then")
		npcsay(npc, "Was nice chatting to you")
	end
end

local function traiborn_keys(player, npc)
	say(player, "Well, have you got any keys knocking around?")
	npcsay(npc, "Now you come to mention it - yes I do have a key")
	npcsay(npc, "Its in my special closet of valuable stuff")
	npcsay(npc, "Now how do I get into that?")
	mes(player, "The wizard scratches his head")
	delay(3)
	npcsay(npc, "I sealed it using one of my magic rituals")
	npcsay(npc, "so it would make sense that another ritual")
	npcsay(npc, "Would open it again")
	mes(player, "The wizard beams")
	delay(3)
	say(player, "So do you know what ritual to use?")
	npcsay(npc, "Let me think a second")
	npcsay(npc, "Yes a simple drazier style ritual should suffice")
	npcsay(npc, "Hmm")
	npcsay(npc, "Main problem with that is I'll need 25 sets of bones")
	npcsay(npc, "Now where am I going to get hold of something like that")
	local resp = multi(player,
		"Hmm, that's too bad. I really need that key",
		"I'll get the bones for you")
	if resp == 1 then
		say(player, "Hmm, thats too bad. I really need that key")
		npcsay(npc, "Ah well sorry I couldn't be any more help")
	elseif resp == 2 then
		say(player, "I'll get the bones for you")
		npcsay(npc, "Ooh that would very good of you")
		say(player, "Ok I'll speak to you when I've got some bones")
		setvar(player, "traiborn_stage", 1)
	end
end

local function traiborn_what_key(player, npc)
	say(player, "Just tell me if you have the key")
	npcsay(npc, "The key?")
	npcsay(npc, "The key to what?")
	npcsay(npc, "There's more than one key in the world, don't you know")
	npcsay(npc, "Would be a bit odd if there was only one")
	local resp = multi(player,
		"It's the key to get a sword called Silverlight",
		"You've lost it, haven't you?")
	if resp == 1 then
		say(player, "Its the key to get a sword called Silverlight")
		npcsay(npc, "Silverlight? Never heard of that")
		npcsay(npc, "Sounds a good name for a ship")
		npcsay(npc, "Are you sure it's not the name of a ship, rather than a sword?")
		resp = multi(player,
			"Yeah, pretty sure",
			"Well, have you got any keys knocking around?")
		if resp == 1 then
			say(player, "Yeah, pretty sure")
			npcsay(npc, "That's a pity")
			npcsay(npc, "Waste of a name")
			resp = multi(player,
				"Err I'd better be off really",
				"Well, have you got any keys knocking around?")
			if resp == 1 then
				traiborn_goodbye(player, npc)
			elseif resp == 2 then
				traiborn_keys(player, npc)
			end
		elseif resp == 2 then
			traiborn_keys(player, npc)
		end
	elseif resp == 2 then
		say(player, "You've lost it, haven't you?")
		npcsay(npc, "Me? Lose things?")
		npcsay(npc, "Thats a nasty accusation")
		traiborn_keys(player, npc)
	end
end

local function traiborn_knight(player, npc)
	say(player, "He's one of the king's knights")
	npcsay(npc, "Say, I remember a knight with a key")
	npcsay(npc, "He had nice shoes")
	npcsay(npc, "and didn't like my homemade spinach rolls")
	npcsay(npc, "Would you like a spinach roll?")
	local resp = multi(player,
		"Yes Please",
		"Just tell me if you have the key")
	if resp == 1 then
		say(player, "Yes Please")
		mes(player, "Traiborn digs around in the pockets of his robes")
		mes(player, "Traiborn hands you a spinach roll")
		give(player, "spinach roll", 1)
		say(player, "Thank you very much")
		resp = multi(player,
			"Err I'd better be off really",
			"Well, have you got any keys knocking around?")
		if resp == 1 then
			traiborn_goodbye(player, npc)
		elseif resp == 2 then
			traiborn_keys(player, npc)
		end
	elseif resp == 2 then
		traiborn_what_key(player, npc)
	end
end

local function traiborn_tell_prysin(player, npc)
	say(player, "Ok, I'll go and tell him that")
	npcsay(npc, "Oh that's great")
	npcsay(npc, "If it wouldn't be too much trouble")
	local resp = multi(player,
		"Err I'd better be off really",
		"Well, have you got any keys knocking around?")
	if resp == 1 then
		traiborn_goodbye(player, npc)
	elseif resp == 2 then
		traiborn_keys(player, npc)
	end
end

local function traiborn_irresponsible(player, npc)
	say(player, "He told me you were looking after it for him")
	npcsay(npc, "That wasn't very clever of him")
	npcsay(npc, "I'd lose my head if it wasn't screwed on proper")
	npcsay(npc, "Go tell him to find someone else")
	npcsay(npc, "to look after his valuables in future")
	local resp = multi(player,
		"Ok, I'll go and tell him that",
		"Well, have you got any keys knocking around?")
	if resp == 1 then
		traiborn_tell_prysin(player, npc)
	elseif resp == 2 then
		traiborn_keys(player, npc)
	end
end

local function traiborn_prysin(player, npc)
	say(player, "I need to get a key given to you by Sir Prysin")
	npcsay(npc, "Sir Prysin? Who's that?")
	npcsay(npc, "What would I want his key for?")
	local resp = multi(player,
		"He told me you were looking after it for him",
		"He's one of the king's knights",
		"Well, have you got any keys knocking around?")
	if resp == 1 then
		traiborn_irresponsible(player, npc)
	elseif resp == 2 then
		traiborn_knight(player, npc)
	elseif resp == 3 then
		traiborn_keys(player, npc)
	end
end

function talknpc_traiborn_the_wizard(player, npc)
	local resp

	if getvar(player, "traiborn_stage") == 1 then
		npcsay(npc, "How are you doing finding bones?")
		-- TODO path for not having any bones
		say(player, "I have some bones")
		npcsay(npc, "Give 'em here then")
		local bones = getvar(player, "traiborn_bones")
		while held(player, "bones", 1) and bones < 25 do
			mes(player, "You give Traiborn a set of bones")
			delay(4)
			remove(player, "bones", 1)
			bones = bones + 1
		end
		setvar(player, "traiborn_bones", bones)
		if bones < 25 then
			say(player, "That's all of them")
			npcsay(npc, "I still need more")
			say(player, "Ok, I'll look for some more")
			return
		end
		npcsay(npc, "Hurrah! That's all 25 sets of bones")
		mes(player, "Traiborn places the bones in a circle on the floor")
		delay(3)
		mes(player, "Traiborn waves his arms about")
		delay(3)
		npcsay(npc, "Wings of dark and colour too")
		npcsay(npc, "Spreading in the morning dew")
		mes(player, "The wizard waves his arms some more")
		delay(3)
		npcsay(npc, "Locked away I have a key")
		npcsay(npc, "Return it now unto me")
		mes(player, "Traiborn smiles")
		delay(3)
		mes(player, "Traiborn hands you a key")
		give(player, "silverlight key 1", 1)
		delay(3)
		say(player, "Thank you very much")
		npcsay(npc, "Not a problem for a friend of sir what's-his-face")
		setvar(player, "traiborn_stage", 2)
		return
	end

	npcsay(npc, "Ello young thingummywut")

	if getvar(player, "silverlight_stage") == 1 and
	    not held(player, "silverlight key 1", 1) then
		resp = multi(player,
			"Whats a thingummywut?",
			"Teach me to be a mighty and powerful wizard",
			"I need to get a key given to you by Sir Prysin")
	else
		resp = multi(player,
			"Whats a thingummywut?",
			"Teach me to be a mighty and powerful wizard")
	end

	if resp == 1 then
		traiborn_thingy(player, npc)
	elseif resp == 2 then
		traiborn_powerful(player, npc)
	elseif resp == 3 then
		traiborn_prysin(player, npc)
	end
end
