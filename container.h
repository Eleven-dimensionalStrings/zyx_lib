#pragma once
#include <deque>
#include <initializer_list>
#include <memory>
namespace my_container
{
	template<class T, class Alloc = std::allocator<T>>
	class my_vector
	{
	private:
		Alloc alloc;
		T * val;
		std::size_t vsize;
		std::size_t vcap;
	public:
		using value_type = T;
		//ini
		my_vector() :alloc(Alloc()), val(nullptr), vsize(0), vcap(0)
		{
		}
		explicit my_vector(std::size_t tsize, const Alloc& talloc = Alloc()) :alloc(talloc), val(alloc.allocate(tsize)), vsize(tsize), vcap(tsize)
		{
			for (std::size_t i = 0; i < vsize; ++i)
			{
				std::allocator_traits<Alloc>::construct(alloc, val + i);
			}

		}
		explicit my_vector(std::size_t tsize, const T& callableObj, const Alloc& talloc = Alloc()) :
			alloc(talloc), val(alloc.allocate(tsize)), vsize(tsize), vcap(tsize)
		{
			for (std::size_t i = 0; i < vsize; ++i)
			{
				std::allocator_traits<Alloc>::construct(alloc, val + i, callableObj);
			}
		}

		my_vector(std::initializer_list<T> ini) :alloc(Alloc()),
			val(alloc.allocate(ini.size())), vsize(ini.size()), vcap(vsize)
		{
			auto p = ini.begin();
			for (std::size_t i = 0; i < vsize; ++i, ++p)
			{
				std::allocator_traits<Alloc>::construct(alloc, val + i, std::move(*p));
			}
		}
		my_vector(const my_vector& other) :alloc(other.alloc), val(alloc.allocate(other.capacity()))
			, vsize(other.size()), vcap(other.capacity())
		{
			//uninitialized_copy(other.begin(), other.end(), val);
			for (std::size_t i = 0; i < size(); ++i)
				std::allocator_traits<Alloc>::construct(alloc, val + i, other[i]);
		}
		my_vector(my_vector&& other) :alloc(std::move(other.alloc)),
			val(other.val), vsize(other.size()), vcap(other.capacity())
		{
			other.val = nullptr;
			other.vsize = 0;
			other.vcap = 0;
			other.~my_vector();
		}
		my_vector& operator=(const my_vector& other)
		{
			this->~my_vector();
			alloc = other.alloc;
			val = alloc.allocate(other.capacity());
			vsize = other.size();
			vcap = other.capacity();
			for (std::size_t i = 0; i < size(); ++i)
			{
				std::allocator_traits<Alloc>::construct(alloc, val + i, other[i]);
			}
			return *this;
		}
		my_vector& operator=(my_vector&& other)
		{
			this->~my_vector();
			alloc = std::move(other.alloc);
			val = other.val;
			vsize = other.size();
			vcap = other.capacity();
			other.val = nullptr;
			other.vsize = 0;
			other.vcap = 0;
			other.~my_vector();
			return *this;
		}
		~my_vector()
		{
			clear();
			alloc.deallocate(val, capacity());
		}

		//iterator
		class iterator
		{
		public:
			friend class my_vector;
			iterator(T* begin, T* now, T* end) :_begin(begin), _now(now), _end(end)
			{

			}
			iterator(const iterator&t) :_begin(t._begin), _now(t._now), _end(t._end)
			{

			}
			iterator& operator++()
			{
				++_now;
				return *this;
			}
			iterator operator++(int)
			{
				++_now;
				return iterator(_begin, _now - 1, _end);
			}
			iterator& operator--()
			{
				--_now;
				return *this;
			}
			iterator operator--(int)
			{
				--_now;
				return iterator(_begin, _now + 1, _end);
			}
			bool operator==(const iterator& other)const
			{
				if (_now != other._now)return 0;
				//if (_begin != other._begin)return 0;
				//if (_end != other._end)return 0;
				return 1;
			}
			bool operator!=(const iterator& other)const
			{
				//maybe should check something on _DEBUG
				return _now != other._now;
			}
			iterator operator+(int os)
			{
				return iterator(_begin, _now + os, _end);
			}
			iterator operator-(int os)
			{
				return iterator(_begin, _now - os, _end);
			}
			int operator-(const iterator& other)
			{
				return _now - other._now;
			}
			T& operator*()
			{
#ifdef _DEBUG
				if (_now < _begin || _now >= _end)
				{
					throw std::out_of_range("iterator out of range");
				}
#endif // _DEBUG
				return *_now;
			}
			T* operator->()
			{
				return _now;
			}
		private:
			T * _begin, *_now, *_end;
		};

		class const_iterator
		{
		public:
			const_iterator(T* begin, T* now, T* end) :_begin(begin), _now(now), _end(end)
			{

			}
			const_iterator(const const_iterator&t) :_begin(t._begin), _now(t._now), _end(t._end)
			{

			}
			const_iterator(const iterator&t) :_begin(t.begin), _now(t._now), _end(t._end)
			{

			}
			const_iterator& operator++()
			{
				++_now;
				return *this;
			}
			const_iterator operator++(int)
			{
				++_now;
				return iterator(_begin, _now - 1, _end);
			}
			const_iterator& operator--()
			{
				--_now;
				return *this;
			}
			const_iterator operator--(int)
			{
				--_now;
				return iterator(_begin, _now + 1, _end);
			}
			bool operator==(const const_iterator& _other)const
			{
				if (_now != _other._now)return 0;
				if (_begin != _other._begin)return 0;
				if (_end != _other._end)return 0;
				return 1;
			}
			bool operator!=(const const_iterator& other)const
			{
				//maybe should check something on _DEBUG
				return _now != other._now;
			}
			const T& operator*()const
			{
#ifdef _DEBUG
				if (_now < _begin || _now >= _end)
				{
					throw std::out_of_range("iterator out of range");
				}
#endif // _DEBUG
				return *_now;
			}
		private:
			T * _begin, *_now, *_end;
		};
		class reverse_iterator
		{
		public:
			reverse_iterator(T* rbegin, T* now, T* rend) :_rbegin(rbegin), _now(now), _rend(rend)
			{

			}
			reverse_iterator(const reverse_iterator&t) :_rbegin(t._rbegin), _now(t._now), _rend(t._rend)
			{

			}
			reverse_iterator& operator++()
			{
				--_now;
				return *this;
			}
			reverse_iterator operator++(int)
			{
				--_now;
				return iterator(_rbegin, _now + 1, _rend);
			}
			reverse_iterator& operator--()
			{
				++_now;
				return *this;
			}
			reverse_iterator operator--(int)
			{
				++_now;
				return iterator(_rbegin, _now - 1, _rend);
			}
			bool operator==(const reverse_iterator& _other)const
			{
				if (_now != _other._now)return 0;
				if (_rbegin != _other._rbegin)return 0;
				if (_rend != _other._rend)return 0;
				return 1;
			}
			bool operator!=(const reverse_iterator& _other)const
			{
				//maybe should check something on _DEBUG
				return _now != _other._now;
			}
			T& operator*()
			{
#ifdef _DEBUG
				if (_now > _rbegin || _now <= _rend)
				{
					throw std::out_of_range("iterator out of range");
				}
#endif // _DEBUG
				return *_now;
			}
		private:
			T * _rbegin, *_now, *_rend;
		};
		iterator begin()
		{
			return iterator(val, val, val + vsize);
		}
		iterator end()
		{
			return iterator(val, val + vsize, val + vsize);
		}
		const_iterator cbegin()
		{
			return iterator(val, val, val + vsize);
		}
		const_iterator cend()
		{
			return iterator(val, val + vsize, val + vsize);
		}
		reverse_iterator rbegin()
		{
			return reverse_iterator(val + vsize - 1, val + vsize - 1, val - 1);
		}
		reverse_iterator rend()
		{
			return reverse_iterator(val + vsize - 1, val - 1, val - 1);
		}

		//revise
		void push_back(const T& value)
		{
			if (size() >= capacity())
			{
				this->reserve((size() + 1) * 2);
			}
			std::allocator_traits<Alloc>::construct(alloc, val + vsize, value);
			++vsize;
		}
		void push_back(T&& value)
		{
			if (size() >= capacity())
			{
				this->reserve((size() + 1) * 2);
			}
			std::allocator_traits<Alloc>::construct(alloc, val + vsize, std::move(value));
			++vsize;
		}
		void pop_back()
		{
#ifdef _DEBUG
			if (empty())
			{
				throw std::invalid_argument("my_vector empty before pop");
			}
#endif // _DEBUG
			--vsize;
			(val + vsize)->~T();
		}
		iterator insert(iterator i, const T& value)
		{
#ifdef _DEBUG
			if (i._begin != val || i._end != val + vsize)
			{
				throw std::invalid_argument("iterator failure,its begin or end is expired");
			}
			if (i._now < val || i._now > val + vsize)
			{
				throw std::out_of_range("iterator out of range");
			}
#endif // _DEBUG
			std::size_t os = i._now - i._begin;
			reserve(size() + 1);
			i._now = val + os;
			for (auto j = val + vsize; j != val + os; --j)
			{
				std::allocator_traits<Alloc>::construct(alloc, j, std::move(*(j - 1)));
				(j - 1)->~T();
			}
			std::allocator_traits<Alloc>::construct(alloc, val + os, value);
			++vsize;
			return iterator(val, val + os, val + size());
		}
		iterator insert(iterator i, T&& value)
		{
#ifdef _DEBUG
			if (i._begin != val || i._end != val + vsize)
			{
				throw std::invalid_argument("iterator failure,its begin or end is expired");
			}
			if (i._now < val || i._now > val + vsize)
			{
				throw std::out_of_range("iterator out of range");
			}
#endif // _DEBUG
			std::size_t os = i._now - i._begin;
			reserve(size() + 1);
			i._now = val + os;
			for (auto j = val + vsize; j != val + os; --j)
			{
				std::allocator_traits<Alloc>::construct(alloc, j, std::move(*(j - 1)));
				(j - 1)->~T();
			}
			std::allocator_traits<Alloc>::construct(alloc, val + os, value);
			++vsize;
			return iterator(val, val + os, val + size());
		}
		iterator erase(iterator i)
		{
#ifdef _DEBUG
			if (i._begin != val || i._end != val + vsize)
			{
				throw std::invalid_argument("iterator failure,its begin or end is expired");
			}
			if (i._now < val || i._now >= val + vsize)
			{
				throw std::out_of_range("iterator out of range");
			}
#endif // _DEBUG
			for (auto* j = i._now; j != i._end - 1; ++j)
			{
				j->~T();
				std::allocator_traits<Alloc>::construct(alloc, j, std::move(*(j + 1)));
			}
			(i._end - 1)->~T();
			--vsize;
			return iterator(val, i._now, val + vsize);
		}
		iterator erase(iterator l, iterator r)
		{
#ifdef _DEBUG
			/*if (l._begin != val || l._end != val + vsize || r._begin != val || r._end != val + vsize)
			{
				throw invalid_argument("iterator failure,its begin or end is expired");
			}
			if (l._now < val || l._now >= val + vsize || r._now < val || r._now > val + vsize)
			{
				throw out_of_range("iterator out of range");
			}*/
#endif // _DEBUG
			if (l + 1 == r)
			{
				erase(l);
			}
			else
			{
				for (auto p = l; p != r - 1; ++p)
				{
					p._now->~T();
				}
				int os = r - l;
				for (auto p = r; p != end(); ++p)
				{
					std::allocator_traits<Alloc>::construct(alloc, p._now - os, std::move(*(p._now)));
				}
			}
			vsize -= r._now - l._now;
			return iterator(val, l._now, val + vsize);
		}
		void clear()noexcept
		{
			for (size_t i = 0; i < vsize; ++i)
			{
				(val + i)->~T();
			}
			vsize = 0;
		}
		void swap(my_vector&other)noexcept
		{
			auto v = std::move(*this);
			*this = std::move(other);
			other = std::move(v);
		}

		//access
		T& operator[](std::size_t pos)
		{
#ifdef _DEBUG
			if (pos < 0 || pos >= vsize)
				throw std::out_of_range("my_vector out of range");
#endif // _DEBUG
			return *(val + pos);
		}
		const T& operator[](std::size_t pos)const
		{
#ifdef _DEBUG
			if (pos < 0 || pos >= vsize)
				throw std::out_of_range("my_vector out of range");
#endif // _DEBUG
			return *(val + pos);
		}
		T& at(std::size_t pos)
		{
			if (pos < 0 || pos >= size())
			{
				throw std::out_of_range("my_vector out of range");
			}
			return *(val + pos);
		}
		T& front()
		{
			return *val;
		}
		T& back()
		{
			return *(val + vsize - 1);
		}
		T* data()
		{
			return val;
		}

		//capacity
		std::size_t size()const
		{
			return vsize;
		}
		std::size_t capacity()const
		{
			return vcap;
		}
		bool empty()const noexcept
		{
			return !static_cast<bool>(vsize);
		}
		void reserve(std::size_t new_cap)
		{
			if (new_cap > this->capacity())
			{
				auto*p = alloc.allocate(new_cap);
				for (std::size_t i = 0; i < this->size(); ++i)
				{
					//*(p + i) = std::move(*(val + i));
					std::allocator_traits<Alloc>::construct(alloc, p + i, std::move_if_noexcept(*(val + i)));
				}
				alloc.deallocate(val, this->capacity());
				val = p;
				vcap = new_cap;
			}
		}
	};

	template<class T, class Container = std::deque<T>>
	class my_queue
	{
	protected:
		Container deq;
	public:
		my_queue() :deq()
		{

		}
		my_queue(const my_queue&other) :deq(other.deq)
		{

		}
		my_queue(my_queue&& other) :deq(std::move(other.deq))
		{

		}
		my_queue& operator=(const my_queue&other)
		{
			deq = other.deq;
			return *this;
		}
		my_queue& operator=(my_queue&&other)
		{
			deq = std::move(other.deq);
			return *this;
		}
		//access
		T& front()
		{
#ifdef _DEBUG

#endif // _DEBUG

			return deq.front();
		}
		T& back()
		{
			return deq.back();
		}
		//capacity
		bool empty()const
		{
			return deq.empty();
		}
		std::size_t size()const
		{
			return deq.size();
		}
		//revise
		void push(const T& value)
		{
			deq.push_back(value);
		}
		void push(T&& value)
		{
			deq.push_back(std::move(value));
		}
		void pop()
		{
			deq.pop_front();
		}
		void swap(my_queue& other)noexcept
		{
			auto t = std::move(other);
			other = std::move(*this);
			*this = std::move(t);
		}
	};
	//Container should be queue, unnecesary too
	template<class T, class Container = my_vector<T>>
	class my_stack
	{
	private:
		Container deq;
	public:
		my_stack() :deq()
		{

		}
		my_stack(const my_stack&other) :deq(other.deq)
		{

		}
		my_stack(my_stack&&other) :deq(std::move(other.deq))
		{

		}
		my_stack& operator=(const my_stack&other)
		{
			deq = other.deq;
			return *this;
		}
		my_stack& operator=(my_stack&&other)
		{
			deq = std::move(other.deq);
			return *this;
		}
		//access
		T& top()
		{
#ifdef _DEBUG

#endif // _DEBUG
			return deq.back();
		}
		//capacity
		bool empty()const
		{
			return deq.empty();
		}
		std::size_t size()const
		{
			return deq.size();
		}
		//revise
		void push(const T& value)
		{
			deq.push_back(value);
		}
		void push(T&& value)
		{
			deq.push_back(std::move(value));
		}
		void pop()
		{
			deq.pop_back();
		}
		void swap(my_stack& other)noexcept
		{
			auto t = std::move(other);
			other = std::move(*this);
			*this = std::move(t);
		}
	};
	template<class T, class Container = my_vector<T>>
	class insert_sort_vector
	{
	private:
		Container vec;
	public:
		insert_sort_vector() :vec()
		{

		}
		insert_sort_vector(const insert_sort_vector& other) :vec(other.vec)
		{

		}
		insert_sort_vector(insert_sort_vector&& other) :vec(std::move(other.vec))
		{

		}
		insert_sort_vector& operator=(const insert_sort_vector& other)
		{
			vec = other.vec;
			return *this;
		}
		insert_sort_vector& operator=(insert_sort_vector&& other)
		{
			vec = std::move(other);
			return *this;
		}
		void push(const T& value)
		{
			auto i = vec.begin();
			for (; i != vec.end(); ++i)
			{
				if (value.compare_value() == i->compare_value())break;
			}
			vec.insert(i, value);
		}
		void push(T&& value)
		{
			auto i = vec.begin();
			for (; i != vec.end(); ++i)
			{
				if (value.compare_value() == i->compare_value())break;
			}
			vec.insert(i, std::move(value));
		}
		typename Container::iterator insert(typename Container::iterator i, const T& value)
		{
			return vec.insert(i, value);
		}
		typename Container::iterator insert(typename Container::iterator i, T&& value)
		{
			return vec.insert(i, std::move(value));
		}
		typename Container::iterator erase(typename Container::iterator i)
		{
			return vec.erase(i);
		}
		std::size_t size()const
		{
			return vec.size();
		}
		std::size_t capacity()const
		{
			return vec.capacity();
		}
		void reserve(std::size_t new_cap)
		{
			vec.reserve(new_cap);
		}
		typename Container::iterator begin()
		{
			return vec.begin();
		}
		typename Container::iterator end()
		{
			return vec.end();
		}
		T& operator[](std::size_t pos)
		{
			return vec[pos];
		}
		void clear()
		{
			vec.clear();
		}
	};
}