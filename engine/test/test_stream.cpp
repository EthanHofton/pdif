#include <gtest/gtest.h>
#include <pdif/stream.hpp>

TEST(PDIFStreamIterator, TestDereference) {
    pdif::stream stream;
    stream.push_back(pdif::stream_elem::create<pdif::text_elem>("Hello,"));
    stream.push_back(pdif::stream_elem::create<pdif::text_elem>(" World"));
    stream.push_back(pdif::stream_elem::create<pdif::text_elem>("!"));

    auto it = stream.begin();
    
    pdif::rstream_elem elem = *it;
    ASSERT_EQ(elem->type(), pdif::stream_type::text);
    ASSERT_EQ(elem->as<pdif::text_elem>()->text(), "Hello,");
}

TEST(PDIFStreamIterator, TestArrow) {
    pdif::stream stream;
    stream.push_back(pdif::stream_elem::create<pdif::text_elem>("Hello,"));
    stream.push_back(pdif::stream_elem::create<pdif::text_elem>(" World"));
    stream.push_back(pdif::stream_elem::create<pdif::text_elem>("!"));

    auto it = stream.begin();
    
    ASSERT_EQ(it->type(), pdif::stream_type::text);
    ASSERT_EQ(it->as<pdif::text_elem>()->text(), "Hello,");
}

TEST(PDIFStreamIterator, TestIncrement) {
    pdif::stream stream;
    stream.push_back(pdif::stream_elem::create<pdif::text_elem>("Hello,"));
    stream.push_back(pdif::stream_elem::create<pdif::text_elem>(" World"));
    stream.push_back(pdif::stream_elem::create<pdif::text_elem>("!"));

    auto it = stream.begin();
    
    ASSERT_EQ(it->type(), pdif::stream_type::text);
    ASSERT_EQ(it->as<pdif::text_elem>()->text(), "Hello,");

    ++it;
    ASSERT_EQ(it->type(), pdif::stream_type::text);
    ASSERT_EQ(it->as<pdif::text_elem>()->text(), " World");

    ++it;
    ASSERT_EQ(it->type(), pdif::stream_type::text);
    ASSERT_EQ(it->as<pdif::text_elem>()->text(), "!");
}

TEST(PDIFStreamIterator, TestDecrement) {
    pdif::stream stream;
    stream.push_back(pdif::stream_elem::create<pdif::text_elem>("Hello,"));
    stream.push_back(pdif::stream_elem::create<pdif::text_elem>(" World"));
    stream.push_back(pdif::stream_elem::create<pdif::text_elem>("!"));

    auto it = stream.end();
    
    ASSERT_EQ(it->type(), pdif::stream_type::text);
    ASSERT_EQ(it->as<pdif::text_elem>()->text(), "!");

    --it;
    ASSERT_EQ(it->type(), pdif::stream_type::text);
    ASSERT_EQ(it->as<pdif::text_elem>()->text(), " World");

    --it;
    ASSERT_EQ(it->type(), pdif::stream_type::text);
    ASSERT_EQ(it->as<pdif::text_elem>()->text(), "Hello,");
}

TEST(PDIFStreamIterator, TestComparison) {
    pdif::stream stream;
    stream.push_back(pdif::stream_elem::create<pdif::text_elem>("Hello,"));
    stream.push_back(pdif::stream_elem::create<pdif::text_elem>(" World"));
    stream.push_back(pdif::stream_elem::create<pdif::text_elem>("!"));

    auto it1 = stream.begin();
    auto it2 = stream.begin();
    auto it3 = stream.end();
    
    ASSERT_EQ(it1, it2);
    ASSERT_NE(it1, it3);
}

TEST(PDIFStreamIterator, TestNotEquals) {
    pdif::stream stream;
    stream.push_back(pdif::stream_elem::create<pdif::text_elem>("Hello,"));
    stream.push_back(pdif::stream_elem::create<pdif::text_elem>(" World"));
    stream.push_back(pdif::stream_elem::create<pdif::text_elem>("!"));

    auto it1 = stream.begin();
    auto it2 = stream.begin();
    auto it3 = stream.end();
    
    ASSERT_FALSE(it1 != it2);
    ASSERT_TRUE(it1 != it3);
}

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

TEST(PDIFStream, TestBegin) {
    // test interface, check does not throw pdif_out_of_bounds
    pdif::stream stream;
    stream.push_back(pdif::stream_elem::create<pdif::text_elem>("Hello,"));
    stream.push_back(pdif::stream_elem::create<pdif::text_elem>(" World"));
    stream.push_back(pdif::stream_elem::create<pdif::text_elem>("!"));

    ASSERT_NO_THROW({stream.begin();});

    auto it = stream.begin();

    ASSERT_EQ(it->type(), pdif::stream_type::text);
    ASSERT_EQ(it->as<pdif::text_elem>()->text(), "Hello,");
}

TEST(PDIFStream, TestBeginInvalid) {
    pdif::stream stream;
    ASSERT_THROW({stream.begin();}, pdif::pdif_out_of_bounds);
}

TEST(PDIFStream, TestEnd) {
    // test interface, check does not throw pdif_out_of_bounds
    pdif::stream stream;
    stream.push_back(pdif::stream_elem::create<pdif::text_elem>("Hello,"));
    stream.push_back(pdif::stream_elem::create<pdif::text_elem>(" World"));
    stream.push_back(pdif::stream_elem::create<pdif::text_elem>("!"));

    ASSERT_NO_THROW({stream.end();});

    auto it = stream.end();

    ASSERT_EQ(it->type(), pdif::stream_type::text);
    ASSERT_EQ(it->as<pdif::text_elem>()->text(), "!");
}

TEST(PDIFStream, TestEndInvalid) {
    pdif::stream stream;
    ASSERT_THROW({stream.end();}, pdif::pdif_out_of_bounds);
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}