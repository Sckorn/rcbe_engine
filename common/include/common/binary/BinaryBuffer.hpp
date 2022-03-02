#ifndef RCBE_ENGINE_BINARYBUFFER_HPP
#define RCBE_ENGINE_BINARYBUFFER_HPP

#include <algorithm>
#include <array>
#include <fstream>
#include <numeric>
#include <vector>

#include <rcbe-engine/binary/base_serializer.hpp>

namespace rcbe::binary {
R_PUBLIC_API class BinaryBuffer {
public:

    using ByteType = unsigned char;

    R_PUBLIC_API BinaryBuffer() = default;
    R_PUBLIC_API explicit BinaryBuffer(std::istream &is);
    R_PUBLIC_API explicit BinaryBuffer(std::basic_ifstream<char> &is);

    R_PUBLIC_API explicit BinaryBuffer(std::vector<ByteType> &&buf);

    R_PUBLIC_API BinaryBuffer(std::initializer_list<BinaryBuffer> &&bb);

    R_PUBLIC_API explicit BinaryBuffer(std::vector<BinaryBuffer> &&bb);

    template <typename V>
    R_PUBLIC_API BinaryBuffer(V v) {
        using rcbe::binary::to_binary;
        to_binary(*this, v);
    }

    template <typename V, size_t S>
    R_PUBLIC_API BinaryBuffer(V (&source)[S]) {
        using rcbe::binary::to_binary;
        for (const auto &v : source) {
            to_binary(*this, v);
        }
    }

    R_PUBLIC_API ~BinaryBuffer() = default;

    R_PUBLIC_API BinaryBuffer(const BinaryBuffer &b) = default;
    R_PUBLIC_API BinaryBuffer(BinaryBuffer &&b) = default;

    R_PUBLIC_API BinaryBuffer &operator=(const BinaryBuffer &bb);

    R_PUBLIC_API BinaryBuffer &operator=(BinaryBuffer &&bb);

    R_PUBLIC_API BinaryBuffer &append(const BinaryBuffer &bb);

    R_PUBLIC_API BinaryBuffer &append(BinaryBuffer &&bb);

    R_PUBLIC_API friend std::istream &operator>>(std::istream &is, BinaryBuffer &bb) {
        bb.read(is, bb.buffer_);

        return is;
    }

    R_PUBLIC_API friend std::ostream &operator<<(std::ostream &os, const BinaryBuffer &bb) {
        os.write(reinterpret_cast<const char *>(bb.buffer_.data()), bb.buffer_.size());

        return os;
    }

    [[nodiscard]] R_PUBLIC_API BinaryBuffer at(size_t &&offset, size_t size = 0) const;
    [[nodiscard]] R_PUBLIC_API BinaryBuffer at(size_t &offset, size_t size = 0) const;

    template <typename ReturnType>
    [[nodiscard]] R_PUBLIC_API ReturnType get() {
        using rcbe::binary::from_binary;
        ReturnType rt {};
        from_binary(*this, rt);
        return rt;
    }

    [[nodiscard]] R_PUBLIC_API size_t size() const;

    [[nodiscard]] R_PUBLIC_API bool isView() const;

private:

    using StorageType = std::vector<ByteType>;
    using ViewType = std::vector<const ByteType *>;

    R_PUBLIC_API void read(std::istream &is, StorageType &s);

    R_PUBLIC_API BinaryBuffer(StorageType::const_iterator begin, StorageType::const_iterator end);

    R_PUBLIC_API BinaryBuffer(ViewType::const_iterator begin, ViewType::const_iterator end);

    template <typename BinaryBufferContainer>
    R_PUBLIC_API BinaryBuffer constructFromChunk(const BinaryBufferContainer &bbc, size_t &offset, const size_t size) const {
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
    R_PUBLIC_API void initView(Invocable &&i, Iterator begin, Iterator end) {
        if (begin == end)
            throw std::runtime_error("Wrong range supplied for constructing binary buffer chunk!");
        buffer_.resize(0);
        buffer_.shrink_to_fit();
        buffer_view_.reserve(std::distance(begin, end));
        std::transform(begin, end, std::back_inserter(buffer_view_), i);
    }

    R_PUBLIC_API BinaryBuffer &appendImplementation(const BinaryBuffer &source);
    R_PUBLIC_API void appendImplementation(const StorageType &storage);
    R_PUBLIC_API void appendImplementation(const ViewType &storage);

    StorageType buffer_; // TODO: StorageType buffer_;
    ViewType buffer_view_; // TODO: ViewType buffer_view_;
    bool view_ = false;
};

template <>
R_PUBLIC_API BinaryBuffer::BinaryBuffer(size_t v);
template <>
R_PUBLIC_API BinaryBuffer::BinaryBuffer(float f);
template <>
R_PUBLIC_API BinaryBuffer::BinaryBuffer(char c);
template <>
R_PUBLIC_API BinaryBuffer::BinaryBuffer(uint32_t c);
template <>
R_PUBLIC_API BinaryBuffer::BinaryBuffer(uint16_t c);
template <>
R_PUBLIC_API BinaryBuffer::BinaryBuffer(uint8_t c);
template <>
R_PUBLIC_API BinaryBuffer::BinaryBuffer(const std::string &s);

template <>
R_PUBLIC_API size_t BinaryBuffer::get();
template <>
R_PUBLIC_API float BinaryBuffer::get();
template <>
R_PUBLIC_API char BinaryBuffer::get();
template <>
R_PUBLIC_API uint32_t BinaryBuffer::get();
template <>
R_PUBLIC_API uint16_t BinaryBuffer::get();
template <>
R_PUBLIC_API uint8_t BinaryBuffer::get();
template <>
R_PUBLIC_API std::string BinaryBuffer::get();

}// namespace rcbe::binary

#endif//RCBE_ENGINE_BINARYBUFFER_HPP
