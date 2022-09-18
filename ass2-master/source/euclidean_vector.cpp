// Copyright (c) Christopher Di Bella.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
#include <comp6771/euclidean_vector.hpp>

namespace comp6771 {

	euclidean_vector::euclidean_vector() {
		dimension_ = 1;
		magnitude_ = std::make_unique<double[]>(1);
		magnitude_[0] = 0.0;
	}

	euclidean_vector::euclidean_vector(int dimension) {
		this->dimension_ = dimension;
		this->magnitude_ = std::make_unique<double[]>(static_cast<std::size_t>(dimension));
	}

	euclidean_vector::euclidean_vector(int dimension, double magnitude) {
		this->dimension_ = dimension;
		this->magnitude_ = std::make_unique<double[]>(static_cast<std::size_t>(dimension));
		std::fill_n(this->magnitude_.get(), dimension, magnitude);
	}

	euclidean_vector::euclidean_vector(const std::vector<double>::const_iterator begin,
	                                   const std::vector<double>::const_iterator end) {
		this->dimension_ = static_cast<int>(end - begin);
		this->magnitude_ = std::make_unique<double[]>(static_cast<std::size_t>(this->dimension_));
		std::copy_n(begin, this->dimension_, this->magnitude_.get());
	}
	euclidean_vector::euclidean_vector(const std::initializer_list<double> list) {
		this->dimension_ = static_cast<int>(list.size());
		this->magnitude_ = std::make_unique<double[]>(static_cast<std::size_t>(this->dimension_));
		std::copy_n(list.begin(), this->dimension_, this->magnitude_.get());
	}

	euclidean_vector::euclidean_vector(const euclidean_vector& orig) {
		this->dimension_ = orig.dimension_;
		this->magnitude_ = std::make_unique<double[]>(static_cast<std::size_t>(orig.dimension_));
		std::copy_n(orig.magnitude_.get(), orig.dimension_, this->magnitude_.get());
	}

	euclidean_vector::euclidean_vector(euclidean_vector&& origin) noexcept {
		// NOLINTNEXTLINE(modernize-avoid-c-arrays)
		magnitude_ = std::exchange(origin.magnitude_, nullptr);
		dimension_ = std::exchange(origin.dimension_, 0);
	}
	// operator
	euclidean_vector& euclidean_vector::operator=(euclidean_vector const& rhs) {
		*this = euclidean_vector(rhs);
		return *this;
	}

	euclidean_vector& euclidean_vector::operator=(euclidean_vector&& rhs) noexcept {
		magnitude_ = std::move(rhs.magnitude_);
		dimension_ = rhs.dimension_;
		rhs.dimension_ = 0;
		return *this;
	}

	double euclidean_vector::operator[](int i) const noexcept {
		assert(i >= 0 || i < dimension_);
		return at(i);
	}

	double& euclidean_vector::operator[](int i) noexcept {
		assert(i >= 0 || i < dimension_);
		return at(i);
	}

	euclidean_vector euclidean_vector::operator+() noexcept {
		auto temp_vector = this->get_magnitude();
		euclidean_vector res = euclidean_vector(temp_vector.begin(), temp_vector.end());
		return res;
	}
	euclidean_vector euclidean_vector::operator-() noexcept {
		auto temp_vector = this->get_magnitude();
		for (auto& val : temp_vector) {
			val *= -1;
		}
		euclidean_vector res = euclidean_vector(temp_vector.begin(), temp_vector.end());
		return res;
	}

	euclidean_vector& euclidean_vector::operator+=(euclidean_vector const& rhs) {
		if (this->dimensions() != rhs.dimensions()) {
			throw "Dimension of LHS(X) and RHS(Y) does not match";
		}
		for (int i = 0; i < rhs.dimensions(); ++i) {
			this->magnitude_[static_cast<std::size_t>(i)] += rhs.magnitude_[static_cast<std::size_t>(i)];
		}
		return *this;
	}

	euclidean_vector& euclidean_vector::operator-=(euclidean_vector const& rhs) {
		if (this->dimensions() != rhs.dimensions()) {
			throw "Dimension of LHS(X) and RHS(Y) does not match";
		}
		for (int i = 0; i < rhs.dimensions(); ++i) {
			this->magnitude_[static_cast<std::size_t>(i)] -= rhs.magnitude_[static_cast<std::size_t>(i)];
		}
		return *this;
	}

	euclidean_vector& euclidean_vector::operator*=(double rhs) noexcept {
		for (int i = 0; i < dimension_; ++i) {
			this->magnitude_[static_cast<std::size_t>(i)] *= rhs;
		}
		return *this;
	}

	euclidean_vector& euclidean_vector::operator/=(const double n) {
		if (n == 0) {
			throw "Invalid vector division by 0";
		}

		for (int i = 0; i < this->dimensions(); ++i) {
			this->magnitude_[static_cast<std::size_t>(i)] /= n;
		}
		return *this;
	}

	euclidean_vector::operator std::vector<double>() const noexcept {
		std::vector<double> vec;
		vec.reserve(static_cast<std::size_t>(dimension_));
		for (int i = 0; i < this->dimension_; ++i) {
			vec.emplace_back(at(i));
		}
		return vec;
	}

	euclidean_vector::operator std::list<double>() const noexcept {
		std::list<double> lis;
		for (int i = 0; i < this->dimension_; ++i) {
			lis.emplace_back(at(i));
		}
		return lis;
	}

	// Memeber
	[[nodiscard]] auto euclidean_vector::at(int index) const -> double {
		if (index < 0 || index >= dimension_) {
			auto throw_string =
			   "Index " + std::to_string(index) + " is not valid for this euclidean_vector object";
			throw euclidean_vector_error(throw_string);
		}
		return magnitude_[static_cast<std::size_t>(index)];
	}
	auto euclidean_vector::at(int index) -> double& {
		if (index < 0 || index >= dimension_) {
			auto throw_string =
			   "Index " + std::to_string(index) + " is not valid for this euclidean_vector object";
			throw euclidean_vector_error(throw_string);
		}
		return magnitude_[static_cast<std::size_t>(index)];
	}
	[[nodiscard]] auto euclidean_vector::dimensions() const noexcept -> int {
		return dimension_;
	}

	// friend

	// utility function
	auto euclidean_norm(euclidean_vector const& v) -> double {
		if (v.dimensions() == 0)
			return 0;
		double count = 0.0;
		auto vector = v.get_magnitude();
		for (auto mag : vector) {
			count += std::pow(mag, 2);
		}
		return std::sqrt(count);
	}

	auto unit(euclidean_vector const& v) -> euclidean_vector {
		if (v.dimensions() == 0) {
			throw "euclidean_vector with no dimensions does not have a unit vector";
		}
		auto norm = euclidean_norm(v);
		if (norm == 0) {
			throw "euclidean_vector with zero euclidean normal does not have a unit vector";
		}
		auto mag = v.get_magnitude();
		auto res = v.get_magnitude();
		for (auto& temp : res) {
			temp /= norm;
		}
		return euclidean_vector(res.begin(), res.end());
	}

	auto dot(euclidean_vector const& x, euclidean_vector const& y) -> double {
		if (x.dimensions() != y.dimensions()) {
			throw "Dimensions of LHS(X) and RHS(Y) do not match";
		}
		auto x_vec = x.get_magnitude();
		auto y_vec = y.get_magnitude();
		auto result = 0.0;

		for (size_t i = 0; i < static_cast<size_t>(x.dimensions()); i++) {
			result += x_vec[i] * y_vec[i];
		}
		return result;
	}

	// helper
	auto euclidean_vector::get_magnitude() const -> std::vector<double> {
		int len = dimensions();
		auto res = std::vector<double>();
		res.reserve(static_cast<size_t>(len));
		for (int i = 0; i < len; ++i) {
			auto temp = at(i);
			res.push_back(temp);
		}
		return res;
	}
	auto euclidean_vector::get_magnitude() -> std::vector<double> {
		int len = dimensions();
		auto res = std::vector<double>();
		res.reserve(static_cast<size_t>(len));
		for (int i = 0; i < len; ++i) {
			auto temp = at(i);
			res.push_back(temp);
		}
		return res;
	}

} // namespace comp6771