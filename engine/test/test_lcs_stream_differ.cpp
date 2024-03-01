#include <gtest/gtest.h>
#include <pdif/lcs_stream_differ.hpp>
#include <pdif/stream.hpp>
#include <pdif/stream_elem.hpp>

TEST(PDIFLcsStreamDiffer, TestAdd) {
    pdif::stream stream1;
    pdif::stream stream2;

    stream1.push_back(pdif::stream_elem::create<pdif::text_elem>("Hello"));
    stream2.push_back(pdif::stream_elem::create<pdif::text_elem>("Hello"));
    stream2.push_back(pdif::stream_elem::create<pdif::text_elem>("World"));

    pdif::lcs_stream_differ differ(stream1, stream2);
    pdif::diff diff;

    ASSERT_NO_THROW(differ.diff(diff));

    ASSERT_EQ(diff.edit_op_size(), 2);

    auto edit_op = diff.get_edit_op(0);
    ASSERT_EQ(edit_op.get_type(), pdif::edit_op_type::EQ);
    ASSERT_EQ(edit_op.has_arg(), false);

    edit_op = diff.get_edit_op(1);

    ASSERT_EQ(edit_op.get_type(), pdif::edit_op_type::INSERT);
    ASSERT_EQ(edit_op.has_arg(), true);
    ASSERT_EQ(edit_op.get_arg()->as<pdif::text_elem>()->text(), "World");
}

TEST(PDIFLcsStreamDiffer, TestDelete) {
    pdif::stream stream1;
    pdif::stream stream2;

    stream1.push_back(pdif::stream_elem::create<pdif::text_elem>("Hello"));
    stream1.push_back(pdif::stream_elem::create<pdif::text_elem>("World"));
    stream2.push_back(pdif::stream_elem::create<pdif::text_elem>("Hello"));

    pdif::lcs_stream_differ differ(stream1, stream2);
    pdif::diff diff;

    ASSERT_NO_THROW(differ.diff(diff));

    // ASSERT_EQ(diff.edit_op_size(), 2);

    auto edit_op = diff.get_edit_op(0);
    ASSERT_EQ(edit_op.get_type(), pdif::edit_op_type::EQ);
    ASSERT_EQ(edit_op.has_arg(), false);

    edit_op = diff.get_edit_op(1);

    ASSERT_EQ(edit_op.get_type(), pdif::edit_op_type::DELETE);
    ASSERT_EQ(edit_op.has_arg(), false);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}