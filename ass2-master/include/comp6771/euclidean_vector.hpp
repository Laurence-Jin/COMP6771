#ifndef COMP6771_EUCLIDEAN_VECTOR_HPP
#define COMP6771_EUCLIDEAN_VECTOR_HPP

#include <cassert>
#include <cmath>
#include <experimental/iterator>
#include <iostream>
#include <list>
#include <memory>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

namespace comp6771 {
	class euclidean_vector_error : public std::runtime_error {
	public:
		explicit euclidean_vector_error(std::string const& what)
		: std::runtime_error(what) {}
	};

	class euclidean_vector {
	public:
		/// Constructor
		euclidean_vector();
		explicit euclidean_vector(int);
		euclidean_vector(int, double);
		euclidean_vector(std::vector<double>::const_iterator, std::vector<double>::const_iterator);
		euclidean_vector(std::initializer_list<double>);
		euclidean_vector(euclidean_vector const&);

		euclidean_vector(euclidean_vector&&) noexcept;

		~euclidean_vector() = default;
		// Operation
		euclidean_vector& operator=(euclidean_vector const& rhs);
		euclidean_vector& operator=(euclidean_vector&& rhs) noexcept;
		double operator[](int i) const noexcept;
		double& operator[](int i) noexcept;
		euclidean_vector operator+() noexcept;
		euclidean_vector operator-() noexcept;
		euclidean_vector& operator+=(euclidean_vector const& rhs);
		euclidean_vector& operator-=(euclidean_vector const& rhs);
		euclidean_vector& operator*=(const double rhs) noexcept;
		euclidean_vector& operator/=(const double rhs);

		explicit operator std::vector<double>() const noexcept;
		explicit operator std::list<double>() const noexcept;

		// Memeber function
		[[nodiscard]] auto at(int index) const -> double;
		auto at(int index) -> double&;
		[[nodiscard]] auto dimensions() const noexcept -> int;

		// friend

		friend bool operator==(euclidean_vector const& a, euclidean_vector const& b) noexcept {
			if (a.dimensions() != b.dimensions())
				return false;
			for (int i = 0; i < a.dimensions(); ++i) {
				if (a.at(i) != b.at(i))
					return false;
			}
			return true;
		}
		friend bool operator!=(euclidean_vector const& a, euclidean_vector const& b) noexcept {
			if (a.dimensions() != b.dimensions())
				return true;
			for (int i = 0; i < a.dimensions(); ++i) {
				if (a.at(i) != b.at(i))
					return true;
			}
			return false;
		}
		friend euclidean_vector operator+(euclidean_vector const& a, euclidean_vector const& b) {
			if (a.dimensions() != b.dimensions()) {
				throw "Dimension of LHS(X) and RHS(Y) does not match";
			}
			auto ev = comp6771::euclidean_vector(a.dimensions());
			for (int i = 0; i < ev.dimension_; ++i) {
				ev.at(i) = a.at(i) + b.at(i);
			}
			return ev;
		}
		friend euclidean_vector operator-(euclidean_vector const& a, euclidean_vector const& b) {
			if (a.dimensions() != b.dimensions()) {
				throw "Dimension of LHS(X) and RHS(Y) does not match";
			}
			auto ev = comp6771::euclidean_vector(a.dimensions());
			for (int i = 0; i < ev.dimension_; ++i) {
				ev.at(i) = a.at(i) - b.at(i);
			}
			return ev;
		}
		friend euclidean_vector operator*(euclidean_vector const& o, double n) noexcept {
			auto ev = comp6771::euclidean_vector(o.dimensions());
			for (int i = 0; i < ev.dimension_; ++i) {
				ev.at(i) = n * o.at(i);
			}
			return ev;
		}
		friend euclidean_vector operator*(double n, euclidean_vector const& o) noexcept {
			auto ev = comp6771::euclidean_vector(o.dimensions());
			for (int i = 0; i < ev.dimension_; ++i) {
				ev.at(i) = n * o.at(i);
			}
			return ev;
		}
		friend euclidean_vector operator/(euclidean_vector const& o, double n) {
			if (n == 0) {
				throw("Invalid vector division by 0");
			}
			auto ev = comp6771::euclidean_vector(o.dimensions());
			for (int i = 0; i < ev.dimension_; ++i) {
				ev.at(i) = o.at(i) / n;
			}
			return ev;
		}

		friend std::ostream& operator<<(std::ostream& os, euclidean_vector const& ev) noexcept {
			auto vec = ev.get_magnitude();
			os << "[";
			auto i = 0;
			for (auto value : vec) {
				if (i == 0) {
					os << value;
				}
				else {
					os << " " << value;
				}
				i++;
			}
			os << "]";
			return os;
		}

		// helper
		// NOLINTNEXTLINE(modernize-avoid-c-arrays)
		auto get_magnitude() const -> std::vector<double>;
		// NOLINTNEXTLINE(modernize-avoid-c-arrays)
		auto get_magnitude() -> std::vector<double>;

		friend auto euclidean_norm(euclidean_vector const& v) -> double;
		friend auto dot(euclidean_vector const& x, euclidean_vector const& y) -> double;

	private:
		// ass2 spec requires we use std::unique_ptr<double[]>
		// NOLINTNEXTLINE(modernize-avoid-c-arrays)
		std::unique_ptr<double[]> magnitude_;
		int dimension_;
	};
	// Utility function
	auto euclidean_norm(euclidean_vector const& v) -> double;
	auto unit(euclidean_vector const& v) -> euclidean_vector;
	auto dot(euclidean_vector const& x, euclidean_vector const& y) -> double;
} // namespace comp6771
#endif // COMP6771_EUCLIDEAN_VECTOR_HPP
