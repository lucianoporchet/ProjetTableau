#include <cstddef> // std::size_t
#include <algorithm>
#include <utility>
class Tableau {
public:
	using value_type = int;
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
	bool empty() const noexcept {
		return !size();
	}
	size_type size() const noexcept {
		return nelems;
	}
	size_type capacity() const noexcept {
		return cap;
	}
private:
	bool full() const noexcept {
		return size() == capacity();
	}
public:
	using iterator = pointer;
	using const_iterator = const_pointer;
	iterator begin() noexcept {
		return elems;
	}
	iterator begin() const noexcept {
		return elems;
	}
	const_iterator cbegin() const noexcept {
		return begin();
	}
	iterator end() noexcept {
		return begin() + size();
	}
	iterator end() const noexcept {
		return begin() + size();
	}
	const_iterator cend() const noexcept {
		return end();
	}
	Tableau() = default;
	Tableau(size_type n, const_reference val)
		: elems{ new value_type[n] }, nelems{ n }, cap{ n } {
		std::fill(begin(), end(), val);	
	}
	Tableau(const Tableau& autre) //john lakos
		: elems{ new value_type[autre.size()] }, nelems{ autre.size() }, cap{ autre.size() } {
		std::copy(autre.begin(), autre.end(), begin());
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
};
