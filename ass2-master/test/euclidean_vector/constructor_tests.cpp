#include <catch2/catch.hpp>
#include <comp6771/euclidean_vector.hpp>

TEST_CASE("Default constructor") {
	auto const constructor = comp6771::euclidean_vector();
	auto default_vector = std::vector<double>();
	default_vector.push_back(0.0);
	CHECK(constructor.get_magnitude() == default_vector);
	CHECK(constructor.dimensions() == 1);
}

TEST_CASE("Single-argument constructor") {
	auto const constructor1 = comp6771::euclidean_vector(4);
	int dimen = 4;
	auto vector = std::vector<double>();
	vector.reserve(5);
	for (int i = 0; i < dimen; ++i) {
		vector.push_back(0.0);
	}
	CHECK(constructor1.get_magnitude() == vector);
	CHECK(constructor1.dimensions() == 4);
	CHECK_NOTHROW(comp6771::euclidean_vector(dimen));
}

TEST_CASE("Int and double argument constructor") {
	auto const constructor = comp6771::euclidean_vector(3, 4.0);
	CHECK(constructor.dimensions() == 3);
	for (int i = 0; i < 3; i++)
		CHECK(constructor.at(i) == 4.0);
}

TEST_CASE("Begin and end iterator argument constructor") {
	auto v = std::vector<double>{5.0, 6.5, 7.0};
	auto constructor = comp6771::euclidean_vector(v.begin(), v.end());
	CHECK(constructor.at(0) == 5.0);
	CHECK(constructor.at(1) == 6.5);
	CHECK(constructor.at(2) == 7.0);
	CHECK(constructor.dimensions() == 3);
}

TEST_CASE("Initializer list argument constructor") {
	auto init_list = {97.2, 835.6, 488.4, 5263.1, 664.0, 7.6};
	auto constructor = comp6771::euclidean_vector(init_list);
	auto init_vector = static_cast<std::vector<double>>(init_list);
	CHECK(constructor.get_magnitude() == init_vector);
	CHECK(constructor.dimensions() == 6);
}

TEST_CASE("Copy constructor") {
	auto init_list = {97.2, 835.6, 488.4, 5263.1, 664.0, 7.6};
	auto init_vector = static_cast<std::vector<double>>(init_list);
	auto init_list_constructor = comp6771::euclidean_vector(init_list);
	auto copy_constructor = comp6771::euclidean_vector(init_list_constructor);
	CHECK(copy_constructor.get_magnitude() == init_vector);
	CHECK(copy_constructor.dimensions() == 6);
}

TEST_CASE("Move constructor") {
	auto v = std::vector<double>{5.0, 6.5, 7.0};
	auto constructor_1 = comp6771::euclidean_vector(v.begin(), v.end());
	auto constructor_2 = comp6771::euclidean_vector(v.begin(), v.end());
	auto move_constructor = comp6771::euclidean_vector(std::move(constructor_1));
	CHECK(move_constructor.get_magnitude() == v);
	CHECK(move_constructor.dimensions() == 3);
	CHECK_NOTHROW(comp6771::euclidean_vector(std::move(constructor_2)));
}