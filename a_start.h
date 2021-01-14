#pragma once

#include <unordered_map>
#include <unordered_set>
#include <array>
#include <queue>

struct GridLocation {
    [[nodiscard]] int x() const {
        return x_;
	}

    [[nodiscard]] int y() const {
        return y_;
    }

	bool operator == (GridLocation const &other) const {
        return other.x() == x() && other.y() == y();
	}

    bool operator != (GridLocation const& other) const {
        return !(*this == other);
    }

    friend bool operator < (const GridLocation& lhs, const GridLocation& rhs) {
		return (lhs.x() < rhs.x()) || ((lhs.x() == rhs.x()) && (lhs.y() < rhs.y()));
    }

    int x_{ 0 };
    int y_{ 0 };
};

namespace std {
    template <> struct hash<GridLocation> {
        typedef GridLocation argument_type;
        typedef std::size_t result_type;
    	
        std::size_t operator()(const GridLocation& id) const noexcept {
            return std::hash<int>()(id.x() ^ (id.y() << 4));
        }
    };
}

template <typename T = GridLocation>
struct SquareGrid {
    static constexpr std::array<T, 4> kDirs = {
        T { 1, 0 },
        T { 0, -1 },
        T { -1, 0 },
        T { 0, -0 },
    };
	
    SquareGrid(int width_, int height_)
		: width(width_)
		, height(height_) {
    }

    [[nodiscard]] bool in_bounds(T const & id) const {
        //return 0 <= id.x() && id.x() < width
        //    && 0 <= id.y() && id.y() < height;
        return id.x() > -100 && id.x() < 100
            && id.y() > -100 && id.y() < 100;
    }

    [[nodiscard]] bool passable(T const& id) const {
        return walls.find(id) == walls.end();
    }

    [[nodiscard]] std::vector<T> neighbors(T const& id) const {
        std::vector<T> results;

        results.reserve(kDirs.size());
    	
        for (auto const &dir : kDirs) {
            T next{ id.x() + dir.x(), id.y() + dir.y() };
            if (in_bounds(next) && passable(next)) {
                results.push_back(next);
            }
        }

        if (static_cast<int>(id.x() + id.y()) % 2 == 0) {
            std::reverse(results.begin(), results.end());
        }
        return results;
    }

    [[nodiscard]] double cost(T const& from_node, T const& to_node) const {
        return forests.find(to_node) != forests.end() ? 5 : 1;
    }

    int width;
	int height;
    std::unordered_set<T> walls;
    std::unordered_set<T> forests;
};

template <typename T, typename Priority>
struct PriorityQueue {
    typedef std::pair<Priority, T> PqElement;
    std::priority_queue<PqElement, std::vector<PqElement>, std::greater<>> elements;

    [[nodiscard]] bool empty() const {
        return elements.empty();
    }

    void put(T item, Priority priority) {
        elements.emplace(priority, item);
    }

    T get() {
        T best_item = elements.top().second;
        elements.pop();
        return best_item;
    }
};

struct Heuristic {
    double operator()(GridLocation const &a, GridLocation const & b) const {
        return std::abs(a.x() - b.x()) + std::abs(a.y() - b.y());
    }
};

template <typename Location>
std::vector<Location> reconstructPath(Location const& start, Location const& goal, std::unordered_map<Location, Location> came_from) {
    std::vector<Location> path;
    auto current = goal;
    while (current != start) {
        path.push_back(current);
        current = came_from[current];
    }
    path.push_back(start);
    std::reverse(path.begin(), path.end());
    return path;
}

template <typename Location, typename Graph, typename Heuristic>
std::vector<Location> Search(Graph const& graph,
    Location const& start,
    Location const& goal,
    std::unordered_map<Location, Location> & came_from,
    std::unordered_map<Location, double> & cost_so_far,
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

        for (auto const &next : graph.neighbors(current)) {
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