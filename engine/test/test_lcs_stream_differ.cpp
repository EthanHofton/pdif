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

    ASSERT_EQ(diff.edit_op_size(), 2);

    auto edit_op = diff.get_edit_op(0);
    ASSERT_EQ(edit_op.get_type(), pdif::edit_op_type::EQ);
    ASSERT_EQ(edit_op.has_arg(), false);

    edit_op = diff.get_edit_op(1);

    ASSERT_EQ(edit_op.get_type(), pdif::edit_op_type::DELETE);
    ASSERT_EQ(edit_op.has_arg(), false);
}

TEST(PDIFLcsStreamDiffer, TestReplace) {
    pdif::stream stream1;
    pdif::stream stream2;

    stream1.push_back(pdif::stream_elem::create<pdif::text_elem>("Hello"));
    stream1.push_back(pdif::stream_elem::create<pdif::text_elem>("World"));
    stream2.push_back(pdif::stream_elem::create<pdif::text_elem>("Hello"));
    stream2.push_back(pdif::stream_elem::create<pdif::text_elem>("World!"));

    pdif::lcs_stream_differ differ(stream1, stream2);
    pdif::diff diff;

    ASSERT_NO_THROW(differ.diff(diff));

    ASSERT_EQ(diff.edit_op_size(), 3);

    auto edit_op = diff.get_edit_op(0);
    ASSERT_EQ(edit_op.get_type(), pdif::edit_op_type::EQ);
    ASSERT_EQ(edit_op.has_arg(), false);

    edit_op = diff.get_edit_op(1);

    ASSERT_EQ(edit_op.get_type(), pdif::edit_op_type::INSERT);
    ASSERT_EQ(edit_op.has_arg(), true);
    ASSERT_EQ(edit_op.get_arg()->as<pdif::text_elem>()->text(), "World!");

    edit_op = diff.get_edit_op(2);

    ASSERT_EQ(edit_op.get_type(), pdif::edit_op_type::DELETE);
    ASSERT_EQ(edit_op.has_arg(), false);
}

TEST(PDIFLcsStreamDiffer, TestEmpty) {
    pdif::stream stream1;
    pdif::stream stream2;

    pdif::lcs_stream_differ differ(stream1, stream2);
    pdif::diff diff;

    ASSERT_NO_THROW(differ.diff(diff));

    ASSERT_EQ(diff.edit_op_size(), 0);
}

TEST(PDIFLcsStreamDiffer, TestNoChange) {
    pdif::stream stream1;
    pdif::stream stream2;

    stream1.push_back(pdif::stream_elem::create<pdif::text_elem>("Hello"));
    stream2.push_back(pdif::stream_elem::create<pdif::text_elem>("Hello"));

    pdif::lcs_stream_differ differ(stream1, stream2);
    pdif::diff diff;

    ASSERT_NO_THROW(differ.diff(diff));

    ASSERT_EQ(diff.edit_op_size(), 1);

    auto edit_op = diff.get_edit_op(0);
    ASSERT_EQ(edit_op.get_type(), pdif::edit_op_type::EQ);
    ASSERT_EQ(edit_op.has_arg(), false);
}

TEST(PDIFLcsStreamDiffer, TestBigChange) {
    pdif::stream stream1;
    pdif::stream stream2;

    stream1.push_back(pdif::stream_elem::create<pdif::text_elem>("The"));
    stream1.push_back(pdif::stream_elem::create<pdif::text_elem>("quick"));
    stream1.push_back(pdif::stream_elem::create<pdif::text_elem>("brown"));
    stream1.push_back(pdif::stream_elem::create<pdif::text_elem>("fox"));
    stream1.push_back(pdif::stream_elem::create<pdif::text_elem>("jumps"));
    stream1.push_back(pdif::stream_elem::create<pdif::text_elem>("over"));
    stream1.push_back(pdif::stream_elem::create<pdif::text_elem>("the"));
    stream1.push_back(pdif::stream_elem::create<pdif::text_elem>("lazy"));
    stream1.push_back(pdif::stream_elem::create<pdif::text_elem>("dog")); 

    stream2.push_back(pdif::stream_elem::create<pdif::text_elem>("The"));
    stream2.push_back(pdif::stream_elem::create<pdif::text_elem>("fast"));
    stream2.push_back(pdif::stream_elem::create<pdif::text_elem>("brown"));
    stream2.push_back(pdif::stream_elem::create<pdif::text_elem>("cat"));
    stream2.push_back(pdif::stream_elem::create<pdif::text_elem>("over"));
    stream2.push_back(pdif::stream_elem::create<pdif::text_elem>("the"));
    stream2.push_back(pdif::stream_elem::create<pdif::text_elem>("lazy"));
    stream2.push_back(pdif::stream_elem::create<pdif::text_elem>("mouse"));

    pdif::lcs_stream_differ differ(stream1, stream2);

    pdif::diff diff;

    ASSERT_NO_THROW(differ.diff(diff));

    ASSERT_EQ(diff.edit_op_size(), 12);

    auto edit_op = diff.get_edit_op(0);
    ASSERT_EQ(edit_op.get_type(), pdif::edit_op_type::EQ);
    
    edit_op = diff.get_edit_op(1);
    ASSERT_EQ(edit_op.get_type(), pdif::edit_op_type::INSERT);
    ASSERT_EQ(edit_op.has_arg(), true);
    ASSERT_EQ(edit_op.get_arg()->as<pdif::text_elem>()->text(), "fast");

    edit_op = diff.get_edit_op(2);
    ASSERT_EQ(edit_op.get_type(), pdif::edit_op_type::DELETE);

    edit_op = diff.get_edit_op(3);
    ASSERT_EQ(edit_op.get_type(), pdif::edit_op_type::EQ);

    edit_op = diff.get_edit_op(4);
    ASSERT_EQ(edit_op.get_type(), pdif::edit_op_type::INSERT);
    ASSERT_EQ(edit_op.has_arg(), true);
    ASSERT_EQ(edit_op.get_arg()->as<pdif::text_elem>()->text(), "cat");

    edit_op = diff.get_edit_op(5);
    ASSERT_EQ(edit_op.get_type(), pdif::edit_op_type::DELETE);

    edit_op = diff.get_edit_op(6);
    ASSERT_EQ(edit_op.get_type(), pdif::edit_op_type::DELETE);

    edit_op = diff.get_edit_op(7);
    ASSERT_EQ(edit_op.get_type(), pdif::edit_op_type::EQ);

    edit_op = diff.get_edit_op(8);
    ASSERT_EQ(edit_op.get_type(), pdif::edit_op_type::EQ);

    edit_op = diff.get_edit_op(9);
    ASSERT_EQ(edit_op.get_type(), pdif::edit_op_type::EQ);

    edit_op = diff.get_edit_op(10);
    ASSERT_EQ(edit_op.get_type(), pdif::edit_op_type::INSERT);
    ASSERT_EQ(edit_op.has_arg(), true);
    ASSERT_EQ(edit_op.get_arg()->as<pdif::text_elem>()->text(), "mouse");

    edit_op = diff.get_edit_op(11);
    ASSERT_EQ(edit_op.get_type(), pdif::edit_op_type::DELETE);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}