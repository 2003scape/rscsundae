local function guide_thankyou(player, npc)
	say(player, "You have been a great help, thankyou")
	npcsay(npc, "Go and use what you have learnt")
	npcsay(npc, "Soon you will be swarming the dungeons with the boldest")
end

local guide_avoid_dying, guide_attacking

local function guide_strength(player, npc)
	say(player, "What is my strength, and how can I improve it?")
	npcsay(
		npc,
		"When you fight, your hit level is displayed in green over your head"
	)
	npcsay(npc, "This is your hit level, as in the statistics box")
	npcsay(npc, "When you get injured, this will get lower. It will rise again")
	npcsay(npc, "if you stay out of trouble. Eating will also help it return")
	npcsay(npc, "As you win more fights, your hit level will increase")

	local resp = multi(
		player,
		"How can I avoid dying?",
		"How can I see who is best to attack?",
		"You have been a great help, thankyou"
	)

	if resp == 1 then
		guide_avoid_dying(player, npc)
	elseif resp == 2 then
		guide_attacking(player, npc)
	elseif resp == 3 then
		guide_thankyou(player, npc)
	end
end

guide_avoid_dying = function(player, npc)
	say(player, "How can I avoid dying?")
	npcsay(npc, "To start with, the castle area is safe")
	npcsay(npc, "If you venture out, check your map for other creatures close")
	npcsay(npc, "And if you think they may attack, move away")
	npcsay(
		npc,
		"You will find which creatures and people attack, and which don't"
	)
	npcsay(npc, "If you get attacked, try to run back where you came from")
	npcsay(npc, "most creatures won't chase you outside their own area")

	local resp = multi(
		player,
		"How can I see who is best to attack?",
		"What is my strength, and how can I improve it?",
		"You have been a great help, thankyou"
	)

	if resp == 1 then
		guide_attacking(player, npc)
	elseif resp == 2 then
		guide_strength(player, npc)
	elseif resp == 3 then
		guide_thankyou(player, npc)
	end
end

guide_attacking = function(player, npc)
	say(player, "How can I see who is best to attack?")
	npcsay(
		npc,
		"When you put the mouse over a character, see if you can attack."
	)
	npcsay(npc, "If the attack choice is red then it will be hard to win.")
	npcsay(
		npc,
		"If the attack is green You should win unless you are already injured,"
	)
	npcsay(npc, "Try the goblins over the bridge, most people can beat them")
	npcsay(
		npc,
		"Just be sure you are wearing your armour and wielding your best weapon"
	)

	local resp = multi(
		player,
		"How can I avoid dying?",
		"What is my strength, and how can I improve it?",
		"You have been a great help, thankyou"
	)

	if resp == 1 then
		guide_avoid_dying(player, npc)
	elseif resp == 2 then
		guide_strength(player, npc)
	elseif resp == 3 then
		guide_thankyou(player, npc)
	end
end

local function guide_go_kill(player, npc)
	say(player, "Thanks, I just want to go kill things now")
	npcsay(npc, "Okay, just take care of yourself.")
	npcsay(npc, "However tough you get, there is always something tougher")
end

local guide_warrior_upgrades, guide_warrior_choice

local function guide_warrior_skills(player, npc)
	say(player, "What other skills will help me fight well?")
	npcsay(npc, "If you really want to be a good warrior,")
	npcsay(npc, "you will find other skills are useful.")
	delay(3)
	npcsay(npc, "If you improve your magic you will find many battle spells ")
	delay(3)
	npcsay(npc, "Cooking is also a good skill to have.")
	npcsay(npc, "As feeding yourself will make you stronger when you need it.")
	delay(2)
	npcsay(
		npc,
		"If you can make your own weapons then you will find it much cheaper."
	)

	local resp = multi(
		player,
		"How can I improve my weapons and my armour?",
		"How can I choose who is best to fight?",
		"How can I avoid dying?",
		"Thanks, I just want to go kill things now"
	)

	if resp == 1 then
		guide_warrior_upgrades(player, npc)
	elseif resp == 2 then
		guide_warrior_choice(player, npc)
	elseif resp == 3 then
		guide_avoid_dying(player, npc)
	elseif resp == 4 then
		guide_go_kill(player, npc)
	end
end

guide_warrior_choice = function(player, npc)
	say(player, "How can I choose who is best to fight?")
	npcsay(npc, "Be careful that you do not fight anyone too strong. ")
	npcsay(npc, "The Palace guards around here are very tough for beginners,")
	npcsay(
		npc,
		"They will probably kill you instantly. Better to fight weaker people."
	)
	npcsay(
		npc,
		"There are some goblins just over the bridge that are not too tough"
	)
	delay(5)
	npcsay(npc, "Mind you, some goblins are strong. Always check the strength")
	npcsay(
		npc,
		"You can always run away, providing you have time to before you die."
	)

	local resp = multi(
		player,
		"How can I improve my weapons and my armour?",
		"What other skills will help me fight well?",
		"How can I avoid dying?",
		"Thanks, I just want to go kill things now"
	)

	if resp == 1 then
		guide_warrior_upgrades(player, npc)
	elseif resp == 2 then
		guide_warrior_skills(player, npc)
	elseif resp == 3 then
		guide_avoid_dying(player, npc)
	elseif resp == 4 then
		guide_go_kill(player, npc)
	end
end

guide_warrior_upgrades = function(player, npc)
	say(player, "How can I improve my weapons and my armour?")
	npcsay(
		npc,
		"To get better armour and weapons you will need to visit the stores."
	)
	npcsay(
		npc,
		"In Varrock there are many weapon and armour makers who will sell to you."
	)
	npcsay(npc, "They will also buy any weapons and armour you do not need, ")
	npcsay(npc, "so keep any you want to sell.")

	local resp = multi(
		player,
		"How can I choose who is best to fight?",
		"What other skills will help me fight well?",
		"How can I avoid dying?",
		"Thanks, I just want to go kill things now"
	)

	if resp == 1 then
		guide_warrior_choice(player, npc)
	elseif resp == 2 then
		guide_warrior_skills(player, npc)
	elseif resp == 3 then
		guide_avoid_dying(player, npc)
	elseif resp == 4 then
		guide_go_kill(player, npc)
	end
end

local function guide_warrior(player, npc)
	say(player, "How do I become a better warrior?")
	npcsay(npc, "To be a good warrior takes practice, and good equipment.")
	npcsay(npc, "Skill is only gained by fighting.")
	npcsay(npc, "The more you win fights, the better you will become.")
	npcsay(
		npc,
		"The tougher the fights you win, the quicker you will progress."
	)

	local resp = multi(
		player,
		"How can I improve my weapons and my armour?",
		"How can I choose who is best to fight?",
		"What other skills will help me fight well?",
		"How can I avoid dying?"
	)

	if resp == 1 then
		guide_warrior_upgrades(player, npc)
	elseif resp == 2 then
		guide_warrior_choice(player, npc)
	elseif resp == 3 then
		guide_warrior_skills(player, npc)
	elseif resp == 4 then
		guide_avoid_dying(player, npc)
	end
end

local guide_why_fight, guide_speak_others

local function guide_attack_others(player, npc)
	say(player, "Should I attack the other players?")
	npcsay(npc, "To be fair, you cannot attack just any of the other players")
	npcsay(
		npc,
		"You can fight with people about the same strength as you, and anyone tougher"
	)
	npcsay(npc, "If you die, you lose everything in your back pack, ")
	npcsay(npc, "so do not be persuaded to attack any player who asks you to")
	npcsay(
		npc,
		"They will just be after your treasure. You can't run away immediately"
	)

	local resp = multi(
		player,
		"Why would I want to fight?",
		"Would I benefit from speaking to other players?",
		"How do I become a better warrior?",
		"How can I avoid dying?"
	)

	if resp == 1 then
		guide_why_fight(player, npc)
	elseif resp == 2 then
		guide_speak_others(player, npc)
	elseif resp == 3 then
		guide_warrior(player, npc)
	elseif resp == 4 then
		guide_avoid_dying(player, npc)
	end
end

guide_speak_others = function(player, npc)
	say(player, "Would I benefit from speaking to other players?")
	npcsay(
		npc,
		"You can type to speak to other players. Anyone close to you will hear."
	)
	npcsay(
		npc,
		"Just be careful about paying for player advice, Its not usually worth it."
	)
	npcsay(
		npc,
		"You can spot the real players because you can trade directly with them."
	)
	npcsay(
		npc,
		"Other players will have knowledge of the game, many will be happy to help"
	)

	local resp = multi(
		player,
		"Why would I want to fight?",
		"Should I attack the other players?",
		"How do I become a better warrior?",
		"How can I avoid dying?"
	)

	if resp == 1 then
		guide_why_fight(player, npc)
	elseif resp == 2 then
		guide_attack_others(player, npc)
	elseif resp == 3 then
		guide_warrior(player, npc)
	elseif resp == 4 then
		guide_avoid_dying(player, npc)
	end
end

guide_why_fight = function(player, npc)
	say(player, "Why would I want to fight?")
	npcsay(npc, "If you fight when you can win, and search everywhere you go, ")
	npcsay(npc, "you will find your strength and treasure will increase.")
	npcsay(npc, "Many characters drop treasure as they die.")

	local resp = multi(
		player,
		"Would I benefit from speaking to other players?",
		"Should I attack the other players?",
		"How do I become a better warrior?",
		"How can I avoid dying?"
	)

	if resp == 1 then
		guide_speak_others(player, npc)
	elseif resp == 2 then
		guide_attack_others(player, npc)
	elseif resp == 3 then
		guide_warrior(player, npc)
	elseif resp == 4 then
		guide_avoid_dying(player, npc)
	end
end

local guide_quests, guide_creatures

local function guide_places(player, npc)
	say(player, "What are the best places to go to?")
	npcsay(npc, "The city of Varrock is the main trading and living place.")
	npcsay(npc, "It is where you can get anything you can afford,")
	npcsay(npc, "but be careful, there are thieves and worse in Varrock")
	npcsay(npc, "Eventually, all roads lead to the city.")
	delay(5)
	npcsay(
		npc,
		"There is a farm on the way to Varrock, just don't annoy the farmer!"
	)
	npcsay(npc, "There is a store near here that can sell you basic items.")
	npcsay(
		npc,
		"The store is always open, so you can buy when you need something"
	)
	delay(5)
	npcsay(
		npc,
		"There is a church, A mill, a graveyard, and a few other places to go "
	)
	npcsay(
		npc,
		"very close to here. You should find something useful in most of them"
	)

	local resp = multi(
		player,
		"Are there any quests near here, and what will I need?",
		"What are all the people and creatures around here? What do they do?",
		"How do I become a better warrior?",
		"How can I avoid dying?"
	)

	if resp == 1 then
		guide_quests(player, npc)
	elseif resp == 2 then
		guide_creatures(player, npc)
	elseif resp == 3 then
		guide_warrior(player, npc)
	elseif resp == 4 then
		guide_avoid_dying(player, npc)
	end
end

guide_quests = function(player, npc)
	say(player, "Are there any quests near here, and what will I need?")
	npcsay(
		npc,
		"A quest is simply a task to help someone or to prove your strength"
	)
	npcsay(npc, "There are a few quests near here to be done.")
	npcsay(
		npc,
		"If you complete the quests you can gain experience and treasure"
	)
	npcsay(npc, "To find a quest, talk to anyone you can.")
	npcsay(npc, "There are some items about that will help you too.")

	local resp = multi(
		player,
		"What are the best places to go to?",
		"What are all the people and creatures around here? What do they do?",
		"How do I become a better warrior?",
		"How can I avoid dying?"
	)

	if resp == 1 then
		guide_places(player, npc)
	elseif resp == 2 then
		guide_creatures(player, npc)
	elseif resp == 3 then
		guide_warrior(player, npc)
	elseif resp == 4 then
		guide_avoid_dying(player, npc)
	end
end

guide_creatures = function(player, npc)
	say(player, "What are all the people and creatures around here?")
	say(player, "What do they do?")
	npcsay(npc, "Some characters here are other games players,")
	npcsay(npc, "Some are creatures and people that live in runescape")
	npcsay(
		npc,
		"You can talk and trade and fight with most of them, if you want to"
	)

	local resp = multi(
		player,
		"Why would I want to fight?",
		"Would I benefit from speaking to other players?",
		"Should I attack the other players?",
		"How do I become a better warrior?"
	)

	if resp == 1 then
		guide_why_fight(player, npc)
	elseif resp == 2 then
		guide_speak_others(player, npc)
	elseif resp == 3 then
		guide_attack_others(player, npc)
	elseif resp == 4 then
		guide_warrior(player, npc)
	end
end

function talknpc_guide(player, npc)
	local resp

	npcsay(npc, "Hello Adventurer, can I guide you on your journeys?")

	resp = multi(
		player,
		"I just got here, where should I start?",
		"I just want to fight, how can I be a good warrior?",
		"I keep getting killed, its annoying me. What should I do?",
		"I am happy to just try things on my own, thanks"
	)

	if resp == 1 then
		say(player, "I just got here, where should I start?")
		npcsay(npc, "You are at Lumbridge castle. ")
		npcsay(
			npc,
			"There are not many creatures close to here that will attack you,"
		)
		npcsay(npc, "but there are always some.")
		npcsay(
			npc,
			"When you go further away from the castle you will meet tougher"
		)
		npcsay(
			npc,
			"and stronger creatures, some will attack you if you go too close."
		)

		resp = multi(
			player,
			"Are there any quests near here, and what will I need?",
			"What are the best places to go to?",
			"What are all the people and creatures around here? What do they do?",
			"How do I become a better warrior?"
		)

		if resp == 1 then
			guide_quests(player, npc)
		elseif resp == 2 then
			guide_places(player, npc)
		elseif resp == 3 then
			guide_creatures(player, npc)
		elseif resp == 4 then
			guide_warrior(player, npc)
		end
	elseif resp == 2 then
		guide_warrior(player, npc)
	elseif resp == 3 then
		say(player, "I keep getting killed, its annoying me. What should I do?")
		npcsay(
			npc,
			"You are probably attacking people that are too strong for you."
		)
		npcsay(npc, "Or going too near to dangerous creatures")

		resp = multi(
			player,
			"How can I avoid dying?",
			"How can I see who is best to attack?",
			"What is my strength, and how can I improve it?",
			"You have been a great help, thankyou"
		)

		if resp == 1 then
			guide_avoid_dying(player, npc)
		elseif resp == 2 then
			guide_attacking(player, npc)
		elseif resp == 3 then
			guide_strength(player, npc)
		elseif resp == 4 then
			guide_thankyou(player, npc)
		end
	elseif resp == 4 then
		say(player, "I am happy to just try things on my own, thanks")
		npcsay(npc, "I wish you luck on your travels, adventurer")
		npcsay(npc, "I am usually here if you think I can help you")
	end
end
