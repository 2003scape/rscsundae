-- pcaps/Revisionism/06-15-2018 04.23.22 goblin diplomacy.pcap

register_useinv("bluedye", "light blue goblin armour", function(player)
	remove(player, "bluedye", 1)
	remove(player, "light blue goblin armour", 1)
	mes(player, "You dye the goblin armour blue")
	give(player, "dark blue goblin armour", 1)
end)


register_useinv("orangedye", "light blue goblin armour", function(player)
	remove(player, "orangedye", 1)
	remove(player, "light blue goblin armour", 1)
	mes(player, "You dye the goblin armour Orange")
	give(player, "orange goblin armour", 1)
end)

