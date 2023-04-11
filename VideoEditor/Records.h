#pragma once
#include "UniqueName.h"
#include "InputDialog.h"
#include <vector>

class Records {
public:
    std::pair<std::vector<UniqueName>::iterator, bool> AddRecord(UniqueName& record);
    std::pair<std::vector<UniqueName>::iterator, bool> UpdateName(std::string);
    void RemoveRecord(std::string name);
    std::pair < std::vector<UniqueName>::iterator, bool > Contains(std::string name);
    std::vector<UniqueName> GetRecords() const;
private:
    std::pair<std::vector<UniqueName>::iterator, bool> NameInput(std::string&);
    std::vector<UniqueName> records;
};
