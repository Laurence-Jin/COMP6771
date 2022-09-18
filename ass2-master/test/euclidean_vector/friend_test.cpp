#include <catch2/catch.hpp>
#include <comp6771/euclidean_vector.hpp>

TEST_CASE("Check equal and not equal") {
	auto e_vector = comp6771::euclidean_vector(6, 10);
	auto e_vector_copy = comp6771::euclidean_vector(6, 10);
	auto e_vector_diff = comp6771::euclidean_vector(6, 10);
	e_vector_diff[0] = 0.0;
	CHECK(e_vector == e_vector_copy);
	CHECK(e_vector != e_vector_diff);
}

TEST_CASE("Check addition") {
	auto e_vector = comp6771::euclidean_vector(6, 10);
	auto two = comp6771::euclidean_vector(5, 2.0);
	auto addition = two + two;
	auto four = comp6771::euclidean_vector(5, 4.0);
	CHECK(addition == four);
	CHECK_NOTHROW(addition = two + two);
	CHECK_THROWS_WITH(addition = two + e_vector, "Dimensions of LHS(X) and RHS(Y) do not match");
}

TEST_CASE("Check Subtraction") {
	auto e_vector = comp6771::euclidean_vector(6, 10);
	auto five = comp6771::euclidean_vector(5, 5.0);
	auto ten = comp6771::euclidean_vector(5, 10.0);
	auto subtraction = ten - five;
	CHECK(subtraction == five);
	CHECK_NOTHROW(subtraction = ten - five);
	CHECK_THROWS_WITH(subtraction = five - e_vector, "Dimensions of LHS(X) and RHS(Y) do not match");
}

TEST_CASE("Check Multiply") {
	auto five = comp6771::euclidean_vector(5, 5.0);
	auto ten = comp6771::euclidean_vector(5, 10.0);
	auto multiply = five * 2;
	CHECK(multiply == ten);
}

TEST_CASE("Check Divide") {
	auto ten = comp6771::euclidean_vector(5, 10.0);
	auto two = comp6771::euclidean_vector(5, 2.0);
	auto division = ten / 5;
	CHECK(division == two);
	CHECK_NOTHROW(division = ten / 5);
	CHECK_THROWS_WITH(division = two / 0, "Invalid vector division by 0");
}

TEST_CASE("Displaying vectors in text form") {
	WHEN("You create an empty vector") {
		comp6771::euclidean_vector a{};
		comp6771::euclidean_vector b{std::move(a)};
		std::cout << a;
		std::stringstream ss;
		ss << a;
		CHECK(ss.str() == "[]");
	}
	WHEN("You create a non-empty vector") {
		std::vector<double> v{-2.3, 1.4, 5.9};
		comp6771::euclidean_vector a{v.begin(), v.end()};
		std::cout << a;
		std::stringstream ss;
		ss << a;
		CHECK(ss.str() == "[-2.3 1.4 5.9]");
	}
}
