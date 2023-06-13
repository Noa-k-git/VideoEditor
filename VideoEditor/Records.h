#pragma once
#include "InputDialog.h"
#include <vector>

//template <typename T,
//    typename = std::enable_if_t<std::is_base_of_v<UniqueName, T>>>

/// <summary>
/// A template class that wraps a vector and gives it new attributes
/// </summary>
/// <typeparam name="T"></typeparam>
template <typename T> 
class Records {
public:
    Records();
    std::pair<typename std::vector<T>::iterator, bool> AddRecord(T record);
    std::pair<typename std::vector<T>::iterator, bool> UpdateName(std::string);
    void RemoveRecord(std::string name);
    void RemoveAllRecords();
    std::pair <typename std::vector<T>::iterator, bool > Contains(std::string name);
    std::vector<T>* GetRecords() const;
private:
    std::pair<typename std::vector<T>::iterator, bool> NameInput(std::string&);
    std::vector<T> * list;
};

//#include "records.h"
#include <algorithm>
#include <stdexcept>

template <typename T>
Records<T>::Records() {
    this->list = new std::vector<T>();
}
template <typename T>
std::pair<typename std::vector<T>::iterator, bool> Records<T>::AddRecord(T record) {
    auto result = Contains(record->GetName());
    if (!result.second)
        list->push_back(record);
    //result.second ? result.second = false : result.second = true;
    result.second = !(result.second);
    return result;
}

template <typename T>
std::pair<typename std::vector<T>::iterator, bool> Records<T>::UpdateName(std::string oldName)
{
    auto old = Contains(oldName);
    if (old.second)
    {
        std::string input = "";
        auto result = NameInput(input);
        old.second = result.second;
        if (!result.second)
        {
            (*old.first)->SetName(input);
        }
    }
     
    return old;
}

template <typename T>
std::pair<typename std::vector<T>::iterator, bool> Records<T>::NameInput(std::string& newName) {
    std::pair<typename std::vector<T>::iterator, bool> result; 
    result.second = true;
    int endModal = 0;
    InputDialog* dlg = nullptr;
    do {
        if (dlg != nullptr)
            delete dlg;
        dlg = new InputDialog(NULL, wxT("Change Name"), wxT("Please enter new name for this object: "));
        endModal = dlg->ShowModal();
        newName = dlg->GetValue().ToStdString();
        result = Contains(newName);
    } while (result.second && endModal == wxID_OK);
    if (endModal != wxID_OK) {
        result.second = true;
    }
    return result;
}

template <typename T>
void Records<T>::RemoveRecord(std::string name) {
    auto result = Contains(name);

    if (result.second)
        list->erase(result.first);
}

template<typename T>
inline void Records<T>::RemoveAllRecords()
{
    list->clear();
}

template <typename T>
std::pair<typename std::vector<T>::iterator, bool> Records<T>::Contains(std::string name) {
    auto it = std::find_if(list->begin(), list->end(), [&](const T record) {
        return record->GetName() == name;
        });
    return std::pair<typename std::vector<T>::iterator, bool>(it, it != list->end());
}

template <typename T>
std::vector<T>* Records<T>::GetRecords() const {
    return list;
}
