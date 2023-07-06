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
