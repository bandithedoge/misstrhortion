cmake_minimum_required(VERSION 3.15)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED On)
set(PROJECT_NAME Misstrhortion)

project(${PROJECT_NAME} VERSION 1.0.0)

add_subdirectory(dpf)

set(JUCE_MODULES_ONLY ON)
add_subdirectory(JUCE)

# https://forum.juce.com/t/compilation-issues-when-supporting-some-old-linux-platforms/32051
target_compile_definitions(juce_core
  INTERFACE
    _NL_IDENTIFICATION_LANGUAGE=0x42
    _NL_IDENTIFICATION_TERRITORY=0x43)

add_subdirectory(Source)

target_compile_definitions(${PROJECT_NAME}
  PUBLIC
    PROJECT_VERSION="${PROJECT_VERSION}"
    PROJECT_VERSION_MAJOR=${PROJECT_VERSION_MAJOR}
    PROJECT_VERSION_MINOR=${PROJECT_VERSION_MINOR}
    PROJECT_VERSION_PATCH=${PROJECT_VERSION_PATCH}
    JUCE_WEB_BROWSER=0
    JUCE_USE_CURL=0
    JUCE_VST3_CAN_REPLACE_VST2=0)

target_include_directories(${PROJECT_NAME}
  PUBLIC
    JUCE/modules
    DPF-Widgets/opengl)

target_link_libraries(${PROJECT_NAME}
  PRIVATE
    juce_dsp)

install(
  TARGETS ${PROJECT_NAME}-clap LIBRARY
  DESTINATION lib/clap)

install(
  TARGETS ${PROJECT_NAME}-jack RUNTIME
  DESTINATION bin
)

install(
  TARGETS ${PROJECT_NAME}-lv2 LIBRARY
  DESTINATION lib/lv2
)

install(
  TARGETS ${PROJECT_NAME}-vst2 LIBRARY
  DESTINATION lib/vst2
)

install(
  TARGETS ${PROJECT_NAME}-vst3 LIBRARY
  DESTINATION lib/vst3
)