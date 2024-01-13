#ifndef RCBE_ENGINE_BINARYBUFFER_HPP
#define RCBE_ENGINE_BINARYBUFFER_HPP

#include <algorithm>
#include <array>
#include <fstream>
#include <numeric>
#include <vector>

#include <rcbe-engine/binary/base_serializer.hpp>

namespace rcbe::binary {
class BinaryBuffer {
public:
    using ByteType = unsigned char;

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
        os.write(reinterpret_cast<const char*>(bb.buffer_.data()), bb.buffer_.size());

        return os;
    }

    [[nodiscard]] BinaryBuffer at(size_t &&offset, size_t size = 0) const;
    [[nodiscard]] BinaryBuffer at(size_t &offset, size_t size = 0) const;

    template <typename ReturnType>
    [[nodiscard]] ReturnType get() {
        using rcbe::binary::from_binary;
        ReturnType rt {};
        from_binary(*this, rt);
        return rt;
    }

    [[nodiscard]] size_t size() const;

    [[nodiscard]] bool isView() const;

private:
    using StorageType = std::vector<ByteType>;
    using ViewType = std::vector<const ByteType*>;

    void read(std::istream &is, StorageType &s);

    BinaryBuffer(StorageType::const_iterator begin, StorageType::const_iterator end);

    BinaryBuffer(ViewType::const_iterator begin, ViewType::const_iterator end);

    template <typename BinaryBufferContainer>
    BinaryBuffer constructFromChunk(const BinaryBufferContainer &bbc, size_t &offset, const size_t size) const {
        if (bbc.empty())
            throw std::runtime_error("BinaryBuffer::at on empty buffer");
        auto it = bbc.begin();
        std::advance(it, offset);
        auto eit = it;
        std::advance(eit, size);
        offset += size;
        return rcbe::binary::BinaryBuffer(it, eit);
    }

    template <typename Invocable, typename Iterator>
    void initView(Invocable &&i, Iterator begin, Iterator end) {
        if (begin == end)
            throw std::runtime_error("Wrong range supplied for constructing binary buffer chunk!");
        buffer_.resize(0);
        buffer_.shrink_to_fit();
        buffer_view_.reserve(std::distance(begin, end));
        std::transform(begin, end, std::back_inserter(buffer_view_), i);
    }

    BinaryBuffer &appendImplementation(const BinaryBuffer &source);
    void appendImplementation(const StorageType &storage);
    void appendImplementation(const ViewType &storage);

    std::vector<ByteType> buffer_;
    std::vector<const ByteType*> buffer_view_;
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
size_t BinaryBuffer::get();
template <>
float BinaryBuffer::get();
template <>
char BinaryBuffer::get();
template <>
uint32_t BinaryBuffer::get();
template <>
uint16_t BinaryBuffer::get();
template <>
uint8_t BinaryBuffer::get();
template <>
std::string BinaryBuffer::get();

}

#endif //RCBE_ENGINE_BINARYBUFFER_HPP
