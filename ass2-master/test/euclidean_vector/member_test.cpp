#include <catch2/catch.hpp>
#include <comp6771/euclidean_vector.hpp>

TEST_CASE("at") {
	auto init_list = {33.2, 24.9, 85.3, 78.4, 45.0, 183.6};
	auto ev = comp6771::euclidean_vector(init_list);
	auto const const_e_vector = comp6771::euclidean_vector(init_list);
	CHECK(ev.at(0) == 33.2);
	ev.at(0) = 0.0;
	CHECK(ev.at(0) == 0.0);
	CHECK_THROWS_WITH(ev.at(-1), "Index X is not valid for this euclidean_vector object");
	CHECK_THROWS_WITH(ev.at(10), "Index X is not valid for this euclidean_vector object");
}

TEST_CASE("Dimension") {
	auto init_list = {33.2, 24.9, 85.3, 78.4, 45.0, 183.6};
	auto ev = comp6771::euclidean_vector(init_list);
	auto mag = ev.get_magnitude();
	CHECK(ev.dimensions() == static_cast<int>(mag.size()));
	CHECK_NOTHROW(ev.dimensions());
}