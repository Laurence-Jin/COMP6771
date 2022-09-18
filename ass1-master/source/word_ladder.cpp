#include <comp6771/word_ladder.hpp>

// using namespace std;
namespace word_ladder {
	auto ladder_comparator(std::vector<std::string> v1, std::vector<std::string> v2) -> bool {
		for (size_t i = 0; i < v1.size(); ++i)
			if (v1[i] != v2[i])
				return v1[i] < v2[i];
		return false;
	}

	[[nodiscard]] auto generate(std::string const& from,
	                            std::string const& to,
	                            std::unordered_set<std::string> const& lexicon)
	   -> std::vector<std::vector<std::string>> {
		(void)from;
		(void)to;
		(void)lexicon;

		std::unordered_set<std::string> dictList = {lexicon.begin(), lexicon.end()}, wordList;
		// The word has been traversed and its corresponding the shortest path
		std::unordered_map<std::string, std::vector<std::vector<std::string>>> map;
		std::vector<std::vector<std::string>> ans;
		std::queue<std::string> que; /*the word in cur layer, be pair with visited, only layer
		detemine the answer, so the explicit path is neglected */
		bool flag = false;

		// only store the same length as word ladder
		for (auto itr = dictList.begin(); itr != dictList.end(); ++itr)
			if (size(*itr) == size(from))
				wordList.emplace(*itr);

		if (wordList.find(to) == wordList.end())
			return {};
		// we insert from into queue and map, then delete it
		wordList.erase(from);
		// initialise with inserting from
		que.push({from});
		map.insert({from, {{from}}});
		while (!que.empty()) {
			std::unordered_set<std::string> visited; // need to delete the word in list
			for (int size = que.size(); size > 0; --size) {
				std::string cur = que.front();
				que.pop(); // each word in a layer
				std::string next = cur;
				for (int i = 0; i < next.length(); ++i) {
					char temp = next[i]; // preserve the string of word
					for (char ch = 'a'; ch <= 'z'; ++ch) {
						if (temp == ch)
							continue;
						next[i] = ch;
						if (next == to) { // find the shortest path
							flag = true;
							// All words in path need to add to answer
							for (auto& path : map[cur]) {
								path.emplace_back(to);
								ans.emplace_back(path);
							}
						}
						// if don't find the shortest path
						else if (!flag && wordList.find(next) != wordList.end()) {
							visited.insert(next);
							for (auto path : map[cur]) {
								path.emplace_back(next);
								if (map.count(next))
									map[next].emplace_back(path);
								else {
									map[next] = {path};
									que.push(next);
								}
							}
						}
					}
					next[i] = temp; // reinstate the word
				}
				map.erase(cur);
			}
			if (flag) {
				std::sort(ans.begin(), ans.end(), ladder_comparator);
				return ans;
			}
			for (auto word = visited.begin(); word != visited.end(); word++)
				wordList.erase(*word);
			if (wordList.empty())
				return {};
		}
		std::sort(ans.begin(), ans.end(), ladder_comparator);
		return ans;
	}
} // namespace word_ladder
