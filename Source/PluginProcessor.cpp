#include "DistrhoPlugin.hpp"
#include "DistrhoPluginInfo.h"
#include "Utils.h"

#define JUCE_GLOBAL_MODULE_SETTINGS_INCLUDED
#include "juce_dsp/juce_dsp.h"

typedef juce::dsp::ProcessorDuplicator<juce::dsp::IIR::Filter<float>,
                                       juce::dsp::IIR::Coefficients<float>>
    FilterType;

START_NAMESPACE_DISTRHO

class Misstortion : public Plugin {
  public:
    Misstortion() : Plugin(m_params, 0, 0){};

  protected:
    const char *getLabel() const override { return DISTRHO_PLUGIN_NAME; }
    const char *getDescription() const override {
        return "Old version of the Misstortion plugin. Does its best to be "
               "like Logic's Clip Distortion, which makes it very useful for "
               "the hardstyle genre.";
    }
    const char *getMaker() const override { return DISTRHO_PLUGIN_BRAND; }
    const char *getHomePage() const override { return DISTRHO_PLUGIN_URI; }
    const char *getLicense() const override { return "GPLv3"; }
    uint32_t getVersion() const override { return d_version(1, 3, 0); }
    int64_t getUniqueId() const override { return d_cconst('M', 's', 's', 't'); }

    void initAudioPort(bool input, uint32_t index, AudioPort &port) override {
        port.groupId = kPortGroupStereo;
        Plugin::initAudioPort(input, index, port);
    };

    void initParameter(uint32_t index, Parameter &parameter) override {
        parameter.hints = kParameterIsAutomatable;
        parameter.ranges.min = 0.0f;
        parameter.ranges.def = 0.0f;

        switch (index) {
        case m_paramMix:
            parameter.name = "Mix";
            parameter.symbol = "mix";
            parameter.ranges.max = 100.0f;
            parameter.ranges.def = 50.0f;
            parameter.unit = "%";
            break;
        case m_paramGainIn:
            parameter.name = "Gain in";
            parameter.symbol = "gainin";
            parameter.ranges.min = -50.0f;
            parameter.ranges.max = 50.0f;
            parameter.unit = "db";
            break;
        case m_paramGainOut:
            parameter.name = "Gain out";
            parameter.symbol = "gainout";
            parameter.ranges.min = -50.0f;
            parameter.ranges.max = 50.0f;
            parameter.unit = "db";
            break;
        case m_paramDriveHard:
            parameter.name = "Drive";
            parameter.symbol = "drive";
            parameter.ranges.max = 50.0f;
            parameter.unit = "db";
            break;
        case m_paramDriveSoft:
            parameter.name = "Drive 2";
            parameter.symbol = "drive2";
            parameter.ranges.max = 50.0f;
            parameter.unit = "db";
            break;
        case m_paramToneHP:
            parameter.hints |= kParameterIsInteger | kParameterIsLogarithmic;
            parameter.name = "Tone";
            parameter.symbol = "tone";
            parameter.ranges.min = 1;
            parameter.ranges.max = 20000;
            parameter.ranges.def = 20000;
            parameter.unit = "Hz";
            break;
        case m_paramToneLP:
            parameter.hints |= kParameterIsInteger | kParameterIsLogarithmic;
            parameter.name = "Tone Post";
            parameter.symbol = "tonepost";
            parameter.ranges.min = 1;
            parameter.ranges.max = 20000;
            parameter.ranges.def = 20000;
            parameter.unit = "Hz";
            break;
        case m_paramSymmetry:
            parameter.name = "Symmetry";
            parameter.symbol = "symmetry";
            parameter.ranges.def = 50.0f;
            parameter.unit = "%";
            break;
        case m_paramFilterMode:
            parameter.hints |= kParameterIsInteger;
            parameter.name = "Filter Mode";
            parameter.symbol = "filtermode";
            parameter.ranges.min = 0;
            parameter.ranges.max = 2;
            parameter.ranges.def = 0;
            break;
        }
    };

    float getParameterValue(uint32_t index) const override { return params[index]; }

    void setParameterValue(uint32_t index, float value) override { params[index] = value; };

    void sampleRateChanged(double newSampleRate) override {
        juce::dsp::ProcessSpec dspSpec;
        dspSpec.sampleRate = newSampleRate;
        dspSpec.maximumBlockSize = getBufferSize();
        dspSpec.numChannels = DISTRHO_PLUGIN_NUM_OUTPUTS;

        m_filterHP.prepare(dspSpec);
        m_filterLP.prepare(dspSpec);
    }

    void run(const float **inputs, float **outputs, uint32_t frames) override {
        using namespace juce;
        auto buffer = AudioBuffer<float>((float **)inputs, DISTRHO_PLUGIN_NUM_INPUTS, frames);

        ScopedNoDenormals noDenormals;

        for (int i = DISTRHO_PLUGIN_NUM_INPUTS; i < DISTRHO_PLUGIN_NUM_OUTPUTS; i++) {
            buffer.clear(i, 0, buffer.getNumSamples());
        }

        double sampleRate = getSampleRate();
        int numSamples = buffer.getNumSamples();

        float mix = (params[m_paramMix] / 100.0f);
        float gainIn = Decibels::decibelsToGain(params[m_paramGainIn], -50.0f);
        float gainOut = Decibels::decibelsToGain(params[m_paramGainOut], -50.0f);

        float driveHard = Decibels::decibelsToGain(params[m_paramDriveHard]);
        float driveSoftDb = params[m_paramDriveSoft];
        float driveSoft = Decibels::decibelsToGain(driveSoftDb);
        int toneHP = params[m_paramToneHP];
        int toneLP = params[m_paramToneLP];
        float symmetry = (params[m_paramSymmetry] / 100.0f);

        int filterMode = params[m_paramFilterMode];
        if (filterMode == 0) {
            // Legacy (1.2 stock, very steep)
            double qo = pow(2.0, 6.0);
            double q = sqrt(qo) / (qo - 1);

            if (toneHP > 0) {
                m_filtersHPLegacy[0].setCoefficients(
                    IIRCoefficients::makeHighPass(sampleRate, (double)toneHP, q));
                m_filtersHPLegacy[1].setCoefficients(
                    IIRCoefficients::makeHighPass(sampleRate, (double)toneHP, q));
            }

            m_filtersLPLegacy[0].setCoefficients(
                IIRCoefficients::makeLowPass(sampleRate, (double)toneLP, q));
            m_filtersLPLegacy[1].setCoefficients(
                IIRCoefficients::makeLowPass(sampleRate, (double)toneLP, q));
        }

        // Make a temporary buffer for the final mix
        AudioSampleBuffer processBuffer;
        processBuffer.makeCopyOf(buffer);

        dsp::AudioBlock<float> dspBlock(processBuffer);
        dsp::ProcessContextReplacing<float> dspContext(dspBlock);

        // Apply input gain
        processBuffer.applyGain(gainIn);

        // Apply tone filter (6db/oct or 12db/oct high pass filter)
        if (filterMode != 0 && toneHP > 0) {
            // filterMode as order means 1st order = 6db/oct, 2nd order = 12db/oct
            auto coeff = dsp::FilterDesign<float>::designIIRHighpassHighOrderButterworthMethod(
                (float)toneHP, sampleRate, filterMode);
            m_filterHP.state->coefficients = coeff[0]->coefficients;
            m_filterHP.process(dspContext);
        }

        // TODO: Turn this into a DSP processor?
        for (int channel = 0; channel < Min(2, DISTRHO_PLUGIN_NUM_INPUTS); ++channel) {
            float *channelData = processBuffer.getWritePointer(channel);

            for (int i = 0; i < numSamples; i++) {
                float &sample = channelData[i];

                // Legacy tone filter (from 1.2, weird Q value on a 2nd order filter)
                if (filterMode == 0 && toneHP > 0) {
                    sample = m_filtersHPLegacy[channel].processSingleSampleRaw(sample);
                }

                // Hard clip distortion
                if (driveHard > 1.0f) {
                    sample = Clamp(-1.0f, 1.0f, sample * driveHard);
                }

                // Hyperbolic soft clip distortion
                if (driveSoft > 1.0f) {
                    sample = atanf(sample * driveSoft);
                    sample = Clamp(-1.0f + symmetry, symmetry, sample);
                }

                // Apply symmetry
                if (sample < 0.0f) {
                    sample *= (1.0f - symmetry);
                } else {
                    sample *= symmetry;
                }

                // Legacy clip filter (from 1.2, weird Q value on a 2nd order filter)
                if (filterMode == 0) {
                    sample = m_filtersLPLegacy[channel].processSingleSampleRaw(sample);
                }
            }
        }

        // Apply clip filter (6db/oct or 12db/oct low pass filter)
        if (filterMode != 0) {
            // filterMode as order means 1st order = 6db/oct, 2nd order = 12db/oct
            auto coeff = dsp::FilterDesign<float>::designIIRLowpassHighOrderButterworthMethod(
                (float)toneLP, sampleRate, filterMode);
            m_filterLP.state->coefficients = coeff[0]->coefficients;
            m_filterLP.process(dspContext);
        }

        // Apply mix from the buffer
        for (int channel = 0; channel < Min(2, DISTRHO_PLUGIN_NUM_INPUTS); ++channel) {
            float *channelData = buffer.getWritePointer(channel);
            const float *processedData = processBuffer.getReadPointer(channel);

            for (int i = 0; i < numSamples; i++) {
                channelData[i] = Lerp(channelData[i], processedData[i], mix);
            }
        }

        // And finally, apply output gain
        buffer.applyGain(gainOut);

        if (outputs[0] != inputs[0])
            std::memcpy(outputs[0], inputs[0], sizeof(float) * frames);

        if (outputs[1] != inputs[1])
            std::memcpy(outputs[1], inputs[1], sizeof(float) * frames);
    }

  private:
    FilterType m_filterHP;
    FilterType m_filterLP;
    juce::IIRFilter m_filtersHPLegacy[2];
    juce::IIRFilter m_filtersLPLegacy[2];
    float params[m_params];

    DISTRHO_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Misstortion)
};

Plugin *createPlugin() { return new Misstortion(); };

END_NAMESPACE_DISTRHO
