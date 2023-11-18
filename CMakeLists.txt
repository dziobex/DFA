cmake_minimum_required(VERSION 3.16)
project(automat LANGUAGES CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
option(BUILD_SHARED_LIBS "Build shared libraries" OFF)

include(FetchContent)
FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 2.6.x)
FetchContent_MakeAvailable(SFML)

add_executable(automat src/main.cpp src/Node.cpp src/Node.h src/Constants.h src/TextureManager.cpp src/TextureManager.h src/Pass.cpp src/Pass.h src/StageManager.cpp src/StageManager.h src/UI.cpp src/UI.h src/Assets.h)
target_link_libraries(automat PRIVATE sfml-graphics)
target_compile_features(automat PRIVATE cxx_std_17)

if(WIN32)
    add_custom_command(
        TARGET automat
        COMMENT "Copy OpenAL DLL"
        PRE_BUILD COMMAND ${CMAKE_COMMAND} -E copy ${SFML_SOURCE_DIR}/extlibs/bin/$<IF:$<EQUAL:${CMAKE_SIZEOF_VOID_P},8>,x64,x86>/openal32.dll $<TARGET_FILE_DIR:automat>
        VERBATIM)
endif()

install(TARGETS automat)
