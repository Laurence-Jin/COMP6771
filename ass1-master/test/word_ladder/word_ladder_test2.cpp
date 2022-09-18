#include <comp6771/word_ladder.hpp>
#include <catch2/catch.hpp>

//These testcases is to detect empty word ladder as base case

TEST_CASE("No word ladder with disconnected path") {
	std::unordered_set<std::string> const lexicon{"hot", "hat", "her", "cat", "dad", "dog"};
    auto const ladders = word_ladder::generate("hot", "dog", lexicon);

	CHECK(std::size(ladders) == 0);
}

TEST_CASE("No 'to' word in lexicon") {
	std::unordered_set<std::string> const lexicon{"hot", "hat", "her", "cat", "dad", "dog"};
    auto const ladders = word_ladder::generate("hot", "dot", lexicon);

	CHECK(std::size(ladders) == 0);
}

TEST_CASE("No word ladder in lexicon of english.txt") {
	auto const english_lexicon = word_ladder::read_lexicon("english.txt");
	auto const ladders = word_ladder::generate("airplane", "tricycle", english_lexicon);

	CHECK(std::size(ladders) == 0);
}
