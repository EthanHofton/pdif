find_package(Doxygen)
if (DOXYGEN_FOUND)
    message(STATUS "Doxygen Found")

    set(DOXYGEN_IN ${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in)
    set(DOXYGEN_OUT ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile)

    configure_file(${DOXYGEN_IN} ${DOXYGEN_OUT} @ONLY)
    message(STATUS "Doxygen doxyfile configured")

    add_custom_target(
        pdif_engine_docs ALL
        COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_OUT}
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
        COMMENT "Generating API documentation with Doxygen"
        VERBATIM
    )
else () # DOXYGEN_FOUND
    message(WARNING "Doxygen need to be installed to generate the doxygen documentation")
endif () # DOXYGEN_FOUND