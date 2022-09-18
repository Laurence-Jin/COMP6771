#include "gdwg/graph.hpp"

#include <catch2/catch.hpp>
#include <vector>

/*
This file is to test all the function in 2.3 modifiers[gdwg.modifiers]
such as insert_node and replace_node with many different way and also erase edge
applied iterator.
*/

TEST_CASE("Test Insert Node") {
	auto g1 = gdwg::graph<std::string, int>();
	g1.insert_node("hello");
	g1.insert_node("how");
	g1.insert_node("are");
	g1.insert_node("you?");
	CHECK(g1.nodes() == std::vector<std::string>{"are", "hello", "how", "you?"});
	CHECK(!g1.insert_node("hello"));
}

TEST_CASE("Test insert edge") {
	auto g1 = gdwg::graph<int, int>{23, 59, 71};
	CHECK(g1.insert_edge(59, 23, 7));
	CHECK(g1.insert_edge(59, 59, 7));
	CHECK(!g1.insert_edge(59, 59, 7));

	CHECK_THROWS(g1.insert_edge(119, 23, 7));
	CHECK_THROWS_WITH(g1.insert_edge(59, 99, 7),
	                  "Cannot call gdwg::graph<N, E>::insert_edge when either src or dst node "
	                  "does not exist");
}

TEST_CASE("Test clear") {
	auto g1 = gdwg::graph<int, int>{23, 59, 71};

	g1.insert_edge(59, 23, 7);
	g1.insert_edge(23, 59, 6);

	g1.clear();
	CHECK(g1.empty());
}

TEST_CASE("Test erase node") {
	auto g1 = gdwg::graph<int, int>{23, 59, 71};

	g1.insert_edge(59, 23, 7);
	g1.insert_edge(23, 59, 6);
	g1.insert_edge(23, 59, 5);
	g1.insert_edge(59, 23, 8);

	CHECK(g1.erase_node(23));
	CHECK(!g1.erase_node(283));

	CHECK_THROWS_WITH(g1.is_connected(23, 59),
	                  "Cannot call gdwg::graph<N, E>::is_connected if src or dst "
	                  "node don't exist in the graph");

	CHECK(g1.nodes() == std::vector<int>{59, 71});
	CHECK(g1.connections(59).empty());
	CHECK(g1.connections(71).empty());
}

TEST_CASE("Test erase_edge") {
	auto g1 = gdwg::graph<int, int>{23, 31, 71};

	g1.insert_edge(31, 23, 7);
	g1.insert_edge(23, 31, 6);
	g1.insert_edge(23, 31, 5);
	g1.insert_edge(31, 23, 8);

	CHECK(g1.erase_edge(31, 23, 8));
	CHECK(!g1.erase_edge(31, 23, 889));
	CHECK_THROWS_WITH(g1.erase_edge(31, 99, 2),
	                  "Cannot call gdwg::graph<N, E>::erase_edge on src or dst if "
	                  "they don't exist in the graph");
}

TEST_CASE("Test erase edge iterator") {
	auto g1 = gdwg::graph<int, int>{23, 31, 71};

	g1.insert_edge(31, 23, 7);
	g1.insert_edge(23, 31, 6);
	g1.insert_edge(23, 31, 5);
	g1.insert_edge(31, 23, 8);

	auto res = g1.erase_edge(g1.begin());
	CHECK((*res).from == 23);
	CHECK((*res).to == 31);
	CHECK((*res).weight == 6);

	res = g1.erase_edge(res);
	CHECK((*res).from == 31);
	CHECK((*res).to == 23);
	CHECK((*res).weight == 7);

	CHECK(g1.connections(23).empty());

	res = g1.erase_edge(res);
	CHECK((*res).from == 31);
	CHECK((*res).to == 23);
	CHECK((*res).weight == 8);

	CHECK(g1.weights(31, 23) == std::vector<int>{8});

	res = g1.erase_edge(res);
	CHECK(res == g1.end());
}

TEST_CASE("Test erase edge two iterators") {
	SECTION("erase edge applied begin and end") {
		auto g1 = gdwg::graph<std::string, int>{"B", "A", "C"};

		g1.insert_edge("A", "B", 7);
		g1.insert_edge("B", "A", 6);
		g1.insert_edge("B", "A", 5);
		g1.insert_edge("A", "B", 8);

		auto res = g1.erase_edge(g1.begin(), g1.end());
		CHECK(res == g1.end());

		CHECK(g1.connections("B").empty());
		CHECK(g1.connections("A").empty());
	}

	SECTION("erase edge applied begin and ++") {
		auto g2 = gdwg::graph<int, int>{23, 31, 71};
		g2.insert_edge(31, 23, 7);
		g2.insert_edge(23, 31, 6);
		g2.insert_edge(23, 31, 5);
		g2.insert_edge(31, 23, 8);

		auto it = g2.begin();
		it++;
		it++;

		g2.erase_edge(g2.begin(), it);

		CHECK(g2.connections(23).empty());
		CHECK(g2.weights(31, 23) == std::vector<int>{7, 8});
	}
}

TEST_CASE("Test replace node") {
	auto g1 = gdwg::graph<std::string, int>{"B", "A", "C"};

	g1.insert_edge("A", "B", 7);
	g1.insert_edge("B", "A", 6);
	g1.insert_edge("B", "A", 5);
	g1.insert_edge("A", "B", 8);
	g1.insert_edge("B", "B", 9);

	CHECK_THROWS(g1.replace_node("D", "D"));

	CHECK(g1.replace_node("B", "D"));

	CHECK(g1.weights("D", "A") == std::vector<int>{5, 6});
	CHECK(g1.weights("A", "D") == std::vector<int>{7, 8});
	CHECK(g1.weights("D", "D") == std::vector<int>{9});
}

TEST_CASE("Test merge and replace node") {
	SECTION("With reflexive edge") {
		auto g2 = gdwg::graph<std::string, int>{"B", "A", "C"};
		g2.insert_edge("B", "B", 7);
		g2.insert_edge("A", "A", 7);

		g2.merge_replace_node("B", "A");

		CHECK(g2.weights("A", "A") == std::vector<int>{7});
	}

	SECTION("Without reflexive edge") {
		auto g1 = gdwg::graph<std::string, int>{"B", "A", "C"};

		g1.insert_edge("A", "B", 7);
		g1.insert_edge("B", "A", 6);
		g1.insert_edge("B", "A", 5);
		g1.insert_edge("A", "B", 8);

		CHECK_THROWS(g1.merge_replace_node("A", "D"));
		CHECK_THROWS(g1.merge_replace_node("D", "B"));

		g1.merge_replace_node("B", "A");

		CHECK(g1.weights("A", "A") == std::vector<int>{5, 6, 7, 8});

		g1.merge_replace_node("A", "C");

		CHECK(g1.weights("C", "C") == std::vector<int>{5, 6, 7, 8});
	}

	SECTION("Test merge and replace node with string and int") {
		auto g3 = gdwg::graph<std::string, int>{"A", "B", "C", "D"};
		g3.insert_edge("A", "B", 1);
		g3.insert_edge("A", "C", 2);
		g3.insert_edge("A", "D", 3);
		g3.insert_edge("B", "B", 1);

		g3.merge_replace_node("A", "B");

		CHECK(g3.weights("B", "B") == std::vector<int>{1});
		CHECK(g3.weights("B", "C") == std::vector<int>{2});
		CHECK(g3.weights("B", "D") == std::vector<int>{3});
	}
}