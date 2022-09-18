#include "gdwg/graph.hpp"

#include <catch2/catch.hpp>
#include <vector>

/*
This file is to test 2.5 Iterator access [gdwg.iterator.access]
which is only begin and end
*/
TEST_CASE("Test iterator constructor") {
	SECTION("iterator constructor with list constructor") {
		auto g1 = gdwg::graph<std::string, int>{"A", "B", "C", "D"};
		g1.insert_edge("A", "B", 1);
		g1.insert_edge("A", "B", 2);
		g1.insert_edge("B", "A", 3);
		g1.insert_edge("B", "A", 6);

		auto a = g1.begin();
		a = g1.end();
	}

	SECTION("iterator constructor with default graph Constructors ") {
		auto g2 = gdwg::graph<int, int>();
		auto a = g2.begin();
		a = g2.end();
	}
}

TEST_CASE("Test iterator increment") {
	auto g1 = gdwg::graph<std::string, int>{"A", "B", "C", "D"};

	g1.insert_edge("A", "B", 1);
	g1.insert_edge("A", "B", 2);
	g1.insert_edge("B", "A", 3);
	g1.insert_edge("B", "A", 6);
	g1.insert_edge("A", "B", 9);

	int count = 0;
	for (auto i = g1.begin(); i != g1.end(); i++) {
		switch (count) {
		case 0:
			CHECK((*i).from == "A");
			CHECK((*i).to == "B");
			CHECK((*i).weight == 1);
			break;
		case 1:
			CHECK((*i).from == "A");
			CHECK((*i).to == "B");
			CHECK((*i).weight == 2);
			break;
		case 2:
			CHECK((*i).from == "A");
			CHECK((*i).to == "B");
			CHECK((*i).weight == 9);
			break;
		case 3:
			CHECK((*i).from == "B");
			CHECK((*i).to == "A");
			CHECK((*i).weight == 3);
			break;
		case 4:
			CHECK((*i).from == "B");
			CHECK((*i).to == "A");
			CHECK((*i).weight == 6);
			break;
		}
		count++;
	}

	CHECK(count == 5);
}

TEST_CASE("Test iterator decrement") {
	auto g1 = gdwg::graph<std::string, int>{"A", "B", "C"};

	g1.insert_edge("B", "A", 7);
	g1.insert_edge("A", "B", 6);
	g1.insert_edge("A", "B", 5);
	g1.insert_edge("B", "A", 8);

	int count = 0;
	auto i = g1.end();
	while (i != g1.begin()) {
		--i;
		switch (count) {
		case 0:
			CHECK((*i).from == "B");
			CHECK((*i).to == "A");
			CHECK((*i).weight == 8);
			break;
		case 1:
			CHECK((*i).from == "B");
			CHECK((*i).to == "A");
			CHECK((*i).weight == 7);
			break;
		case 2:
			CHECK((*i).from == "A");
			CHECK((*i).to == "B");
			CHECK((*i).weight == 6);
			break;
		case 3:
			CHECK((*i).from == "A");
			CHECK((*i).to == "B");
			CHECK((*i).weight == 5);
			break;
		}
		count++;
	}

	CHECK(count == 4);
}

TEST_CASE("Test iterator general") {
	auto g1 = gdwg::graph<std::string, int>{"A", "B", "C"};

	g1.insert_edge("B", "A", 7);
	g1.insert_edge("A", "B", 6);
	g1.insert_edge("A", "B", 5);
	g1.insert_edge("B", "A", 8);

	int count = 0;
	for (auto i = g1.end(); i != g1.begin();) {
		--i;
		switch (count) {
		case 0:
			CHECK((*i).from == "B");
			CHECK((*i).to == "A");
			CHECK((*i).weight == 8);
			break;
		case 1:
			CHECK((*i).from == "B");
			CHECK((*i).to == "A");
			CHECK((*i).weight == 7);
			break;
		case 2:
			CHECK((*i).from == "A");
			CHECK((*i).to == "B");
			CHECK((*i).weight == 6);
			break;
		case 3:
			CHECK((*i).from == "A");
			CHECK((*i).to == "B");
			CHECK((*i).weight == 5);
			break;
		}
		count++;
	}

	CHECK(count == 4);
	count = 0;
	for (auto i = g1.begin(); i != g1.end(); i++) {
		switch (count) {
		case 0:
			CHECK((*i).from == "A");
			CHECK((*i).to == "B");
			CHECK((*i).weight == 5);
			break;
		case 1:
			CHECK((*i).from == "A");
			CHECK((*i).to == "B");
			CHECK((*i).weight == 6);
			break;
		case 2:
			CHECK((*i).from == "B");
			CHECK((*i).to == "A");
			CHECK((*i).weight == 7);
			break;
		case 3:
			CHECK((*i).from == "B");
			CHECK((*i).to == "A");
			CHECK((*i).weight == 8);
			break;
		}
		count++;
	}

	CHECK(count == 4);
}

TEST_CASE("Test iterator equal") {
	auto g1 = gdwg::graph<std::string, int>{"A", "B"};
	auto g2 = gdwg::graph<std::string, int>();

	CHECK(g1.begin() == g1.begin());
	CHECK(g1.begin() == g1.end());

	CHECK(g2.begin() == g2.end());
}