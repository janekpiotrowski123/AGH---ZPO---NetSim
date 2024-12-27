//
// Created by fsaw1 on 27.12.2024.
//
# include "storage_types.hpp"

Package PackageQueue::pop()
{
    Package p;
    if (queue_type_ == PackageQueueType::FIFO)
    {
        p = std::move(packages.front());
        packages.pop_front();
    }
    else if (queue_type_ == PackageQueueType::LIFO)
    {
        p = std::move(packages.back());
        packages.pop_back();
    }

    return p;
}
