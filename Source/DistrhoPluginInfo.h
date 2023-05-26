#pragma once

#define DISTRHO_PLUGIN_BRAND "Nimble Tools"
#define DISTRHO_PLUGIN_NAME "Misstortion"
#define DISTRHO_PLUGIN_URI "https://nimble.itch.io/misstortion"
#define DISTRHO_PLUGIN_CLAP_ID "tools.nimble.misstortion"

#define DISTRHO_PLUGIN_HAS_UI 0
#define DISTRHO_PLUGIN_IS_RT_SAFE 1
#define DISTRHO_PLUGIN_NUM_INPUTS 2
#define DISTRHO_PLUGIN_NUM_OUTPUTS 2

enum Parameters {
    m_paramMix = 0,
    m_paramGainIn,
    m_paramGainOut,
    m_paramDriveHard,
    m_paramDriveSoft,
    m_paramToneHP,
    m_paramToneLP,
    m_paramSymmetry,
    m_paramFilterMode,

    m_params
};
