/**
 * \file	tuple_vector.h
 * \author  Sinisa Susnjar <sinisa.susnjar@gmail.com>
 * \version 0.01
 */

#ifndef __tuple_vector_h
#define __tuple_vector_h

#include <stdexcept>
#include <vector>
#include <iostream>
#include <iomanip>
#include <memory>

#include <assert.h>

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
 * \brief This class is a custom container based on std::vector containing std::pair<K,V> tuples
 *		for *fast* find() and lower_bound() operations on strictly increasing timeseries.
 *		The find() and lower_bound() methods piggyback on the properties of strictly increasing
 *		timeseries by implementing an interpolation search with ~O(log log n) complexity.
 *		The container does not sort, so any data needs to be presorted. This is generally the
 *		case if your use-case involves reading timeseries data from a database or revceiving
 *		it via some data provider api.
 *		All modifying operations need to set m_recompute_range to true so that find() and
 *		lower_bound() can update internal book-keeping variables.
 * \tparam K A datetime type, e.g. time_t, boost::posix_time::ptime or similar.
 */
template<typename K, typename V>
class tuple_vector : public std::vector<std::pair<K,V>> {
public:
	typedef std::pair<K,V>										value_type;
	typedef typename std::vector<value_type>::iterator			iterator;
	typedef typename std::vector<value_type>::const_iterator	const_iterator;
	typedef typename std::vector<value_type>::size_type			size_type;

	tuple_vector() { }

	tuple_vector(size_type n) : std::vector<value_type>(n) { }

	~tuple_vector() { }

	// Modifying operations.
	inline void assign (const_iterator first, const_iterator last) {
		std::vector<value_type>::assign(first, last);
		m_recompute_range = true;
	}
	inline void assign (size_type n, const value_type& val) {
		std::vector<value_type>::assign(n, val);
		m_recompute_range = true;
	}
	inline void assign (std::initializer_list<value_type> il) {
		std::vector<value_type>::assign(il);
		m_recompute_range = true;
	}
	void clear() noexcept {
		std::vector<value_type>::clear();
		m_recompute_range = true;
	}
	inline void emplace_back(const value_type &&p) {
		std::vector<value_type>::emplace_back(std::forward<const value_type>(p));
		m_recompute_range = true;
	}
	template <typename... Args> iterator emplace (const_iterator pos, Args&&... args) {
		m_recompute_range = true;
		return std::vector<value_type>::emplace(pos, std::forward<Args>(args)...);
	}
	inline void emplace_back(const value_type &p) {
		std::vector<value_type>::emplace_back(p);
		m_recompute_range = true;
	}
	inline size_type erase(const K &key) {
		auto pos = find(key);
		m_recompute_range = true;
		std::vector<value_type>::erase(pos);
		return 1;
	}
	inline iterator erase(const_iterator pos) {
		m_recompute_range = true;
		return std::vector<value_type>::erase(pos);
	}
	inline iterator erase(const_iterator start, const_iterator end) {
		m_recompute_range = true;
		return std::vector<value_type>::erase(start, end);
	}
	inline iterator insert(const_iterator position, const value_type& val) {
		m_recompute_range = true;
		return std::vector<value_type>::insert(position, val);
	}
	inline iterator insert(const_iterator position, size_type n, const value_type& val) {
		m_recompute_range = true;
		return std::vector<value_type>::insert(position, n, val);
	}
	inline iterator insert(const_iterator position, const_iterator first, const_iterator last) {
		m_recompute_range = true;
		return std::vector<value_type>::insert(position, first, last);
	}
	inline iterator insert(const_iterator position, value_type&& val) {
		m_recompute_range = true;
		return std::vector<value_type>::insert(position, std::forward<value_type>(val));
	}
	inline iterator insert(const_iterator position, std::initializer_list<value_type> il) {
		m_recompute_range = true;
		return std::vector<value_type>::insert(position, il);
	}
	inline tuple_vector<K, V>& operator=(const std::vector<value_type>& x) {
		m_recompute_range = true;
		return std::vector<value_type>::operator=(x);
	}
	inline tuple_vector<K, V>& operator=(std::vector<value_type>&& x) {
		m_recompute_range = true;
		return std::vector<value_type>::operator=(std::forward<std::vector<value_type>>(x));
	}
	inline tuple_vector<K, V>& operator=(std::initializer_list<value_type> il) {
		m_recompute_range = true;
		return std::vector<value_type>::operator=(il);
	}
	inline void pop_back() {
		std::vector<value_type>::pop_back();
		m_recompute_range = true;
	}
	inline void push_back(const value_type& val) {
		m_recompute_range = true;
		std::vector<value_type>::push_back(val);
	}
	inline void push_back(value_type&& val) {
		m_recompute_range = true;
		std::vector<value_type>::push_back(std::forward<value_type>(val));
	}
	inline void resize(size_type n) {
		m_recompute_range = true;
		std::vector<value_type>::resize(n);
	}
	inline void resize(size_type n, const value_type& val) {
		m_recompute_range = true;
		std::vector<value_type>::resize(n, val);
	}
	inline void shrink_to_fit() {
		m_recompute_range = true;
		std::vector<value_type>::shrink_to_fit();
	}
	inline void swap (std::vector<value_type> &x) {
		m_recompute_range = true;
		std::vector<value_type>::swap(x);
	}

	// Access operations
	inline iterator at(const K &key) {
		auto pos = find(key);
		if (pos == std::vector<value_type>::end())
			throw std::out_of_range("iterator tuple_vector::at(const K &key)");
		return pos;
	}
	inline const_iterator at(const K &key) const {
		auto pos = find(key);
		if (pos == std::vector<value_type>::end())
			throw std::out_of_range("const_iterator tuple_vector::at(const K &key) const");
		return pos;
	}
	inline const value_type &operator[](size_t idx) const {
		return std::vector<value_type>::operator [](idx);
	}
	inline value_type &operator[](size_t idx) {
		return std::vector<value_type>::operator [](idx);
	}
	inline const value_type &operator[](const K &key) const {
		return lower_bound(key);
	}
	inline value_type &operator[](const K &key) {
		return lower_bound(key);
	}
	inline iterator lower_bound(const K &key) {
		return iterator(const_cast<value_type *>(_lower_bound(key)));
	}
	inline const_iterator lower_bound(const K &key) const {
		return const_iterator(_lower_bound(key));
	}
	inline iterator find(const K &key) {
		return iterator(const_cast<value_type *>(_find(key)));
	}
	inline const_iterator find(const K &key) const {
		return const_iterator(_find(key));
	}

	/// Reset housekeeping variables. Just for completeness - normally you don't need to call this.
	void reset() {
		m_recompute_range = true;
		m_hits = 0;
		m_outofbound = 0;
		m_recompute = 0;
		m_resync = 0;
		m_avg_diff = 0;
	}

	int hits() const { return m_hits; }
	int outofbound() const { return m_outofbound; }
	int recompute() const { return m_recompute; }
	int resync() const { return m_resync; }
	double avg_diff() const { return m_avg_diff; }

protected:
	inline const value_type *_find(const K &key) const
	{
		const value_type *rc = nullptr;

		if (m_recompute_range) {
			m_size = std::vector<value_type>::size();
			m_total_range = ::key<K>()(std::vector<value_type>::back().first) - ::key<K>()(std::vector<value_type>::front().first);
			m_element_range = m_total_range / (m_size-1);
			m_offset = 0;
			m_recompute_range = false;
			m_front = &std::vector<value_type>::front();
			m_back = &std::vector<value_type>::back();
			++m_recompute;
		}

		if (m_size == 0)
			return std::vector<value_type>::_M_impl._M_finish;

		size_t idx = (::key<K>()(key) - ::key<K>()(std::vector<value_type>::front().first)) / m_element_range + m_offset;

		if (idx >= 0 && idx < m_size) {
			rc = &std::vector<value_type>::operator[](idx);
		} else {
			if (idx > m_size)
				rc = &std::vector<value_type>::back();
			else
				rc = &std::vector<value_type>::front();
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

		return std::vector<value_type>::_M_impl._M_finish;
	}	// _find()

	inline const value_type *_lower_bound(const K &key) const
	{
		const value_type *rc = nullptr;

		if (m_recompute_range) {
			m_size = std::vector<value_type>::size();
			m_total_range = ::key<K>()(std::vector<value_type>::back().first) - ::key<K>()(std::vector<value_type>::front().first);
			m_element_range = m_total_range / (m_size-1);
			m_offset = 0;
			m_recompute_range = false;
			m_front = &std::vector<value_type>::front();
			m_back = &std::vector<value_type>::back();
			++m_recompute;
		}

		if (m_size == 0)
			return std::vector<value_type>::_M_impl._M_finish;

		size_t idx = (::key<K>()(key) - ::key<K>()(std::vector<value_type>::front().first)) / m_element_range + m_offset;

		if (idx >= 0 && idx < m_size) {
			rc = &std::vector<value_type>::operator[](idx);
		} else {
			if (idx > m_size)
				rc = &std::vector<value_type>::back();
			else
				rc = &std::vector<value_type>::front();
			m_offset = 0;
			++m_outofbound;
		}
		const value_type *old_rc = rc;
		if (rc->first > key) {
			for (; rc->first > key && rc > m_front; --rc);
			if (rc->first < key)
				++rc;
		} else {
			for (; rc->first < key && rc < m_back; ++rc);
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
		return std::vector<value_type>::_M_impl._M_finish;
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
	mutable const value_type *m_front = nullptr;
	mutable const value_type *m_back = nullptr;
	mutable double m_avg_diff = 0;
};

#endif /* __tuple_vector_h */
