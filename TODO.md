### TODO

#### meta edit command
- [x] add meta edit command to pdif::edit_op_type
- [x] add meta edit command to pdif::edit_op
- [x] write tests for meta edit command
- [ ] add types of meta edit command (META_ADD, META_DELETE, META_UPDATE)
- [ ] updated unit tests for meta edit command

#### meta stream

There is no need to create a new stream elem for metadata since they will be kept separate from the stream data as to not pollute the stream. The metadata will be stored in a dict in the stream object.  

- [ ] create a meta dict in pdif::stream
- [ ] accessors for meta stream in pdif::stream
- [ ] unit tests for meta stream in pdif::stream

#### binary_elem

- [ ] rework binary_elem class to use util::bitstream
- [ ] use util::bitstream comparitor for stream_elem::compare
- [ ] util::binary_elem::to_string for serialization
- [ ] util::binary_elem::from_string for deserialization
- [ ] rewrite unit tests for binary_elem

#### stream_differ_base

The stream_differ_base should contain the functionallity to generate a pdif::diff for 2 streams metadata. The stream_differ_base should have an abstract function diff that will be implamented by the stream_differ subclasses.

- [ ] write meta stream differ + unit tests

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

- [ ] Allow for callback functions to be passed to diff::apply to allow for custom behavior. This will be useful when applying the diff to a PDF stream, the callback can be used to apply the changes from the stream to the PDF object.
- [ ] Add unit tests for setting of callback function
- [ ] update unit tests for diff::apply to use callback function
- [ ] fix diff::to_json. (maybe use a json library like nlohmann::json)
- [ ] fix diff::from_json. (maybe use a json library like nlohmann::json)
**!important!**
- [ ] diff apply meta commands should not increase the index of the stream