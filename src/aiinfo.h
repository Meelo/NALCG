#ifndef _NALCG_A_I_INFO_H_
#define _NALCG_A_I_INFO_H_

// class includes

// system includes
#include <string>

class AIInfo
{
    // Constants

    // Members
    std::string name;
    std::string description;

    // Methods

public:
    AIInfo(const std::string& name, const std::string& description);
    virtual ~AIInfo() { }

    // Methods

    // Getters
    const std::string& getName() const { return name; }
    const std::string& getDescription() const { return description; }

    // Setters
};

#endif // _NALCG_A_I_INFO_H_
