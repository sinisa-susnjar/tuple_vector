#include <algorithm>
#include <iostream>
#include <array>
#include <vector>
#include <map>

// please also clone my cppbench library ( https://github.com/sinisa-susnjar/cppbench.git )
#include "../cppbench/cppbench.h"

#include "tuple_vector.h"
#include "common.h"

using namespace std;

/**
 * \brief Specialised output function to generate data that will later be plotted by R.
 * \param os Where to write the test results.
 * \param result A multimap with the runtimes of all tests obtained by a call to cppbench::time().
 * \param name Test group name, e.g. "emplace".
 * \param sz Container size.
 * \param delim What delimiter character to use, default is tab.
 * \param prec Numerical precision for floating value, default is 5.
 * \tparam T An optional template parameter used to cast to the desired time resolution.
 */
template<typename T = chrono::microseconds>
void Rdata(ostream &os, const cppbench::runtime_result &result, const string &name, int sz, char delim = '\t', int prec = 5) {
	double conv = chrono::high_resolution_clock::period::den / T::period::den;
	for (const auto &a : result) 
		os << name << delim << sz << delim << a.second.name << delim
			<< chrono::duration_cast<chrono::microseconds>(a.first).count() << delim
			<< fixed << setprecision(prec) << (a.second.min/conv) << delim
			<< fixed << setprecision(prec) << (a.second.max/conv) << delim
			<< fixed << setprecision(prec) << (a.second.avg/conv) << delim
			<< fixed << setprecision(prec) << (a.second.var/(conv*conv)) << delim
			<< fixed << setprecision(prec) << (a.second.dev/conv)
			<< endl;
}

/**
 * \brief run_tests template function for running same tests for different types of datetime types
 * \param n_tests number of runs for each test
 * \param start_sz container start size
 * \param end_sz container max size
 * \param sz_step container size increment
 * \tparam K a datetime type supporting initialisation from int and post-increment
 */
template<typename K>
void run_tests(ofstream &&ofs, size_t n_tests, size_t start_sz, size_t end_sz, size_t sz_step)
{
	ofs << "test" << '\t' << "size" << '\t' << "container" << '\t'
		<< "runtime" << '\t' << "min" << '\t' << "max" << '\t'
		<< "avg" << '\t' << "var" << '\t' << "dev" << endl;
	for (size_t sz = start_sz; sz <= end_sz; sz += sz_step) {
		vector<pair<K, double>> ts;
		K dt = 0;

		// create dummy timeseries with strictly increasing time values
		for (size_t n = 0; n < sz; n++, dt++) {
			ts.emplace_back(dt, 3.1415926);
			// if (n%23 == 0) dt++; // uncomment to add some "sparsity" - this should decrease the access performance (a little)
		}

		vector<pair<K, double>> vec;
		tuple_vector<K, double> tv;
		map<K, double> map;

		{	// emplace performance
			cout << "emplace with size " << sz << endl;
			#include "tests/emplace_test.h"
			Rdata(ofs, rt, "emplace", sz);
		}
		{	// operator[] performance
			cout << "array with size " << sz << endl;
			#include "tests/array_test.h"
			Rdata(ofs, rt, "array", sz);
		}
		{	// iterator performance
			cout << "iterator with size " << sz << endl;
			#include "tests/iterator_test.h"
			Rdata(ofs, rt, "iterator", sz);
		}
		{	// find() performance
			cout << "find with size " << sz << endl;
			#include "tests/find_test.h"
			Rdata(ofs, rt, "find", sz);
		}
		{	// lower_bound() performance
			cout << "lower_bound with size " << sz << endl;
			#include "tests/lower_bound_test.h"
			Rdata(ofs, rt, "lower_bound", sz);
		}
	}
}

int main()
{
	// run 100x tests, using container sizes from 10000 to 1000000 with step size 10000

	cout << "RUNNING TESTS FOR time_t" << endl;
	run_tests<time_t>(ofstream("timet.txt"), 100, 10000, 1000000, 10000);

	cout << endl << "RUNNING TESTS FOR boost::posix_time::ptime" << endl;
	run_tests<my_ptime>(ofstream("ptime.txt"), 100, 10000, 1000000, 10000);
}
