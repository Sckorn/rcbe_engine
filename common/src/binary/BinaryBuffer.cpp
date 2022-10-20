#include <rcbe-engine/binary/BinaryBuffer.hpp>

namespace {
template <typename ArrayBasedContainer>
size_t compute_overall_size(const ArrayBasedContainer &abc) {
    return std::accumulate(abc.begin(), abc.end(), 0, [](auto sum, const auto& entry) {
        return sum + entry.size();
    });
}
}

namespace rcbe::binary {
BinaryBuffer::BinaryBuffer(std::istream &is) {
    read(is, buffer_);
}

BinaryBuffer::BinaryBuffer(std::basic_ifstream<char> &is) {
    read(is, buffer_);
}

BinaryBuffer::BinaryBuffer(std::vector<ByteType> &&buf)
:
buffer_(std::move(buf))
{}

BinaryBuffer::BinaryBuffer(std::initializer_list<BinaryBuffer> &&bb) {
    buffer_.reserve(buffer_.size() + compute_overall_size(bb));
    for (auto &b : bb) {
        append(std::move(b));
    }
}

BinaryBuffer::BinaryBuffer(std::vector<BinaryBuffer> &&bb) {
    buffer_.reserve(buffer_.size() + compute_overall_size(bb));
    for (const auto &b : bb) {
        append(b);
    }
}

BinaryBuffer &BinaryBuffer::operator=(const BinaryBuffer &bb) {
    append(bb);
    return *this;
}

BinaryBuffer &BinaryBuffer::operator=(BinaryBuffer &&bb) {
    (void)append(std::move(bb));
    return *this;
}

BinaryBuffer &BinaryBuffer::append(const BinaryBuffer &bb) {
    return appendImplementation(bb);
}

BinaryBuffer &BinaryBuffer::append(BinaryBuffer &&bb) {
    return appendImplementation(bb);
}

BinaryBuffer::BinaryBufferView BinaryBuffer::at(BinaryBuffer::BinaryBufferConstIterator offset, size_t size) const {
    return BinaryBuffer::constructFromChunk(buffer_, offset, size);
}

size_t BinaryBuffer::size() const {
    return buffer_.size();
}

void BinaryBuffer::read(std::istream &is, StorageType &s) {
    while (!is.eof()) {
        ByteType b {};
        is.read(reinterpret_cast<char*>(&b), sizeof(b));
        s.push_back(b);
    }
}

BinaryBuffer &BinaryBuffer::appendImplementation(const BinaryBuffer &source) {
    buffer_.reserve(buffer_.size() + source.size());

    appendImplementation(source.buffer_);

    return *this;
}

void BinaryBuffer::appendImplementation(const StorageType &storage) {
    buffer_.insert(buffer_.end(), storage.begin(), storage.end());
}

BinaryBuffer::BinaryBufferConstIterator BinaryBuffer::constBegin() const noexcept {
    return buffer_.cbegin();
}

BinaryBuffer::BinaryBufferConstIterator BinaryBuffer::constEnd() const noexcept {
    return buffer_.cbegin();
}

template <>
BinaryBuffer::BinaryBuffer(size_t v) {
    buffer_.resize(sizeof(size_t));
    auto * _v = reinterpret_cast<char*>(&v);
    std::copy(_v, _v + sizeof(v), buffer_.data());
}

template <>
BinaryBuffer::BinaryBuffer(float f) {
    buffer_.resize(sizeof(float));
    auto * _v = reinterpret_cast<char*>(&f);
    std::copy(_v, _v + sizeof(float), buffer_.data());
}

template <>
BinaryBuffer::BinaryBuffer(char c) {
    buffer_.resize(sizeof(char));
    auto * _v = reinterpret_cast<char*>(&c);
    std::copy(_v, _v + sizeof(char), buffer_.data());
}

template <>
BinaryBuffer::BinaryBuffer(uint32_t c) {
    buffer_.resize(sizeof(uint32_t));
    auto * _v = reinterpret_cast<uint32_t*>(&c);
    std::copy(_v, _v + sizeof(uint32_t), buffer_.data());
}

template <>
BinaryBuffer::BinaryBuffer(uint16_t c) {
    buffer_.resize(sizeof(uint16_t));
    auto * _v = reinterpret_cast<uint16_t*>(&c);
    std::copy(_v, _v + sizeof(uint16_t), buffer_.data());
}

template <>
BinaryBuffer::BinaryBuffer(uint8_t c) {
    buffer_.resize(sizeof(uint8_t));
    auto * _v = reinterpret_cast<uint8_t*>(&c);
    std::copy(_v, _v + sizeof(uint8_t), buffer_.data());
}

template <>
BinaryBuffer::BinaryBuffer(const std::string &s) {
    for (const auto &c : s) {
        this->append(BinaryBuffer(c));
    }
}

template <>
BinaryBuffer::BinaryBufferView::BinBuffGetResult<size_t> BinaryBuffer::BinaryBufferView::get() {
    std::array<ByteType, sizeof(size_t)> tmp;
    std::transform(view_.begin(), view_.end(), tmp.begin(), [](const auto &entry) {
        return *entry;
    });
    return {*reinterpret_cast<const size_t*>(tmp.data()), current_pos_};
}

template <>
BinaryBuffer::BinaryBufferView::BinBuffGetResult<float> BinaryBuffer::BinaryBufferView::get() {
    std::array<ByteType, sizeof(float)> tmp;
    std::transform(view_.begin(), view_.end(), tmp.begin(), [](const auto &entry) {
        return *entry;
    });
    return {*reinterpret_cast<const float*>(tmp.data()), current_pos_};
}

template <>
BinaryBuffer::BinaryBufferView::BinBuffGetResult<char> BinaryBuffer::BinaryBufferView::get() {
    std::array<ByteType, sizeof(char )> tmp;
    std::transform(view_.begin(), view_.end(), tmp.begin(), [](const auto &entry) {
        return *entry;
    });
    return {*reinterpret_cast<const char *>(tmp.data()), current_pos_};
}

template <>
BinaryBuffer::BinaryBufferView::BinBuffGetResult<uint32_t>  BinaryBuffer::BinaryBufferView::get() {
    std::array<ByteType, sizeof(uint32_t )> tmp;
    std::transform(view_.begin(), view_.end(), tmp.begin(), [](const auto &entry) {
        return *entry;
    });
    return {*reinterpret_cast<const uint32_t *>(tmp.data()), current_pos_};
}

template <>
BinaryBuffer::BinaryBufferView::BinBuffGetResult<uint16_t> BinaryBuffer::BinaryBufferView::get() {
    std::array<ByteType, sizeof(uint16_t)> tmp;
    std::transform(view_.begin(), view_.end(), tmp.begin(), [](const auto &entry) {
        return *entry;
    });
    return {*reinterpret_cast<const uint16_t *>(tmp.data()), current_pos_};
}

template <>
BinaryBuffer::BinaryBufferView::BinBuffGetResult<uint8_t> BinaryBuffer::BinaryBufferView::get() {
    std::array<ByteType, sizeof(uint8_t)> tmp;
    std::transform(view_.begin(), view_.end(), tmp.begin(), [](const auto &entry) {
        return *entry;
    });
    return {*reinterpret_cast<const uint8_t *>(tmp.data()), current_pos_};
}

template <>
BinaryBuffer::BinaryBufferView::BinBuffGetResult<std::string> BinaryBuffer::BinaryBufferView::get() {
    static_assert(sizeof(std::string::value_type) == sizeof(uint8_t), "std::string char type is bigger than one byte!");
    std::string ret;
    ret.reserve(view_.size());
    for (auto it = view_.cbegin(); it != view_.cend(); ++it) {
        const auto [res, iter] = at(it, sizeof(char)).get<char>();
        ret.push_back(res);
    }

    return {ret, current_pos_};
}

}