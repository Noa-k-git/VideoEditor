#pragma once
#include "Map.h"

template<typename Key, typename T>
inline std::pair<typename std::map<Key, T>::iterator, bool> Map<Key, T>::insert(T& value)
{
    std::pair<std::map<Key, T>::iterator, bool> result(nullptr, false);
    InputDialog* dlg = new InputDialog(NULL, wxT("Change Name"), wxT("Please enter new name for this Video Source:"));
    while (!result.second && dlg->ShowModal() == wxID_OK) {
        result = myMap.insert(std::pair<const Key, T>(dlg->GetValue(), value));
    }
    return result;
}

template<typename Key, typename T>
inline std::pair<typename std::map<Key, T>::iterator, bool> Map<Key, T>::insert(const std::pair<const Key, T>& kv)
{
    std::pair<typename std::map<Key, T>::iterator, bool> result = myMap.insert(kv);
    // Additional code to execute after insertion
    if (result.second)
        return result;
    return this->insert(kv.second);
}

template<typename Key, typename T>
inline std::pair<typename std::map<Key, T>::iterator, bool> Map<Key, T>::replaceKey(const Key& oldKey)
{
    std::pair<typename std::map<Key, T>::iterator, bool> result(nullptr, false);
    // Check if the old key exists in the map
    if (myMap.find(oldKey) != myMap->_Unchecked_end()) {
        // Get the value of that key
        T value = std::move(myMap.at(oldKey));
        // Erase the old key pair
        myMap.erase(oldKey);
        // Insert the new key value pair
        result = this->insert(value);
        if (!result.second) {
            result = myMap.insert(std::pair<const Key, T>(oldKey, value));
        }
    }
    return result;
}
