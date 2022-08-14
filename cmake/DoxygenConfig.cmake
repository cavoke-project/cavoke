# Standard configuration for Cavoke Docs with Doxygen

set(DOXYGEN_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/docs)
set(DOXYGEN_PROJECT_LOGO ${CMAKE_CURRENT_SOURCE_DIR}/.github/assets/logo55.png)
set(DOXYGEN_BUILTIN_STL_SUPPORT YES) # custom
set(DOXYGEN_RECURSIVE YES)
set(DOXYGEN_EXCLUDE
        ${CMAKE_CURRENT_SOURCE_DIR}/server/sql-models
        ${CMAKE_CURRENT_SOURCE_DIR}/client/cavoke_client_autogen
        ${CMAKE_CURRENT_SOURCE_DIR}/server/pytest/.pytest_cache
        ${CMAKE_CURRENT_SOURCE_DIR}/README.md)
set(DOXYGEN_IMAGE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/img ${CMAKE_CURRENT_SOURCE_DIR}/docs/img)
set(DOXYGEN_USE_MDFILE_AS_MAINPAGE ${CMAKE_CURRENT_SOURCE_DIR}/docs/mainpage.md)
set(DOXYGEN_HTML_HEADER ${CMAKE_CURRENT_SOURCE_DIR}/.github/assets/doxygen-header.html)
set(DOXYGEN_HTML_EXTRA_STYLESHEET
        ${CMAKE_CURRENT_SOURCE_DIR}/third_party/doxygen-awesome-css/doxygen-awesome.css
        ${CMAKE_CURRENT_SOURCE_DIR}/third_party/doxygen-awesome-css/doxygen-custom/custom.css
        ${CMAKE_CURRENT_SOURCE_DIR}/third_party/doxygen-awesome-css/doxygen-awesome-sidebar-only.css
        ${CMAKE_CURRENT_SOURCE_DIR}/third_party/doxygen-awesome-css/doxygen-awesome-sidebar-only-darkmode-toggle.css
        ${CMAKE_CURRENT_SOURCE_DIR}/third_party/doxygen-awesome-css/doxygen-custom/custom-alternative.css)
set(DOXYGEN_HTML_EXTRA_FILES
        ${CMAKE_CURRENT_SOURCE_DIR}/third_party/doxygen-awesome-css/doxygen-awesome-darkmode-toggle.js
        ${CMAKE_CURRENT_SOURCE_DIR}/third_party/doxygen-awesome-css/doxygen-awesome-fragment-copy-button.js
        ${CMAKE_CURRENT_SOURCE_DIR}/third_party/doxygen-awesome-css/doxygen-awesome-paragraph-link.js
        ${CMAKE_CURRENT_SOURCE_DIR}/third_party/doxygen-awesome-css/doxygen-custom/toggle-alternative-theme.js)
set(DOXYGEN_HTML_COLORSTYLE_HUE 209)
set(DOXYGEN_HTML_COLORSTYLE_SAT 255)
set(DOXYGEN_HTML_COLORSTYLE_GAMMA 113)
set(DOXYGEN_GENERATE_TREEVIEW YES)
set(DOXYGEN_TREEVIEW_WIDTH 335)
set(DOXYGEN_PREDEFINED cavoke_orm=cavoke) # custom
set(DOXYGEN_HAVE_DOT YES)
set(DOXYGEN_COLLABORATION_GRAPH NO)
set(DOXYGEN_DOT_IMAGE_FORMAT svg)
set(DOXYGEN_INTERACTIVE_SVG YES) # custom
set(DOXYGEN_DOT_TRANSPARENT YES)
set(DOXYGEN_SOURCE_BROWSER YES) # custom
