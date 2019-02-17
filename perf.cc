#include <algorithm>
#include <iostream>
#include <array>
#include <vector>
#include <map>

#include <boost/date_time/posix_time/posix_time_types.hpp>
#include <boost/date_time.hpp>

// please also clone my cppbench library ( https://github.com/sinisa-susnjar/cppbench.git )
#include "../cppbench/cppbench.h"

#include "tuple_vector.h"

using namespace boost::posix_time;
using namespace boost::gregorian;
using namespace std;

/// dummy helper class to make boost::posix_time::ptime work with run_test()
class my_ptime : public ptime {
public:
	my_ptime(int n = 0) : ptime(boost::gregorian::date(1970, Jan, 1)) { }
	my_ptime &operator++(int) {
		*this += seconds(1);
		return *this;
	}
};

/// sample key functor specialisation for ptime
template<> double key<my_ptime>::operator()(const my_ptime &x) const { return (x-ptime(date(1900, Jan, 1))).total_milliseconds(); }
/// sample key functor specialisation for time_t
template<> double key<time_t>::operator()(const time_t &x) const { return x; }

/**
 * \brief run_tests template function for running same tests for different types of datetime types
 * \tparam K a datetime type supporting initialisation from int and post-increment
 */
template<typename K>
void run_tests()
{
	vector<pair<K, double>> ts;

	constexpr size_t SZ = 5000000;
	constexpr size_t N = 1;

	K dt = 0;

	// create dummy timeseries with strictly increasing time values
	for (size_t n = 0; n < SZ; n++, dt++) {
		ts.emplace_back(make_pair(dt, 3.1415926));
		// if (n%23 == 0) dt++; // uncomment to add some "sparsity" - this should decrease the access performance (a little)
	}

	tuple_vector<K, double> tv;
	map<K, double> map;
	vector<K> vec;

	// emplace performance
	auto rt = cppbench::time(N, {
		 { "vector", [ts,&vec]() {
			vec.reserve(ts.size());
			for (const auto &it : ts)
				vec.emplace_back(it.first);
		}},
		{ "map",	[ts,&map]() {
			for (const auto &it : ts)
				map.emplace_hint(map.end(), it.first, it.second);
		
		}},
		{ "tuple",	[ts,&tv]() {
			tv.reserve(ts.size());
			for (const auto &it : ts)
				tv.emplace_back(it);
		}}
	});

	cout << "emplace()" << endl;
	cppbench::print( cppbench::compare(rt) );

	// operator[] performance
	rt = cppbench::time(N, {
		 { "vector", [ts,vec]() {
			size_t i = 0;
			for (const auto &it : ts)
				if (vec[i++] != it.first)
					abort();
		}},
		{ "map",	[ts,&map]() {
			for (const auto &it : ts)
				if (map[it.first] != it.second)
					abort();
		}},
		{ "tuple",	[ts,tv]() {
			size_t i = 0;
			for (const auto &it : ts)
				if (tv[i++].second != it.second)
					abort();
		}}
	});

	cout << endl << "operator []" << endl;
	cppbench::print( cppbench::compare(rt) );

	// iterator performance
	rt = cppbench::time(N, {
		 { "vector", [ts,vec]() {
			auto it = ts.begin();
			for (auto &x : vec) {
				if (x != it->first)
					abort();
				++it;
			}
		}},
		{ "map",	[ts,map]() {
			auto x = map.begin();
			auto i = ts.begin();
			for (; i != ts.end(); ++i, ++x) {
				if (x->first != i->first)
					abort();
			}
		
		}},
		{ "tuple",	[ts,tv]() {
			auto x = tv.begin();
			auto i = ts.begin();
			for (; i != ts.end(); ++i, ++x) {
				if (x->first != i->first)
					abort();
			}
		}}
	});

	cout << endl << "iterator" << endl;
	cppbench::print( cppbench::compare(rt) );

	// find() performance
	rt = cppbench::time(N, {
		{ "map",	[ts,map]() {
			for (auto &i : ts) {
				auto it = map.find(i.first);
				if (it->first != i.first)
					abort();
			}
		}},
		{ "tuple",	[ts,&tv]() {
			for (auto &i : ts) {
				auto it = tv.find(i.first);
				if (it == tv.end())
					abort();
				if ((*it).first != i.first)
					abort();
			}
		}}
	});

	cout << endl << "find()" << endl;
	cppbench::print( cppbench::compare(rt) );

	// lower_bound() performance
	rt = cppbench::time(N, {
		{ "map",	[ts,map]() {
			K dt;
			K l = ts.crbegin()->first;
			for (auto &i : ts) {
				dt = i.first;
				dt++;
				if (dt > l)
					break;
				auto it = map.lower_bound(dt);
				if (it == map.end())
					abort();
				if ((*it).first < dt)
					abort();
			}
		}},
		{ "tuple",	[ts,&tv]() {
			K dt;
			K l = ts.crbegin()->first;
			for (auto &i : ts) {
				dt = i.first;
				dt++;
				if (dt > l)
					break;
				auto it = tv.lower_bound(dt);
				if (it == tv.end())
					abort();
				if ((*it).first < dt)
					abort();
			}
		}}
	});

	cout << endl << "lower_bound()" << endl;
	cppbench::print( cppbench::compare(rt) );
}

int
main()
{
	cout << "RUNNING TESTS FOR time_t" << endl;
	run_tests<time_t>();
	cout << endl << "RUNNING TESTS FOR boost::posix_time::ptime" << endl;
	run_tests<my_ptime>();
}
