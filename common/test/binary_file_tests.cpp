#include <gtest/gtest.h>

#include <filesystem>

#include <rcbe-engine/binary/BinaryBuffer.hpp>

/**
 *
 * dummy header has structure
 * size_t[1] - version
 * size_t[1] - number of chunks
 *
 * */
struct dummy_header {
    static constexpr size_t SIZE = sizeof(size_t) * 2;
    size_t version = 1;
    size_t num_chunks;
};

namespace rcbe::binary {
void to_binary(BinaryBuffer &b, const dummy_header &dh) {
    b = BinaryBuffer({dh.version, dh.num_chunks});
}

void from_binary(const BinaryBuffer &b, dummy_header &dh) {
    dh.version = b.at(0, sizeof(decltype(dh.version))).get<decltype(dh.version)>();
    dh.num_chunks = b.at(sizeof(decltype(dh.version)), sizeof(decltype(dh.num_chunks))).get<decltype(dh.num_chunks)>();
}
}

/**
 *
 * dummy chunk has structure
 * float[1] - some float value
 * size_t[1] - number of chars
 * char[2] - some two chars
 *
 * */

struct dummy_chunk {
    static constexpr size_t SIZE = sizeof(float) + sizeof(size_t) + sizeof(char) * 2;
    float some_float;
    size_t num_chars = 2;
    char chars[2];
};

namespace rcbe::binary {
void to_binary(BinaryBuffer &b, const dummy_chunk &dc) {
    b = BinaryBuffer({ dc.some_float, dc.num_chars, dc.chars });
}

void from_binary(const BinaryBuffer &b, dummy_chunk &dc) {
    dc.some_float = b.at(0, sizeof(float)).get<float>();
    dc.num_chars = b.at(sizeof(float), sizeof(size_t)).get<size_t>();
    for (size_t i = 0; i < dc.num_chars; ++i) {
        dc.chars[i] = b.at(sizeof(float) + sizeof(size_t) + sizeof(char) * i, sizeof(char)).get<char>();
    }
}
}

class BinaryFileTests : public ::testing::Test {
public:
    [[nodiscard]] std::filesystem::path filePath(size_t invocation) const {
        return file_path_str_ + std::to_string(invocation);
    }

private:
    const std::string file_path_str_ = "/tmp/binary_test.dat";
};

TEST_F(BinaryFileTests, OutputSimple) {
    dummy_header dh {1, 2};
    rcbe::binary::BinaryBuffer bb;
    rcbe::binary::to_binary(bb, dh);

    for (size_t i = 0; i < dh.num_chunks; ++i) {
        dummy_chunk dc { static_cast<float>(0.5 * i), 2, {'a', (i != 0) ? 'b' : 'c'} } ;
        rcbe::binary::to_binary(bb, dc);
    }

    std::ofstream ofs(filePath(1).string(), std::ios::out | std::ios::binary);
    if (!ofs)
        throw std::runtime_error("Can't open file for write!");
    ofs << bb;

    ASSERT_TRUE(std::filesystem::exists(filePath(1)));
}

TEST_F(BinaryFileTests, InputSimple) {
    std::ifstream ifs(filePath(1), std::ios::in | std::ios::binary);
    rcbe::binary::BinaryBuffer bb;
    ifs >> bb;
    dummy_header ndh {};
    rcbe::binary::from_binary(bb, ndh);
    ASSERT_EQ(ndh.version, 1);
    ASSERT_EQ(ndh.num_chunks, 2);

    for (size_t i = 0; i < ndh.num_chunks; ++i) {
        dummy_chunk ndc {};
        auto bbb = bb.at(dummy_header::SIZE  + dummy_chunk::SIZE * i, dummy_chunk::SIZE);
        rcbe::binary::from_binary(bbb, ndc);
        ASSERT_EQ(ndc.num_chars, 2);
        ASSERT_FLOAT_EQ(static_cast<float>(0.5 * i), ndc.some_float);
        ASSERT_EQ(ndc.chars[0], 'a');

        if (i) {
            ASSERT_EQ(ndc.chars[1], 'b');
        } else {
            ASSERT_EQ(ndc.chars[1], 'c');
        }
    }
}

