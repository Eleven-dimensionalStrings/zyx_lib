#pragma once
#include <iostream>
#include <memory>
namespace LemonJavaArray
{

	template<class T>
	class my_ptr
	{
	private:
		int flag;
		int* c;
		T* v;
	public:
		my_ptr();
		my_ptr(T* t);
		my_ptr(my_ptr&t);
		my_ptr(my_ptr&& t);
		~my_ptr();

		my_ptr<T>& my_ptr::operator=(my_ptr<T>&t);
		T& my_ptr::operator*();
		T* my_ptr::operator->();
		T* get();
	};


	class jarray1
	{
	public:
		jarray1();
		jarray1(std::initializer_list<int>li);
		jarray1(int ts);
		jarray1(int ts1, int ini);
		jarray1(jarray1&t);
		int& jarray1::operator[](int k);
		int size();
		int* begin();
		const int* const end();
	private:
		my_ptr<int>data;
		int s;
	};




	class jarray2
	{
	public:
		class c
		{
			friend class jarray2;
			int s2, k1, s1;
			int* v;
		public:
			c(int ts1, int ts2);
			~c();
			int& c::operator[](int k);
			int size();
		};
		jarray2();
		jarray2(int ts1, int ts2);
		jarray2(int ts1, int ts2, int ini);
		jarray2(jarray2& t);
		jarray2(jarray2&& t);
		c& jarray2::operator[](int k);
		jarray2 operator=(jarray2& t);
		jarray2 operator=(jarray2&& t);
		int size();
	private:
		my_ptr<c>data;
		int s1;
	};



/*	so fucking to make it right
	class jarray3
	{
	public:
		class a
		{
		public:
			a()
			{
			}
			~a()
			{
				delete[] v;
			}
			int s;
			int * v;
			int index;
			int size()
			{
				return s;
			}
			int& a::operator[](int k)
			{
				return v[index + k];
			}
		};
		class b
		{
		public:
			b()
			{
				aa = new a();
			}
			~b()
			{
				delete aa;
			}
			int s;
			a* aa;
			int size()
			{
				return s;
			}
			a& b::operator[](int k)
			{
				aa->index += k*aa->s;
				return *aa;
			}
		};
		jarray3()
		{
		}
		jarray3(int ts1, int ts2, int ts3)
		{
			data = my_ptr<b>(new b());
			s = ts1;
			data->s = ts2;
			data->aa->s = ts3;
			data->aa->v = new int[ts1*ts2*ts3];
		}
		jarray3(int ts1, int ts2, int ts3, int ini) :jarray3(ts1, ts2, ts3)
		{
			if (ini)
				memset(data->aa->v, 0, ts1*ts2*ts3 * 4);
		}
		jarray3(jarray3&t)
		{
			data = t.data;
			s = t.s;
			data->s = t.data->s;
			data->aa->s = t.data->aa->s;
		}
		b& jarray3::operator[](int k)
		{
			data->aa->index = k*(data->aa->s)*(data->s);
			return (*data);
		}
		int size()
		{
			return s;
		}
		my_ptr<b>data;
		int s;
	};
	class jarrayn
	{
	public:
		jarrayn()
		{
			//下面这块本来被我注释掉了,忘了是什么原因
			int* a = new int[1];
			int* b = new int[1];
			int **c = new int*(a);
			int ** d = new int*(b);
			p = my_ptr<int*>(c);
			s = my_ptr<int*>(d);

			d = 0;
		}
		jarrayn(int ts)
		{
			d = 1;
			int* a = new int[ts];
			int* b = new int[1];
			int **c = new int*(a);
			int ** e = new int*(b);
			p = my_ptr<int*>(c);
			s = my_ptr<int*>(e);
			(*s)[0] = ts;
		}
		jarrayn(int ts, int in)
		{
			d = 1;
			int* a = new int[ts];
			int* b = new int[1];
			int **c = new int*(a);
			int ** e = new int*(b);
			p = my_ptr<int*>(c);
			s = my_ptr<int*>(e);
			(*s)[0] = ts;
			if (in)
				memset(*p, 0, (*s)[0] * 4);
		}
		jarrayn(std::initializer_list<int>li)
		{
			int size = 1;
			d = static_cast<int>(li.size() - 1);
			int * a = new int[d];
			int **c = new int*(a);
			s = my_ptr<int*>(c);
			for (int i = 0; i < d; ++i)
			{
				size *= *(li.begin() + i);
				(*s)[i] = *(li.begin() + i);
			}
			int * b = new int[size];
			int ** e = new int*(b);
			p = my_ptr<int*>(e);
			if (*(li.end() - 1))
				memset(&(*p)[0], 0, size * 4);
		}
		jarrayn(jarrayn& t)
		{
			d = t.d;
			p = t.p;
			s = t.s;
		}
		int* get()
		{
			return *p;
		}
		int& get(int index)
		{
			return (*p)[index];
		}
		int& get(std::initializer_list<int>in)
		{
			int index = 0;
			index += *(in.begin());
			for (int i = 1; i < d; ++i)
			{
				index *= (*s)[i];
				index += *(in.begin() + i);
			}
			return (*p)[index];
		}
		int size()
		{
			return (*s)[0];
		}
		int size(std::initializer_list<int>in)
		{
			return (*s)[in.size()];
		}
		int dimension()
		{
			return d;
		}
	private:
		int d;
		my_ptr<int*>p;
		my_ptr<int*>s;
	};
	template<class T> class my_array
	{
	public:
		my_array()
		{
		}
		my_array(std::initializer_list<T>li)
		{
			T* k = new T(li.size());
			s = li.size();
			data = my_ptr<T>(k);
			for (int i = 0; i < int(li.size()); ++i)
			{
				(*data)[i] = *(li.begin() + i);
			}
		}
		my_array(int ts) : data(new T[ts]), s(ts)
		{
		}
		my_array(my_array&t) :data(t.data), s(t.s)
		{
		}
		T& my_array::operator[](int k)
		{
			return (*data)[k];
		}
		my_array<T>& my_array::operator=(my_array<T>& t)
		{
			s = t.s;
			data = t.data;
			return *this;
		}
		int size()
		{
			return s;
		}
		T* begin()
		{
			return *data;
		}
		const T* const end()
		{
			return *data + s;
		}
	private:
		my_ptr<T>data;
		int s;
	};*/
}