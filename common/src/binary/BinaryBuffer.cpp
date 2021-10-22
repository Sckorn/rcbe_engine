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
    (void)append(bb);
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

BinaryBuffer BinaryBuffer::at(size_t offset, size_t size) const {
    if (view_)
        return constructFromChunk(buffer_view_, offset, size);
    else
        return constructFromChunk(buffer_, offset, size);
}

size_t BinaryBuffer::size() const {
    if (view_)
        return buffer_view_.size();
    else
        return buffer_.size();
}

[[nodiscard]] bool BinaryBuffer::isView() const {
    return view_;
}

void BinaryBuffer::read(std::istream &is, StorageType &s) {
    while (!is.eof()) {
        ByteType b {};
        is.read(reinterpret_cast<char*>(&b), sizeof(b));
        s.push_back(b);
    }
}

BinaryBuffer::BinaryBuffer(StorageType::const_iterator begin, StorageType::const_iterator end)
:
view_(true)
{
    initView([](const auto &entry) {
        return &(entry);
    }, begin, end);

    for (auto p : buffer_view_) {
        if (p == nullptr)
            throw std::runtime_error("Null pointer detected in binary buffer chunk!");
    }
}

BinaryBuffer::BinaryBuffer(ViewType::const_iterator begin, ViewType::const_iterator end)
:
view_(true)
{
    initView([](const auto &entry) {
        if (entry == nullptr)
            throw std::runtime_error("Null pointer detected in binary buffer chunk!");
        return entry;
    }, begin, end);
}

BinaryBuffer &BinaryBuffer::appendImplementation(const BinaryBuffer &source) {
    if (view_)
        throw std::runtime_error("Appending to BinaryBuffer view is prohibited!");

    buffer_.reserve(buffer_.size() + source.size());

    if (source.isView()) {
        appendImplementation(source.buffer_view_);
    } else {
        appendImplementation(source.buffer_);
    }

    return *this;
}

void BinaryBuffer::appendImplementation(const StorageType &storage) {
    buffer_.insert(buffer_.end(), storage.begin(), storage.end());
}

void BinaryBuffer::appendImplementation(const ViewType &storage) {
    std::transform(storage.begin(), storage.end(), std::back_inserter(buffer_), [](auto entry) {
        return *entry;
    });
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
size_t BinaryBuffer::get() {
    if (view_) {
        std::array<ByteType, sizeof(size_t)> tmp;
        std::transform(buffer_view_.begin(), buffer_view_.end(), tmp.begin(), [](const auto &entry) {
            return *entry;
        });
        return *reinterpret_cast<const size_t*>(tmp.data());
    } else {
        return *reinterpret_cast<const size_t*>(buffer_.data());
    }
}

template <>
float BinaryBuffer::get() {
    if (view_) {
        std::array<ByteType, sizeof(float)> tmp;
        std::transform(buffer_view_.begin(), buffer_view_.end(), tmp.begin(), [](const auto &entry) {
            return *entry;
        });
        return *reinterpret_cast<const float*>(tmp.data());
    } else {
        return *reinterpret_cast<const float*>(buffer_.data());
    }
}

template <>
char BinaryBuffer::get() {
    if (view_) {
        std::array<ByteType, sizeof(char )> tmp;
        std::transform(buffer_view_.begin(), buffer_view_.end(), tmp.begin(), [](const auto &entry) {
            return *entry;
        });
        return *reinterpret_cast<const char *>(tmp.data());
    } else {
        return *reinterpret_cast<const char*>(buffer_.data());
    }
}

template <>
uint32_t BinaryBuffer::get() {
    if (view_) {
        std::array<ByteType, sizeof(uint32_t )> tmp;
        std::transform(buffer_view_.begin(), buffer_view_.end(), tmp.begin(), [](const auto &entry) {
            return *entry;
        });
        return *reinterpret_cast<const uint32_t *>(tmp.data());
    } else {
        return *reinterpret_cast<const uint32_t *>(buffer_.data());
    }
}

template <>
uint16_t BinaryBuffer::get() {
    if (view_) {
        std::array<ByteType, sizeof(uint16_t)> tmp;
        std::transform(buffer_view_.begin(), buffer_view_.end(), tmp.begin(), [](const auto &entry) {
            return *entry;
        });
        return *reinterpret_cast<const uint16_t *>(tmp.data());
    } else {
        return *reinterpret_cast<const uint16_t*>(buffer_.data());
    }
}

template <>
uint8_t BinaryBuffer::get() {
    if (view_) {
        std::array<ByteType, sizeof(uint8_t)> tmp;
        std::transform(buffer_view_.begin(), buffer_view_.end(), tmp.begin(), [](const auto &entry) {
            return *entry;
        });
        return *reinterpret_cast<const uint8_t *>(tmp.data());
    } else {
        return *reinterpret_cast<const uint8_t*>(buffer_.data());
    }
}

template <>
std::string BinaryBuffer::get() {
    static_assert(sizeof(std::string::value_type) == sizeof(uint8_t), "std::string char type is bigger than one byte!");
    std::string ret;
    ret.reserve(buffer_.size());
    for (size_t i = 0; i < size(); ++i) {
        ret.push_back(at(i, sizeof(char)).get<char>());
    }

    return ret;
}

}