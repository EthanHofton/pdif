#include <gtest/gtest.h>
#include <pdif/diff.hpp>
#include <pdif/edit_op.hpp>
#include <pdif/stream.hpp>
#include <pdif/stream_elem.hpp>
#include <pdif/meta_edit_op.hpp>

TEST(PDIFDiff, TestAddEditOp) {
    pdif::diff diff;
    pdif::edit_op op(pdif::edit_op_type::INSERT, pdif::stream_elem::create<pdif::text_elem>("Inserted"));
    ASSERT_NO_THROW({diff.add_edit_op(op);});
    ASSERT_EQ(diff.edit_op_size(), 1);
}

TEST(PDIFDiff, TestGetEditOp) {
    pdif::diff diff;
    pdif::edit_op op(pdif::edit_op_type::INSERT, pdif::stream_elem::create<pdif::text_elem>("Inserted"));
    diff.add_edit_op(op);

    ASSERT_EQ(diff.get_edit_op(0).get_type(), pdif::edit_op_type::INSERT);
    ASSERT_EQ(diff.get_edit_op(0).has_arg(), true);
    ASSERT_EQ(diff.get_edit_op(0).get_arg()->as<pdif::text_elem>()->text(), "Inserted");
}

TEST(PDIFDiff, TestGetEditOpFront) {
    pdif::diff diff;

    pdif::edit_op op1(pdif::edit_op_type::INSERT, pdif::stream_elem::create<pdif::text_elem>("Inserted"));
    pdif::edit_op op2(pdif::edit_op_type::DELETE);
    pdif::edit_op op3(pdif::edit_op_type::EQ);

    diff.add_edit_op(op1);
    diff.add_edit_op(op2);
    diff.add_edit_op(op3);

    ASSERT_EQ(diff.get_edit_op(0).get_type(), pdif::edit_op_type::INSERT);
    ASSERT_EQ(diff.get_edit_op(0).has_arg(), true);
    ASSERT_EQ(diff.get_edit_op(0).get_arg()->as<pdif::text_elem>()->text(), "Inserted");
}

TEST(PDIFDiff, TestGetEditOpBack) {
    pdif::diff diff;

    pdif::edit_op op1(pdif::edit_op_type::INSERT, pdif::stream_elem::create<pdif::text_elem>("Inserted"));
    pdif::edit_op op2(pdif::edit_op_type::DELETE);
    pdif::edit_op op3(pdif::edit_op_type::EQ);

    diff.add_edit_op(op1);
    diff.add_edit_op(op2);
    diff.add_edit_op(op3);

    ASSERT_EQ(diff.get_edit_op(2).get_type(), pdif::edit_op_type::EQ);
    ASSERT_EQ(diff.get_edit_op(2).has_arg(), false);
}

TEST(PDIFDiff, TestGetEditOpInvalid) {
    pdif::diff diff;

    pdif::edit_op op1(pdif::edit_op_type::INSERT, pdif::stream_elem::create<pdif::text_elem>("Inserted"));
    pdif::edit_op op2(pdif::edit_op_type::DELETE);
    pdif::edit_op op3(pdif::edit_op_type::EQ);

    diff.add_edit_op(op1);
    diff.add_edit_op(op2);
    diff.add_edit_op(op3);

    ASSERT_THROW(diff.get_edit_op(3), pdif::pdif_out_of_bounds);
}

TEST(PDIFDiff, TestEditOpSize) {
    pdif::diff diff;

    pdif::edit_op op1(pdif::edit_op_type::INSERT, pdif::stream_elem::create<pdif::text_elem>("Inserted"));
    pdif::edit_op op2(pdif::edit_op_type::DELETE);
    pdif::edit_op op3(pdif::edit_op_type::EQ);

    diff.add_edit_op(op1);
    diff.add_edit_op(op2);
    diff.add_edit_op(op3);

    ASSERT_EQ(diff.edit_op_size(), 3);
}

TEST(PDIFDiff, TestAddMetaEditOp) {
    pdif::diff diff;

    pdif::meta_edit_op op1(pdif::meta_edit_op_type::META_ADD, "key", "value");
    pdif::meta_edit_op op2(pdif::meta_edit_op_type::META_DELETE, "key");
    pdif::meta_edit_op op3(pdif::meta_edit_op_type::META_UPDATE, "key", "value");

    ASSERT_NO_THROW({diff.add_meta_edit_op(op1);});
    ASSERT_NO_THROW({diff.add_meta_edit_op(op2);});
    ASSERT_NO_THROW({diff.add_meta_edit_op(op3);});

    ASSERT_EQ(diff.meta_edit_op_size(), 3);
}

TEST(PDIFDiff, TestGetMetaEditOp) {
    pdif::diff diff;

    pdif::meta_edit_op op1(pdif::meta_edit_op_type::META_ADD, "key", "value");
    pdif::meta_edit_op op2(pdif::meta_edit_op_type::META_DELETE, "key");
    pdif::meta_edit_op op3(pdif::meta_edit_op_type::META_UPDATE, "key", "value");

    diff.add_meta_edit_op(op1);
    diff.add_meta_edit_op(op2);
    diff.add_meta_edit_op(op3);

    ASSERT_EQ(diff.get_meta_edit_op(0).get_type(), pdif::meta_edit_op_type::META_ADD);
    ASSERT_EQ(diff.get_meta_edit_op(0).get_meta_key(), "key");
    ASSERT_EQ(diff.get_meta_edit_op(0).has_meta_val(), true);
    ASSERT_EQ(diff.get_meta_edit_op(0).get_meta_val(), "value");

    ASSERT_EQ(diff.get_meta_edit_op(1).get_type(), pdif::meta_edit_op_type::META_DELETE);
    ASSERT_EQ(diff.get_meta_edit_op(1).get_meta_key(), "key");
    ASSERT_FALSE(diff.get_meta_edit_op(1).has_meta_val());

    ASSERT_EQ(diff.get_meta_edit_op(2).get_type(), pdif::meta_edit_op_type::META_UPDATE);
    ASSERT_EQ(diff.get_meta_edit_op(2).get_meta_key(), "key");
    ASSERT_TRUE(diff.get_meta_edit_op(2).has_meta_val());
    ASSERT_EQ(diff.get_meta_edit_op(2).get_meta_val(), "value");
}

TEST(PDIFDiff, TestGetMetaEditOpFront) {
    pdif::diff diff;

    pdif::meta_edit_op op1(pdif::meta_edit_op_type::META_ADD, "key", "value");
    pdif::meta_edit_op op2(pdif::meta_edit_op_type::META_DELETE, "key");
    pdif::meta_edit_op op3(pdif::meta_edit_op_type::META_UPDATE, "key", "value");

    diff.add_meta_edit_op(op1);
    diff.add_meta_edit_op(op2);
    diff.add_meta_edit_op(op3);

    ASSERT_EQ(diff.get_meta_edit_op(0).get_type(), pdif::meta_edit_op_type::META_ADD);
    ASSERT_EQ(diff.get_meta_edit_op(0).get_meta_key(), "key");
    ASSERT_EQ(diff.get_meta_edit_op(0).has_meta_val(), true);
    ASSERT_EQ(diff.get_meta_edit_op(0).get_meta_val(), "value");
}


TEST(PDIFDiff, TestGetMetaEditOpBack) {
    pdif::diff diff;

    pdif::meta_edit_op op1(pdif::meta_edit_op_type::META_ADD, "key", "value");
    pdif::meta_edit_op op2(pdif::meta_edit_op_type::META_DELETE, "key");
    pdif::meta_edit_op op3(pdif::meta_edit_op_type::META_UPDATE, "key", "value");

    diff.add_meta_edit_op(op1);
    diff.add_meta_edit_op(op2);
    diff.add_meta_edit_op(op3);

    ASSERT_EQ(diff.get_meta_edit_op(2).get_type(), pdif::meta_edit_op_type::META_UPDATE);
    ASSERT_EQ(diff.get_meta_edit_op(2).get_meta_key(), "key");
    ASSERT_TRUE(diff.get_meta_edit_op(2).has_meta_val());
    ASSERT_EQ(diff.get_meta_edit_op(2).get_meta_val(), "value");
}

TEST(PDIFDiff, TestGetMetaEditOpInvalid) {
    pdif::diff diff;

    pdif::meta_edit_op op1(pdif::meta_edit_op_type::META_ADD, "key", "value");
    pdif::meta_edit_op op2(pdif::meta_edit_op_type::META_DELETE, "key");
    pdif::meta_edit_op op3(pdif::meta_edit_op_type::META_UPDATE, "key", "value");

    diff.add_meta_edit_op(op1);
    diff.add_meta_edit_op(op2);
    diff.add_meta_edit_op(op3);

    ASSERT_THROW(diff.get_meta_edit_op(3), pdif::pdif_out_of_bounds);
}

TEST(PDIFDiff, TestMetaEditOpSize) {
    pdif::diff diff;

    pdif::meta_edit_op op1(pdif::meta_edit_op_type::META_ADD, "key", "value");
    pdif::meta_edit_op op2(pdif::meta_edit_op_type::META_DELETE, "key");
    pdif::meta_edit_op op3(pdif::meta_edit_op_type::META_UPDATE, "key", "value");

    diff.add_meta_edit_op(op1);
    diff.add_meta_edit_op(op2);
    diff.add_meta_edit_op(op3);
    
    ASSERT_EQ(diff.meta_edit_op_size(), 3);
}

TEST(PDIFDiff, TestApplyEditScript) {
    pdif::diff diff;
    pdif::edit_op op2(pdif::edit_op_type::DELETE);
    pdif::edit_op op(pdif::edit_op_type::INSERT, pdif::stream_elem::create<pdif::text_elem>("Inserted"));
    pdif::edit_op op3(pdif::edit_op_type::EQ);

    diff.add_edit_op(op2);
    diff.add_edit_op(op3);
    diff.add_edit_op(op);

    pdif::stream stream;
    stream.push(0, pdif::stream_elem::create<pdif::text_elem>("test"));
    stream.push(1, pdif::stream_elem::create<pdif::text_elem>("test2"));
    stream.push(2, pdif::stream_elem::create<pdif::text_elem>("test3"));

    ASSERT_NO_THROW({diff.apply_edit_script(stream);});

    ASSERT_EQ(stream.size(), 3);

    ASSERT_EQ(stream[0]->type(), pdif::stream_type::text);
    ASSERT_EQ(stream[0]->as<pdif::text_elem>()->text(), "test2");

    ASSERT_EQ(stream[1]->type(), pdif::stream_type::text);
    ASSERT_EQ(stream[1]->as<pdif::text_elem>()->text(), "Inserted");

    ASSERT_EQ(stream[2]->type(), pdif::stream_type::text);
    ASSERT_EQ(stream[2]->as<pdif::text_elem>()->text(), "test3");
}

TEST(PDIFDiff, TestApplyEditScriptInsert) {
    pdif::diff diff;
    pdif::edit_op op(pdif::edit_op_type::INSERT, pdif::stream_elem::create<pdif::text_elem>("Inserted"));

    diff.add_edit_op(op);

    pdif::stream stream;

    ASSERT_NO_THROW({diff.apply_edit_script(stream);});

    ASSERT_EQ(stream.size(), 1);
}

TEST(PDIFDiff, TestApplyEditScriptDelete) {
    pdif::diff diff;
    pdif::edit_op op(pdif::edit_op_type::DELETE);

    diff.add_edit_op(op);

    pdif::stream stream;
    stream.push(0, pdif::stream_elem::create<pdif::text_elem>("test"));

    ASSERT_EQ(stream.size(), 1);

    ASSERT_NO_THROW({diff.apply_edit_script(stream);});

    ASSERT_EQ(stream.size(), 0);
}

TEST(PDIFDiff, TestApplyEditScriptDeleteInvalid) {
    pdif::diff diff;
    pdif::edit_op op(pdif::edit_op_type::DELETE);

    diff.add_edit_op(op);

    pdif::stream stream;

    ASSERT_THROW(diff.apply_edit_script(stream), pdif::pdif_out_of_bounds);
}

TEST(PDIFDiff, TestApplyEditScriptEq) {
    pdif::diff diff;
    pdif::edit_op op(pdif::edit_op_type::EQ);

    diff.add_edit_op(op);

    pdif::stream stream;
    stream.push(0, pdif::stream_elem::create<pdif::text_elem>("test"));

    ASSERT_NO_THROW({diff.apply_edit_script(stream);});

    ASSERT_EQ(stream.size(), 1);
}

TEST(PDIFDiff, TestApplyEditScriptStreamCallback) {
    int i = 0;
    pdif::stream::stream_callback_f callback = [&i](const pdif::edit_op& op) {
        ASSERT_EQ(op.get_type(), pdif::edit_op_type::INSERT);
        ASSERT_EQ(op.has_arg(), true);
        ASSERT_EQ(op.get_arg()->as<pdif::text_elem>()->text(), "Inserted");
        i++;
    };

    pdif::diff diff;
    pdif::edit_op op(pdif::edit_op_type::INSERT, pdif::stream_elem::create<pdif::text_elem>("Inserted"));

    diff.add_edit_op(op);

    pdif::stream stream;
    stream.set_stream_callback(callback);

    ASSERT_EQ(i, 0);
    ASSERT_NO_THROW({diff.apply_edit_script(stream);});
    ASSERT_EQ(i, 1);

    ASSERT_EQ(stream.size(), 1);

    ASSERT_EQ(stream[0]->type(), pdif::stream_type::text);
    ASSERT_EQ(stream[0]->as<pdif::text_elem>()->text(), "Inserted");
}

TEST(PDIFDiff, TestApplyMetaEditScript) {
    pdif::diff diff;
    pdif::meta_edit_op op1(pdif::meta_edit_op_type::META_ADD, "key", "value");
    pdif::meta_edit_op op2(pdif::meta_edit_op_type::META_UPDATE, "ex1", "new_value");
    pdif::meta_edit_op op3(pdif::meta_edit_op_type::META_DELETE, "ex2");

    diff.add_meta_edit_op(op1);
    diff.add_meta_edit_op(op2);
    diff.add_meta_edit_op(op3);

    pdif::stream_meta stream;
    stream.add_metadata("ex1", "old_value");
    stream.add_metadata("ex2", "value");

    ASSERT_NO_THROW({diff.apply_meta_edit_script(stream);});

    ASSERT_EQ(stream.get_metadata("key"), "value");
    ASSERT_EQ(stream.get_metadata("ex1"), "new_value");
    ASSERT_FALSE(stream.has_key("ex2"));
}

TEST(PDIFDiff, TestApplyMetaEditScriptAdd) {
    pdif::diff diff;
    pdif::meta_edit_op op1(pdif::meta_edit_op_type::META_ADD, "key", "value");

    diff.add_meta_edit_op(op1);

    pdif::stream_meta stream;

    ASSERT_NO_THROW({diff.apply_meta_edit_script(stream);});

    ASSERT_EQ(stream.get_metadata("key"), "value");
}

TEST(PDIFDiff, TestApplyMetaEditScriptAddInvalid) {
    pdif::diff diff;
    pdif::meta_edit_op op1(pdif::meta_edit_op_type::META_ADD, "key", "value");

    diff.add_meta_edit_op(op1);

    pdif::stream_meta stream;
    stream.add_metadata("key", "old_value");

    ASSERT_THROW({diff.apply_meta_edit_script(stream);}, pdif::pdif_invalid_operation);

    ASSERT_EQ(stream.get_metadata("key"), "old_value");
}

TEST(PDIFDiff, TestApplyMetaEditScriptDelete) {
    pdif::diff diff;
    pdif::meta_edit_op op1(pdif::meta_edit_op_type::META_DELETE, "key");

    diff.add_meta_edit_op(op1);

    pdif::stream_meta stream;
    stream.add_metadata("key", "value");

    ASSERT_NO_THROW({diff.apply_meta_edit_script(stream);});

    ASSERT_FALSE(stream.has_key("key"));
}

TEST(PDIFDiff, TestApplyMetaEditScriptDeleteInvalid) {
    pdif::diff diff;
    pdif::meta_edit_op op1(pdif::meta_edit_op_type::META_DELETE, "key");

    diff.add_meta_edit_op(op1);

    pdif::stream_meta stream;

    ASSERT_THROW({diff.apply_meta_edit_script(stream);}, pdif::pdif_invalid_operation);
}

TEST(PDIFDiff, TestApplyMetaEditScriptUpdate) {
    pdif::diff diff;
    pdif::meta_edit_op op1(pdif::meta_edit_op_type::META_UPDATE, "key", "value");

    diff.add_meta_edit_op(op1);

    pdif::stream_meta stream;
    stream.add_metadata("key", "old_value");

    ASSERT_NO_THROW({diff.apply_meta_edit_script(stream);});

    ASSERT_EQ(stream.get_metadata("key"), "value");
}

TEST(PDIFDiff, TestApplyMetaEditScriptUpdateInvalid) {
    pdif::diff diff;
    pdif::meta_edit_op op1(pdif::meta_edit_op_type::META_UPDATE, "key", "value");

    diff.add_meta_edit_op(op1);

    pdif::stream_meta stream;

    ASSERT_THROW({diff.apply_meta_edit_script(stream);}, pdif::pdif_invalid_operation);
}

TEST(PDIFDiff, TestApplyMetaEditScriptStreamCallback) {
    int i = 0;
    pdif::stream_meta::meta_callback_f callback = [&i](const pdif::meta_edit_op& op) {
        ASSERT_EQ(op.get_type(), pdif::meta_edit_op_type::META_ADD);
        ASSERT_EQ(op.get_meta_key(), "key");
        ASSERT_EQ(op.has_meta_val(), true);
        ASSERT_EQ(op.get_meta_val(), "value");
        i++;
    };

    pdif::meta_edit_op op(pdif::meta_edit_op_type::META_ADD, "key", "value");

    pdif::diff diff;
    diff.add_meta_edit_op(op);

    pdif::stream_meta stream;
    stream.set_meta_callback(callback);

    ASSERT_EQ(i, 0);
    ASSERT_NO_THROW({diff.apply_meta_edit_script(stream);});
    ASSERT_EQ(i, 1);

    ASSERT_EQ(stream.get_metadata("key"), "value");
}

TEST(PDIFDiff, TestApplyMetaEditScriptStreamCallbackErrorInCallback) {
    pdif::stream_meta::meta_callback_f callback = [](const pdif::meta_edit_op&) {
        throw std::exception();
    };

    pdif::meta_edit_op op(pdif::meta_edit_op_type::META_ADD, "key", "value");

    pdif::diff diff;
    diff.add_meta_edit_op(op);

    pdif::stream_meta stream;

    stream.set_meta_callback(callback);

    ASSERT_THROW({diff.apply_meta_edit_script(stream);}, pdif::pdif_error_in_callback);
}

TEST(PDIFDiff, TestCountEditOpTypes) {
    pdif::diff diff;
    pdif::edit_op op1(pdif::edit_op_type::INSERT, pdif::stream_elem::create<pdif::text_elem>("Inserted"));
    pdif::edit_op op2(pdif::edit_op_type::DELETE);
    pdif::edit_op op3(pdif::edit_op_type::EQ);

    diff.add_edit_op(op1);
    diff.add_edit_op(op2);
    diff.add_edit_op(op3);

    int insert_count = 0;
    int delete_count = 0;
    int eq_count = 0;

    ASSERT_NO_THROW({diff.count_edit_op_types(insert_count, delete_count, eq_count);});

    ASSERT_EQ(insert_count, 1);
    ASSERT_EQ(delete_count, 1);
    ASSERT_EQ(eq_count, 1);
}

TEST(PDIFDiff, TestCountMetaOpTypes) {
    pdif::diff diff;
    pdif::meta_edit_op op1(pdif::meta_edit_op_type::META_ADD, "key", "value");
    pdif::meta_edit_op op2(pdif::meta_edit_op_type::META_DELETE, "key");
    pdif::meta_edit_op op3(pdif::meta_edit_op_type::META_UPDATE, "key", "value");

    diff.add_meta_edit_op(op1);
    diff.add_meta_edit_op(op2);
    diff.add_meta_edit_op(op3);

    int add_count = 0;
    int delete_count = 0;
    int update_count = 0;

    ASSERT_NO_THROW({diff.count_meta_op_types(add_count, delete_count, update_count);});

    ASSERT_EQ(add_count, 1);
    ASSERT_EQ(delete_count, 1);
    ASSERT_EQ(update_count, 1);
}

TEST(PDIFDiff, TestCountEditOpTypesEmpty) {
    pdif::diff diff;

    int insert_count = 0;
    int delete_count = 0;
    int eq_count = 0;

    ASSERT_NO_THROW({diff.count_edit_op_types(insert_count, delete_count, eq_count);});

    ASSERT_EQ(insert_count, 0);
    ASSERT_EQ(delete_count, 0);
    ASSERT_EQ(eq_count, 0);
}

TEST(PDIFDiff, TestCountMetaOpTypesEmpty) {
    pdif::diff diff;

    int add_count = 0;
    int delete_count = 0;
    int update_count = 0;

    ASSERT_NO_THROW({diff.count_meta_op_types(add_count, delete_count, update_count);});

    ASSERT_EQ(add_count, 0);
    ASSERT_EQ(delete_count, 0);
    ASSERT_EQ(update_count, 0);
}

TEST(PDIFDiff, TestReverseEditOps) {
    pdif::diff diff;
    pdif::edit_op op1(pdif::edit_op_type::INSERT, pdif::stream_elem::create<pdif::text_elem>("Inserted"));
    pdif::edit_op op2(pdif::edit_op_type::DELETE);
    pdif::edit_op op3(pdif::edit_op_type::EQ);

    diff.add_edit_op(op1);
    diff.add_edit_op(op2);
    diff.add_edit_op(op3);

    ASSERT_NO_THROW({diff.reverse_edit_ops();});

    ASSERT_EQ(diff.edit_op_size(), 3);

    ASSERT_EQ(diff.get_edit_op(0).get_type(), pdif::edit_op_type::EQ);
    ASSERT_EQ(diff.get_edit_op(0).has_arg(), false);

    ASSERT_EQ(diff.get_edit_op(1).get_type(), pdif::edit_op_type::DELETE);
    ASSERT_EQ(diff.get_edit_op(1).has_arg(), false);

    ASSERT_EQ(diff.get_edit_op(2).get_type(), pdif::edit_op_type::INSERT);
    ASSERT_EQ(diff.get_edit_op(2).has_arg(), true);
    ASSERT_EQ(diff.get_edit_op(2).get_arg()->as<pdif::text_elem>()->text(), "Inserted");
}

TEST(PDIFDiff, TestReverseEditOpsEmpty) {
    pdif::diff diff;
    ASSERT_NO_THROW({diff.reverse_edit_ops();});
    ASSERT_EQ(diff.edit_op_size(), 0);
}

TEST(PDIFDiff, TestReverseEditOpsRange) {
    pdif::diff diff;
    pdif::edit_op op1(pdif::edit_op_type::INSERT, pdif::stream_elem::create<pdif::text_elem>("Inserted"));
    pdif::edit_op op2(pdif::edit_op_type::DELETE);
    pdif::edit_op op3(pdif::edit_op_type::EQ);
    pdif::edit_op op4(pdif::edit_op_type::EQ);
    pdif::edit_op op5(pdif::edit_op_type::EQ);
    pdif::edit_op op6(pdif::edit_op_type::DELETE);
    pdif::edit_op op7(pdif::edit_op_type::INSERT, pdif::stream_elem::create<pdif::text_elem>("Inserted 2"));

    diff.add_edit_op(op1);
    diff.add_edit_op(op2);
    diff.add_edit_op(op3);
    diff.add_edit_op(op4);
    diff.add_edit_op(op5);
    diff.add_edit_op(op6);
    diff.add_edit_op(op7);

    ASSERT_NO_THROW({diff.reverse_edit_ops(diff.begin() + 2, diff.begin() + 6);});

    ASSERT_EQ(diff.edit_op_size(), 7);

    ASSERT_EQ(diff.get_edit_op(0).get_type(), pdif::edit_op_type::INSERT);
    ASSERT_EQ(diff.get_edit_op(0).has_arg(), true);
    ASSERT_EQ(diff.get_edit_op(0).get_arg()->as<pdif::text_elem>()->text(), "Inserted");

    ASSERT_EQ(diff.get_edit_op(1).get_type(), pdif::edit_op_type::DELETE);
    ASSERT_EQ(diff.get_edit_op(1).has_arg(), false);

    ASSERT_EQ(diff.get_edit_op(2).get_type(), pdif::edit_op_type::DELETE);
    ASSERT_EQ(diff.get_edit_op(2).has_arg(), false);

    ASSERT_EQ(diff.get_edit_op(3).get_type(), pdif::edit_op_type::EQ);

    ASSERT_EQ(diff.get_edit_op(4).get_type(), pdif::edit_op_type::EQ);

    ASSERT_EQ(diff.get_edit_op(5).get_type(), pdif::edit_op_type::EQ);

    ASSERT_EQ(diff.get_edit_op(6).get_type(), pdif::edit_op_type::INSERT);
    ASSERT_EQ(diff.get_edit_op(6).has_arg(), true);
    ASSERT_EQ(diff.get_edit_op(6).get_arg()->as<pdif::text_elem>()->text(), "Inserted 2");
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}