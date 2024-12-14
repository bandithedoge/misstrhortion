cmake_minimum_required(VERSION 3.25)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED On)

project(Misstrhortion VERSION 1.0.0)

add_subdirectory(dpf)

set(JUCE_MODULES_ONLY ON)
add_subdirectory(JUCE)

# musl builds fail without this
# see: https://forum.juce.com/t/compilation-issues-when-supporting-some-old-linux-platforms/32051
# target_compile_definitions(juce_core
#   INTERFACE
#     _NL_IDENTIFICATION_LANGUAGE=0x42
#     _NL_IDENTIFICATION_TERRITORY=0x43)

add_subdirectory(Source)

target_include_directories(${PROJECT_NAME}
  PUBLIC
    JUCE/modules
    DPF-Widgets/opengl)

target_link_libraries(${PROJECT_NAME}
  PRIVATE
    juce_dsp)

if(MINGW)
    find_package(GLEW REQUIRED)
    target_include_directories(${PROJECT_NAME}
    PUBLIC
      ${GLEW_INCLUDE_DIRS}/GL)
    target_compile_definitions(${PROJECT_NAME}
    PUBLIC
      __MSYS__)
    target_link_libraries(${PROJECT_NAME}
    PUBLIC
      comdlg32
      imm32
      ole32
      oleaut32
      rpcrt4
      shlwapi
      uuid
      version
      wininet
      winmm
      ws2_32
      wsock32)
endif()

include(GNUInstallDirs)
set(COMMON_FILES_PATH "C:\\\\Program Files\\\\Common Files")

install(
  TARGETS ${PROJECT_NAME}-jack
  COMPONENT JACK
  RUNTIME)

install(
  TARGETS ${PROJECT_NAME}-clap
  DESTINATION $<IF:$<BOOL:${WIN32}>,CLAP,${CMAKE_INSTALL_LIBDIR}/clap>
  COMPONENT CLAP
  LIBRARY)
set(CPACK_NSIS_CLAP_INSTALL_DIRECTORY ${COMMON_FILES_PATH})

install(
  TARGETS
    ${PROJECT_NAME}-lv2
    ${PROJECT_NAME}-lv2-ui
  DESTINATION $<IF:$<BOOL:${WIN32}>,LV2,${CMAKE_INSTALL_LIBDIR}/lv2>
  COMPONENT LV2
  LIBRARY)
install(
  FILES
    ${CMAKE_BINARY_DIR}/bin/${PROJECT_NAME}.lv2/manifest.ttl
    ${CMAKE_BINARY_DIR}/bin/${PROJECT_NAME}.lv2/${PROJECT_NAME}_dsp.ttl
    ${CMAKE_BINARY_DIR}/bin/${PROJECT_NAME}.lv2/${PROJECT_NAME}_ui.ttl
  DESTINATION $<IF:$<BOOL:${WIN32}>,LV2,${CMAKE_INSTALL_LIBDIR}/lv2>
  COMPONENT LV2)
set(CPACK_NSIS_LV2_INSTALL_DIRECTORY ${COMMON_FILES_PATH})

install(
  TARGETS ${PROJECT_NAME}-vst2
  DESTINATION $<IF:$<BOOL:${WIN32}>,VSTPlugins,${CMAKE_INSTALL_LIBDIR}/vst2>
  COMPONENT VST2
  LIBRARY)
set(CPACK_NSIS_VST2_INSTALL_DIRECTORY "C:\\\\Program Files\\\\Steinberg")

install(
  TARGETS ${PROJECT_NAME}-vst3
  DESTINATION $<IF:$<BOOL:${WIN32}>,VST3,${CMAKE_INSTALL_LIBDIR}/vst3>
  COMPONENT VST3
  LIBRARY)
set(CPACK_NSIS_VST3_INSTALL_DIRECTORY ${COMMON_FILES_PATH})

# HACK: JUCE always installs its own headers regardless of settings so we have
#       to remove all unspecified components and add everything else manually.
get_cmake_property(CPACK_COMPONENTS_ALL COMPONENTS)
list(REMOVE_ITEM CPACK_COMPONENTS_ALL Unspecified)

set(CPACK_PACKAGE_VENDOR bandithedoge)
set(CPACK_PACKAGE_CONTACT bandithedoge@protonmail.com)
set(CPACK_PACKAGE_INSTALL_DIRECTORY ${PROJECT_NAME})
set(CPACK_PACKAGE_EXECUTABLES ${PROJECT_NAME};${PROJECT_NAME})

include(InstallRequiredSystemLibraries)
include(CPack)

cpack_add_component(JACK DISPLAY_NAME JACK/Standalone)
cpack_add_component(CLAP)
cpack_add_component(LV2)
cpack_add_component(VST2)
cpack_add_component(VST3)
