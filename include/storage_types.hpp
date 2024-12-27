//
// Created by fsaw1 on 27.12.2024.
//

#ifndef AGH___ZPO___NETSIM_STORAGE_TYPES_HPP
#define AGH___ZPO___NETSIM_STORAGE_TYPES_HPP

#include "package.hpp"

#include <list>

enum class PackageQueueType {
    FIFO,
    LIFO
};

class IPackageStockpile {
public:
    using const_iterator = std::list<Package>::const_iterator;

    virtual void push(Package&& package) = 0;

    virtual std::size_t size() const = 0;

    virtual bool empty() const = 0;

    virtual const_iterator cbegin() const = 0;

    virtual const_iterator cend() const = 0;

    virtual const_iterator begin() const = 0;

    virtual const_iterator end() const = 0;

    virtual ~IPackageStockpile() = default;
};

class IPackageQueue : public IPackageStockpile {
public:
    virtual Package pop() = 0;

    [[nodiscard ]] virtual  PackageQueueType get_queue_type() const = 0;

    ~IPackageQueue() override = default;
};

class PackageQueue : public IPackageQueue {
public:
    explicit PackageQueue(PackageQueueType queue_type) : packages(), queue_type_(queue_type) {}

    void push(Package&& package) override { packages.emplace_back(std::move(package)); }

    [[nodiscard ]] std::size_t size() const override { return packages.size(); }

    [[nodiscard ]] bool empty() const override { return packages.empty(); }

    [[nodiscard ]] const_iterator cbegin() const override { return packages.cbegin(); }

    [[nodiscard ]] const_iterator cend() const override { return packages.cend(); }

    [[nodiscard ]] const_iterator begin() const override { return packages.cbegin(); }

    [[nodiscard ]] const_iterator end() const override { return packages.cend(); }

    Package pop() override;

    [[nodiscard ]] PackageQueueType get_queue_type() const override { return queue_type_; }

    ~PackageQueue() override = default;
private:
    std::list<Package> packages;
    PackageQueueType queue_type_;
};

#endif //AGH___ZPO___NETSIM_STORAGE_TYPES_HPP
