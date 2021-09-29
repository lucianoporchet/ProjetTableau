#include <cstddef> // std::size_t
#include <algorithm>
#include <utility> // std::exchange
#include <initializer_list>
#include <iterator>

template<class T>
class Tableau {
public:
	using value_type = T;
	using size_type = std::size_t;
	using pointer = value_type*;
	using const_pointer = const value_type*;
	using reference = value_type&;
	using const_reference = const value_type&;
private:
	pointer elems{};
	size_type nelems{},
			  cap{};
public:
	[[nodiscard]] bool empty() const noexcept {
		return !size();
	}
	[[nodiscard]] size_type size() const noexcept {
		return nelems;
	}
	[[nodiscard]] size_type capacity() const noexcept {
		return cap;
	}
private:
	[[nodiscard]] bool full() const noexcept {
		return size() == capacity();
	}
public:
	using iterator = pointer;
	using const_iterator = const_pointer;
	[[nodiscard]] iterator begin() noexcept {
		return elems;
	}
	[[nodiscard]] iterator begin() const noexcept {
		return elems;
	}
	[[nodiscard]] const_iterator cbegin() const noexcept {
		return begin();
	}
	[[nodiscard]] iterator end() noexcept {
		return begin() + size();
	}
	[[nodiscard]] iterator end() const noexcept {
		return begin() + size();
	}
	[[nodiscard]] const_iterator cend() const noexcept {
		return end();
	}
	Tableau() = default;

	Tableau(size_type n, const_reference val)
		: elems{ new value_type[n] }, nelems{ n }, cap{ n } {
		try {
			std::fill(begin(), end(), val);
		}
		catch (...) {
			delete[]elems;
			throw;
		}
	}

	Tableau(const Tableau& autre) //john lakos
		: elems{ new value_type[autre.size()] }, nelems{ autre.size() }, cap{ autre.size() } {
		try {
			std::copy(autre.begin(), autre.end(), begin());
		}
		catch (...) {
			delete[]elems;
			throw;
		}
	}

	Tableau(Tableau&& autre) noexcept
		: elems{ std::exchange(autre.elems, nullptr) },
		  nelems{ std::exchange(autre.nelems, 0) },
		  cap{ std::exchange(autre.cap, 0) } {
	}

	~Tableau() {
		delete[] elems;
	}

	void swap(Tableau& autre) noexcept {
		using std::swap;
		swap(elems, autre.elems);
		swap(nelems, autre.nelems);
		swap(cap, autre.cap);
	}
	Tableau& operator=(const Tableau & autre){
		Tableau{ autre }.swap(*this); //idiome d'affect securitaire
		return *this;
	}
	Tableau& operator=(Tableau&& autre) noexcept {
		Tableau{ std::move(autre) }.swap(*this); //idiome d'affect securitaire
		return *this;
	}
	reference operator[](size_type n) noexcept {
		return elems[n];
	}
	const_reference operator[](size_type n) const noexcept {
		return elems[n];
	}
	bool operator==(const Tableau& autre) const noexcept {
		return size() == autre.size() && 
			   std::equal(begin(), end(), autre.begin());
	}
	bool operator!=(const Tableau& autre) const noexcept {
		return !(*this == autre);
	}
	void push_back(const_reference obj) {
		if (full()) grow();
		//option A
		/*elems[nelems++] = obj;*/ //DANGER quand generique
		//option B
		elems[size()] = obj;
		++nelems;
	}
private:
	void grow() {
		const auto nouv_cap = capacity() ? capacity() * 2 : 64;
		auto p = new value_type[nouv_cap];
		try {
			std::copy(begin(), end(), p);
			delete[]elems;
			elems = p;
			cap = nouv_cap;
		}
		catch (...) {
			delete[]p;
			throw;
		}
	}

public:

	Tableau(std::initializer_list<value_type>lst)
		: elems{ new value_type[lst.size()] }, nelems{ lst.size() }, cap{ lst.size() }{
		try {
			std::copy(lst.begin(), lst.end(), begin());
		}
		catch (...) {
			delete[]elems;
			throw;
		}
	}

	template<class It>
	Tableau(It deb, It fin) 
		: nelems( std::distance(deb, fin) ) {
		cap = size();
		elems = new value_type[capacity()];
		try {
			std::copy(de, fin, begin());
		}
		catch (...) {
			delete[]elems;
			throw;
		}
	}

	template<class U>
	Tableau(const Tableau<U> &autre)
		: elems{ new value_type[autre.size()] }, nelems{ autre.size() }, cap{ autre.cap }{
		try {
			std::copy(autre.begin(), autre.end(), begin());
		}
		catch (...) {
			delete[]elems;
			throw;
		}
	}

	template<class U>
	Tableau& operator=(const Tableau<U>& autre) {
		Tableau{ autre }.swap(*this);
		return *this;
	}
	
};


//#include <vector>
//bool operator==(const std::vector<int>&, const Tableau&);
//bool operator==(const Tableau&, const std::vector<int>&);
//
//inline bool operator==(const Tableau& t0, const Tableau& t1){
//	return t0.size() == t1.size() &&
//		std::equal(t0.begin(), t0.end(), t1.begin());
//}