dpf_add_plugin(${PROJECT_NAME}
  TARGETS
    clap
    jack
    lv2
    vst2
    vst3
  FILES_DSP
    PluginProcessor.cpp
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
    PROJECT_VERSION_PATCH=${PROJECT_VERSION_PATCH}
    JUCE_WEB_BROWSER=0
    JUCE_USE_CURL=0
    JUCE_VST3_CAN_REPLACE_VST2=0)
