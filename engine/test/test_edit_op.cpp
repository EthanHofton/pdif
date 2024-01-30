#include <gtest/gtest.h>
#include <pdif/edit_op.hpp>

TEST(PDIFEditOp, TestConstructor) {
    ASSERT_NO_THROW({pdif::edit_op op(pdif::edit_op_type::INSERT, pdif::stream_elem::create<pdif::text_elem>("Hello, World!"));});
    pdif::edit_op op(pdif::edit_op_type::INSERT, pdif::stream_elem::create<pdif::text_elem>("Hello, World!"));

    ASSERT_EQ(op.get_type(), pdif::edit_op_type::INSERT);
    ASSERT_EQ(op.has_arg(), true);
    ASSERT_EQ(op.get_arg()->type(), pdif::stream_type::text);
    ASSERT_EQ(op.get_arg()->as<pdif::text_elem>()->text(), "Hello, World!");

    ASSERT_NO_THROW({pdif::edit_op op(pdif::edit_op_type::DELETE);});
    op = pdif::edit_op(pdif::edit_op_type::DELETE);

    ASSERT_EQ(op.get_type(), pdif::edit_op_type::DELETE);
    ASSERT_EQ(op.has_arg(), false);

    ASSERT_NO_THROW({pdif::edit_op op(pdif::edit_op_type::EQ);});
    op = pdif::edit_op(pdif::edit_op_type::EQ);

    ASSERT_EQ(op.get_type(), pdif::edit_op_type::EQ);
    ASSERT_EQ(op.has_arg(), false);
}

TEST(PDIFEditOp, TestConstructorInvalidArg) {
    ASSERT_THROW({pdif::edit_op op(pdif::edit_op_type::EQ, pdif::stream_elem::create<pdif::text_elem>("Hello, World!"));}, pdif::pdif_invalid_argment);

    ASSERT_THROW({pdif::edit_op op(pdif::edit_op_type::DELETE, pdif::stream_elem::create<pdif::text_elem>("Hello, World!"));}, pdif::pdif_invalid_argment);

    ASSERT_THROW({pdif::edit_op op(pdif::edit_op_type::INSERT, std::nullopt);}, pdif::pdif_invalid_argment);

    ASSERT_THROW({pdif::edit_op op(pdif::edit_op_type::INSERT);}, pdif::pdif_invalid_argment);
}

TEST(PDIFEditOp, TestGetType) {
    pdif::edit_op op(pdif::edit_op_type::INSERT, pdif::stream_elem::create<pdif::text_elem>("Hello, World!"));

    ASSERT_EQ(op.get_type(), pdif::edit_op_type::INSERT);

    op = pdif::edit_op(pdif::edit_op_type::DELETE);

    ASSERT_EQ(op.get_type(), pdif::edit_op_type::DELETE);

    op = pdif::edit_op(pdif::edit_op_type::EQ);

    ASSERT_EQ(op.get_type(), pdif::edit_op_type::EQ);
}

TEST(PDIFEditOp, TestGetArg) {
    pdif::edit_op op(pdif::edit_op_type::INSERT, pdif::stream_elem::create<pdif::text_elem>("Hello, World!"));

    ASSERT_EQ(op.has_arg(), true);
    ASSERT_EQ(op.get_arg()->type(), pdif::stream_type::text);
    ASSERT_EQ(op.get_arg()->as<pdif::text_elem>()->text(), "Hello, World!");
}

TEST(PDIFEditOp, TestGetArgInvalid) {
    pdif::edit_op op(pdif::edit_op_type::DELETE);

    ASSERT_EQ(op.has_arg(), false);
    ASSERT_THROW({op.get_arg();}, pdif::pdif_invalid_argment);

    op = pdif::edit_op(pdif::edit_op_type::EQ);
    ASSERT_THROW({op.get_arg();}, pdif::pdif_invalid_argment);
}

TEST(PDIFEditOp, TestHasArg) {
    pdif::edit_op op(pdif::edit_op_type::INSERT, pdif::stream_elem::create<pdif::text_elem>("Hello, World!"));

    ASSERT_EQ(op.has_arg(), true);

    op = pdif::edit_op(pdif::edit_op_type::DELETE);

    ASSERT_EQ(op.has_arg(), false);

    op = pdif::edit_op(pdif::edit_op_type::EQ);

    ASSERT_EQ(op.has_arg(), false);
}

TEST(PDIFEditOp, TestExecuteInsert) {
    pdif::stream s;
    s.push_back(pdif::stream_elem::create<pdif::text_elem>("Hello,"));
    s.push_back(pdif::stream_elem::create<pdif::text_elem>(" World"));
    s.push_back(pdif::stream_elem::create<pdif::text_elem>("!"));    

    size_t index = 0;

    pdif::edit_op op(pdif::edit_op_type::INSERT, pdif::stream_elem::create<pdif::text_elem>("Hello, World!"));

    ASSERT_NO_THROW({op.execute(s, index);});
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}