#pragma once

#define DISTRHO_PLUGIN_BRAND "bandithedoge"
#define DISTRHO_PLUGIN_NAME "Misstrhortion"
#define DISTRHO_PLUGIN_URI "https://github.com/bandithedoge/misstrhortion"

#define DISTRHO_PLUGIN_CLAP_ID "com.bandithedoge.misstrhortion"
#define DISTRHO_PLUGIN_CLAP_FEATURES "audio-effect", "distortion"
#define DISTRHO_PLUGIN_LV2_CATEGORY "lv2:DistortionPlugin"
#define DISTRHO_PLUGIN_VST3_CATEGORIES "Fx|Distortion"

#define DISTRHO_PLUGIN_HAS_UI 1
#define DISTRHO_UI_USE_CUSTOM 1
#define DISTRHO_UI_CUSTOM_INCLUDE_PATH "DearImGui.hpp"
#define DISTRHO_UI_CUSTOM_WIDGET_TYPE DGL_NAMESPACE::ImGuiTopLevelWidget
#define DISTRHO_UI_DEFAULT_WIDTH 610
#define DISTRHO_UI_DEFAULT_HEIGHT 180

#define DISTRHO_PLUGIN_IS_RT_SAFE 1
#define DISTRHO_PLUGIN_NUM_INPUTS 2
#define DISTRHO_PLUGIN_NUM_OUTPUTS 2
