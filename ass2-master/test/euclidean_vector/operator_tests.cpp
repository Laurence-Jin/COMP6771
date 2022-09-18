#include <catch2/catch.hpp>
#include <comp6771/euclidean_vector.hpp>

TEST_CASE("Copy assignment") {
	std::vector<double> v1{10, 13, 2, 7, 3};
	auto ev1 = comp6771::euclidean_vector(v1.begin(), v1.end());

	std::vector<double> v2{-20, 0, 13};
	auto ev2 = comp6771::euclidean_vector(v2.begin(), v2.end());

	ev2 = ev1;
	CHECK(ev2.get_magnitude() == ev1.get_magnitude());
	CHECK(ev2.dimensions() == ev1.dimensions());
}

TEST_CASE("Move assignment") {
	std::vector<double> v1{1, 2, 7, 3};
	auto ev1 = comp6771::euclidean_vector(v1.begin(), v1.end());

	std::vector<double> v2{-20, 0, 3};
	auto ev2 = comp6771::euclidean_vector(v2.begin(), v2.end());

	ev2 = std::move(ev1);
	CHECK(ev2.dimensions() == 4);
	CHECK(ev2.get_magnitude() == v1);
	CHECK(ev1.dimensions() == 0);
}

TEST_CASE("Access/Modify a magnitude by subscript operator") {
	std::vector<double> v1{4.8, 2.2, 7.3, 3.1};
	auto ev = comp6771::euclidean_vector(v1.begin(), v1.end());
	CHECK(ev.dimensions() == 4);
	double m = ev[1];
	CHECK(m == 2.2);

	// modify
	ev[1] = 0;
	CHECK(ev.at(1) == 0);
}

TEST_CASE("Unary Plus Assignment") {
	auto ev = comp6771::euclidean_vector(5, 22.0);
	auto unary_plus = +ev;
	CHECK(unary_plus.get_magnitude() == ev.get_magnitude());
	CHECK(unary_plus.dimensions() == ev.dimensions());
}

TEST_CASE("Negation Assignment") {
	auto ev = comp6771::euclidean_vector(5, 22.0);
	auto negation = -ev;
	auto mag = ev.get_magnitude(); // used to negation and compare - overloading
	for (auto& temp : mag) {
		temp *= -1;
	}
	CHECK(negation.get_magnitude() == mag);
	CHECK(static_cast<size_t>(negation.dimensions()) == mag.size());
}

TEST_CASE("Adding Operator") {
	auto a = comp6771::euclidean_vector(4, 4.6);
	auto b = comp6771::euclidean_vector(4, 5.4);
	auto test = comp6771::euclidean_vector(10, 32.0);
	auto b_copy = b;
	a += b;
	auto adding_check = std::vector<double>{10, 10, 10, 10};
	CHECK(a.get_magnitude() == adding_check);
	CHECK(a.dimensions() == b_copy.dimensions());
	CHECK(b.get_magnitude() == b_copy.get_magnitude());
	CHECK_THROWS_WITH(a += test, "Dimension of LHS(X) and RHS(Y) does not match");
	CHECK_NOTHROW(a += b);
}

TEST_CASE("Subtracting Operator") {
	auto a = comp6771::euclidean_vector(4, 30.0);
	auto b = comp6771::euclidean_vector(4, 15.0);
	auto test = comp6771::euclidean_vector(10, 32.0);
	auto b_copy = b;
	a -= b;
	auto subtracting_check = std::vector<double>{15, 15, 15, 15};
	CHECK(a.get_magnitude() == subtracting_check);
	CHECK(a.dimensions() == b_copy.dimensions());
	CHECK(b.get_magnitude() == b_copy.get_magnitude());
	CHECK_THROWS_WITH(a -= test, "Dimension of LHS(X) and RHS(Y) does not match");
	CHECK_NOTHROW(a -= b);
}

TEST_CASE("Multiplying Operator Overload") {
	auto multi = comp6771::euclidean_vector(4, 2.0);
	multi *= 5.0;
	auto multiplying_check = std::vector<double>{10, 10, 10, 10};
	;
	CHECK(multi.get_magnitude() == multiplying_check);
	CHECK(multi.dimensions() == 4);
}

TEST_CASE("Dividing Operator Overload") {
	auto divid = comp6771::euclidean_vector(4, 20.0);
	divid /= 2.0;
	auto multiplying_check = std::vector<double>{10, 10, 10, 10};
	;
	CHECK(divid.get_magnitude() == multiplying_check);
	CHECK(divid.dimensions() == 4);
	CHECK_THROWS_WITH(divid /= 0, "Invalid vector division by 0");
}

TEST_CASE("Vector Conversion") {
	auto vec = static_cast<std::vector<double>>(comp6771::euclidean_vector(4, 10.0));
	auto vector_check = std::vector<double>{10, 10, 10, 10};
	CHECK(vec == vector_check);
	CHECK_NOTHROW(static_cast<std::vector<double>>(comp6771::euclidean_vector(4, 10.0)));
}

TEST_CASE("List Conversion") {
	auto list = static_cast<std::list<double>>(comp6771::euclidean_vector(4, 10.0));
	auto list_check = std::list<double>{10, 10, 10, 10};
	CHECK(list == list_check);
	CHECK_NOTHROW(static_cast<std::list<double>>(comp6771::euclidean_vector(4, 10.0)));
}