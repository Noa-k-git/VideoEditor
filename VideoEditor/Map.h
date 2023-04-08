#pragma once
#include <map>
#include "InputDialog.h"
template < typename Key, typename T>
class Map
{
public:
	std::map<Key, T> myMap;
	inline std::pair<typename std::map<Key, T>::iterator, bool> insert(T&);
	inline std::pair<typename std::map<Key, T>::iterator, bool> insert(const std::pair<const Key, T>&);
	inline std::pair<typename std::map<Key, T>::iterator, bool> replaceKey(const Key&);
};

