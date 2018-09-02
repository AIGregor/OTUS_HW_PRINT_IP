/**
	\file
	\brief Print IP.
	\details This file contains all function for print_ip

	\author Garik
	\version 1.0
	\date  08/2018 года
	\warning This class was created only for educational purposes
*/

#include <iostream>
#include <type_traits>
#include <vector>
#include <string>
#include <list>
#include <tuple>

using namespace std;

/// All inregral types
template<typename T> typename std::enable_if_t<std::is_integral<T>::value, void>
	print_ip(const T& ip)
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
	};

template <typename T>
struct is_cstring : std::false_type {};

template <>
struct is_cstring<char[]> : std::true_type {};

template <size_t N>
struct is_cstring<char[N]> : std::true_type {};

/// String or c-style string type
template <typename T> std::enable_if_t<
	std::is_same<T, std::string>::value || is_cstring<T>::value>
	print_ip(const T& ip)
	{
		std::cout << ip << endl;
	}

template <typename T>
struct is_vector : std::false_type {};

template <typename U, typename Alloc>
struct is_vector<std::vector<U, Alloc>> : std::true_type {};

/// Vector type
template <typename T> std::enable_if_t<is_vector<T>::value>
	print_ip(const T& ip)
	{
		printIpString(ip);
	}

template <typename T>
struct is_list : std::false_type {};

template <typename T, typename Alloc>
struct is_list<std::list<T, Alloc>> : std::true_type {};

/// List type
template <typename T> std::enable_if_t<is_list<T>::value>
	print_ip(const T& ip)
	{
		printIpString(ip);
	}

/// Common print function
template <typename T>
void printIpString(const std::vector<T>& ip)
{
	size_t szCount = 0;
	for (const auto& ip_part : ip)
	{
		std::cout << (int) ip_part; // (int)
		++szCount;

		if (szCount < ip.size())
		{
			std::cout << ".";
		}
	}
	std::cout << std::endl;
}

///  Common print function - list
template <typename T>
void printIpString(const std::list<T>& ip)
{
	// переводит все в vector 
	std::vector<T> ip_v{ std::make_move_iterator(std::begin(ip)),
		std::make_move_iterator(std::end(ip)) };
	// Печать
	printIpString(ip_v);
}

template <typename T>
struct is_tuple : std::false_type {};

template <typename... Args>
struct is_tuple<std::tuple<Args...>> : std::true_type {};

template<int index, typename Callback, typename... Args>
struct iterate_tuple
{
	static void next(std::tuple<Args...> tpl, Callback callback)
	{
		static int const size = std::tuple_size<std::tuple<Args...>>::value;
		iterate_tuple<index - 1, Callback, Args...>::next(tpl, callback);
		callback(size - 1, index, std::get<index>(tpl));
	}
};

template<typename Callback, typename... Args>
struct iterate_tuple<0, Callback, Args...>
{
	static void next(std::tuple<Args...> tpl, Callback callback)
	{
		callback(NULL, -1, std::get<0>(tpl));
	}
};

// -1 element
template<typename Callback, typename... Args>
struct iterate_tuple<-1, Callback, Args...>
{
	static void next(std::tuple<Args...> tpl, Callback callback)
	{
	}
};

template<typename Callback, typename... Args>
void for_each(const std::tuple<Args...> t, Callback callback)
{
	int const size = std::tuple_size<std::tuple<Args...>>::value;
	iterate_tuple<size - 1, Callback, Args...>::next(t, callback);
}

struct ptintTupleIP
{
	template<typename T>
	void operator()(int size, int index, T& t)
	{
		std::cout << t;
		if (index < size)
			cout << ".";
	}
};

template <class Tuple>
void printTuple(Tuple& tuple)
{
	for_each(tuple, ptintTupleIP());
	cout << endl;
}

// Tuple type
template <typename T> std::enable_if_t<is_tuple<T>::value>
	print_ip(const T& ip)
	{
		// Печать
		printTuple(ip);
	}

/// Check the tuple that it is homogeneous 
template <typename T, typename... Ts>
struct has_homogeneous;

template <typename T>
struct has_homogeneous<T, std::tuple<>> : std::true_type {};

template <typename T, typename U, typename... Ts>
struct has_homogeneous<T, std::tuple<U, Ts...>> : std::false_type {};
	
template <typename T, typename... Ts>
struct has_homogeneous<T, std::tuple<T, Ts...>> : has_homogeneous<T, std::tuple<Ts...>> {};

template<typename T, typename Tuple>
using is_homogeneous = typename has_homogeneous<T, Tuple>::type;

/// Main programm
int main() {

	print_ip(char(-1));
	print_ip(short(0));
	print_ip(2130706433);
	print_ip(8875824491850138409);
	
	std::string test{ "test string" };
	print_ip(test);
	
	print_ip("test print ip c-style string");
		
	std::list<int> lIp_v4{255, 255, 225, 255};
	print_ip(lIp_v4);
	
	std::vector<int> vIp_v4 = { 192, 168, 0, 41 };
	print_ip(vIp_v4);
		
	auto tplIp_v4 = std::make_tuple(192, 0, 0, 0, 0, 0, 1);
	
	//using T = std::tuple<long, float, int>;
	//using T = std::tuple<long, long, long, long, long>;
	//using T = std::tuple<long, long, long, long, int>;

	using T = decltype(tplIp_v4);
	using U = std::remove_reference<decltype(std::get<0>(tplIp_v4))>::type;

	if (is_homogeneous<U, T>::value)
	{
		print_ip(tplIp_v4);
	}
	
	//system("pause");
	return 0;
}


