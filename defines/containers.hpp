//#pragma once
//#include "_pch.hpp"
//
//namespace cep
//{
//template<typename T>
//using allocator = std::pmr::polymorphic_allocator<T>;
//
//using string = std::basic_string<char, std::char_traits<char>, cep::allocator<char>>;
//
//template<typename T>
//using vector = std::vector<T, cep::allocator<T>>;
//
//template<typename key, typename value>
//using map = std::map<key, value, std::less<key>, cep::allocator<std::pair<const key, value>>>;
//
//template<typename key, typename value>
//using unordered_map = std::unordered_map<key,
//                                         value,
//                                         std::hash<key>,
//                                         std::equal_to<key>,
//                                         cep::allocator<std::pair<const key, value>>>;
//
//template<typename T>
//using list = std::list<T, cep::allocator<T>>;
//
//template<typename T>
//using forward_list = std::forward_list<T, cep::allocator<T>>;
//}    // namespace cep
