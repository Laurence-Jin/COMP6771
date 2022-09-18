#ifndef GDWG_GRAPH_HPP
#define GDWG_GRAPH_HPP

#include <algorithm>
#include <cassert>
#include <experimental/iterator>
#include <initializer_list>
#include <iostream>
#include <iterator>
#include <map>
#include <memory>
#include <set>
#include <sstream>
#include <tuple>
#include <utility>
#include <vector>

namespace gdwg {
	template<typename N, typename E>
	class graph {
	public:
		struct value_type {
			N from;
			N to;
			E weight;
		};

		class iterator;
		// Iterator access
		[[nodiscard]] auto begin() const -> iterator {
			return iterator(edgeList_, 1);
		}
		[[nodiscard]] auto end() const -> iterator {
			return iterator(edgeList_, 0);
		}

		// constructor
		graph() noexcept {
			nodeList_ = std::set<std::unique_ptr<N>>();
			edgeList_ = std::map<std::unique_ptr<N>, std::set<Edge>, nodeCmp>();
		}

		graph(std::initializer_list<N> il) {
			nodeList_ = std::set<std::unique_ptr<N>>();
			edgeList_ = std::map<std::unique_ptr<N>, std::set<Edge>, nodeCmp>();
			for (auto itr = il.begin(); itr != il.end(); ++itr) {
				this->insert_node(*itr);
			}
		}
		template<typename InputIt>
		graph(InputIt first, InputIt last) {
			std::for_each(first, last, [this](N const& i) { insert_node(i); });
		}

		graph(graph&& other) noexcept {
			nodeList_ = std::move(other.nodeList_);
			edgeList_ = std::move(other.edgeList_);
		}

		auto operator=(graph&& other) noexcept -> graph& {
			std::swap(nodeList_, other.nodeList_);
			std::swap(edgeList_, other.edgeList_);
			other.nodeList_ = std::set<std::unique_ptr<N>>();
			other.edgeList_ = std::map<std::unique_ptr<N>, std::set<Edge>, nodeCmp>();
			return *this;
		}

		graph(graph const& other) {
			nodeList_ = std::set<std::unique_ptr<N>>();
			edgeList_ = std::map<std::unique_ptr<N>, std::set<Edge>, nodeCmp>();
			for (auto itr : other.nodes()) {
				this->insert_node(itr);
			}
			for (auto const& g : other.edgeList_) {
				edgeList_.emplace(std::make_unique<N>(*(g.first)), g.second);
			}
		}

		auto operator=(graph const& other) -> graph& {
			auto new_graph = graph(other);
			std::swap(nodeList_, new_graph.nodeList_);
			std::swap(edgeList_, new_graph.edgeList_);
			return *this;
		}

		// modifier
		auto insert_node(N const& value) -> bool {
			if (is_node(value)) {
				return false;
			}
			nodeList_.insert(std::make_unique<N>(value));
			return true;
		}

		auto insert_edge(N const& src, N const& dst, E const& weight) -> bool {
			if (!is_node(src) || !is_node(dst)) {
				throw std::runtime_error("Cannot call gdwg::graph<N, E>::insert_edge "
				                         "when either src or dst node does not exist");
			}
			auto found_src = find_node_ptr(src);
			auto found_dst = find_node_ptr(dst);

			auto set_edge = edgeList_[std::make_unique<N>(*found_src)];
			auto it = std::find_if(set_edge.begin(), set_edge.end(), [dst, weight](Edge const& eg) {
				return *(eg.to) == dst && *(eg.weight) == weight;
			});
			if (it != set_edge.end())
				return false;
			auto path = Edge();
			path.to = found_dst;
			path.weight = std::make_unique<E>(weight);
			edgeList_[std::make_unique<N>(*found_src)].insert(std::move(path));
			return true;
		}

		auto replace_helper(N const& old_data, N const& new_data) -> void {
			for (auto const& edge : edgeList_) {
				if (*(edge.first) == old_data) {
					for (auto const& x : edge.second) {
						if (*(x.to) == old_data) {
							insert_edge(new_data, new_data, *(x.weight));
						}
						else {
							insert_edge(new_data, *(x.to), *(x.weight));
						}
					}
				}
				else {
					for (auto const& x : edge.second) {
						if (*(x.to) == old_data) {
							insert_edge(*(edge.first), new_data, *(x.weight));
						}
					}
				}
			}
		}

		auto replace_node(N const& old_data, N const& new_data) -> bool {
			if (is_node(new_data)) {
				return false;
			}
			if (!is_node(old_data)) {
				throw std::runtime_error("Cannot call gdwg::graph<N, E>::replace_node on a node that "
				                         "doesn't exist");
			}
			insert_node(new_data);

			replace_helper(old_data, new_data);

			erase_node(old_data);
			return true;
		}

		auto merge_replace_node(N const& old_data, N const& new_data) -> void {
			if (!is_node(old_data) || !is_node(new_data)) {
				throw std::runtime_error("Cannot call gdwg::graph<N, E>::merge_replace_node on old or "
				                         "new data if they don't exist in the graph");
			}

			replace_helper(old_data, new_data);
			erase_node(old_data);
		}

		auto erase_node(N const& value) -> bool {
			if (!is_node(value)) {
				return false;
			}
			auto node = find_node_ptr(value);

			for (auto& edge : edgeList_) {
				std::erase_if(edge.second, [value](auto const& x) { return *(x.to) == value; });
			}
			edgeList_.erase(std::make_unique<N>(*node));

			auto it2 = nodeList_.begin();
			while (it2 != nodeList_.end()) {
				if (**it2 == value) {
					it2 = nodeList_.erase(it2);
				}
				else {
					it2++;
				}
			}
			return true;
		}

		auto erase_edge(N const& src, N const& dst, E const& weight) -> bool {
			if (!is_node(src) || !is_node(dst)) {
				throw std::runtime_error("Cannot call gdwg::graph<N, E>::erase_edge on src or dst if "
				                         "they don't exist in the graph");
			}
			auto found_src = find_node_ptr(src);
			auto set_edge = edgeList_[std::make_unique<N>(*found_src)];

			auto const& e = std::find_if(set_edge.begin(), set_edge.end(), [dst, weight](Edge const& ed) {
				return *(ed.to) == dst && *(ed.weight) == weight;
			});
			if (e == set_edge.end()) {
				return false;
			}
			edgeList_.erase(std::make_unique<N>(*found_src));
			return true;
		}

		auto clear() noexcept -> void {
			nodeList_.clear();
			edgeList_.clear();
		}

		auto erase_edge(iterator i) -> iterator;
		auto erase_edge(iterator i, iterator s) -> iterator;

		// Accessor
		[[nodiscard]] auto is_node(N const& value) -> bool {
			auto it = find_if(nodeList_.begin(), nodeList_.end(), [&](auto& p) { return value == *p; });
			if (it == nodeList_.end()) {
				return false;
			}
			return true;
		}

		[[nodiscard]] auto is_node(N const& value) const -> bool {
			auto it = find_if(nodeList_.begin(), nodeList_.end(), [&](auto& p) { return value == *p; });
			if (it == nodeList_.end()) {
				return false;
			}
			return true;
		}

		[[nodiscard]] auto empty() -> bool {
			return static_cast<bool>(nodeList_.empty() && edgeList_.empty());
		}

		[[nodiscard]] auto is_connected(N const& src, N const& dst) -> bool {
			if (!is_node(src) || !is_node(dst))
				throw std::runtime_error("Cannot call gdwg::graph<N, E>::is_connected if src or dst "
				                         "node don't exist in the graph");
			auto node = find_node_ptr(src);
			if (node != nullptr) {
				auto set_edge = edgeList_[std::make_unique<N>(*node)];

				auto it = std::find_if(set_edge.begin(), set_edge.end(), [dst](Edge const& eg) {
					return *(eg.to) == dst;
				});
				if (it != set_edge.end()) {
					return true;
				}
			}
			return false;
		}

		[[nodiscard]] auto nodes() const -> std::vector<N> {
			auto vec = std::vector<N>();
			auto set = std::set<N>();
			std::transform(nodeList_.begin(),
			               nodeList_.end(),
			               std::inserter(set, set.begin()),
			               [&](auto& p) { return *p; });
			for (auto const itr : set) {
				vec.push_back(itr);
			}
			return vec;
		}

		[[nodiscard]] auto weights(N const& src, N const& dst) const -> std::vector<E> {
			if (!is_node(src) || !is_node(dst)) {
				throw std::runtime_error("Cannot call gdwg::graph<N, E>::weights if src or dst node "
				                         "don't exist in the graph");
			}
			auto res = std::vector<E>{};
			auto const& node = find_node_ptr(src);

			if (edgeList_.find(std::make_unique<N>(*node)) == edgeList_.end()) {
				return res;
			}

			for (auto const& it : edgeList_.at(std::make_unique<N>(*node))) {
				if (*(it.to) == dst) {
					res.push_back(*(it.weight));
				}
			}
			return res;
		}

		[[nodiscard]] auto find(N const& src, N const& dst, E const& weight) const -> iterator {
			auto const& i = find_node_iterator(src);
			if (i == edgeList_.end())
				return end();
			auto const& e =
			   std::find_if(i->second.begin(), i->second.end(), [dst, weight](Edge const& i) {
				   return *(i.to) == dst && *(i.weight) == weight;
			   });

			if (e == i->second.end())
				return end();
			return iterator(e, i, edgeList_.begin(), edgeList_.end());
		}

		[[nodiscard]] auto connections(N const& src) const -> std::vector<N> {
			if (!is_node(src)) {
				throw std::runtime_error("Cannot call gdwg::graph<N, E>::connections if src doesn't "
				                         "exist in the graph");
			}
			auto res = std::vector<N>{};
			auto const& node = find_node_ptr(src);

			if (edgeList_.find(std::make_unique<N>(*node)) == edgeList_.end()) {
				return res;
			}
			N prev;
			auto const& edge = edgeList_.at(std::make_unique<N>(*node));
			for (auto x = edge.begin(); x != edge.end(); x++) {
				if (x == edge.begin()) {
					prev = *(x->to);
					res.push_back(*(x->to));
				}
				else if (*(x->to) != prev) {
					prev = *(x->to);
					res.push_back(*(x->to));
				}
			}
			return res;
		}

		// comparsion
		[[nodiscard]] auto operator==(graph const& other) const -> bool {
			return static_cast<bool>(this->nodes() == other.nodes() && edge_cmp(other));
		}

		// helper
		auto edge_cmp(graph const& other) const -> bool {
			if (other.edgeList_.size() != edgeList_.size()) {
				return false;
			}
			auto src = other.edgeList_.begin();
			for (auto const& i : edgeList_) {
				if (*(i.first) != *(src->first) || i.second.size() != src->second.size()) {
					return false;
				}
				auto set_edge = src->second.begin();
				for (auto const& set_i : i.second) {
					if (*(set_i.to) != *(set_edge->to) || *(set_i.weight) != *(set_edge->weight)) {
						return false;
					}
					set_edge++;
				}
				src++;
			}
			return true;
		}

		friend auto operator<<(std::ostream& os, graph const& g) -> std::ostream& {
			auto nodes = g.nodes();
			for (auto node : nodes) {
				os << node << " (" << std::endl;
				if (g.edgeList_.find(std::make_unique<N>(node)) == g.edgeList_.end()) {
					os << ")" << std::endl;
					continue;
				}
				for (auto const& x : g.edgeList_.at(std::make_unique<N>(node))) {
					os << "  " << *(x.to) << " | " << *(x.weight) << std::endl;
				}
				os << ")" << std::endl;
			}
			return os;
		}

	private:
		struct Edge {
			Edge() {}

			Edge(Edge const& other)
			: to{other.to}
			, weight{std::make_unique<E>(*(other.weight))} {}

			auto operator<(const Edge& rhs) const -> bool {
				return *to != *rhs.to ? *to < *rhs.to : *weight < *(rhs.weight);
			}

			N* to;
			std::unique_ptr<E> weight;
		};

		struct nodeCmp {
			bool operator()(const std::unique_ptr<N>& lhs, const std::unique_ptr<N>& rhs) const {
				return *lhs < *rhs;
			}
		};

		std::set<std::unique_ptr<N>> nodeList_;
		std::map<std::unique_ptr<N>, std::set<Edge>, nodeCmp> edgeList_;

		auto find_node_ptr(N node) -> N* {
			auto it = find_if(nodeList_.begin(), nodeList_.end(), [&](auto& p) { return node == *p; });
			if (it == nodeList_.end()) {
				return nullptr;
			}
			auto& curNode = *it;
			return curNode.get();
		}

		auto find_node_ptr(N node) const -> N* {
			auto it = find_if(nodeList_.begin(), nodeList_.end(), [&](auto& p) { return node == *p; });
			if (it == nodeList_.end()) {
				return nullptr;
			}
			auto& curNode = *it;
			return curNode.get();
		}

		auto find_node_iterator(N const& value) const ->
		   typename std::map<std::unique_ptr<N>, std::set<Edge>, nodeCmp>::const_iterator {
			return std::find_if(edgeList_.begin(), edgeList_.end(), [&value](auto const& i) {
				return *(i.first) == value;
			});
		}
	};
	// iterator
	template<typename N, typename E>
	class graph<N, E>::iterator {
	public:
		using value_type = graph<N, E>::value_type;
		using reference = value_type;
		using pointer = void;
		using difference_type = std::ptrdiff_t;
		using iterator_category = std::bidirectional_iterator_tag;

		iterator() = default;

		auto operator*() const -> reference {
			auto ret = value_type();
			ret.from = *(node_->first);
			ret.to = *(edges_->to);
			ret.weight = *(edges_->weight);
			return ret;
		}

		auto operator++() -> iterator& {
			auto edge_set = node_->second.end();
			if (edges_ != edge_set) {
				++edges_;
			}
			if (edges_ == edge_set) {
				++node_;
				if (node_ != node_end_) {
					edges_ = find_next_edge();
				}
			}
			return *this;
		}

		auto operator++(int) -> iterator {
			auto res = *this;
			++(*this);
			return res;
		}

		auto operator--() -> iterator& {
			while (node_ == node_end_ || node_->second.empty()) {
				--node_;
				edges_ = node_->second.end();
			}
			if (edges_ == node_->second.begin()) {
				--node_;
				edges_ = find_before_edge();
			}
			--edges_;
			return *this;
		}

		auto operator--(int) -> iterator {
			auto res = *this;
			--(*this);
			return res;
		}

		auto operator==(iterator const& other) const -> bool {
			// Only compare the node_ if cmp is the end
			if (node_ == node_end_ || other.node_ == other.node_end_) {
				return node_ == other.node_;
			}
			return node_ == other.node_ && edges_ == other.edges_;
		}

	private:
		explicit iterator(std::map<std::unique_ptr<N>, std::set<Edge>, nodeCmp> const& g, int flag) {
			if (g.begin() == g.end() || flag == 0) {
				node_ = g.end();
			}
			else {
				node_ = g.begin();
				edges_ = find_next_edge();
			}
			node_end_ = g.end();
			node_begin_ = g.begin();
		}

		iterator(typename std::set<Edge>::iterator e,
		         typename std::map<std::unique_ptr<N>, std::set<Edge>, nodeCmp>::const_iterator i,
		         typename std::map<std::unique_ptr<N>, std::set<Edge>, nodeCmp>::const_iterator i_begin,
		         typename std::map<std::unique_ptr<N>, std::set<Edge>, nodeCmp>::const_iterator i_end) noexcept
		: node_{i}
		, node_begin_{i_begin}
		, node_end_{i_end}
		, edges_{e} {}

		auto find_next_edge() -> typename std::set<Edge>::iterator {
			while (node_ != node_end_ && node_->second.empty()) {
				++node_;
			}
			if (node_ != node_end_) {
				edges_ = node_->second.begin();
			}
			return edges_;
		}

		auto find_before_edge() -> typename std::set<Edge>::iterator {
			while (node_ == node_end_ || node_->second.empty()) {
				--node_;
			}
			edges_ = node_->second.end();
			return edges_;
		}

		auto erase() -> void {
			const_cast<std::set<Edge>&>(node_->second).erase(edges_);
		}

		typename std::map<std::unique_ptr<N>, std::set<Edge>, nodeCmp>::const_iterator node_;
		typename std::map<std::unique_ptr<N>, std::set<Edge>, nodeCmp>::const_iterator node_begin_;
		typename std::map<std::unique_ptr<N>, std::set<Edge>, nodeCmp>::const_iterator node_end_;
		typename std::set<Edge>::iterator edges_;
		friend class graph;
	};

	template<typename N, typename E>
	auto graph<N, E>::erase_edge(iterator i) -> iterator {
		if (i == end())
			return end();
		auto ed = i;
		++ed;
		i.erase();
		return ed;
	}

	template<typename N, typename E>
	auto graph<N, E>::erase_edge(iterator i, iterator s) -> iterator {
		while (i != s) {
			i = erase_edge(i);
		}
		return i;
	}

} // namespace gdwg

#endif // GDWG_GRAPH_HPP