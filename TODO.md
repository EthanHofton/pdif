### TODO

#### code base

- [x] add README.md
- [ ] add LICENSE
- [x] add CONTRIBUTING.md
- [x] add -wall -werror to CMakeLists.txt
- [x] fix all warnings
- [ ] add doxygen documentation
- [ ] add doxygen
 

### TESTING:

- [ ] LCS_stream_differ::case - test different stream_elems types
- [ ] stream elem - text/stroke color_set, xobject, font
- [ ] test pdf_content_stream_filter
- [ ] test pdf::compare. Use `test_pdfs/` as test files
- [ ] test content_extractor


### Features:

- [ ] MAYBE: add support for image resizing (think on how to do nicely)
- [ ] implement cli options
- [ ] annotation diffs
- [ ] form diffs

### Cleanup:

- Remove dormant apply code from cli.


## Scoping

Does layout need to be included? The focus of the project is on the content of the PDF, not the layout.

- Cover most content types: text, images (xobject), fonts, colors, text, annotations, forms, metadata