#ifndef RCBE_ENGINE_BINARYBUFFER_HPP
#define RCBE_ENGINE_BINARYBUFFER_HPP

#include <algorithm>
#include <array>
#include <fstream>
#include <numeric>
#include <memory>
#include <type_traits>
#include <vector>

#include <rcbe-engine/binary/base_serializer.hpp>

namespace rcbe::binary {
class BinaryBuffer {
public:
    using ByteType = unsigned char;

private:
    using StorageType = std::vector<ByteType>;

public:
    using BinaryBufferConstIterator = typename StorageType::const_iterator;

    class BinaryBufferView {
    private:
        using ViewStorageType = std::vector<const ByteType *>;
    public:
        using BinaryBufferPosition = BinaryBufferConstIterator;
        using BinaryBufferViewConstIterator = typename ViewStorageType::const_iterator;

        template <typename Value>
        struct  BinBuffGetResult {
            using ValueType = Value;

            Value return_value;
            BinaryBufferConstIterator last_position;

            template <size_t Index>
            auto &&get() & { return get_helper<Index>(*this); }

            template <size_t Index>
            auto &&get() && { return get_helper<Index>(*this); }

            template <size_t Index>
            auto &&get() const& { return get_helper<Index>(*this); }

            template <size_t Index>
            auto &&get() const&& { return get_helper<Index>(*this); }

            template<typename  T, typename U>
            operator std::tuple<T, U>() {
                return {return_value, last_position};
            }

        private:
            template <size_t Index, typename T>
            auto &&get_helper(T &&t) const {
                static_assert(Index < 2, "Index is out of bounds for BinBuffGetResult");
                if constexpr (Index == 0) return std::forward<T>(t).return_value;
                if constexpr (Index == 1) return std::forward<T>(t).last_position;
            }
        };

        BinaryBufferView(BinaryBufferPosition begin, BinaryBufferPosition end) {
            initView([](const auto &entry) {
                return std::addressof(entry);
            }, begin, end);

            for (auto p : view_) {
                if (p == nullptr)
                    throw std::runtime_error("Null pointer detected in binary buffer chunk!");
            }
        }

        BinaryBufferView(BinaryBufferViewConstIterator begin, BinaryBufferViewConstIterator end) {
            initView([](const auto &entry) {
                if (entry == nullptr)
                    throw std::runtime_error("Null pointer detected in binary buffer chunk!");
                return entry;
            }, begin, end);
        }

        template <typename ReturnType>
        [[nodiscard]] BinBuffGetResult<ReturnType> get() {
            using rcbe::binary::from_binary;
            ReturnType rt {};
            from_binary(*this, rt);
            return {rt, current_pos_};
        }

        [[nodiscard]] BinaryBufferView at(BinaryBufferViewConstIterator offset, size_t size = 0) const {
            return constructFromChunk(view_, offset, size);
        }

        [[nodiscard]] size_t size() const {
            return view_.size();
        }

        [[nodiscard]] BinaryBufferViewConstIterator constBegin() const noexcept {
            return view_.cbegin();
        }

        [[nodiscard]] BinaryBufferViewConstIterator constEnd() const noexcept {
            return view_.cend();
        }

    private:

        template <typename Container, typename ConstIterator = typename Container::const_iterator>
        [[nodiscard]] BinaryBufferView constructFromChunk(
                const Container &storage,
                ConstIterator offset,
                const size_t size = 0) const {
            if (storage.empty())
                throw std::runtime_error("BinaryBuffer::at on empty buffer");
            auto eit = offset;
            std::advance(eit, size);
            return BinaryBufferView(offset, eit);
        }

        template <typename Invocable, typename Iterator>
        void initView(Invocable &&i, Iterator begin, Iterator end) {
            if (begin == end)
                throw std::runtime_error("Wrong range supplied for constructing binary buffer chunk!");
            view_.resize(0);
            view_.shrink_to_fit();
            view_.reserve(std::distance(begin, end));
            std::transform(begin, end, std::back_inserter(view_), i);
        }
        ViewStorageType view_;
        BinaryBufferPosition current_pos_;
    };

    BinaryBuffer() = default;
    explicit BinaryBuffer(std::istream& is);
    explicit BinaryBuffer(std::basic_ifstream<char> &is);

    explicit BinaryBuffer(std::vector<ByteType> &&buf);

    BinaryBuffer(std::initializer_list<BinaryBuffer> &&bb);

    explicit BinaryBuffer(std::vector<BinaryBuffer> &&bb);

    template <typename V>
    BinaryBuffer(V v) {
        using rcbe::binary::to_binary;
        to_binary(*this, v);
    }

    template <typename V, size_t S>
    BinaryBuffer(V(&source)[S]) {
        using rcbe::binary::to_binary;
        for (const auto &v : source) {
            to_binary(*this, v);
        }
    }

    ~BinaryBuffer() = default;

    BinaryBuffer(const BinaryBuffer &b) = default;
    BinaryBuffer(BinaryBuffer &&b) = default;

    BinaryBuffer &operator=(const BinaryBuffer &bb);

    BinaryBuffer &operator=(BinaryBuffer &&bb);

    BinaryBuffer &append(const BinaryBuffer &bb);

    BinaryBuffer &append(BinaryBuffer &&bb);

    friend std::istream &operator>>(std::istream &is, BinaryBuffer &bb) {
        bb.read(is, bb.buffer_);

        return is;
    }

    friend std::ostream  &operator<<(std::ostream &os, const BinaryBuffer &bb) {
        os.write(reinterpret_cast<const char*>(bb.buffer_.data()), static_cast<long>(bb.buffer_.size()));

        return os;
    }

    [[nodiscard]] BinaryBufferView at(BinaryBufferConstIterator offset, size_t size = 0) const;

    [[nodiscard]] size_t size() const;

    [[nodiscard]] BinaryBufferConstIterator constBegin() const noexcept;
    [[nodiscard]] BinaryBufferConstIterator constEnd() const noexcept;

private:

    void read(std::istream &is, StorageType &s);

    [[nodiscard]] BinaryBufferView constructFromChunk(const StorageType &bbc, BinaryBufferConstIterator offset, const size_t size) const {
        if (bbc.empty())
            throw std::runtime_error("BinaryBuffer::at on empty buffer");
        auto eit = offset;
        std::advance(eit, size);
        return BinaryBufferView(offset, eit);
    }

    BinaryBuffer &appendImplementation(const BinaryBuffer &source);
    void appendImplementation(const StorageType &storage);

    void appendImplementation(const BinaryBufferView &storage) {
        std::transform(storage.constBegin(), storage.constEnd(), std::back_inserter(buffer_), [](auto entry) {
            return *entry;
        });
    }

    std::vector<ByteType> buffer_;
    bool view_ = false;
};

template <>
BinaryBuffer::BinaryBuffer(size_t v);
template <>
BinaryBuffer::BinaryBuffer(float f);
template <>
BinaryBuffer::BinaryBuffer(char c);
template <>
BinaryBuffer::BinaryBuffer(uint32_t c);
template <>
BinaryBuffer::BinaryBuffer(uint16_t c);
template <>
BinaryBuffer::BinaryBuffer(uint8_t c);
template <>
BinaryBuffer::BinaryBuffer(const std::string &s);

template <>
BinaryBuffer::BinaryBufferView::BinBuffGetResult<size_t> BinaryBuffer::BinaryBufferView::get();
template <>
BinaryBuffer::BinaryBufferView::BinBuffGetResult<float> BinaryBuffer::BinaryBufferView::get();
template <>
BinaryBuffer::BinaryBufferView::BinBuffGetResult<char> BinaryBuffer::BinaryBufferView::get();
template <>
BinaryBuffer::BinaryBufferView::BinBuffGetResult<uint32_t> BinaryBuffer::BinaryBufferView::get();
template <>
BinaryBuffer::BinaryBufferView::BinBuffGetResult<uint16_t> BinaryBuffer::BinaryBufferView::get();
template <>
BinaryBuffer::BinaryBufferView::BinBuffGetResult<uint8_t> BinaryBuffer::BinaryBufferView::get();
template <>
BinaryBuffer::BinaryBufferView::BinBuffGetResult<std::string> BinaryBuffer::BinaryBufferView::get();

}

namespace std {
template <typename T>
struct tuple_size<rcbe::binary::BinaryBuffer::BinaryBufferView::BinBuffGetResult<T>>
        : std::integral_constant<size_t, 2> {};

template <typename T>
struct tuple_element<0, rcbe::binary::BinaryBuffer::BinaryBufferView::BinBuffGetResult<T>> {
    using type = T;
};

template <typename T>
struct tuple_element<1, rcbe::binary::BinaryBuffer::BinaryBufferView::BinBuffGetResult<T>> {
    using type = rcbe::binary::BinaryBuffer::BinaryBufferConstIterator;
};
}

#endif //RCBE_ENGINE_BINARYBUFFER_HPP
