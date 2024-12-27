//
// Created by fsaw1 on 27.12.2024.
//

#ifndef AGH_ZPO_NETSIM_Package_HPP
#define AGH_ZPO_NETSIM_Package_HPP

#include <set>

class Package
{
public:
    Package(): ID_(Package::generate_ID())  {}
    explicit Package(unsigned int ID)  : ID_(ID) {}

    Package(Package &&p) noexcept : ID_(p.ID_){}

    Package& operator=(Package &&p) noexcept ;

    [[nodiscard("IDs are important...")]] unsigned int get_id() const { return ID_; }

    ~Package();


private:
    unsigned int ID_;
    static std::set<unsigned int> freed_IDs;
    static unsigned int max_taken_ID;

    static unsigned int generate_ID();
};



#endif //AGH_ZPO_NETSIM_Package_HPP
