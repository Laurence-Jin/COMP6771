#include <comp6771/word_ladder.hpp>
#include <catch2/catch.hpp>

//Test several word ladders with simple short test

TEST_CASE("Single shortest word ladders") {
	std::unordered_set<std::string> const lexicon{
		"hot", "hat", "her", "cat", "dad", "dog", "dot"};

	std::vector<std::vector<std::string>> expected_result = {{"hot", "dot", "dog"}};
	CHECK(word_ladder::generate("hot", "dog", lexicon) == expected_result);
}

TEST_CASE("The shortest word ladder ignored the longer path but in order") {
	//Discard the longer path with the word met before: hot->hog->dog->dot
	std::unordered_set<std::string> const lexicon{
		"hot", "her", "hog", "dog", "dot", "cog", "hat"};

	std::vector<std::vector<std::string>> expected_result = {{"hot", "dot"}};
	CHECK(word_ladder::generate("hot", "dot", lexicon) == expected_result);
}

TEST_CASE("Two same shortest word ladders") {
	std::unordered_set<std::string> const lexicon{
		"hit", "hot", "dot", "dog", "lot", "log", "cog"};

	std::vector<std::vector<std::string>> expected_result = {{"hit", "hot", "dot", "dog", "cog"},
															 {"hit", "hot", "lot", "log", "cog"}};

	CHECK(word_ladder::generate("hit", "cog", lexicon) == expected_result);
}

TEST_CASE("Two same shortest word ladders in lexicographical order") {
	std::unordered_set<std::string> const lexicon{
		"cat", "cap", "cop", "con", "can", "cot"};

	std::vector<std::vector<std::string>> expected_result = {{"cat", "can", "con"},
															 {"cat", "cot", "con"}};

	CHECK(word_ladder::generate("cat", "con", lexicon) == expected_result);
}
