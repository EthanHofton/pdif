#include <gtest/gtest.h>
#include <pdif/diff.hpp>
#include <pdif/edit_op.hpp>
#include <pdif/stream.hpp>
#include <pdif/stream_elem.hpp>

TEST(PDIFDiff, TestAddEditOp) {
    pdif::diff diff;
    pdif::edit_op op2(pdif::edit_op_type::DELETE);
    pdif::edit_op op(pdif::edit_op_type::INSERT, pdif::stream_elem::create<pdif::text_elem>("test"));

    ASSERT_NO_THROW({diff.add_edit_op(op);});
    ASSERT_NO_THROW({diff.add_edit_op(op2);});

    ASSERT_EQ(diff.size(), 2);

    ASSERT_EQ(diff[0].get_type(), pdif::edit_op_type::INSERT);
    ASSERT_EQ(diff[0].get_arg()->type(), pdif::stream_type::text);
    ASSERT_EQ(diff[0].get_arg()->as<pdif::text_elem>()->text(), "test");

    ASSERT_EQ(diff[1].get_type(), pdif::edit_op_type::DELETE);
}

TEST(PDIFDiff, TestBracketOperator) {
    pdif::diff diff;
    pdif::edit_op op2(pdif::edit_op_type::DELETE);
    pdif::edit_op op(pdif::edit_op_type::INSERT, pdif::stream_elem::create<pdif::text_elem>("test"));
    diff.add_edit_op(op);
    diff.add_edit_op(op2);

    ASSERT_NO_THROW({diff[0];});
    ASSERT_EQ(diff[0].get_type(), pdif::edit_op_type::INSERT);
    ASSERT_EQ(diff[0].get_arg()->type(), pdif::stream_type::text);
    ASSERT_EQ(diff[0].get_arg()->as<pdif::text_elem>()->text(), "test");

    ASSERT_NO_THROW({diff[1];});
    ASSERT_EQ(diff[1].get_type(), pdif::edit_op_type::DELETE);
    ASSERT_FALSE(diff[1].has_arg());
}

TEST(PDIFDiff, TestBracketOperatorInvalid) {
    pdif::diff diff;
    pdif::edit_op op2(pdif::edit_op_type::DELETE);
    pdif::edit_op op(pdif::edit_op_type::INSERT, pdif::stream_elem::create<pdif::text_elem>("test"));
    diff.add_edit_op(op);
    diff.add_edit_op(op2);

    ASSERT_THROW({diff[2];}, pdif::pdif_out_of_bounds);
}

TEST(PDIFDiff, TestBracketOperatorFront) {
    pdif::diff diff;
    pdif::edit_op op2(pdif::edit_op_type::DELETE);
    pdif::edit_op op(pdif::edit_op_type::INSERT, pdif::stream_elem::create<pdif::text_elem>("test"));
    diff.add_edit_op(op);
    diff.add_edit_op(op2);

    ASSERT_NO_THROW({diff[0];});
    ASSERT_EQ(diff[0].get_type(), pdif::edit_op_type::INSERT);
    ASSERT_EQ(diff[0].get_arg()->type(), pdif::stream_type::text);
    ASSERT_EQ(diff[0].get_arg()->as<pdif::text_elem>()->text(), "test");
}

TEST(PDIFDiff, TestBracketOperatorBack) {
    pdif::diff diff;
    pdif::edit_op op2(pdif::edit_op_type::DELETE);
    pdif::edit_op op(pdif::edit_op_type::INSERT, pdif::stream_elem::create<pdif::text_elem>("test"));
    diff.add_edit_op(op);
    diff.add_edit_op(op2);

    ASSERT_NO_THROW({diff[1];});
    ASSERT_EQ(diff[1].get_type(), pdif::edit_op_type::DELETE);
    ASSERT_FALSE(diff[1].has_arg());
}

TEST(PDIFDiff, TestSize) {
    pdif::diff diff;
    pdif::edit_op op2(pdif::edit_op_type::DELETE);
    pdif::edit_op op(pdif::edit_op_type::INSERT, pdif::stream_elem::create<pdif::text_elem>("test"));

    ASSERT_EQ(diff.size(), 0);

    diff.add_edit_op(op);

    ASSERT_EQ(diff.size(), 1);

    diff.add_edit_op(op2);

    ASSERT_EQ(diff.size(), 2);
}

TEST(PDIFDiff, TestToJson) {
    pdif::diff diff;
    pdif::edit_op op(pdif::edit_op_type::INSERT, pdif::stream_elem::create<pdif::text_elem>("test"));
    pdif::edit_op op2(pdif::edit_op_type::DELETE);
    diff.add_edit_op(op);
    diff.add_edit_op(op2);

std::string json = "{\n    \"edit_script\": [\n        {            \"type\": \"INSERT\",\n            \"arg\": {                \"type\": \"text\",\n                \"val\": \"test\"\n            }\n        },\n        {            \"type\": \"DELETE\",\n        }\n    ]\n}";

    ASSERT_EQ(diff.to_json(), json);
}

TEST(PDIFDiff, TestFromJson) {
    pdif::diff diff;
    pdif::edit_op op(pdif::edit_op_type::INSERT, pdif::stream_elem::create<pdif::text_elem>("test"));
    pdif::edit_op op2(pdif::edit_op_type::DELETE);
    diff.add_edit_op(op);
    diff.add_edit_op(op2);

std::string json = "{\n    \"edit_script\": [\n        {            \"type\": \"INSERT\",\n            \"arg\": {                \"type\": \"text\",\n                \"val\": \"test\"\n            }\n        },\n        {            \"type\": \"DELETE\",\n        }\n    ]\n}";

    pdif::diff diff2;
    ASSERT_NO_THROW({diff2.from_json(json);});
    
    ASSERT_EQ(diff2.size(), 2);

    ASSERT_TRUE(diff2[0].has_arg());
    ASSERT_EQ(diff2[0].get_type(), pdif::edit_op_type::INSERT);
    ASSERT_EQ(diff2[0].get_arg()->type(), pdif::stream_type::text);
    ASSERT_EQ(diff2[0].get_arg()->as<pdif::text_elem>()->text(), "test");

    ASSERT_FALSE(diff2[1].has_arg());
    ASSERT_EQ(diff2[1].get_type(), pdif::edit_op_type::DELETE); 
}

TEST(PDIFDiff, TestApply) {
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

    ASSERT_NO_THROW({diff.apply(stream);});

    ASSERT_EQ(stream.size(), 3);

    ASSERT_EQ(stream[0]->type(), pdif::stream_type::text);
    ASSERT_EQ(stream[0]->as<pdif::text_elem>()->text(), "test2");

    ASSERT_EQ(stream[1]->type(), pdif::stream_type::text);
    ASSERT_EQ(stream[1]->as<pdif::text_elem>()->text(), "Inserted");

    ASSERT_EQ(stream[2]->type(), pdif::stream_type::text);
    ASSERT_EQ(stream[2]->as<pdif::text_elem>()->text(), "test3");
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}