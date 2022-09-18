#include "gdwg/graph.hpp"

#include <catch2/catch.hpp>
#include <vector>

/*
This part is to test Comparisons [gdwg.cmp] and Extractor [gdwg.io]
*/

TEST_CASE("Test extractor") {
	using graph = gdwg::graph<int, double>;
	auto const v = std::vector<graph::value_type>{
	   {4, 1, -48.8},
	   {3, 2, 2.0},
	   {2, 4, 2.0},
	   {2, 1, 1.8},
	   {6, 2, 5.0},
	   {6, 3, 10.9},
	   {1, 5, -1.7},
	   {3, 6, -8.9},
	   {4, 5, 3.0},
	   {5, 2, 7.0},
	};

	auto g = graph{};
	for (const auto& [from, to, weight] : v) {
		g.insert_node(from);
		g.insert_node(to);
		g.insert_edge(from, to, weight);
	}

	g.insert_node(64);
	auto out = std::ostringstream{};
	out << g;
	auto const expected_output = std::string_view(R"(1 (
  5 | -1.7
)
2 (
  1 | 1.8
  4 | 2
)
3 (
  2 | 2
  6 | -8.9
)
4 (
  1 | -48.8
  5 | 3
)
5 (
  2 | 7
)
6 (
  2 | 5
  3 | 10.9
)
64 (
)
)");
	CHECK(out.str() == expected_output);
}

TEST_CASE("Comparisons  test") {
	auto g1 = gdwg::graph<int, std::string>{};
	auto g2 = gdwg::graph<int, std::string>{46, 47, 1231, 0};
	auto g3 = gdwg::graph<int, std::string>{46, 47, 1231, 0};

	CHECK(g1 == g1);
	CHECK(g2 == g2);
	CHECK(g2 == g3);

	g2.insert_edge(46, 0, "UNSW");
	CHECK(g2 != g3);

	g3.insert_edge(46, 0, "UNSW");
	CHECK(g2 == g3);
}