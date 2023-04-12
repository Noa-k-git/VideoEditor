#pragma once
#include "UniqueName.h"
#include "InputDialog.h"
#include <vector>

//template <typename T,
//    typename = std::enable_if_t<std::is_base_of_v<UniqueName, T>>>

template <typename T> 
class Records {
public:
    Records();
    std::pair<typename std::vector<T>::iterator, bool> AddRecord(T record);
    std::pair<typename std::vector<T>::iterator, bool> UpdateName(std::string);
    void RemoveRecord(std::string name);
    std::pair <typename std::vector<T>::iterator, bool > Contains(std::string name);
    std::vector<T>* GetRecords() const;
private:
    std::pair<typename std::vector<T>::iterator, bool> NameInput(std::string&);
    std::vector<T> * records;
};

//#include "records.h"
#include <algorithm>
#include <stdexcept>

template <typename T>
Records<T>::Records() {
    static_assert(std::is_base_of<UniqueName, std::remove_pointer_t<T>>::value, "Derived not derived from UniqueName");
    this->records = new std::vector<T>();
}
template <typename T>
std::pair<typename std::vector<T>::iterator, bool> Records<T>::AddRecord(T record) {
    auto result = Contains(record->GetName());
    if (result.second) {
        InputDialog* dlg = nullptr;
        std::string input = "";
        result = NameInput(input);
        record->SetName(input);
    }
    if (!result.second)
        records->push_back(record);
    return result;
}
template <typename T>
std::pair<typename std::vector<T>::iterator, bool> Records<T>::UpdateName(std::string oldName)
{
    auto result = Contains(oldName);
    if (result.second)
    {
        std::string input = "";
        result = NameInput(input);
        if (!result.second)
        {
            (*result.first)->SetName(input);
        }
    }

    return result;
}

template <typename T>
std::pair<typename std::vector<T>::iterator, bool> Records<T>::NameInput(std::string& newName) {
    std::pair<typename std::vector<T>::iterator, bool> result; 
    result.second = true;
    InputDialog* dlg = nullptr;
    do {
        if (dlg != nullptr)
            delete dlg;
        dlg = new InputDialog(NULL, wxT("Change Name"), wxT("Please enter new name for this object: "));
        newName = dlg->GetValue().ToStdString();
        result = Contains(newName);
    } while (result.second && dlg->ShowModal() == wxID_OK);
    return result;
}

template <typename T>
void Records<T>::RemoveRecord(std::string name) {
    auto result = Contains(name);

    if (result.second)
        records->erase(result.first);
}

template <typename T>
std::pair<typename std::vector<T>::iterator, bool> Records<T>::Contains(std::string name) {
    auto it = std::find_if(records->begin(), records->end(), [&](const T record) {
        return record->GetName() == name;
        });
    return std::make_pair(it, it != records->end());
}

template <typename T>
std::vector<T>* Records<T>::GetRecords() const {
    return records;
}
