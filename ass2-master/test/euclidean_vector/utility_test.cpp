#include <catch2/catch.hpp>
#include <comp6771/euclidean_vector.hpp>

TEST_CASE("Euclidean Norm") {
	auto norm_list = {1.0, 2.0, 3.0};
	auto e_vector = comp6771::euclidean_vector(norm_list);
	CHECK(comp6771::euclidean_norm(e_vector) == sqrt(14));
}

TEST_CASE("Unit") {
	auto norm_list = {1.0, 2.0, 3.0};
	auto unit_vector = comp6771::euclidean_vector(norm_list);
	auto e_vector = comp6771::euclidean_vector(norm_list);
	unit_vector /= sqrt(14);
	CHECK(comp6771::unit(e_vector) == unit_vector);
	CHECK_THROWS_WITH(comp6771::unit(comp6771::euclidean_vector()),
	                  "euclidean_vector with zero euclidean normal does not have a unit vector");
}

TEST_CASE("Dot Product") {
	auto norm_list = {1.0, 2.0, 3.0};
	auto e_vector = comp6771::euclidean_vector(norm_list);
	auto dot_vector = comp6771::euclidean_vector({1, 2});
	auto dot_vector2 = comp6771::euclidean_vector({3, 4});
	CHECK(comp6771::dot(dot_vector, dot_vector2) == 11);
	CHECK_THROWS_WITH(comp6771::dot(dot_vector, e_vector),
	                  "Dimensions of LHS(X) and RHS(Y) do not match");
}