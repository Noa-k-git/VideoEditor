#include "records.h"
#include <algorithm>
#include <stdexcept>

std::pair<std::vector<UniqueName>::iterator, bool> Records::AddRecord(UniqueName& record) {
    auto result = Contains(record.getName());
    if (result.second) {
        InputDialog* dlg = nullptr;
        std::string input = "";
        result = NameInput(input);
        record.setName(input);
    }
    if (!result.second)
        records.push_back(record);
    return result;
}

std::pair<std::vector<UniqueName>::iterator, bool> Records::UpdateName(std::string oldName)
{
    auto result = Contains(oldName);
    if (result.second)
    {
        std::string input = "";
        result = NameInput(input);
        if (!result.second)
        {
            result.first->setName(input);
        }
    }
    
    return result;
}

std::pair<std::vector<UniqueName>::iterator, bool> Records::NameInput(std::string& newName) {
    std::pair<std::vector<UniqueName>::iterator, bool> result(std::make_pair(records.end(), true)); // The name is not available, dummy data
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


void Records::RemoveRecord(std::string name) {
    auto result = Contains(name);

    if (result.second) {
        records.erase(result.first);
    }
}

std::pair<std::vector<UniqueName>::iterator, bool> Records::Contains(std::string name) {
    auto it = std::find_if(records.begin(), records.end(), [&](const UniqueName& record) {
        return record.getName() == name;
        });
        return std::make_pair(it, it != records.end());
}

std::vector<UniqueName> Records::GetRecords() const {
    return records;
}
