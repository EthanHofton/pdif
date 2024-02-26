#include <gtest/gtest.h>
#include <pdif/stream_meta.hpp>
#include <pdif/meta_edit_op.hpp>

TEST(PDIFStreamMeta, TestAddMetadata) {
    pdif::stream_meta stream;
    
    ASSERT_NO_THROW({stream.add_metadata("key", "value");});

    ASSERT_EQ(stream.get_metadata("key"), "value");
    ASSERT_EQ(stream.has_key("key"), true);
}

TEST(PDIFStreamMeta, TestAddMetadataInvalid) {
    pdif::stream_meta stream;
    stream.add_metadata("key", "value");
    ASSERT_THROW({stream.add_metadata("key", "value");}, pdif::pdif_invalid_key);
}

TEST(PDIFStreamMeta, TestRemoveMetadata) {
    pdif::stream_meta stream;
    stream.add_metadata("key", "value");
    ASSERT_NO_THROW({stream.remove_metadata("key");});

    ASSERT_EQ(stream.has_key("key"), false);
}

TEST(PDIFStreamMeta, TestRemoveMetadataInvalid) {
    pdif::stream_meta stream;
    ASSERT_THROW({stream.remove_metadata("key");}, pdif::pdif_invalid_key);
}

TEST(PDIFStreamMeta, TestUpdateMetadata) {
    pdif::stream_meta stream;
    stream.add_metadata("key", "value");

    ASSERT_EQ(stream.get_metadata("key"), "value");

    ASSERT_NO_THROW({stream.update_metadata("key", "new_value");});

    ASSERT_EQ(stream.get_metadata("key"), "new_value");
}

TEST(PDIFStreamMeta, TestUpdateMetadataInvalid) {
    pdif::stream_meta stream;
    ASSERT_THROW({stream.update_metadata("key", "value");}, pdif::pdif_invalid_key);
}

TEST(PDIFStreamMeta, TestGetMetadata) {
    pdif::stream_meta stream;
    stream.add_metadata("key", "value");
    ASSERT_EQ(stream.get_metadata("key"), "value");
}

//
TEST(PDIFStreamMeta, TestGetMetadataInvalid) {
    pdif::stream_meta stream;
    ASSERT_THROW({stream.get_metadata("key");}, pdif::pdif_invalid_key);
}

TEST(PDIFStreamMeta, TestHasKey) {
    pdif::stream_meta stream;

    ASSERT_NO_THROW({stream.has_key("key");});
    ASSERT_EQ(stream.has_key("key"), false);

    stream.add_metadata("key", "value");

    ASSERT_NO_THROW({stream.has_key("key");});
    ASSERT_EQ(stream.has_key("key"), true);
}

TEST(PDIFStreamMeta, TestSetMetaCallback) {
    pdif::stream_meta stream;
    ASSERT_NO_THROW({stream.set_meta_callback([](const pdif::meta_edit_op&){});});
}

TEST(PDIFStreamMeta, TestHasMetaCallback) {
    pdif::stream_meta stream;
    ASSERT_EQ(stream.has_meta_callback(), false);
    stream.set_meta_callback([](const pdif::meta_edit_op&){});
    ASSERT_EQ(stream.has_meta_callback(), true);
}

TEST(PDIFStreamMeta, TestMetaCallback) {
    int i = 0;
    pdif::stream_meta::meta_callback_f callback = [&i](const pdif::meta_edit_op&){i++;};

    pdif::stream_meta stream;
    stream.set_meta_callback(callback);

    ASSERT_EQ(i, 0);

    ASSERT_NO_THROW({stream.meta_callback(pdif::meta_edit_op(pdif::meta_edit_op_type::META_DELETE, "key"));});

    ASSERT_EQ(i, 1);
}

TEST(PDIFStreamMeta, TestMetaCallbackInvalidNoCallback) {
    pdif::stream_meta stream;
    ASSERT_THROW({stream.meta_callback(pdif::meta_edit_op(pdif::meta_edit_op_type::META_DELETE, "key"));}, pdif::pdif_invalid_operation);
}

TEST(PDIFStreamMeta, TestMetaCallbackInvalidErrorInCallback) {
    pdif::stream_meta stream;
    pdif::stream_meta::meta_callback_f callback = [](const pdif::meta_edit_op&){ throw std::exception(); };

    stream.set_meta_callback(callback);

    ASSERT_THROW({stream.meta_callback(pdif::meta_edit_op(pdif::meta_edit_op_type::META_DELETE, "key"));}, pdif::pdif_error_in_callback);
}

TEST(PDIFStreamMeta, TestGetMetadataDict) {
    pdif::stream_meta stream;
    stream.add_metadata("key1", "value1");
    stream.add_metadata("key2", "value2");

    auto dict = stream.get_metadata();

    ASSERT_EQ(dict.size(), 2);
    ASSERT_EQ(dict.at("key1"), "value1");
    ASSERT_EQ(dict.at("key2"), "value2");
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}