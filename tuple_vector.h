/*
 * tuple_vector.h
 *
 *  Created on: 23.10.2016
 *      Author: sini
 */

#ifndef __tuple_vector_h
#define __tuple_vector_h

#include <stdexcept>
#include <vector>
#include <iostream>
#include <iomanip>
#include <memory>

#include <assert.h>
#include <pthread.h>

using namespace std;

/**
 * \brief key functor used to do numerical calculations with the given type K
 * 		since not all date/time types are readily convertible into numeric types
 * 		the user has to provide a template specialisation
 * \tparam K this should be the same key type as used for the tuple_vector definition
 */
template<typename K>
struct key {
    double operator()(const K &x) const;
};

/**
 * \brief This class represents a generic timeseries template.
 *
 * Keys and values can be chosen at will, where a key will usually be a date based entity,
 * while a value will be something else. Example:
 * \code
 * tuple_vector<time_t, double> ts;
 * \endcode
 * \author Sinisa Susnjar
 */

// declare a custom container based on a vector containing pair<K,V> tuples
template<typename K, typename V>
class tuple_vector : public vector<pair<K,V>> {
static constexpr bool debug = false;
public:
	typedef pair<K,V>											value_type;
	typedef typename vector<value_type>::iterator				iterator;
	typedef typename vector<value_type>::const_iterator			const_iterator;
	typedef typename vector<value_type>::size_type				size_type;

	tuple_vector() { }

	tuple_vector(size_type n) : vector<value_type>(n) { }

	~tuple_vector() { }

	// all modifying operations need to set m_recompute_range to true so that find(),
	// lower_bound() can update internal book-keeping variables
	inline void assign (const_iterator first, const_iterator last) {
		vector<value_type>::assign(first, last);
		m_recompute_range = true;
	}
	inline void assign (size_type n, const value_type& val) {
		vector<value_type>::assign(n, val);
		m_recompute_range = true;
	}
	inline void assign (initializer_list<value_type> il) {
		vector<value_type>::assign(il);
		m_recompute_range = true;
	}
	void clear() noexcept {
		vector<value_type>::clear();
		m_recompute_range = true;
	}
	inline void emplace_back(const value_type &&p)
	{
		vector<value_type>::emplace_back(p);
		m_recompute_range = true;
	}
	inline iterator emplace(const_iterator pos, value_type &v) {
		m_recompute_range = true;
		return vector<value_type>::emplace(pos, v);
	}
	inline void emplace_back(const value_type &p)
	{
		vector<value_type>::emplace_back(p);
		m_recompute_range = true;
	}
	inline size_type erase(const K &key)
	{
		auto pos = find(key);
		m_recompute_range = true;
		vector<value_type>::erase(pos);
		return 1;
	}
	inline iterator erase(const_iterator pos)
	{
		m_recompute_range = true;
		return vector<value_type>::erase(pos);
	}
	inline iterator erase(const_iterator start, const_iterator end)
	{
		m_recompute_range = true;
		return vector<value_type>::erase(start, end);
	}
	inline iterator insert(const_iterator position, const value_type& val) {
		m_recompute_range = true;
		return vector<value_type>::insert(position, val);
	}
	inline iterator insert(const_iterator position, size_type n, const value_type& val) {
		m_recompute_range = true;
		return vector<value_type>::insert(position, n, val);
	}
	inline iterator insert(const_iterator position, const_iterator first, const_iterator last) {
		m_recompute_range = true;
		return vector<value_type>::insert(position, first, last);
	}
	inline iterator insert(const_iterator position, value_type&& val) {
		m_recompute_range = true;
		return vector<value_type>::insert(position, val);
	}
	inline iterator insert(const_iterator position, initializer_list<value_type> il) {
		m_recompute_range = true;
		return vector<value_type>::insert(position, il);
	}
	inline tuple_vector<K, V>& operator=(const vector<value_type>& x) {
		m_recompute_range = true;
		return vector<value_type>::operator=(x);
	}
	inline tuple_vector<K, V>& operator=(vector<value_type>&& x) {
		m_recompute_range = true;
		return vector<value_type>::operator=(x);
	}
	inline tuple_vector<K, V>& operator=(initializer_list<value_type> il) {
		m_recompute_range = true;
		return vector<value_type>::operator=(il);
	}
	inline void pop_back() {
		vector<value_type>::pop_back();
		m_recompute_range = true;
	}
	inline void push_back(const value_type& val) {
		m_recompute_range = true;
		vector<value_type>::push_back(val);
	}
	inline void push_back(value_type&& val) {
		m_recompute_range = true;
		vector<value_type>::push_back(val);
	}
	inline void resize(size_type n) {
		m_recompute_range = true;
		vector<value_type>::resize(n);
	}
	inline void resize(size_type n, const value_type& val) {
		m_recompute_range = true;
		vector<value_type>::resize(n, val);
	}
	inline void shrink_to_fit() {
		m_recompute_range = true;
		vector<value_type>::shrink_to_fit();
	}
	inline void swap (vector<value_type> &x) {
		m_recompute_range = true;
		vector<value_type>::swap(x);
	}

	// access operators
	// map::at()
	inline iterator at(const K &key)
	{
		auto pos = find(key);
		if (pos == vector<value_type>::end())
			throw std::out_of_range("iterator tuple_vector::at(const K &key)");
		return pos;
	}
	inline const_iterator at(const K &key) const
	{
		auto pos = find(key);
		if (pos == vector<value_type>::end())
			throw std::out_of_range("const_iterator tuple_vector::at(const K &key) const");
		return pos;
	}

	/**
	 * \brief Const array access operator.
	 * \param idx Index into the timeseries data.
	 * \return Const reference to the appropriate timeseries value.
	 */
	/*
	inline const V &operator[](unsigned idx) const {
		return vector<value_type>::operator [](idx).second;
	}
	*/

	/**
	 * \brief Mutable array access operator.
	 * \param idx Index into the timeseries data.
	 * \return Mutable reference to the appropriate timeseries value.
	 */
	/*
	inline V &operator[](unsigned idx)	{
		return vector<value_type>::operator [](idx).second;
	}
	*/

	/**
	 * \brief Const array access operator.
	 * \param key The key whose value should be found.
	 * \return Const reference to the appropriate timeseries value.
	 */
	inline const V &operator[](const K &key) const { return lower_bound(key)->second; }

	/**
	 * \brief Mutable array access operator.
	 * \param key The key whose value should be found.
	 * \return Mutable reference to the appropriate timeseries value.
	 */
	inline V &operator[](const K &key)	{ return lower_bound(key)->second; }

	/*
	friend std::ostream &operator<< (std::ostream &os, const tuple_vector<K, V> &t) {
		auto pos = vector<value_type>::crbegin();
		os << "timeseries[" << std::hex << &t << std::dec << "] = {" << std::endl
			<< "  size = " << t.size() << std::endl
			<< "  last = " << pos->second << " @ " << pos->first << std::endl
			<< "}" << std::endl;
		return os;
	}
	*/

	inline iterator lower_bound(const K &key)
	{
		return iterator(const_cast<value_type *>(_lower_bound(key)));
	}

	inline const_iterator lower_bound(const K &key) const
	{
		return const_iterator(_lower_bound(key));
	}

	// reset housekeeping variables
	void reset()
	{
		m_recompute_range = true;
		m_hits = 0;
		m_outofbound = 0;
		m_recompute = 0;
		m_resync = 0;
		m_avg_diff = 0;
	}

	inline iterator find(const K &key)
	{
		return iterator(const_cast<value_type *>(_find(key)));
	}

	inline const_iterator find(const K &key) const
	{
		return const_iterator(_find(key));
	}

	int resync() const { return m_resync; }
	int recompute() const { return m_recompute; }
	int outofbound() const { return m_outofbound; }
	int hits() const { return m_hits; }
	double avg_diff() const { return m_avg_diff; }

protected:
	inline const value_type *_find(const K &key) const
	{
		const value_type *rc = nullptr;

		if (m_recompute_range) {
			m_size = vector<value_type>::size();
			m_total_range = ::key<K>()(vector<value_type>::back().first) - ::key<K>()(vector<value_type>::front().first);
			m_element_range = m_total_range / (m_size-1);
			m_offset = 0;
			m_recompute_range = false;
			m_front = &vector<value_type>::front();
			m_back = &vector<value_type>::back();
			++m_recompute;
		}

		if (m_size == 0)
			return vector<value_type>::_M_impl._M_finish;

		size_t idx = (::key<K>()(key) - ::key<K>()(vector<value_type>::front().first)) / m_element_range + m_offset;

		if (idx >= 0 && idx < m_size) {
			rc = &vector<value_type>::operator[](idx);
		} else {
			if (idx > m_size)
				rc = &vector<value_type>::back();
			else
				rc = &vector<value_type>::front();
			m_offset = 0;
			++m_outofbound;
		}
		if (rc->first == key) {
			++m_hits;
			return rc;
		}
		const value_type *old_rc = rc;
		if (rc->first > key)
			for (; rc->first > key && rc > m_front; --rc);
		else
			for (; rc->first < key && rc < m_back; ++rc);
		m_avg_diff *= m_resync;
		m_avg_diff += rc - old_rc;
		m_offset += rc - old_rc;
		++m_resync;
		m_avg_diff /= m_resync;
		m_offset = (double)m_offset + m_avg_diff + .5;

		if (rc->first == key)
			return rc;

		return vector<value_type>::_M_impl._M_finish;
	}	// _find()

	inline const value_type *_lower_bound(const K &key) const
	{
		const value_type *rc = nullptr;

		if (m_recompute_range) {
			m_size = vector<value_type>::size();
			m_total_range = ::key<K>()(vector<value_type>::back().first) - ::key<K>()(vector<value_type>::front().first);
			m_element_range = m_total_range / (m_size-1);
			m_offset = 0;
			m_recompute_range = false;
			m_front = &vector<value_type>::front();
			m_back = &vector<value_type>::back();
			++m_recompute;
		}

		if (m_size == 0)
			return vector<value_type>::_M_impl._M_finish;

		size_t idx = (::key<K>()(key) - ::key<K>()(vector<value_type>::front().first)) / m_element_range + m_offset;

		if (idx >= 0 && idx < m_size) {
			rc = &vector<value_type>::operator[](idx);
		} else {
			if (idx > m_size)
				rc = &vector<value_type>::back();
			else
				rc = &vector<value_type>::front();
			m_offset = 0;
			++m_outofbound;
		}
		/*
		if (rc->first >= key) {
			++m_hits;
			// return iterator(rc);
			return rc;
		}
		*/
		const value_type *old_rc = rc;
		if (rc->first > key) {
			/*
			if (rc->first > key) {
				cout << "rc->first > key" << endl;
			} else {
				cout << "! rc->first > key" << endl;
			}
			*/
			for (; rc->first > key && rc > m_front; --rc) {
				// if (debug) rc->first.dump(cout);
			}
			/*
			if (rc == m_front && rc->first < key)
				return vector<value_type>::_M_impl._M_finish;
				*/
			if (rc->first < key)
				++rc;
		} else {
			/*
			if (rc->first < key) {
				cout << "rc->first < key" << endl;
			} else {
				cout << "! rc->first < key" << endl;
			}
			*/
			for (; rc->first < key && rc < m_back; ++rc) {
				// if (debug) rc->first.dump(cout);
			}
			/*
			if (rc == m_back && rc->first < key)
				return vector<value_type>::_M_impl._M_finish;
				*/
			if (rc->first < key)
				--rc;
		}
		m_avg_diff *= m_resync;
		m_avg_diff += rc - old_rc;
		m_offset += rc - old_rc;
		++m_resync;
		m_avg_diff /= m_resync;
		m_offset = (double)m_offset + m_avg_diff + .5;

		if (rc->first >= key)
			return rc;
			// return iterator(rc);
		if (debug) cout << "end!" << endl;

		return vector<value_type>::_M_impl._M_finish;
		// return vector<value_type>::end();
	}	// _lower_bound()

private:
	mutable size_t m_size = 0;
	mutable double m_total_range = 0;
	mutable double m_element_range = 0;
	mutable int m_offset = 0;
	mutable bool m_recompute_range = false;
	mutable int m_resync = 0;
	mutable int m_recompute = 0;
	mutable int m_outofbound = 0;
	mutable int m_hits = 0;
	mutable const value_type *m_last = nullptr;
	mutable const value_type *m_front = nullptr;
	mutable const value_type *m_back = nullptr;
	mutable double m_avg_diff = 0;
};

#endif /* __tuple_vector_h */
