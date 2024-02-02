#include <gtest/gtest.h>
#include <pdif/stream.hpp>

TEST(PDIFStream, TestIndex) {
    pdif::stream stream;
    stream.push_back(pdif::stream_elem::create<pdif::text_elem>("Hello,"));
    stream.push_back(pdif::stream_elem::create<pdif::text_elem>(" World"));
    stream.push_back(pdif::stream_elem::create<pdif::text_elem>("!"));

    ASSERT_EQ(stream[0]->type(), pdif::stream_type::text);
    ASSERT_EQ(stream[0]->as<pdif::text_elem>()->text(), "Hello,");

    ASSERT_EQ(stream[1]->type(), pdif::stream_type::text);
    ASSERT_EQ(stream[1]->as<pdif::text_elem>()->text(), " World");

    ASSERT_EQ(stream[2]->type(), pdif::stream_type::text);
    ASSERT_EQ(stream[2]->as<pdif::text_elem>()->text(), "!");
}

TEST(PDIFStream, TestIndexInvalid) {
    pdif::stream stream;
    ASSERT_THROW({stream[0];}, pdif::pdif_out_of_bounds);
}

TEST(PDIFStream, TestPushBack) {
    pdif::stream stream;
    stream.push_back(pdif::stream_elem::create<pdif::text_elem>("Hello,"));
    ASSERT_EQ(stream[0]->type(), pdif::stream_type::text);
    ASSERT_EQ(stream[0]->as<pdif::text_elem>()->text(), "Hello,");
}

TEST(PDIFStream, TestPushFront) {
    pdif::stream stream;
    stream.push_front(pdif::stream_elem::create<pdif::text_elem>("Hello,"));
    ASSERT_EQ(stream[0]->type(), pdif::stream_type::text);
    ASSERT_EQ(stream[0]->as<pdif::text_elem>()->text(), "Hello,");
}

TEST(PDIFStream, TestPush) {
    pdif::stream stream;
    stream.push_back(pdif::stream_elem::create<pdif::text_elem>("Hello,"));
    stream.push_back(pdif::stream_elem::create<pdif::text_elem>("!"));
    stream.push(1, pdif::stream_elem::create<pdif::text_elem>("World"));
    ASSERT_EQ(stream[0]->type(), pdif::stream_type::text);
    ASSERT_EQ(stream[0]->as<pdif::text_elem>()->text(), "Hello,");
    ASSERT_EQ(stream[1]->type(), pdif::stream_type::text);
    ASSERT_EQ(stream[1]->as<pdif::text_elem>()->text(), "World");
    ASSERT_EQ(stream[2]->type(), pdif::stream_type::text);
    ASSERT_EQ(stream[2]->as<pdif::text_elem>()->text(), "!");
}

TEST(PDIFSteam, TestPushToBack) {
    pdif::stream stream;
    stream.push_back(pdif::stream_elem::create<pdif::text_elem>("Hello,"));
    stream.push_back(pdif::stream_elem::create<pdif::text_elem>("!"));
    ASSERT_NO_THROW({stream.push(2, pdif::stream_elem::create<pdif::text_elem>("World"));});

    ASSERT_EQ(stream[0]->type(), pdif::stream_type::text);
    ASSERT_EQ(stream[0]->as<pdif::text_elem>()->text(), "Hello,");

    ASSERT_EQ(stream[1]->type(), pdif::stream_type::text);
    ASSERT_EQ(stream[1]->as<pdif::text_elem>()->text(), "!");

    ASSERT_EQ(stream[2]->type(), pdif::stream_type::text);
    ASSERT_EQ(stream[2]->as<pdif::text_elem>()->text(), "World");

    ASSERT_EQ(stream.size(), 3);
}

TEST(PDIFStream, TestPushInvalid) {
    pdif::stream stream;
    ASSERT_THROW({stream.push(1, pdif::stream_elem::create<pdif::text_elem>("Hello,"));}, pdif::pdif_out_of_bounds);
}

TEST(PDIFStream, TestPopBack) {
    pdif::stream stream;
    stream.push_back(pdif::stream_elem::create<pdif::text_elem>("Hello,"));
    stream.pop_back();
    ASSERT_TRUE(stream.empty());
}

TEST(PDIFStream, TestPopBackInvalid) {
    pdif::stream stream;
    ASSERT_THROW({stream.pop_back();}, pdif::pdif_out_of_bounds);
}

TEST(PDIFStream, TestPopFront) {
    pdif::stream stream;
    stream.push_back(pdif::stream_elem::create<pdif::text_elem>("Hello,"));
    stream.pop_front();
    ASSERT_TRUE(stream.empty());
}

TEST(PDIFStream, TestPopFrontInvalid) {
    pdif::stream stream;
    ASSERT_THROW({stream.pop_front();}, pdif::pdif_out_of_bounds);
}

TEST(PDIFStream, TestPop) {
    pdif::stream stream;
    stream.push_back(pdif::stream_elem::create<pdif::text_elem>("Hello,"));
    stream.push_back(pdif::stream_elem::create<pdif::text_elem>("!"));
    stream.push(1, pdif::stream_elem::create<pdif::text_elem>("World"));
    stream.pop(1);
    ASSERT_EQ(stream[0]->type(), pdif::stream_type::text);
    ASSERT_EQ(stream[0]->as<pdif::text_elem>()->text(), "Hello,");
    ASSERT_EQ(stream[1]->type(), pdif::stream_type::text);
    ASSERT_EQ(stream[1]->as<pdif::text_elem>()->text(), "!");
}

TEST(PDIFStream, TestPopInvalid) {
    pdif::stream stream;
    ASSERT_THROW({stream.pop(1);}, pdif::pdif_out_of_bounds);
}

TEST(PDIFStream, TestSize) {
    pdif::stream stream;
    ASSERT_EQ(stream.size(), 0);
    stream.push_back(pdif::stream_elem::create<pdif::text_elem>("Hello,"));
    ASSERT_EQ(stream.size(), 1);
    stream.push_back(pdif::stream_elem::create<pdif::text_elem>("!"));
    ASSERT_EQ(stream.size(), 2);
    stream.push(1, pdif::stream_elem::create<pdif::text_elem>("World"));
    ASSERT_EQ(stream.size(), 3);
    stream.pop(1);
    ASSERT_EQ(stream.size(), 2);
    stream.pop_front();
    ASSERT_EQ(stream.size(), 1);
    stream.pop_back();
    ASSERT_EQ(stream.size(), 0);
}

TEST(PDIFStream, TestEmpty) {
    pdif::stream stream;
    ASSERT_TRUE(stream.empty());
    stream.push_back(pdif::stream_elem::create<pdif::text_elem>("Hello,"));
    ASSERT_FALSE(stream.empty());
    stream.pop_back();
    ASSERT_TRUE(stream.empty());
}

TEST(PDIFStream, TestClear) {
    pdif::stream stream;
    stream.push_back(pdif::stream_elem::create<pdif::text_elem>("Hello,"));
    stream.push_back(pdif::stream_elem::create<pdif::text_elem>("!"));
    stream.push(1, pdif::stream_elem::create<pdif::text_elem>("World"));
    stream.clear();
    ASSERT_TRUE(stream.empty());
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}