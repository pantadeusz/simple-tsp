#include <iostream>
#include <algorithm>
#include <vector>
#include <numeric>
#include <array>
#include <cmath>

namespace alo_tsp {

/**
 * solucion contains vector of id-s of vertices to visit
 * */
using solution_t = std::vector<int>;
/**
 * coordinate
 * */
class coordinate_t : public std::array<double,2> {};
using problem_t = std::vector<coordinate_t>;
using problem_with_solution_t = std::pair <problem_t, solution_t>;


double evaluate_tsp(const problem_with_solution_t pws);
solution_t tsp(const problem_t cities);

/**
 * helper operator that calculates difference between two points
 * */
coordinate_t operator-(const coordinate_t a, const coordinate_t b) {
	auto ret = a;
	for (size_t i = 0; i < b.size(); i++) ret[i] -= b[i];
	return std::move(ret);
}
/**
 * Calculate vector length
 * */
double operator~(const coordinate_t a) {
	
	return //std::sqrt(
		std::accumulate(a.begin(), a.end(), 0.0, [](double s, auto e) {return s + e*e;})
	//)
	;
}
/**
 * compare two solutions - are they identical?
 * */
bool operator==(const solution_t &a, const solution_t &b) {
	for (int i = 0; i < std::min(a.size(),b.size()); i++) if (a[i] != b[i]) return false;
	return a.size() == b.size();
} 



std::ostream &operator<<(std::ostream &o, const solution_t &s) {
	using namespace std;
	o << "[ ";
	for (auto e : s) o << e << " ";
	o << "]";
	return o;
}

std::ostream &operator<<(std::ostream&o, const coordinate_t &c) {
	using namespace std;
	o << "[ ";
	for (auto e : c) o << e << " ";
	o << "]";
	return o;
}

std::ostream &operator<<(std::ostream&o, const problem_with_solution_t &s) {
	using namespace std;
	o << evaluate_tsp(s) << "::[ ";
	for (auto e : s.second) o << "{ #" << e << " : " << s.first[e] << "} ";
	o << "]";
	return o;
}

/**
 * calculate how long is the path.
 * 
 * do not go back.
 * 
 * */
double evaluate_tsp(const problem_with_solution_t pws) {
	using namespace std;
	double dist  = 0.0;
	auto &[problem, solution] = pws;
	for (size_t i = 0; i < solution.size(); i++) {
		dist += ~(problem[solution[(i+1) % solution.size()]]-problem[solution[i]]);
	}
	return dist;
}

auto create_sol_sequence_vector = [](auto &cities){solution_t r; for (auto &c:cities) r.push_back(r.size()); return r;};

/**
 * Brute force algorithm to calculate TSP
 * */
solution_t tsp_brute_force(const problem_t cities) {
	solution_t current,best;
	// create vector of values 0, 1, 2, 3, ... based on argument
	current = best = create_sol_sequence_vector(cities);
	do if (evaluate_tsp({cities,current}) < evaluate_tsp({cities,best})) best = current;
    while ( std::next_permutation(current.begin(),current.end()) );
	return best;
}

} // namespace tsp

int main() {
	using namespace alo_tsp;
	
	problem_t problem = {
	{-1, -1},
	{0,-1},       {1, -1},
	{-1,  1},     {0, 1},      
	 {1,  1},
	{-1,  0},{-1,  0},            {1,  0},
	};
	problem_with_solution_t pwsolution = {problem, tsp_brute_force(problem)};
	std::cout << pwsolution << std::endl;
	return 0;
}
