#include "gdwg/graph.hpp"

#include <catch2/catch.hpp>
#include <vector>

/*
This part is to 2.4 test accessor [gdwg.accessors]
I can't test time complexity but test the other case as far as I am concerned
also like some throw case
*/
TEST_CASE("Test if it is Node") {
	auto g1 = gdwg::graph<std::string, std::string>{std::initializer_list<std::string>{"A", "B"}};

	CHECK(g1.is_node("A"));
	CHECK(!g1.is_node("Z"));
}

TEST_CASE("Test graph empty") {
	auto g = gdwg::graph<int, std::string>();
	auto g1 = gdwg::graph<int, std::string>{46, 47, 48, 0};

	CHECK(g.empty());
	CHECK(!g1.empty());
}

TEST_CASE("Test if two nodes are connected") {
	auto g1 = gdwg::graph<int, int>{56, 31, 71};

	g1.insert_edge(31, 56, 7);
	g1.insert_edge(56, 31, 6);
	g1.insert_edge(56, 31, 5);
	g1.insert_edge(31, 56, 8);

	CHECK(g1.is_connected(56, 31));
	CHECK(!g1.is_connected(56, 56));

	// should be false as throw it away
	CHECK_THROWS(g1.is_connected(24536, 31));
	CHECK_THROWS_WITH(g1.is_connected(56, 99234),
	                  "Cannot call gdwg::graph<N, E>::is_connected if src or dst "
	                  "node don't exist in the graph");
}

TEST_CASE("Test nodes function") {
	SECTION("Test ascending order of node vector with vector input constructor") {
		auto v1 = std::vector<int>{0, 46, 77, 98, 78798};
		auto g1 = gdwg::graph<int, std::string>(v1.begin(), v1.end());
		CHECK(g1.nodes() == v1);
	}
	SECTION("Test ascending order of node vector with list input constructor") {
		auto v = std::vector<int>{0, 46, 77, 98};
		auto g = gdwg::graph<int, std::string>{46, 77, 98, 0};
		CHECK(v == g.nodes());
		CHECK(g.nodes() == g.nodes());
	}
}

TEST_CASE("Test weights function") {
	auto g1 = gdwg::graph<int, int>{56, 31, 71};

	g1.insert_edge(31, 56, 7);
	g1.insert_edge(56, 31, 6);
	g1.insert_edge(56, 31, 5);
	g1.insert_edge(31, 56, 8);
	g1.insert_edge(31, 31, 9);
	g1.insert_edge(31, 31, 10);

	CHECK(g1.weights(56, 31) == std::vector<int>{5, 6});
	CHECK(g1.weights(31, 56) == std::vector<int>{7, 8});
	CHECK(g1.weights(31, 31) == std::vector<int>{9, 10});

	CHECK_THROWS_WITH(g1.weights(31, 99),
	                  "Cannot call gdwg::graph<N, E>::weights if src or dst node don't exist in "
	                  "the graph");
}

TEST_CASE("Test find function") {
	auto g1 = gdwg::graph<int, int>{56, 31, 71};

	g1.insert_edge(31, 56, 7);
	g1.insert_edge(56, 31, 6);
	g1.insert_edge(56, 31, 5);
	g1.insert_edge(31, 56, 8);

	CHECK(g1.find(56, 31, 5) != g1.begin());
	CHECK(g1.find(31, 56, 7) == g1.begin());
	CHECK(g1.find(99, 99, 99) == g1.end());
}

TEST_CASE("Test connections function") {
	auto g1 = gdwg::graph<int, int>{56, 31, 71};

	g1.insert_edge(31, 56, 7);
	g1.insert_edge(56, 31, 6);
	g1.insert_edge(56, 31, 5);
	g1.insert_edge(31, 56, 8);
	g1.insert_edge(31, 31, 9);
	g1.insert_edge(31, 31, 10);

	CHECK(g1.connections(31) == std::vector<int>{31, 56});
	CHECK(g1.connections(56) == std::vector<int>{31});

	CHECK_THROWS_WITH(g1.connections(3245),
	                  "Cannot call gdwg::graph<N, E>::connections if src doesn't exist in the "
	                  "graph");
}