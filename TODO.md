### TODO

#### code base

- [ ] add README.md
- [ ] add LICENSE
- [ ] add CONTRIBUTING.md
- [x] add -wall -werror to CMakeLists.txt
- [x] fix all warnings
- [ ] add doxygen documentation

#### meta edit command
- [x] add meta edit command to pdif::edit_op_type
- [x] add meta edit command to pdif::edit_op
- [x] write tests for meta edit command
- [x] add types of meta edit command (META_ADD, META_DELETE, META_UPDATE)
- [x] updated unit tests for meta edit command

#### meta stream

There is no need to create a new stream elem for metadata since they will be kept separate from the stream data as to not pollute the stream. The metadata will be stored in a dict in the stream object.  

- [x] create a meta dict in pdif::stream
- [x] accessors for meta stream in pdif::stream
- [x] add ability to set callback function
- [x] Add unit tests for setting of callback function
- [x] unit tests for meta stream in pdif::stream

#### binary_elem

- [ ] rework binary_elem class to use util::bitstream
- [ ] use util::bitstream comparitor for stream_elem::compare
- [ ] util::binary_elem::to_string for serialization
- [ ] util::binary_elem::from_string for deserialization
- [ ] rewrite unit tests for binary_elem

#### stream_differ_base

The stream_differ_base should contain the functionallity to generate a pdif::diff for 2 streams metadata. The stream_differ_base should have an abstract function diff that will be implamented by the stream_differ subclasses.

- [x] write meta stream differ + unit tests

#### lcs_stream_differ

The lcs stream differ should use the modified LCS algorithm to generate a pdif::diff object. Use rstream_elem::compare to compare the stream elements.

- [ ] implament stream_differ_base::diff for lcs_stream_differ
- [ ] write unit tests for lcs_stream_differ

#### myers_stream_differ

The myers stream differ should use the myers diff algorithm to generate a pdif::diff object. Use rstream_elem::compare to compare the stream elements.

- [ ] implament stream_differ_base::diff for myers_stream_differ
- [ ] write unit tests for myers_stream_differ

#### diff

When applying a diff to a stream, when each edit operation (EQ, DELETE, INSERT, META) is applied, the callback function will be called with the edit operation that was just executed along with the index within the stream. Use `std::function<void(edit_op, size_t)>` for the callback function. The callback function will be set using diff::set_callback and will be called from diff::apply. Use: `using callback_t = std::function<void(edit_op, size_t)>;`

- [ ] fix diff::to_json. (maybe use a json library like nlohmann::json)
- [ ] fix diff::from_json. (maybe use a json library like nlohmann::json)
**!important!**
- [x] diff apply meta commands should not increase the index of the stream


#### misc

- [ ] nlohmann::json very slow on download. Look into speeding this up (maybe use fetch content with zip instead of repo or use a different json library)
- [ ] test diff::from_json and diff::to_json