/**
	\file
	\brief Печать IP.
	\details Данный файл содержит в себе шаблонную ф-цию print_ip

	\author Garik
	\version 1.0
	\date  08/2018 года
	\warning Данный класс создан только в учебных целях
*/

#include <iostream>
#include <vector>
#include <string>
#include <list>
#include <tuple>

using namespace std;

template <typename T>
struct print_ip_impl
{
	static void print_ip(T& ip)
	{
		union {
			T number;
			uint8_t n8[sizeof(T)];
		} T2Bytes;

		T2Bytes.number = ip;

		std::vector<uint8_t> ip_v;
		for (char i = sizeof(T) - 1; i >= 0; --i)
		{
			ip_v.push_back(T2Bytes.n8[i]);
		}
		// Печать
		printIpString(ip_v);
	}
};

template<>
struct print_ip_impl<std::string>
{
	static void print_ip(std::string& ip)
	{
		std::cout << ip << endl; // Печатаем как есть
	}
};

template<typename T>
struct print_ip_impl<std::vector<T>>
{
	static void print_ip(std::vector<T>& ip)
	{
		printIpString(ip);
	}
};

template<typename T>
struct print_ip_impl<std::list<T>>
{
	static void print_ip(std::list<T>& ip)
	{
		// переводит все в vector 
		std::vector<T> ip_v{ std::make_move_iterator(std::begin(ip)),
			std::make_move_iterator(std::end(ip)) };
		// Печать
		printIpString(ip_v);
	}
};

template<typename ...T>
struct print_ip_impl<std::tuple<T...>>
{
	static void print_ip(std::tuple<T...>& ip)
	{
		printTuple(ip);
	}
};

template<typename T>
void print_ip(T ip)
{
	print_ip_impl<T>::print_ip(ip);
};

template<int index, typename Callback, typename... Args>
struct iterate_tuple
{
	static void next(std::tuple<Args...>& tpl, Callback callback)
	{
		static int const size = std::tuple_size<std::tuple<Args...>>::value;
		iterate_tuple<index - 1, Callback, Args...>::next(tpl, callback);
		callback(size - 1, index, std::get<index>(tpl));
	}
};

template<typename Callback, typename... Args>
struct iterate_tuple<0, Callback, Args...>
{
	static void next(std::tuple<Args...>& tpl, Callback callback)
	{
		callback(NULL, -1, std::get<0>(tpl));
	}
};


// -1 element
template<typename Callback, typename... Args>
struct iterate_tuple<-1, Callback, Args...>
{
	static void next(std::tuple<Args...>& tpl, Callback callback)
	{
	}
};

template<typename Callback, typename... Args>
void for_each(std::tuple<Args...>& t, Callback callback)
{
	int const size = std::tuple_size<std::tuple<Args...>>::value;
	iterate_tuple<size - 1, Callback, Args...>::next(t, callback);
}

struct ptintTupleIP
{
	template<typename T>
	void operator()(int size, int index, T&& t)
	{
		std::cout << t;
		if (index < size)
			cout << ".";
	}
};

template <class Tuple>
void printTuple(Tuple&& tuple)
{
	for_each(tuple, ptintTupleIP());
	cout << endl;
}


// потом печатаем вектор
template <typename T>
void printIpString(const std::vector<T>& ip)
{
	size_t szCount = 0;
	for (const auto& ip_part : ip)
	{
		std::cout << (int) ip_part;
		++szCount;

		if (szCount < ip.size())
		{
			std::cout << ".";
		}
	}
	std::cout << std::endl;
}

int main() {

	print_ip(char(-1));
	print_ip(short(0));
	print_ip(2130706433);
	print_ip(8875824491850138409);

	std::string test{ "test string" };
	print_ip(test);

	std::list<int> lIp_v4{ 255, 255, 225, 255 };
	print_ip(lIp_v4);

	std::vector<int> vIp_v4 = { 192, 168, 0, 41 };
	print_ip(vIp_v4);

	auto tplIp_v4 = std::make_tuple(192, 0, 0, 0, 0, 0, 1);
	print_ip(tplIp_v4);

	// system("pause");
	return 0;
}