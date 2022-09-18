#include "gdwg/graph.hpp"

#include <catch2/catch.hpp>
#include <vector>

/*
This file is to test 2.2 constructors [gdwg.ctor]
The majority of test case is based on is_node and connected to test the correct
generation of constructor
*/

TEST_CASE("Test empty constructor") {
	auto g = gdwg::graph<std::string, int>();
	CHECK(g.empty());

	CHECK(g.insert_node("A"));
	CHECK(g.insert_node("B"));

	CHECK(g.insert_edge("A", "B", 79));

	CHECK(g.empty() == false);
	CHECK(g.is_node("A"));
	CHECK(g.is_node("B"));
	CHECK(g.is_connected("A", "B"));
}

TEST_CASE("Test initializer list constructor") {
	auto g = gdwg::graph<std::string, int>{"A", "B"};

	CHECK(g.empty() == false);
	CHECK(g.is_node("A"));
	CHECK(g.is_node("B"));
	CHECK(g.is_node("C") == false);
}

TEST_CASE("Test iterator constructor") {
	auto v = std::vector<std::string>{"A", "B"};
	auto g = gdwg::graph<std::string, int>(v.begin(), v.end());

	CHECK(g.empty() == false);
	CHECK(g.is_node("A"));
	CHECK(g.is_node("B"));
	CHECK(g.is_node("C") == false);
}

TEST_CASE("Test move constructor") {
	auto g1 = gdwg::graph<std::string, int>{"A", "B"};
	auto g2 = gdwg::graph(std::move(g1));

	CHECK(g1.empty());
	CHECK(g2.empty() == false);
	CHECK(g2.is_node("A"));
	CHECK(g2.is_node("B"));
	CHECK(g2.is_node("C") == false);
}

TEST_CASE("Test move assignment") {
	auto g1 = gdwg::graph<std::string, int>{"A", "B"};
	auto g2 = gdwg::graph<std::string, int>();
	g2 = std::move(g1);

	CHECK(g1.empty());
	CHECK(g2.empty() == false);
	CHECK(g2.is_node("A"));
	CHECK(g2.is_node("B"));
	CHECK(g2.is_node("C") == false);
}

TEST_CASE("Test copy constructor") {
	auto g1 = gdwg::graph<std::string, int>{"A", "B"};
	g1.insert_edge("A", "B", 79);

	auto g2 = gdwg::graph(g1);

	CHECK(g1 == g2);

	CHECK(g1.empty() == false);
	CHECK(g2.empty() == false);
	CHECK(g1.is_node("A"));
	CHECK(g1.is_node("B"));
	CHECK(g2.is_node("A"));
	CHECK(g2.is_node("B"));

	CHECK(g1.is_connected("A", "B"));
	CHECK(g2.is_connected("A", "B"));

	CHECK(g1.is_node("C") == false);
	CHECK(g1.is_node("C") == false);
}

TEST_CASE("Test copy assignment") {
	auto g1 = gdwg::graph<std::string, int>{"A", "B"};
	auto g2 = gdwg::graph<std::string, int>();
	g2 = g1;

	CHECK(g1 == g2);

	CHECK(g1.empty() == false);
	CHECK(g2.empty() == false);
	CHECK(g1.is_node("A"));
	CHECK(g1.is_node("B"));
	CHECK(g2.is_node("A"));
	CHECK(g2.is_node("B"));

	CHECK(g1.is_node("C") == false);
	CHECK(g1.is_node("C") == false);
}