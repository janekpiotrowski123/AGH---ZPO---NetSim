//
// Created by fsaw1 on 27.12.2024.
//

#ifndef AGH_ZPO_NETSIM_Package_HPP
#define AGH_ZPO_NETSIM_Package_HPP

#include "types.hpp"
#include <set>


class Package
{
public:
    Package(): ID_(Package::generate_ID())  {}
    explicit Package(ElementID ID)  : ID_(ID) {}

    Package(Package &&p) noexcept : ID_(p.ID_){}

    Package& operator=(Package &&p) noexcept ;

    [[nodiscard("IDs are important...")]] ElementID get_id() const { return ID_; }

    ~Package();


private:
    ElementID ID_;
    static std::set<ElementID> freed_IDs;
    static ElementID max_taken_ID;

    static ElementID generate_ID();
};



#endif //AGH_ZPO_NETSIM_Package_HPP
