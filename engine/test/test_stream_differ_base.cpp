#include <gtest/gtest.h>
#include <pdif/stream_differ_base.hpp>

// make a concrete class for testing
class stream_differ_base_concrete : public pdif::stream_differ_base {
public:
    stream_differ_base_concrete(const pdif::stream& stream1, const pdif::stream& stream2) : pdif::stream_differ_base(stream1, stream2) {}
    void diff(pdif::diff&) override {}
};

TEST(PDIFStreamDifferBase, TestConstructor) {
    pdif::stream stream1;
    pdif::stream stream2;
    ASSERT_NO_THROW({stream_differ_base_concrete differ(stream1, stream2);});
}

TEST(PDIFStreamDifferBase, TestMetaDiffAdd) {
    pdif::stream stream1;
    pdif::stream stream2;

    stream2.add_metadata("key", "value");

    stream_differ_base_concrete differ(stream1, stream2);

    pdif::diff d;
    ASSERT_NO_THROW({differ.meta_diff(d);});

    ASSERT_EQ(d.meta_edit_op_size(), 1);

    auto op = d.get_meta_edit_op(0);

    ASSERT_EQ(op.get_type(), pdif::meta_edit_op_type::META_ADD);
    ASSERT_EQ(op.get_meta_key(), "key");
    ASSERT_TRUE(op.has_meta_val());
    ASSERT_EQ(op.get_meta_val(), "value");
}

TEST(PDIFStreamDifferBase, TestMetaDiffUpdate) {
    pdif::stream stream1;
    pdif::stream stream2;

    stream1.add_metadata("key", "value1");
    stream2.add_metadata("key", "value2");

    stream_differ_base_concrete differ(stream1, stream2);

    pdif::diff d;
    ASSERT_NO_THROW({differ.meta_diff(d);});

    ASSERT_EQ(d.meta_edit_op_size(), 1);

    auto op = d.get_meta_edit_op(0);

    ASSERT_EQ(op.get_type(), pdif::meta_edit_op_type::META_UPDATE);
    ASSERT_EQ(op.get_meta_key(), "key");
    ASSERT_TRUE(op.has_meta_val());
    ASSERT_EQ(op.get_meta_val(), "value2");
}

TEST(PDIFStreamDifferBase, TestMetaDiffDelete) {
    pdif::stream stream1;
    pdif::stream stream2;

    stream1.add_metadata("key", "value");

    stream_differ_base_concrete differ(stream1, stream2);

    pdif::diff d;
    ASSERT_NO_THROW({differ.meta_diff(d);});

    ASSERT_EQ(d.meta_edit_op_size(), 1);

    auto op = d.get_meta_edit_op(0);

    ASSERT_EQ(op.get_type(), pdif::meta_edit_op_type::META_DELETE);
    ASSERT_EQ(op.get_meta_key(), "key");
    ASSERT_FALSE(op.has_meta_val());
}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}