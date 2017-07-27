#pragma once
#include <memory>

class jarray
{
public:
	jarray() {}
	jarray(std::initializer_list<int>li)
	{
		b* k = new b(li.size());
		s = li.size();
		data = std::shared_ptr<b>(k);
		for (int i = 0; i < int(li.size()); ++i)
		{
			((*data).v)[i] = *(li.begin() + i);
		}
	}
	jarray(int s)
	{
		b* k = new b(s);
		s = s;
		data = std::shared_ptr<b>(k);
	}
	jarray(jarray&t)
	{
		data = t.data;
		s = t.s;
	}
	int& jarray::operator[](int k);
	int size()
	{
		return s;
	}
private:
	class b
	{
	public:
		int* v;
		b(int s)
		{
			v = new int[s];
		}
		~b()
		{
			delete[]v;
		}
	};
	std::shared_ptr<b>data;
	int s;
};
int& jarray::operator[](int k)
{
	if (k >= 0 && k < s)
		return ((*data).v)[k];
	else
		throw std::exception("this index is invalid");
}


class jarray2
{
public:
	jarray2() {}
	jarray2(int ts1, int ts2)
	{
		c* k = new c(ts1, ts2);
		s1 = ts1;
		s2 = ts2;
		data = std::shared_ptr<c>(k);
	}
	jarray2(jarray2&t)
	{
		data = t.data;
		s1 = t.s1;
		s2 = t.s2;
		data->s1 = t.data->s1;
		data->s2 = t.data->s2;
	}
	class c;
	c& jarray2::operator[](int k)
	{
		if (k >= 0 && k < s1)
		{
			(*data).k1 = k;
			return (*data);
		}
		else
		{
			//throw std::exception("this index is invalid");
			std::cout << "this index is invalid\n";
			c i(1, 1);
			return i;
		}
	}
	std::pair<int, int> size()
	{
		return std::pair<int, int>(s1, s2);
	}
	class c
	{
	public:
		int* v;
		int s2, k1, s1;
		c(int ts1, int ts2)
		{
			v = new int[ts1*ts2];
			s1 = ts1;
			s2 = ts2;
			k1 = 0;
		}
		~c()
		{
			delete[]v;
		}
		int& c::operator[](int k)
		{
			if (k < s2 && k >= 0)
				return v[k1*s1 + k];
			else
			{
				//throw std::exception("this index is invalid");
				std::cout << "this index is invalid\n";
				int i = 0;
				return i;
			}
		}
	};
private:
	std::shared_ptr<c>data;
	int s1, s2;
};