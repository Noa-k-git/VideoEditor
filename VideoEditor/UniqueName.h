#pragma once
#include <string>
class UniqueName
{
public:
    UniqueName(const std::string& name) : name(name) {}

    const std::string& getName() const { return name; }
    void setName(const std::string name) { this->name = name; }

private:
    std::string name;
};

