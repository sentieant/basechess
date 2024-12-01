project(ChessConsole 
    VERSION 1.0.0 
    LANGUAGES CXX
)

add_executable(${PROJECT_NAME} main.cpp)

# Make sure library exists before linking
add_dependencies(${PROJECT_NAME} BaseChess)

# Link with the correct library name
target_link_libraries(${PROJECT_NAME} 
    PRIVATE 
        BaseChess
)