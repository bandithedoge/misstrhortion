dpf_add_plugin(${PROJECT_NAME}
  TARGETS
    clap
    jack
    ladspa
    lv2
    vst2
    vst3
  FILES_DSP
    PluginProcessor.cpp)

target_include_directories(${PROJECT_NAME}
  PUBLIC ".")
