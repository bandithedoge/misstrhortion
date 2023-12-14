dpf_add_plugin(${PROJECT_NAME}
  TARGETS
    clap
    jack
    lv2
    vst2
    vst3
  FILES_DSP
    PluginProcessor.cpp
    $<TARGET_OBJECTS:juce>
  FILES_UI
    PluginUI.cpp
    ../DPF-Widgets/opengl/DearImGui.cpp
  UI_TYPE opengl)

target_include_directories(${PROJECT_NAME}
  PUBLIC
    ".")

target_compile_definitions(${PROJECT_NAME}
  PUBLIC
    PROJECT_VERSION="${PROJECT_VERSION}"
    PROJECT_VERSION_MAJOR=${PROJECT_VERSION_MAJOR}
    PROJECT_VERSION_MINOR=${PROJECT_VERSION_MINOR}
    PROJECT_VERSION_PATCH=${PROJECT_VERSION_PATCH})
