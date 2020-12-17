#include <gtest/gtest.h>

#include <unordered_map>
#include <iostream>
#include <bitset>

#include <rcbe-engine/delegate/Delegate.hpp>
#include <rcbe-engine/delegate/AbstractDelegate.hpp>

TEST(DelegateTests, MainDelegateTest) {
    rcbe::core::Delegate<void, int, int&> delegate {};
    int accumulator = 0;

    delegate += [](int addition, int& accum) -> void {
        accum += addition;
    };
    delegate.add([](int addition, int& accum) -> void {
        accum += 2 * addition;
    });

    delegate(1, accumulator);

    ASSERT_EQ(delegate.maxSize(), 100);
    ASSERT_EQ(delegate.size(), 2);
    ASSERT_EQ(accumulator, 3);
}

TEST(DelegateTests, AbstractDelegateTest) {
    using delegate_type = rcbe::core::Delegate<void, int, int&>;

    delegate_type del {10};
    int accumulate = 0;
    del += [](int add, int& acc) -> void {
        acc += add;
    };
    del += [](int add, int& acc) -> void {
        acc += 3 * add;
    };

    rcbe::core::AbstractDelegate ad { std::move(del) };
    ad.add<int, int&>([](int add, int& acc) -> void {
        acc += 2;
    });
    ad.invoke(1, accumulate);

    ASSERT_EQ(ad.maxSize(), 10);
    ASSERT_EQ(ad.size(), 3);
    ASSERT_EQ(accumulate, 6);
}

TEST(DelegateTests, SeveralAbstractDelegatesTest) {
    using first_delegate_type = rcbe::core::Delegate<void, int, int&>;
    using second_delegate_type = rcbe::core::Delegate<void, int>;

    first_delegate_type fd {5};
    fd += [](int add, int& accum) -> void {
        accum += add;
    };
    fd += [](int add, int& accum) -> void {
        accum += 4 * add;
    };

    second_delegate_type sd {5};
    sd += [](int out) -> void {
        std::cout << out << std::endl;
    };
    sd += [](int out) -> void {
        std::cout << std::hex << "0x" << out << std::endl;
    };
    sd += [](int out) -> void {
        std::bitset<sizeof(decltype(out)) * 8> bts { out };
        std::cout << "0b" << bts << std::endl;
    };

    using abstract_delegate_map = std::unordered_map<std::string, rcbe::core::AbstractDelegate>;

    abstract_delegate_map map = {
            {"first", rcbe::core::AbstractDelegate { std::move(fd) } },
            {"second", rcbe::core::AbstractDelegate { std::move(sd) } }
    };

    int accumulate = 0;

    ASSERT_EQ(map.at("first").size(), 2);
    ASSERT_EQ(map.at("second").size(), 3);

    auto& del_ref = map.at("first").as<int, int&>();

    del_ref.add([](int add, int& acc) -> void {
        acc += 2 * add;
    });

    map.at("first").invoke(1, accumulate);
    // TODO: figure out what's the deal with this, and fix it
    map.at("second").invoke(static_cast<std::remove_reference_t<decltype(accumulate)> >(accumulate));

    ASSERT_EQ(accumulate, 7);
}