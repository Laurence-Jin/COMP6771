#include <comp6771/word_ladder.hpp>
#include <catch2/catch.hpp>
//This test case loaded from english.txt
TEST_CASE("word ladders from english.txt") {
	auto const lexicon = word_ladder::read_lexicon("english.txt");

    std::vector<std::vector<std::string>> expected_result =
    {{"hate", "have", "hove", "love"},
    {"hate", "have", "lave", "love"},
    {"hate", "late", "lave", "love"}};

	CHECK(word_ladder::generate("hate", "love", lexicon) == expected_result);
}

TEST_CASE("Sample serveral word ladders") {
	auto const lexicon = word_ladder::read_lexicon("english.txt");

    std::vector<std::vector<std::string>> expected_result =
    {{"work", "fork", "form", "foam", "flam", "flay", "play"},
    {"work", "pork", "perk", "peak", "pean", "plan", "play"},
    {"work", "pork", "perk", "peak", "peat", "plat", "play"},
    {"work", "pork", "perk", "pert", "peat", "plat", "play"},
    {"work", "pork", "porn", "pirn", "pian", "plan", "play"},
    {"work", "pork", "port", "pert", "peat", "plat", "play"},
    {"work", "word", "wood", "pood", "plod", "ploy", "play"},
    {"work", "worm", "form", "foam", "flam", "flay", "play"},
    {"work", "worn", "porn", "pirn", "pian", "plan", "play"},
    {"work", "wort", "bort", "boat", "blat", "plat", "play"},
    {"work", "wort", "port", "pert", "peat", "plat", "play"},
    {"work", "wort", "wert", "pert", "peat", "plat", "play"}};

	CHECK(word_ladder::generate("work", "play", lexicon) == expected_result);
}