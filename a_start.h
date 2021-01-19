#pragma once

#include <unordered_map>
#include <unordered_set>
#include <array>
#include <iso646.h>
#include <queue>

struct GridLocation {    
    [[nodiscard]] int x() const noexcept {
        return x_;
	}

    [[nodiscard]] int y() const noexcept {
        return y_;
    }

	bool operator == (GridLocation const & other) const noexcept {
        return other.x() == x() && other.y() == y();
	}

    bool operator != (GridLocation const & other) const noexcept {
        return !(*this == other);
    }

    friend bool operator < (GridLocation const & lhs, GridLocation const & rhs) noexcept {
		return (lhs.x() < rhs.x()) || ((lhs.x() == rhs.x()) && (lhs.y() < rhs.y()));
    }

    int x_{ 0 };
    int y_{ 0 };
};

struct GridRect {
	[[nodiscard]] bool contains(GridLocation const &location) const noexcept {
        if (location.x() > x1_ and location.x() < x2_ and location.y() > y1_ and location.y() < y2_) {
            return true;
        }
        return false;
    }
	
    int x1_{ 0 };
    int y1_{ 0 };
    int x2_{ 0 };
    int y2_{ 0 };
};

namespace std {
    template <> struct hash<GridLocation> {
        typedef GridLocation argument_type;
        typedef size_t result_type;
    	
        size_t operator()(GridLocation const & id) const noexcept {
            return std::hash<int>()(id.x() ^ (id.y() << 4));
        }
    };
}

template <typename T = GridLocation>
class SquareGrid final {
public:
    static constexpr std::array<T, 4> kDirs = {
        T { 1, 0 },
        T { 0, -1 },
        T { -1, 0 },
        T { 0, 1 },
    };
	
    SquareGrid(const int width, const int height) noexcept
		: width_(width)
		, height_(height) {
    }

    [[nodiscard]] bool inBounds(T const & id) const noexcept {
        //return 0 <= id.x() && id.x() < width_
        //    && 0 <= id.y() && id.y() < height_;
        return id.x() > -width_ && id.x() < width_
            && id.y() > -height_ && id.y() < height_;
    }

    [[nodiscard]] bool passable(T const& id) const {
        return walls_.find(id) == walls_.end();
    }

    [[nodiscard]] std::vector<T> neighbors(T const& id) const {
        std::vector<T> results;

        results.reserve(kDirs.size());
    	
        for (auto const &dir : kDirs) {
            T next{ id.x() + dir.x(), id.y() + dir.y() };
            if (inBounds(next) && passable(next)) {
                results.push_back(next);
            }
        }

        if (static_cast<int>(id.x() + id.y()) % 2 == 0) {
            std::reverse(results.begin(), results.end());
        }
        return results;
    }

    [[nodiscard]] double cost(T const& from_node, T const& to_node) const {
        return forests_.find(to_node) != forests_.end() ? 5 : 1;
    }

    void clearWalls() {
        walls_.clear();
    }

    void addWalls(T const& node) {
        walls_.insert(node);
    }
private:
    int width_;
	int height_;
    std::unordered_set<T> walls_;
    std::unordered_set<T> forests_;
};

template <typename T, typename Priority>
struct PriorityQueue {
    typedef std::pair<Priority, T> PqElement;
    std::priority_queue<PqElement, std::vector<PqElement>, std::greater<>> elements;

    [[nodiscard]] bool empty() const {
        return elements.empty();
    }

    void put(T const &item, Priority const& priority) {
        elements.emplace(priority, item);
    }

    T get() {
        auto best_item = elements.top().second;
        elements.pop();
        return best_item;
    }
};

struct ManhattanDistance final {
    double operator()(GridLocation const &a, GridLocation const & b) const noexcept {
        return std::abs(a.x() - b.x()) + std::abs(a.y() - b.y());
    }
};

namespace AStart {
    template <typename Location>
    std::vector<Location> reconstructPath(Location const& start,
        Location const& goal,
        std::unordered_map<Location, Location>& came_from) {
        std::vector<Location> path;
        auto current = goal;
        while (current != start) {
            path.push_back(current);
            auto itr = came_from.find(current);
            if (itr != came_from.end()) {
                current = (*itr).second;
            }
            else {
                break;
            }
        }
        path.push_back(start);
        std::reverse(path.begin(), path.end());
        return path;
    }
	
    template <typename Location, typename Graph, typename Heuristic>
    std::vector<Location> search(Graph const& graph,
        Location const& start,
        Location const& goal,
        std::unordered_map<Location, Location>& came_from,
        std::unordered_map<Location, double>& cost_so_far,
        Heuristic heuristic) {
        PriorityQueue<Location, double> frontier;
        frontier.put(start, 0);

        came_from[start] = start;
        cost_so_far[start] = 0;

        while (!frontier.empty()) {
            auto current = frontier.get();

            if (current == goal) {
                break;
            }

            for (auto const& next : graph.neighbors(current)) {
                double new_cost = cost_so_far[current] + graph.cost(current, next);

                if (cost_so_far.find(next) == cost_so_far.end()
                    || new_cost < cost_so_far[next]) {
                    cost_so_far[next] = new_cost;
                    double priority = new_cost + heuristic(next, goal);
                    frontier.put(next, priority);
                    came_from[next] = current;
                }
            }
        }

        return reconstructPath(start, goal, came_from);
    }
}

