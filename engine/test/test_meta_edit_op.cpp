#include <gtest/gtest.h>
#include <pdif/meta_edit_op.hpp>

TEST(PDIFMetaEditOp, TestConstructor) {
    ASSERT_NO_THROW({pdif::meta_edit_op op(pdif::meta_edit_op_type::META_ADD, "test_key", "test_val");});
    pdif::meta_edit_op op = pdif::meta_edit_op(pdif::meta_edit_op_type::META_ADD, "test_key", "test_val");

    ASSERT_EQ(op.get_type(), pdif::meta_edit_op_type::META_ADD);
    ASSERT_EQ(op.has_meta_val(), true);
    ASSERT_EQ(op.get_meta_val(), "test_val");
    ASSERT_EQ(op.get_meta_key(), "test_key");

    ASSERT_NO_THROW({pdif::meta_edit_op op(pdif::meta_edit_op_type::META_UPDATE, "test_key", "test_val");});
    op = pdif::meta_edit_op(pdif::meta_edit_op_type::META_UPDATE, "test_key", "test_val");

    ASSERT_EQ(op.get_type(), pdif::meta_edit_op_type::META_UPDATE);
    ASSERT_EQ(op.get_meta_key(), "test_key");
    ASSERT_TRUE(op.has_meta_val());
    ASSERT_EQ(op.get_meta_val(), "test_val");

    ASSERT_NO_THROW({pdif::meta_edit_op op(pdif::meta_edit_op_type::META_DELETE, "test_key");});
    op = pdif::meta_edit_op(pdif::meta_edit_op_type::META_DELETE, "test_key");

    ASSERT_EQ(op.get_type(), pdif::meta_edit_op_type::META_DELETE);
    ASSERT_EQ(op.get_meta_key(), "test_key");
    ASSERT_FALSE(op.has_meta_val());
}

TEST(PDIFMetaEditOp, TestConstructorInvalid) {
    ASSERT_THROW({pdif::meta_edit_op op(pdif::meta_edit_op_type::META_ADD, "test_key");}, pdif::pdif_invalid_argment);

    ASSERT_THROW({pdif::meta_edit_op op(pdif::meta_edit_op_type::META_UPDATE, "test_key");}, pdif::pdif_invalid_argment);

    ASSERT_THROW({pdif::meta_edit_op op(pdif::meta_edit_op_type::META_DELETE, "test_key", "test_val");}, pdif::pdif_invalid_argment);
}

TEST(PDIFMetaEditOp, TestGetType) {
    pdif::meta_edit_op op = pdif::meta_edit_op(pdif::meta_edit_op_type::META_ADD, "test_key", "test_val");

    ASSERT_EQ(op.get_type(), pdif::meta_edit_op_type::META_ADD);

    op = pdif::meta_edit_op(pdif::meta_edit_op_type::META_UPDATE, "test_key", "test_val");

    ASSERT_EQ(op.get_type(), pdif::meta_edit_op_type::META_UPDATE);

    op = pdif::meta_edit_op(pdif::meta_edit_op_type::META_DELETE, "test_key");

    ASSERT_EQ(op.get_type(), pdif::meta_edit_op_type::META_DELETE);
}

TEST(PDIFMetaEditOp, TestGetMetaKey) {
    pdif::meta_edit_op op = pdif::meta_edit_op(pdif::meta_edit_op_type::META_ADD, "test_key", "test_val");

    ASSERT_EQ(op.get_meta_key(), "test_key");
}

TEST(PDIFMetaEditOp, TestGetMetaVal) {
    pdif::meta_edit_op op = pdif::meta_edit_op(pdif::meta_edit_op_type::META_ADD, "test_key", "test_val");

    ASSERT_NO_THROW({op.get_meta_val();});

    ASSERT_EQ(op.get_meta_val(), "test_val");
}

TEST(PDIFMetaEditOp, TestGetMetaValInvalid) {
    pdif::meta_edit_op op = pdif::meta_edit_op(pdif::meta_edit_op_type::META_DELETE, "test_key");

    ASSERT_THROW({op.get_meta_val();}, pdif::pdif_invalid_argment);
}

TEST(PDIFMetaEditOp, TestHasMetaVal) {
    pdif::meta_edit_op op = pdif::meta_edit_op(pdif::meta_edit_op_type::META_ADD, "test_key", "test_val");

    ASSERT_TRUE(op.has_meta_val());

    op = pdif::meta_edit_op(pdif::meta_edit_op_type::META_DELETE, "test_key");

    ASSERT_FALSE(op.has_meta_val());
}

TEST(PDIFMetaEditOp, TestExecuteMetaAdd) {
    pdif::meta_edit_op op = pdif::meta_edit_op(pdif::meta_edit_op_type::META_ADD, "test_key", "test_val");
    pdif::stream_meta stream;

    ASSERT_NO_THROW({op.execute(stream);});

    ASSERT_EQ(stream.get_metadata("test_key"), "test_val");
}

TEST(PDIFMetaEditOp, TestExecuteMetaDelete) {
    pdif::stream_meta stream;
    stream.add_metadata("test_key", "test_val");

    ASSERT_TRUE(stream.has_key("test_key"));

    pdif::meta_edit_op op = pdif::meta_edit_op(pdif::meta_edit_op_type::META_DELETE, "test_key");

    ASSERT_NO_THROW({op.execute(stream);});

    ASSERT_FALSE(stream.has_key("test_key"));
}

TEST(PDIFMetaEditOp, TestExecuteMetaUpdate) {
    pdif::stream_meta stream;
    stream.add_metadata("test_key", "test_val");

    ASSERT_TRUE(stream.has_key("test_key"));

    pdif::meta_edit_op op = pdif::meta_edit_op(pdif::meta_edit_op_type::META_UPDATE, "test_key", "new_val");

    ASSERT_NO_THROW({op.execute(stream);});

    ASSERT_EQ(stream.get_metadata("test_key"), "new_val");
}

TEST(PDIFMetaEditOp, TestExecuteMetaUpdateInvalid) {
    pdif::stream_meta stream;

    pdif::meta_edit_op op = pdif::meta_edit_op(pdif::meta_edit_op_type::META_UPDATE, "test_key", "new_val");

    ASSERT_THROW({op.execute(stream);}, pdif::pdif_invalid_operation);
}

TEST(PDIFMetaEditOp, TestExecuteMetaAddInvalid) {
    pdif::stream_meta stream;
    stream.add_metadata("test_key", "test_val");

    pdif::meta_edit_op op = pdif::meta_edit_op(pdif::meta_edit_op_type::META_ADD, "test_key", "new_val");

    ASSERT_THROW({op.execute(stream);}, pdif::pdif_invalid_operation);
}

TEST(PDIFMetaEditOp, TestExecuteMetaDeleteInvalid) {
    pdif::stream_meta stream;

    pdif::meta_edit_op op = pdif::meta_edit_op(pdif::meta_edit_op_type::META_DELETE, "test_key");

    ASSERT_THROW({op.execute(stream);}, pdif::pdif_invalid_operation);
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}