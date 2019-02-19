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
 * \brief run_tests template function for running same tests for different types of datetime types
 * \tparam K a datetime type supporting initialisation from int and post-increment
 */
template<typename K>
void run_tests()
{
	vector<pair<K, double>> ts;

	constexpr size_t SZ = 5000000;
	constexpr size_t n_tests = 1;

	K dt = 0;

	// create dummy timeseries with strictly increasing time values
	for (size_t n = 0; n < SZ; n++, dt++) {
		ts.emplace_back(make_pair(dt, 3.1415926));
		// if (n%23 == 0) dt++; // uncomment to add some "sparsity" - this should decrease the access performance (a little)
	}

	tuple_vector<K, double> tv;
	map<K, double> map;
	vector<pair<K, double>> vec;

	{	// emplace performance
		#include "tests/emplace_test.h"
		cout << "emplace()" << endl;
		cppbench::print( cppbench::compare(rt) );
	}

	{	// operator[] performance
		#include "tests/array_test.h"
		cout << endl << "operator []" << endl;
		cppbench::print( cppbench::compare(rt) );
	}

	{	// iterator performance
		#include "tests/iterator_test.h"
		cout << endl << "iterator" << endl;
		cppbench::print( cppbench::compare(rt) );
	}

	{	// find() performance
		#include "tests/find_test.h"
		cout << endl << "find()" << endl;
		cppbench::print( cppbench::compare(rt) );
	}

	{	// lower_bound() performance
		#include "tests/lower_bound_test.h"
		cout << endl << "lower_bound()" << endl;
		cppbench::print( cppbench::compare(rt) );
	}
}

int main()
{
	cout << "RUNNING TESTS FOR time_t" << endl;
	run_tests<time_t>();
	cout << endl << "RUNNING TESTS FOR boost::posix_time::ptime" << endl;
	run_tests<my_ptime>();
}
