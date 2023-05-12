// SPDX-License-Identifier: GPL-3.0-or-later

#include "random-words.h"

#include <array>
#include <cstdlib>
#include <sstream>

static std::array<std::string, 52> words {
	"zenith",
	"zealot",
	"yearn",
	"yawner",
	"xenophobia",
	"x-axis",
	"wonky",
	"wanton",
	"vermillion",
	"vague",
	"unique",
	"uncanny",
	"tenacious",
	"tangible",
	"serene",
	"saquinavir",
	"rhetorical",
	"rambunctious",
	"quixotic",
	"quell",
	"pique",
	"paradigm",
	"oxymoron",
	"optimistically",
	"nostalgic",
	"narrative",
	"misanthrope",
	"melancholy",
	"lucid",
	"lethargic",
	"ken",
	"karma",
	"jurisdiction",
	"jejune",
	"irony",
	"integrity",
	"hypnosis",
	"hyperbole",
	"guise",
	"gallivant",
	"fortitude",
	"fervent",
	"esoteric",
	"empathy",
	"dubious",
	"disposition",
	"cynical",
	"capricious",
	"bypass",
	"benevolent",
	"ambiguous",
	"alliteration"
};

std::string random_words(size_t count) {
	std::ostringstream words_out;
	while (count--) words_out << words[rand() % words.size()] << ' ';
	return words_out.str();
}
