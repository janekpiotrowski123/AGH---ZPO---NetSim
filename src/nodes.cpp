#include "nodes.hpp"
#include "package.hpp"
void ReceiverPreferences::add_receiver(IPackageReceiver* r) noexcept {
    double map_size = static_cast<double>(preferences_.size());
    double new_probability = 1.0 / (map_size + 1.0);
    for (auto& [key, value] : preferences_) {
        value *= map_size / (map_size + 1.0);
    }
    preferences_[r] = new_probability;
}


void ReceiverPreferences::remove_receiver(IPackageReceiver* r) {
    if (preferences_.find(r) != preferences_.end()) {
        preferences_.erase(r);

        if (!preferences_.empty()) {
            double new_probability = 1.0 / static_cast<double>(preferences_.size());
            for (auto& [receiver, probability] : preferences_) {
                probability = new_probability;
            }
        }
    }
}

IPackageReceiver* ReceiverPreferences::choose_receiver() const {
    double random_value = pg_();
    double cumulative_probability = 0.0;

    for (const auto& [receiver, probability] : preferences_) {
        cumulative_probability += probability;

        if (random_value <= cumulative_probability) {
            return receiver;
        }
    }

    return nullptr;
}


void PackageSender::send_package(){
    if(sending_buffer_){
        IPackageReceiver* receiver = receiver_preferences_.choose_receiver();

        if(receiver){
            receiver->receive_package(std::move(*sending_buffer_));
            sending_buffer_.reset();
        }
    }
}

void Ramp::deliver_goods(Time t){
    if(t % di_ == 0){
        Package product = Package(id_);
        push_package(std::move(product));
        send_package(); // chyba to nie tu: 
    }
}

// void Storehouse::receive_package(Package&& p) override {
//     d_->push(std::move(p));
// } nie działało podkreślało override

void Worker::do_work(Time t) {
    if (sending_buffer_) {
        if (t - processing_start_time_.value() >= pd_) {
            push_package(std::move(*sending_buffer_));
            send_package();
            sending_buffer_.reset();
            processing_start_time_.reset();
        }
    }

    if (!sending_buffer_ && !q_->empty()) {
        sending_buffer_ = q_->pop();
        processing_start_time_ = t;
    }
}
