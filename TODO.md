### TODO

### HIGH PRIORITY:

- [ ] implement testing section
- [x] BUG FIX: add to_unicode to font elem, if the font has one. Use the to_unicode map to convert the raw stream value to unicode
- [ ] BUG FIX: Fix ligature issue. When doing text extraction, liguatures are not being handeled correctly, causing fi, ff ect to be some wird escape sequence. Potentally use unicode normalization to fix this issue. OR if this fails, research a unicode map for ligatures and use that to fix the issue.


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
- Note on above: QPDF has a function to get the current graphic matrix for an object, could use this function to get the graphic matrix for the image and extract the scaling factor and skew.
- [x] implement cli options. 
- [ ] annotation diffs
- [ ] form diffs

### Cleanup:

- [x] Remove dormant apply code from cli.

## Scoping

Does layout need to be included? The focus of the project is on the content of the PDF, not the layout.

- Cover most content types: text, images (xobject), fonts, colors, text, annotations, forms, metadata


### Font extraction, unicode mapping:

Currently, the font extraction works but will not work for all kinds.

In order to make the code more robust, we need to check the mapping range. For example, a ToUnicode map can map charicter from 0x0000 - 0x00FF. This means that for every 4 bytes, we have 1 unicode charicter. For other fonts, such as CMR10, the post script font definition is over 0-256 bits (2 bytes). This means for every 2 bytes, we have 1 unicode charicter.