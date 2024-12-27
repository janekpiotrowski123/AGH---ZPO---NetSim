//
// Created by fsaw1 on 27.12.2024.
//

#include"package.hpp"

unsigned int Package::max_taken_ID=0;
std::set<unsigned int> Package::freed_IDs={};


unsigned int Package::generate_ID()
{
    if(!freed_IDs.empty())
        return *freed_IDs.begin();

    max_taken_ID++;
    return max_taken_ID;
}

Package::~Package()
{
    freed_IDs.insert(this->ID_);
}

Package& Package::operator=(Package&&p) noexcept
{
    if (this == &p)
        return *this;

    Package::freed_IDs.insert(this->ID_);

    this->ID_ = p.ID_;
    return *this;
}








