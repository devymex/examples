//#include <glog/logging.h>
#include <algorithm>
#include <cmath>
#include <iostream>
#include <set>
#include <list>
#include <vector>

typedef int vertex_t;
typedef double weight_t;
 
const weight_t max_weight = std::numeric_limits<double>::infinity();
 
struct neighbor {
	vertex_t target;
	weight_t weight;
	neighbor(vertex_t arg_target, weight_t arg_weight)
		: target(arg_target), weight(arg_weight) { }
};
 
typedef std::vector<std::vector<neighbor> > adjacency_list_t;
 
 
void DijkstraComputePaths(vertex_t source,
						  const adjacency_list_t &adjacency_list,
						  std::vector<weight_t> &min_distance,
						  std::vector<vertex_t> &previous)
{
	int n = adjacency_list.size();
	min_distance.clear();
	min_distance.resize(n, max_weight);
	min_distance[source] = 0;
	previous.clear();
	previous.resize(n, -1);
	std::set<std::pair<weight_t, vertex_t> > vertex_queue;
	vertex_queue.insert(std::make_pair(min_distance[source], source));
 
	while (!vertex_queue.empty()) 
	{
		weight_t dist = vertex_queue.begin()->first;
		vertex_t u = vertex_queue.begin()->second;
		vertex_queue.erase(vertex_queue.begin());
 
		// Visit each edge exiting u
	const std::vector<neighbor> &neighbors = adjacency_list[u];
		for (std::vector<neighbor>::const_iterator neighbor_iter = neighbors.begin();
			 neighbor_iter != neighbors.end();
			 neighbor_iter++)
		{
			vertex_t v = neighbor_iter->target;
			weight_t weight = neighbor_iter->weight;
			weight_t distance_through_u = dist + weight;
		if (distance_through_u < min_distance[v]) {
			vertex_queue.erase(std::make_pair(min_distance[v], v));
 
			min_distance[v] = distance_through_u;
			previous[v] = u;
			vertex_queue.insert(std::make_pair(min_distance[v], v));
 
		}
 
		}
	}
}
 
 
std::list<vertex_t> DijkstraGetShortestPathTo(
	vertex_t vertex, const std::vector<vertex_t> &previous)
{
	std::list<vertex_t> path;
	for ( ; vertex != -1; vertex = previous[vertex])
		path.push_front(vertex);
	return path;
}
 
struct STATUS {
	char dir;
	int head;
	int tail;
};

int main(int nArgCnt, char *ppArgs[]) {
	STATUS arg = {ppArgs[1][0], ppArgs[1][1] - '0', ppArgs[1][2] - '0'};
	
	std::cout << "Neighbors:" << std::endl;
	std::vector<std::vector<int>> neighbors(10);
	for (int i = 0; i < 9; ++i) {
		auto &nb = neighbors[i + 1];
		int x = i % 3, y = i / 3;
		std::cout << i + 1 << ":";
		for (int r = -1; r < 2; ++r) {
			for (int c = -1; c < 2; ++c) {
				if (r != 0 || c != 0) {
					int x1 = x + c, y1 = y + r;
					if (x1 >= 0 && x1 < 3 && y1 >= 0 && y1 < 3) {
						nb.push_back(y1 * 3 + x1 + 1);
						std::cout << " " << y1 * 3 + x1 + 1;
					}
				}
			}
		}
		nb.push_back(0);
		neighbors[0].push_back(i + 1);
		std::cout << std::endl;
	}
	std::cout << std::endl;
	
	std::vector<STATUS> status;
	status.emplace_back(STATUS{' ', 0, 0});
	for (int i = 0; i < (int)neighbors.size(); ++i) {
		for (auto nb : neighbors[i]) {
			status.emplace_back(STATUS{'+', i, nb});
			status.emplace_back(STATUS{'-', i, nb});
		}
	}
	std::cout << "Status:" << std::endl;
	for (int i = 0; i < (int)status.size(); ++i) {
		auto &s = status[i];
		std::cout << i << ": " << s.dir << " " << s.head << " " << s.tail << std::endl;
	}
	std::cout << std::endl;
	
	std::vector<std::pair<int, int>> conns;
	for (int i = 0; i < (int)status.size(); ++i) {
		STATUS opposite = status[i];
		if (opposite.dir != ' ') {
			std::swap(opposite.head, opposite.tail);
			if (opposite.dir == '+') {
				opposite.dir = '-';
			} else {
				opposite.dir = '+';
			}
			auto iFound = std::find_if(status.begin(), status.end(),
					[&](STATUS &s) {
						return opposite.dir == s.dir &&
							opposite.head == s.head && opposite.tail == s.tail;
				});
			if (iFound != status.end()) {
				conns.emplace_back(i, iFound - status.begin());
			} else {
				std::cout << "error1" << std::endl;
			}
		}
	}
	
	std::vector<std::pair<STATUS, STATUS>> manConn;

	manConn.emplace_back(STATUS{'+', 1, 0}, STATUS{'+', 4, 0});
	manConn.emplace_back(STATUS{'+', 1, 2}, STATUS{'+', 4, 2});
	manConn.emplace_back(STATUS{'+', 1, 5}, STATUS{'+', 4, 5});

	manConn.emplace_back(STATUS{'+', 2, 3}, STATUS{'+', 5, 3});
	manConn.emplace_back(STATUS{'+', 2, 6}, STATUS{'+', 5, 6});
	manConn.emplace_back(STATUS{'+', 2, 1}, STATUS{'+', 5, 1});
	manConn.emplace_back(STATUS{'+', 2, 4}, STATUS{'+', 5, 4});

	manConn.emplace_back(STATUS{'+', 2, 0}, STATUS{'+', 3, 0});
	manConn.emplace_back(STATUS{'+', 2, 5}, STATUS{'+', 3, 5});
	manConn.emplace_back(STATUS{'+', 2, 6}, STATUS{'+', 3, 6});

	manConn.emplace_back(STATUS{'+', 4, 1}, STATUS{'+', 5, 1});
	manConn.emplace_back(STATUS{'+', 4, 2}, STATUS{'+', 5, 2});
	manConn.emplace_back(STATUS{'+', 4, 7}, STATUS{'+', 5, 7});
	manConn.emplace_back(STATUS{'+', 4, 8}, STATUS{'+', 5, 8});

	manConn.emplace_back(STATUS{'+', 5, 3}, STATUS{'+', 6, 3});
	manConn.emplace_back(STATUS{'+', 5, 2}, STATUS{'+', 6, 2});
	manConn.emplace_back(STATUS{'+', 5, 9}, STATUS{'+', 6, 9});
	manConn.emplace_back(STATUS{'+', 5, 8}, STATUS{'+', 6, 8});

	manConn.emplace_back(STATUS{'+', 5, 4}, STATUS{'+', 8, 4});
	manConn.emplace_back(STATUS{'+', 5, 7}, STATUS{'+', 8, 7});
	manConn.emplace_back(STATUS{'+', 5, 9}, STATUS{'+', 8, 9});
	manConn.emplace_back(STATUS{'+', 5, 6}, STATUS{'+', 8, 6});

	manConn.emplace_back(STATUS{'+', 6, 0}, STATUS{'+', 9, 0});

	manConn.emplace_back(STATUS{'+', 7, 4}, STATUS{'+', 8, 4});
	manConn.emplace_back(STATUS{'+', 7, 5}, STATUS{'+', 8, 5});
	manConn.emplace_back(STATUS{'+', 7, 0}, STATUS{'+', 8, 0});

	manConn.emplace_back(STATUS{'+', 0, 8}, STATUS{'+', 7, 8});
	manConn.emplace_back(STATUS{'+', 0, 7}, STATUS{'+', 8, 7});
	manConn.emplace_back(STATUS{' ', 0, 0}, STATUS{'+', 7, 0});
	manConn.emplace_back(STATUS{' ', 0, 0}, STATUS{'+', 8, 0});

	manConn.emplace_back(STATUS{'-', 1, 0}, STATUS{'-', 2, 0});
	manConn.emplace_back(STATUS{'-', 1, 4}, STATUS{'-', 2, 4});
	manConn.emplace_back(STATUS{'-', 1, 5}, STATUS{'-', 2, 5});

	manConn.emplace_back(STATUS{'-', 2, 3}, STATUS{'-', 5, 3});
	manConn.emplace_back(STATUS{'-', 2, 6}, STATUS{'-', 5, 6});

	manConn.emplace_back(STATUS{'-', 4, 1}, STATUS{'-', 5, 1});
	manConn.emplace_back(STATUS{'-', 4, 2}, STATUS{'-', 5, 2});

	manConn.emplace_back(STATUS{'-', 5, 8}, STATUS{'-', 6, 8});
	manConn.emplace_back(STATUS{'-', 5, 9}, STATUS{'-', 6, 9});

	manConn.emplace_back(STATUS{'-', 4, 0}, STATUS{'-', 7, 0});

	manConn.emplace_back(STATUS{'-', 8, 5}, STATUS{'-', 9, 5});
	manConn.emplace_back(STATUS{'-', 8, 6}, STATUS{'-', 9, 6});

	manConn.emplace_back(STATUS{'-', 8, 4}, STATUS{'-', 5, 4});
	manConn.emplace_back(STATUS{'-', 8, 6}, STATUS{'-', 5, 6});
	manConn.emplace_back(STATUS{'-', 8, 7}, STATUS{'-', 5, 7});
	manConn.emplace_back(STATUS{'-', 8, 9}, STATUS{'-', 5, 9});

	manConn.emplace_back(STATUS{'-', 0, 2}, STATUS{'-', 1, 2});
	manConn.emplace_back(STATUS{'-', 0, 1}, STATUS{'-', 2, 1});
	manConn.emplace_back(STATUS{'-', 0, 3}, STATUS{'-', 2, 3});
	manConn.emplace_back(STATUS{'-', 0, 2}, STATUS{'-', 3, 2});
	manConn.emplace_back(STATUS{'-', 0, 6}, STATUS{'-', 3, 6});
	manConn.emplace_back(STATUS{'-', 0, 3}, STATUS{'-', 6, 3});
	manConn.emplace_back(STATUS{'-', 0, 1}, STATUS{'-', 4, 1});
	manConn.emplace_back(STATUS{'-', 0, 7}, STATUS{'-', 4, 7});
	manConn.emplace_back(STATUS{'-', 0, 4}, STATUS{'-', 7, 4});
	manConn.emplace_back(STATUS{'-', 0, 8}, STATUS{'-', 7, 8});
	manConn.emplace_back(STATUS{'-', 0, 9}, STATUS{'-', 6, 9});
	manConn.emplace_back(STATUS{'-', 9, 0}, STATUS{'-', 8, 0});

	manConn.emplace_back(STATUS{' ', 0, 0}, STATUS{'-', 1, 0});
	manConn.emplace_back(STATUS{' ', 0, 0}, STATUS{'-', 2, 0});
	manConn.emplace_back(STATUS{' ', 0, 0}, STATUS{'-', 3, 0});
	manConn.emplace_back(STATUS{' ', 0, 0}, STATUS{'-', 4, 0});
	manConn.emplace_back(STATUS{' ', 0, 0}, STATUS{'-', 6, 0});
	manConn.emplace_back(STATUS{' ', 0, 0}, STATUS{'-', 7, 0});

	for (auto &mc : manConn) {
		auto iFirst = std::find_if(status.begin(), status.end(),
				[&](STATUS &s) {
					return mc.first.dir == s.dir && mc.first.head == s.head &&
							mc.first.tail == s.tail;
			});
		auto iSecond = std::find_if(status.begin(), status.end(),
				[&](STATUS &s) {
					return mc.second.dir == s.dir && mc.second.head == s.head &&
							mc.second.tail == s.tail;
			});
		if (iFirst != status.end() && iSecond != status.end()) {
			conns.emplace_back(iFirst - status.begin(), iSecond - status.begin());
			conns.emplace_back(iSecond - status.begin(), iFirst - status.begin());
		} else {
			std::cout << "error2" << std::endl;
			std::cout << mc.first.dir << " " << mc.first.head << " " << mc.first.tail << std::endl;
		}
	}
	
	adjacency_list_t adjacency_list(status.size());
	std::cout << "Connections:" << std::endl;
	for (int i = 0; i < (int)conns.size(); ++i) {
		adjacency_list[conns[i].first].emplace_back(conns[i].second, 1);
		//std::cout << i << ": " << conns[i].first << " " << conns[i].second << std::endl;
	}
	std::cout << std::endl;
	
	std::vector<weight_t> min_distance;
	std::vector<vertex_t> previous;
	DijkstraComputePaths(0, adjacency_list, min_distance, previous);
	
	auto iFound = std::find_if(status.begin(), status.end(),
			[&](STATUS &s) {
				return arg.dir == s.dir && arg.head == s.head &&
						arg.tail == s.tail;
		});
	std::cout << "Path: " << std::endl;
	std::list<vertex_t> path = DijkstraGetShortestPathTo(iFound - status.begin(), previous);
	for (auto v : path) {
		std::cout << status[v].dir << " " << status[v].head
				  << " " << status[v].tail << std::endl;
	}
	//std::copy(path.begin(), path.end(), std::ostream_iterator<vertex_t>(std::cout, " "));
	std::cout << std::endl;
	
	return 0;
}
