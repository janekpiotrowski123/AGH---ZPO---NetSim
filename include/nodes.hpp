#ifndef AGH_ZPO_NETSIM_Nodes_HPP
#define AGH_ZPO_NETSIM_Nodes_HPP

#include <optional>
#include <memory>
#include <functional>
#include <map>
#include <cstdlib>
#include <cstdio>
#include "storage_types.hpp"
#include "helpers.hpp"
#include "types.hpp"


class IPackageReceiver {
public:
    virtual void receive_package(Package&& p) = 0;
    virtual ElementID get_id() const = 0;
    virtual ~IPackageReceiver() = default;

    virtual IPackageStockpile::const_iterator begin() const = 0;
    virtual IPackageStockpile::const_iterator cbegin() const = 0;
    virtual IPackageStockpile::const_iterator end() const = 0;
    virtual IPackageStockpile::const_iterator cend() const = 0;

};

using preferences_t = std::map<IPackageReceiver*, double>;
using const_iterator = preferences_t::const_iterator;


class ReceiverPreferences {
public:

    explicit ReceiverPreferences(std::function<double()> pg = probability_generator) : pg_(pg) {}
    void add_receiver(IPackageReceiver* r) noexcept;
    void remove_receiver(IPackageReceiver* r);
    [[ nodiscard]]  IPackageReceiver* choose_receiver() const;
    [[ nodiscard]] const std::map<IPackageReceiver*, double>& get_preferences () const { return preferences_ ;}

    const_iterator begin() const noexcept { return preferences_.begin(); }
    const_iterator end() const noexcept { return preferences_.end(); }
    const_iterator cbegin() const noexcept { return preferences_.cbegin(); }
    const_iterator cend() const noexcept { return preferences_.cend(); }

private:
    std::function<double()> pg_;
    std::map<IPackageReceiver*, double> preferences_; 
};

class PackageSender {
public:
    PackageSender() = default;
    PackageSender(PackageSender&& other) = default;
    void send_package();    
    const std::optional<Package>& get_sending_buffer() const { return sending_buffer_; }
    ReceiverPreferences receiver_preferences_;
protected:
    std::optional<Package> sending_buffer_;

    void push_package(Package&& p) { sending_buffer_ = std::move(p); }
};

class Ramp : public PackageSender{
public:
    explicit Ramp(ElementID id, TimeOffset di) noexcept : id_(id), di_(di) {}
    void deliver_goods(Time t);
    TimeOffset get_delivery_interval() const {return di_; }
    ElementID get_id() const { return id_; }
private:
    ElementID id_;
    TimeOffset di_;
};

class Storehouse : public IPackageReceiver {
public:
    explicit Storehouse(ElementID id, std::unique_ptr<IPackageStockpile> d) noexcept : id_(id), d_(std::move(d)) {}
    void receive_package(Package&& p) override { d_->push(std::move(p)); };
    ElementID get_id() const noexcept override { return id_; }

    IPackageStockpile::const_iterator begin() const override { return d_->begin(); }
    IPackageStockpile::const_iterator cbegin() const override { return d_->cbegin(); }
    IPackageStockpile::const_iterator end() const override { return d_->end(); }
    IPackageStockpile::const_iterator cend() const override { return d_->cend(); }

private:
    ElementID id_;
    std::unique_ptr<IPackageStockpile> d_;
};

class Worker : public PackageSender, public IPackageReceiver {
public:
    explicit Worker(ElementID id, TimeOffset pd, std::unique_ptr<IPackageQueue> q) noexcept
        : id_(id), pd_(pd), q_(std::move(q)), processing_start_time_(std::nullopt) {}

    void do_work(Time t);
    
    TimeOffset get_processing_duration() const noexcept { return pd_; }
    std::optional<Time> get_package_processing_start_time() const noexcept { return processing_start_time_; }

    void receive_package(Package&& p) override { q_->push(std::move(p)); }
    ElementID get_id() const noexcept override { return id_; }

    IPackageStockpile::const_iterator begin() const override { return q_->begin(); }
    IPackageStockpile::const_iterator cbegin() const override { return q_->cbegin(); }
    IPackageStockpile::const_iterator end() const override { return q_->end(); }
    IPackageStockpile::const_iterator cend() const override { return q_->cend(); }

private:
    ElementID id_;
    TimeOffset pd_;
    std::unique_ptr<IPackageQueue> q_;
    std::optional<Time> processing_start_time_;
};


#endif //AGH_ZPO_NETSIM_Nodes_HPP