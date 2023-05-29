#include <stdio.h>
#include <stdlib.h>
#include "game_types.h"
#include "inventory.h"

Item globalItems[] = {
    {"Bread & Blood", MELEE_WEAPON, "Perfect for slicing through the soft, pillowy texture\nof freshly baked bread, but it is also a formidable weapon,\ncapable of drawing blood with just a flick of the wrist.\nThis knife is a contradiction,\na blend of the mundane and the macabre,\na tool for both nourishment and destruction.", "assets/sprites/inventory/items/breadandblood.sprite", 10, 250000, 0},
    {"Kitty Cut", MELEE_WEAPON, "This knife is a whimsical yet practical tool\nthat exudes charm and playfulness.\nBut what sets this knife apart is\nthe intricate carving of a playful cat,\nwith its tail curled around the handle\nand its paws holding onto the blade.", "assets/sprites/inventory/items/kittycut.sprite", 10, 250000, 0},
    {"Excalibur", MELEE_WEAPON, "A legendary weapon of unparalleled power and mystique,\nsteeped in centuries of myth and folklore.\nLegend has it that only the truest of heart\ncan wield Excalibur,\nand that the sword possesses the power\nto vanquish any foe,\nno matter how fearsome.", "assets/sprites/inventory/items/excalibur.sprite", 10, 250000, 0},
    {"Freyr", MELEE_WEAPON, "The sword Freyr is a weapon of mythic proportions,\nsaid to have been wielded by the Norse god of the same name.\nYou can't help but feel a sense of awe and reverence\nfor the incredible craftsmanship and divine power\nthat went into its creation.", "assets/sprites/inventory/items/freyr.sprite", 10, 450000, 0},
    {"Lightsaber", MELEE_WEAPON, "The lightsaber is a weapon unlike any other,\na weapon of the Jedi and Sith\nthat emits a blade of pure energy,\nhumming with a distinctive, high-pitched sound\nthat echoes through the air.\nWith its sleek, futuristic design\nand unparalleled cutting power,\nthe lightsaber is a symbol of mastery, skill, and the force.", "assets/sprites/inventory/items/lightsaber.sprite", 10, 250000, 0},
    {"Annabeth", MELEE_WEAPON, "Annabeth is a mace of formidable power and striking beauty.\nIt is a weapon that seems almost alive,\na part of the wielder's own body,\nand capable of delivering devastating strikes with ease.", " ", 10, 250000, 0},
    {"Spiky", MELEE_WEAPON, "Spiky is a mace of ferocious power and unrelenting brutality.\nThere is no finesse or grace to its movements,\nonly raw power and unbridled aggression.\nWielding Spiky may grant you victory on the battlefield,\nbut at what cost to your own soul?", " ", 10, 250000, 0},
    {"God Glock", RANGED_WEAPON, "God Glock is a pistol of exceptional power\nand divine precision.\nSome even say that the weapon is divinely blessed,\nguided by a higher power that ensures\nits bullets always find their mark.", " ", 10, 1000000, 0},
    {"Shy Sniper", RANGED_WEAPON, "A weapon that demands both precision and secrecy,\nwith mystical properties that makes it almost undetectable.\nWith Shy Sniper in your hand,\nyou will be a master of the art of stealth,\na true warrior who strikes from the shadows\nand disappears without a trace.", " ", 10, 250000, 0},
    {"Arabella", RANGED_WEAPON, "Arabella is a pistol of elegance and power,\na weapon that combines beauty with deadly efficiency.\nWith a classic, vintage design and exquisite details,\nthis pistol is a true work of art.\nStreaking hearts with a single shot will never be so intense.", " ", 10, 250000, 0},
    {"TNT", BOMB, "Makes noise and damage.", "assets/sprites/inventory/items/tnt.sprite", 10, 5000000, 0},
    {"Blind Gas", BOMB, "Turns off the light.", " ", 10, 5000000, 0},
    {"The Fireman's Worst Nightmare", BOMB, "Light, heat and fire: the arsonist.", " ", 10, 250000, 0},
    {"Flashbang", BOMB, "Flash and noise, dizziness and confusion.", " ", 10, 250000, 0},
    {"Portable beacon", SPECIAL, "Enlightening your path.", " ", 10, 250000, 0},
    {"Key", KEY, "This key will open special rooms\nfull with new items waiting for you!", " ", 10, 250000, 0},
    {"Rock", MISCELLANEOUS, "Collect a rock,\nhold a piece of Rogue Pointers' History!", " ", 10, 250000, 0}
};