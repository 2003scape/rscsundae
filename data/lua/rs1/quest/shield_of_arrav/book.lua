-- RSC 2001/7/iteminfo- book- (shield of arrav)- read- 25% quest.pcap
-- RSC 2001/7/scenery- varrock- palace- bookshelf- 0% arrav quest- search- a collection.pcap
-- RSC 2001/7/scenery- varrock- palace- bookshelf- 25% arrav quest- search- obtain book.pcap

function opinv_the_shield_of_arrav(player)
	mes(player, "The shield of Arrav")
	delay(4)
	mes(player, "By A.R.Wright")
	delay(4)
	mes(player, "Arrav is probably the best known hero of the 4th age.")
	delay(4)
	mes(player, "One surviving artifact from the 4th age is a fabulous shield.")
	delay(4)
	mes(player, "This shield is believed to have once belonged to Arrav")
	delay(4)
	mes(player, "And is now indeed known as the shield of Arrav.")
	delay(4)
	mes(player, "For 150 years it was the prize piece in the royal museum of Varrock.")
	delay(4)
	mes(player, "However in the year 143 of the 5th age")
	delay(4)
	mes(player, "A gang of thieves called the phoenix gang broke into the museum")
	delay(4)
	mes(player, "And stole the shield.")
	delay(4)
	mes(player, "King Roald the VII put a 1200 gold reward on the return on the shield.")
	delay(4)
	mes(player, "The thieves who stole the shield")
	delay(4)
	mes(player, "Have now become the most powerful crime gang in Varrock.")
	delay(4)
	mes(player, "The reward for the return of the shield still stands.")
end

function oploc2_questbookcase(player)
	local stage = getvar(player, "arrav_stage")
	if stage == 1 and not held(player, "the shield of arrav", 1) then
		say(player, "Aha the shield of Arrav")
		say(player, "That was what I was looking for")
		mes(player, "You take the book from the bookcase")
		give(player, "the shield of arrav", 1)
		setvar(player, "arrav_stage", 2)
	else
		mes(player, "A large collection of books")
	end
end
