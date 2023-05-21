cmake_minimum_required(VERSION 3.15)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED On)

set(PROJECT_NAME Misstortion1)

project(${PROJECT_NAME}
  VERSION 1.3
)

add_subdirectory(JUCE)
add_subdirectory(clap-juce-extensions)

juce_add_plugin(${PROJECT_NAME}
  FORMATS
    VST3
    LV2
    Standalone
  PRODUCT_NAME "Misstortion"
  COMPANY_NAME "Nimble Tools"
  PLUGIN_MANUFACTURER_CODE "Nbts"
  PLUGIN_CODE "Msst"
  IS_SYNTH FALSE
  NEEDS_MIDI_INPUT FALSE
  NEEDS_MIDI_OUTPUT FALSE
  IS_MIDI_EFFECT FALSE
  EDITOR_WANTS_KEYBOARD_FOCUS FALSE
  COPY_PLUGIN_AFTER_BUILD FALSE
)

clap_juce_extensions_plugin(TARGET ${PROJECT_NAME}
  CLAP_ID "tools.nimble.misstortion"
  CLAP_FEATURES audio-effect stereo distortion
)

add_subdirectory(Source)

juce_generate_juce_header(${PROJECT_NAME})

target_compile_definitions(${PROJECT_NAME}
  PUBLIC
    JUCE_WEB_BROWSER=0
    JUCE_USE_CURL=0
    JUCE_VST3_CAN_REPLACE_VST2=0
)

juce_add_binary_data(${PROJECT_NAME}Data
  SOURCES
    Misstortion.png
)

target_link_libraries(${PROJECT_NAME}
  PRIVATE
    ${PROJECT_NAME}Data
    juce::juce_audio_basics
    juce::juce_audio_devices
    juce::juce_audio_formats
    juce::juce_audio_plugin_client
    juce::juce_audio_processors
    juce::juce_audio_utils
    juce::juce_core
    juce::juce_data_structures
    juce::juce_dsp
    juce::juce_events
    juce::juce_graphics
    juce::juce_gui_basics
    juce::juce_gui_extra
  PUBLIC
    juce::juce_recommended_config_flags
    juce::juce_recommended_lto_flags
    juce::juce_recommended_warning_flags
)
