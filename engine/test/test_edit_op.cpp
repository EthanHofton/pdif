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

    pdif::edit_op op(pdif::edit_op_type::INSERT, pdif::stream_elem::create<pdif::text_elem>("Inserted"));

    ASSERT_NO_THROW({op.execute(s, index);});

    ASSERT_EQ(s.size(), 4);

    ASSERT_EQ(s[0]->type(), pdif::stream_type::text);
    ASSERT_EQ(s[0]->as<pdif::text_elem>()->text(), "Inserted");

    ASSERT_EQ(s[1]->type(), pdif::stream_type::text);
    ASSERT_EQ(s[1]->as<pdif::text_elem>()->text(), "Hello,");

    ASSERT_EQ(s[2]->type(), pdif::stream_type::text);
    ASSERT_EQ(s[2]->as<pdif::text_elem>()->text(), " World");

    ASSERT_EQ(s[3]->type(), pdif::stream_type::text);
    ASSERT_EQ(s[3]->as<pdif::text_elem>()->text(), "!");

    ASSERT_EQ(index, 1);

    // try insert at a different position
    index = 2;

    op = pdif::edit_op(pdif::edit_op_type::INSERT, pdif::stream_elem::create<pdif::text_elem>("Inserted 2"));

    ASSERT_NO_THROW({op.execute(s, index);});

    ASSERT_EQ(s.size(), 5);

    ASSERT_EQ(s[0]->type(), pdif::stream_type::text);
    ASSERT_EQ(s[0]->as<pdif::text_elem>()->text(), "Inserted");

    ASSERT_EQ(s[1]->type(), pdif::stream_type::text);
    ASSERT_EQ(s[1]->as<pdif::text_elem>()->text(), "Hello,");

    ASSERT_EQ(s[2]->type(), pdif::stream_type::text);
    ASSERT_EQ(s[2]->as<pdif::text_elem>()->text(), "Inserted 2");

    ASSERT_EQ(s[3]->type(), pdif::stream_type::text);
    ASSERT_EQ(s[3]->as<pdif::text_elem>()->text(), " World");

    ASSERT_EQ(s[4]->type(), pdif::stream_type::text);
    ASSERT_EQ(s[4]->as<pdif::text_elem>()->text(), "!");

    ASSERT_EQ(index, 3);
}

TEST(PDIFEditOp, TestExecuteInsertBack) {
    pdif::stream s;

    s.push_back(pdif::stream_elem::create<pdif::text_elem>("Hello,"));
    s.push_back(pdif::stream_elem::create<pdif::text_elem>(" World"));
    s.push_back(pdif::stream_elem::create<pdif::text_elem>("!"));

    size_t index = 3;

    pdif::edit_op op(pdif::edit_op_type::INSERT, pdif::stream_elem::create<pdif::text_elem>("Inserted"));
    
    ASSERT_NO_THROW({op.execute(s, index);});

    ASSERT_EQ(s.size(), 4);

    ASSERT_EQ(s[0]->type(), pdif::stream_type::text);
    ASSERT_EQ(s[0]->as<pdif::text_elem>()->text(), "Hello,");

    ASSERT_EQ(s[1]->type(), pdif::stream_type::text);
    ASSERT_EQ(s[1]->as<pdif::text_elem>()->text(), " World");

    ASSERT_EQ(s[2]->type(), pdif::stream_type::text);
    ASSERT_EQ(s[2]->as<pdif::text_elem>()->text(), "!");

    ASSERT_EQ(s[3]->type(), pdif::stream_type::text);
    ASSERT_EQ(s[3]->as<pdif::text_elem>()->text(), "Inserted");

    ASSERT_EQ(index, 4);
}

TEST(PDIFEditOp, TestExecuteInsertInvalid) {
    pdif::stream s;

    s.push_back(pdif::stream_elem::create<pdif::text_elem>("Hello,"));
    s.push_back(pdif::stream_elem::create<pdif::text_elem>(" World"));
    s.push_back(pdif::stream_elem::create<pdif::text_elem>("!"));

    size_t index = 4;

    pdif::edit_op op(pdif::edit_op_type::INSERT, pdif::stream_elem::create<pdif::text_elem>("Inserted"));
    
    ASSERT_THROW({op.execute(s, index);}, pdif::pdif_out_of_bounds);

    ASSERT_EQ(s.size(), 3);
}

TEST(PDIFEditOp, TestExecuteDelete) {
    pdif::stream s;

    s.push_back(pdif::stream_elem::create<pdif::text_elem>("Hello,"));
    s.push_back(pdif::stream_elem::create<pdif::text_elem>(" World"));
    s.push_back(pdif::stream_elem::create<pdif::text_elem>("!"));

    size_t index = 1;

    pdif::edit_op op(pdif::edit_op_type::DELETE);

    ASSERT_NO_THROW({op.execute(s, index);});

    ASSERT_EQ(s.size(), 2);

    ASSERT_EQ(s[0]->type(), pdif::stream_type::text);
    ASSERT_EQ(s[0]->as<pdif::text_elem>()->text(), "Hello,");

    ASSERT_EQ(s[1]->type(), pdif::stream_type::text);
    ASSERT_EQ(s[1]->as<pdif::text_elem>()->text(), "!");

    ASSERT_EQ(index, 1);
}

TEST(PDIFEditOp, TestExecuteDeleteFront) {
    pdif::stream s;

    s.push_back(pdif::stream_elem::create<pdif::text_elem>("Hello,"));
    s.push_back(pdif::stream_elem::create<pdif::text_elem>(" World"));
    s.push_back(pdif::stream_elem::create<pdif::text_elem>("!"));

    size_t index = 0;

    pdif::edit_op op(pdif::edit_op_type::DELETE);

    ASSERT_NO_THROW({op.execute(s, index);});

    ASSERT_EQ(s.size(), 2);

    ASSERT_EQ(s[0]->type(), pdif::stream_type::text);
    ASSERT_EQ(s[0]->as<pdif::text_elem>()->text(), " World");

    ASSERT_EQ(s[1]->type(), pdif::stream_type::text);
    ASSERT_EQ(s[1]->as<pdif::text_elem>()->text(), "!");

    ASSERT_EQ(index, 0);
}

TEST(PDIFEditOp, TestExecuteDeleteBack) {
    pdif::stream s;

    s.push_back(pdif::stream_elem::create<pdif::text_elem>("Hello,"));
    s.push_back(pdif::stream_elem::create<pdif::text_elem>(" World"));
    s.push_back(pdif::stream_elem::create<pdif::text_elem>("!"));

    size_t index = 2;

    pdif::edit_op op(pdif::edit_op_type::DELETE);

    ASSERT_NO_THROW({op.execute(s, index);});

    ASSERT_EQ(s.size(), 2);

    ASSERT_EQ(s[0]->type(), pdif::stream_type::text);
    ASSERT_EQ(s[0]->as<pdif::text_elem>()->text(), "Hello,");

    ASSERT_EQ(s[1]->type(), pdif::stream_type::text);
    ASSERT_EQ(s[1]->as<pdif::text_elem>()->text(), " World");

    ASSERT_EQ(index, 2);

}

TEST(PDIFEditOp, TestExecuteDeleteInvalid) {
    pdif::stream s;

    s.push_back(pdif::stream_elem::create<pdif::text_elem>("Hello,"));
    s.push_back(pdif::stream_elem::create<pdif::text_elem>(" World"));
    s.push_back(pdif::stream_elem::create<pdif::text_elem>("!"));

    size_t index = 3;

    pdif::edit_op op(pdif::edit_op_type::DELETE);

    ASSERT_THROW({op.execute(s, index);}, pdif::pdif_out_of_bounds);

    ASSERT_EQ(s.size(), 3);
}

TEST(PDIFEditOp, TestExecuteEq) {
    pdif::stream s;

    s.push_back(pdif::stream_elem::create<pdif::text_elem>("Hello,"));
    s.push_back(pdif::stream_elem::create<pdif::text_elem>(" World"));
    s.push_back(pdif::stream_elem::create<pdif::text_elem>("!"));

    size_t index = 0;

    pdif::edit_op op(pdif::edit_op_type::EQ);

    ASSERT_NO_THROW({op.execute(s, index);});

    ASSERT_EQ(s.size(), 3);

    ASSERT_EQ(s[0]->type(), pdif::stream_type::text);
    ASSERT_EQ(s[0]->as<pdif::text_elem>()->text(), "Hello,");

    ASSERT_EQ(s[1]->type(), pdif::stream_type::text);
    ASSERT_EQ(s[1]->as<pdif::text_elem>()->text(), " World");

    ASSERT_EQ(s[2]->type(), pdif::stream_type::text);
    ASSERT_EQ(s[2]->as<pdif::text_elem>()->text(), "!");

    ASSERT_EQ(index, 1);
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}