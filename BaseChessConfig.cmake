# Get source directory
get_filename_component(CHESS_SOURCE_DIR "${CMAKE_CURRENT_LIST_FILE}" DIRECTORY)

# Collect source files
file(GLOB_RECURSE CHESS_HEADERS 
    ${CHESS_SOURCE_DIR}/include/BaseChess/*.hpp
)
file(GLOB_RECURSE CHESS_SOURCES 
    ${CHESS_SOURCE_DIR}/src/*.cpp
)

# Create library
add_library(BaseChess STATIC 
    ${CHESS_SOURCES} 
    ${CHESS_HEADERS}
)

# Set modern C++ properties with higher standard
set_target_properties(BaseChess 
    PROPERTIES
        CXX_STANDARD 17      
        CXX_STANDARD_REQUIRED ON
        CXX_EXTENSIONS OFF
)

# Set include directories
target_include_directories(BaseChess
    PUBLIC
        ${CHESS_SOURCE_DIR}/include
)

# Add compiler-specific flags
if(CMAKE_CXX_COMPILER_ID MATCHES "AppleClang|Clang|GNU")
    target_compile_options(BaseChess
        PRIVATE
            -std=c++17        
            -Wall
            -Wpedantic
    )
endif()