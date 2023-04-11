#pragma once
#include <string>
class UniqueName
{
public:
    UniqueName(const std::string& name) : name(name) {}

    const std::string& GetName() const { return name; }
    void SetName(const std::string name) { this->name = name; }

private:
    std::string name;
};

