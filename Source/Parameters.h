#pragma once
#include "DistrhoDetails.hpp"
#include "DistrhoPlugin.hpp"

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
    m_paramBypass,

    m_params
};

inline Parameter *getParamInfo(int index, Parameter *parameter = new Parameter()) {
    parameter->hints = kParameterIsAutomatable;
    parameter->unit = "";
    parameter->ranges.min = 0.0f;
    parameter->ranges.def = 0.0f;

    switch (index) {
    case m_paramMix:
        parameter->name = "Mix";
        parameter->symbol = "mix";
        parameter->ranges.max = 100.0f;
        parameter->ranges.def = 50.0f;
        parameter->unit = "%";
        break;
    case m_paramGainIn:
        parameter->name = "In";
        parameter->symbol = "gainin";
        parameter->ranges.min = -50.0f;
        parameter->ranges.max = 50.0f;
        parameter->unit = "db";
        break;
    case m_paramGainOut:
        parameter->name = "Out";
        parameter->symbol = "gainout";
        parameter->ranges.min = -50.0f;
        parameter->ranges.max = 50.0f;
        parameter->unit = "db";
        break;
    case m_paramDriveHard:
        parameter->name = "Hard clip";
        parameter->symbol = "drive";
        parameter->ranges.max = 50.0f;
        parameter->unit = "db";
        break;
    case m_paramDriveSoft:
        parameter->name = "Soft clip";
        parameter->symbol = "drive2";
        parameter->ranges.max = 50.0f;
        parameter->unit = "db";
        break;
    case m_paramToneHP:
        parameter->hints |= kParameterIsInteger | kParameterIsLogarithmic;
        parameter->name = "Tone";
        parameter->symbol = "tone";
        parameter->ranges.min = 0;
        parameter->ranges.max = 20000;
        parameter->ranges.def = 20;
        parameter->unit = "Hz";
        break;
    case m_paramToneLP:
        parameter->hints |= kParameterIsInteger | kParameterIsLogarithmic;
        parameter->name = "Clip filter";
        parameter->symbol = "tonepost";
        parameter->ranges.min = 1;
        parameter->ranges.max = 20000;
        parameter->ranges.def = 20000;
        parameter->unit = "Hz";
        break;
    case m_paramSymmetry:
        parameter->name = "Symmetry";
        parameter->symbol = "symmetry";
        parameter->ranges.max = 100.0f;
        parameter->ranges.def = 50.0f;
        parameter->unit = "%";
        break;
    case m_paramFilterMode:
        parameter->hints |= kParameterIsInteger;
        parameter->name = "Filter Mode";
        parameter->symbol = "filtermode";
        parameter->ranges.min = 0.0f;
        parameter->ranges.max = 2.0f;
        parameter->ranges.def = 0.0f;
        if (ParameterEnumerationValue *const values = new ParameterEnumerationValue[3]) {
            parameter->enumValues.count = 3;
            parameter->enumValues.values = values;
            values[0].label = "Legacy";
            values[0].value = 0.0f;
            values[1].label = "6 db/oct";
            values[1].value = 1.0f;
            values[2].label = "12 db/oct";
            values[2].value = 2.0f;
        }
        break;
    case m_paramBypass:
        parameter->initDesignation(kParameterDesignationBypass);
        break;
    }

    return parameter;
};
