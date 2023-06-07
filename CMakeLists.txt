cmake_minimum_required(VERSION 3.15)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED On)
set(PROJECT_NAME Misstrhortion)

project(${PROJECT_NAME} VERSION 1.0.0)

add_subdirectory(dpf)

set(JUCE_MODULES_ONLY ON)
add_subdirectory(JUCE)

if(WIN32)
  if(MSVC)
    target_compile_definitions(juce_dsp
      PUBLIC
        /FI"mutex")
  else()
    target_compile_definitions(juce_dsp
      PUBLIC
        -include mutex)
  endif()
endif()

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
