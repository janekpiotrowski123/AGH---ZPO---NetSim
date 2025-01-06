//
// Created by HP on 06.01.2025.
//

#include <gtest/gtest.h>
#include "nodes.hpp"
#include "storage_types.hpp"
#include "package.hpp"

// Klasa pomocnicza dla testowania chronionych metod PackageSender
class TestPackageSender : public PackageSender {
public:
    using PackageSender::push_package;
};
enum class ReceiverType {
    WORKER,
    STOREHOUSE
};

class MockReceiver : public IPackageReceiver {
public:
    void receive_package(Package&& p) override {
        packages_.push_back(std::move(p));
    }

    ElementID get_id() const override {
        return id_;
    }

    IPackageStockpile::const_iterator begin() const override {
        return packages_.begin();
    }

    IPackageStockpile::const_iterator cbegin() const override {
        return packages_.cbegin();
    }

    IPackageStockpile::const_iterator end() const override {
        return packages_.end();
    }

    IPackageStockpile::const_iterator cend() const override {
        return packages_.cend();
    }

    ReceiverType get_receiver_type() const override {
        return ReceiverType::WORKER; // Ustaw odpowiedni typ, np. WORKER lub STOREHOUSE
    }


    size_t get_package_count() const {
        return packages_.size();
    }

private:
    ElementID id_ = 1; // Przykładowy ID odbiornika.
    std::list<Package> packages_; // Bufor przechowywania pakietów.
};

TEST(WorkerTest, HasBuffer) {
    Worker w(1, 2, std::make_unique<PackageQueue>(PackageQueueType::FIFO));
    Time t = 1;

    w.receive_package(Package(1));
    w.do_work(t);
    ++t;
    w.receive_package(Package(2));
    w.do_work(t);
    auto& buffer = w.get_sending_buffer();

    ASSERT_TRUE(buffer.has_value());
    EXPECT_EQ(buffer.value().get_id(), 1);
}

TEST(PackageTest, IsAssignedIdLowest) {
    Package p1;
    Package p2;

    EXPECT_EQ(p1.get_id(), 1);
    EXPECT_EQ(p2.get_id(), 2);
}

TEST(PackageTest, IsIdReused) {
    {
        Package p1;
    }
    Package p2;

    EXPECT_EQ(p2.get_id(), 1);
}

TEST(PackageTest, IsMoveConstructorCorrect) {
    Package p1;
    Package p2(std::move(p1));

    EXPECT_EQ(p2.get_id(), 1);
}

TEST(PackageTest, IsAssignmentOperatorCorrect) {
    Package p1;
    Package p2 = std::move(p1);

    EXPECT_EQ(p2.get_id(), 1);
}

TEST(PackageQueueTest, IsFifoCorrect) {
    PackageQueue q(PackageQueueType::FIFO);
    q.push(Package(1));
    q.push(Package(2));

    Package p(std::move(q.pop()));
    EXPECT_EQ(p.get_id(), 1);

    p = q.pop();
    EXPECT_EQ(p.get_id(), 2);
}

TEST(PackageQueueTest, IsLifoCorrect) {
    PackageQueue q(PackageQueueType::LIFO);
    q.push(Package(1));
    q.push(Package(2));

    Package p(std::move(q.pop()));
    EXPECT_EQ(p.get_id(), 2);

    p = q.pop();
    EXPECT_EQ(p.get_id(), 1);
}

TEST(ReceiverPreferencesTest, AddReceiversRescalesProbability) {
    ReceiverPreferences prefs;
    MockReceiver receiver1, receiver2;

    prefs.add_receiver(&receiver1);
    prefs.add_receiver(&receiver2);

    auto preferences = prefs.get_preferences();
    EXPECT_EQ(preferences.size(), 2);
    for (const auto& [receiver, probability] : preferences) {
        EXPECT_DOUBLE_EQ(probability, 0.5);
    }
}

TEST(ReceiverPreferencesTest, RemoveReceiversRescalesProbability) {
    ReceiverPreferences prefs;
    MockReceiver receiver1, receiver2;

    prefs.add_receiver(&receiver1);
    prefs.add_receiver(&receiver2);
    prefs.remove_receiver(&receiver1);

    auto preferences = prefs.get_preferences();
    EXPECT_EQ(preferences.size(), 1);
    for (const auto& [receiver, probability] : preferences) {
        EXPECT_DOUBLE_EQ(probability, 1.0);
    }
}

TEST(ReceiverPreferencesChoosingTest, ChooseFirstReceiver) {
    ReceiverPreferences prefs;
    MockReceiver receiver1, receiver2;

    prefs.add_receiver(&receiver1);
    prefs.add_receiver(&receiver2);

    EXPECT_EQ(prefs.choose_receiver(), &receiver1);
}

TEST(ReceiverPreferencesChoosingTest, ChooseSecondReceiver) {
    ReceiverPreferences prefs;
    MockReceiver receiver1, receiver2;

    prefs.add_receiver(&receiver1);
    prefs.add_receiver(&receiver2);

    EXPECT_EQ(prefs.choose_receiver(), &receiver2);
}

TEST(PackageSenderTest, SendPackage) {
    TestPackageSender sender;
    MockReceiver mock_receiver;
    sender.receiver_preferences_.add_receiver(&mock_receiver);

    Package package(1);
    sender.push_package(std::move(package));
    sender.send_package();

    EXPECT_EQ(mock_receiver.get_package_count(), 1);
}
