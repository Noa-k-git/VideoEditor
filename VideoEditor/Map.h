#pragma once
#include <map>
#include <utility>
#include "InputDialog.h"

template <typename Tvalue>
class Map
{
public:
	std::map<std::string, Tvalue> myMap;
	std::pair<typename std::map<std::string, Tvalue>::iterator, bool> Insert(Tvalue);
	std::pair<typename std::map<std::string, Tvalue>::iterator, bool> Insert(std::pair<std::string, Tvalue>);
	//std::pair<typename std::map<std::string, Tvalue>::iterator, bool> ReplaceKey(std::string);
};


template<typename Tvalue>
std::pair<typename std::map<std::string, Tvalue>::iterator, bool> Map<Tvalue>::Insert(Tvalue value)
{
    std::pair<std::map<std::string, Tvalue>::iterator, bool> result;
    InputDialog* dlg = nullptr;
    do {
        if (dlg != nullptr)
            delete dlg;
        dlg = new InputDialog(NULL, wxT("Change Name"), wxT("Please enter new name for this Video Source:"));
        result = myMap.insert(std::pair<std::string, Tvalue>(dlg->GetValue().ToStdString(), value));
    } while (!result.second && dlg->ShowModal() == wxID_OK);
    return result;

}

template<typename Tvalue>
std::pair<typename std::map<std::string, Tvalue>::iterator, bool> Map<Tvalue>::Insert(std::pair<std::string, Tvalue> kv)
{
    std::pair<typename std::map<std::string, Tvalue>::iterator, bool> result = myMap.insert(kv);
    // Additional code to execute after insertion
    if (result.second)
        return result;
    return this->Insert(kv.second);
}

//template<typename Tvalue>
//std::pair<typename std::map<std::string, Tvalue>::iterator, bool> Map<Tvalue>::ReplaceKey( std::string oldKey)
//{
//    std::pair<typename std::map<std::string, Tvalue>::iterator, bool> result(nullptr, false);
//    // Check if the old key exists in the map
//    if (myMap.find(oldKey) != myMap->_Unchecked_end()) {
//        // Get the value of that key
//        Tvalue value = std::move(myMap.at(oldKey));
//        // Erase the old key pair
//        myMap.erase(oldKey);
//        // Insert the new key value pair
//        result = this->insert(value);
//        if (!result.second) {
//            result = myMap.insert(std::pair<std::string, Tvalue>(oldKey, value));
//        }
//    }
//    return result;
//}
