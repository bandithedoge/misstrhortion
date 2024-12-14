#ifdef __MSYS__
#include "glew.h"
#endif

#include "DistrhoPluginUtils.hpp"
#include "DistrhoUI.hpp"
#include "Parameters.h"
#include <ctime>

START_NAMESPACE_DISTRHO

class MisstortionUI : public UI {
  public:
    MisstortionUI() : UI(DISTRHO_UI_DEFAULT_WIDTH, DISTRHO_UI_DEFAULT_HEIGHT) {
        std::srand(std::time(nullptr));

        for (int i = 0; i < m_params; i++)
            params[i] = getParamInfo(i);
    };

  protected:
    void parameterChanged(uint32_t index, float value) override {
        paramValues[index] = value;
        repaint();
    };

    void onImGuiDisplay() override {
        const float width = getWidth();
        const float height = getHeight();

        ImGui::SetNextWindowPos(ImVec2(0, 0));
        ImGui::SetNextWindowSize(ImVec2(width, height));

        if (ImGui::Begin(DISTRHO_PLUGIN_NAME, NULL,
                         ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoDecoration |
                             ImGuiWindowFlags_NoScrollWithMouse)) {
            makeSlider(m_paramGainIn, true);
            ImGui::SameLine();
            makeSlider(m_paramGainOut, true);
            ImGui::SameLine();

            ImGui::BeginGroup();

            if (ImGui::BeginTable("knobs", 5,
                                  ImGuiTableFlags_SizingStretchProp | ImGuiTableFlags_NoBordersInBody |
                                      ImGuiTableFlags_NoHostExtendX)) {
                ImGui::TableNextColumn();
                makeKnob(m_paramMix);
                ImGui::TableNextColumn();
                makeKnob(m_paramSymmetry);
                ImGui::TableNextColumn();
                makeKnob(m_paramDriveHard);
                ImGui::TableNextColumn();
                makeKnob(m_paramDriveSoft);

                ImGui::TableNextColumn();
                if (ImGui::Button("..."))
                    ImGui::OpenPopup("settings");

                if (ImGui::BeginPopup("settings")) {
                    if (ImGui::Button("Reset parameters"))
                        resetParameters();
                    if (ImGui::IsItemHovered())
                        ImGui::SetTooltip("Right-click to randomize");
                    if (ImGui::IsItemClicked(ImGuiMouseButton_Right))
                        randomizeParameters();
                    ImGui::Separator();
                    ImGui::TextDisabled("%s", getPluginFormatName());
                    ImGui::EndPopup();
                }
                ImGui::SameLine();
                ImGui::Text(DISTRHO_PLUGIN_NAME);
                ImGui::SameLine();
                ImGui::TextDisabled(PROJECT_VERSION);
                ImGui::Text("bandithedoge.com");
                ImGui::Separator();
                ImGui::TextWrapped("DPF port based on Misstortion 1.3");
                ImGui::Text("nimble.tools");
            }
            ImGui::EndTable();

            makeSlider(m_paramToneHP);
            makeSlider(m_paramToneLP);

            {
                auto param = params[m_paramFilterMode];
                const char *mode_names[3] = {
                    param->enumValues.values[0].label,
                    param->enumValues.values[1].label,
                    param->enumValues.values[2].label,
                };
                int mode = floor(paramValues[m_paramFilterMode]);
                const char *name = mode_names[mode];

                ImGui::BeginTable("Filter", 3);
                for (int i = 0; i < 3; i++) {
                    ImGui::TableNextColumn();
                    if (ImGui::RadioButton(mode_names[i], &mode, i)) {
                        if (ImGui::IsItemActivated())
                            editParameter(m_paramFilterMode, true);
                        paramValues[m_paramFilterMode] = mode;
                        setParameterValue(m_paramFilterMode, paramValues[m_paramFilterMode]);
                    };
                    if (ImGui::IsItemDeactivated())
                        editParameter(m_paramFilterMode, false);
                }
                ImGui::EndTable();
            }

            ImGui::EndGroup();
        }
        ImGui::End();
    };

  private:
    Parameter *params[m_params];
    float paramValues[m_params];

    void resetParameters() {
        for (int i = 0; i < m_params; i++) {
            const float val = params[i]->ranges.def;
            setParameterValue(i, val);
            parameterChanged(i, val);
        }
    }

    void randomizeParameters() {
        for (int i = 0; i < m_params; i++) {
            if (i == m_paramGainIn || i == m_paramGainOut || i == m_paramBypass)
                continue;
            const float val = params[i]->ranges.min +
                              (rand() % (params[i]->hints & kParameterIsInteger ? (int)params[i]->ranges.max + 1
                                                                                : (int)params[i]->ranges.max));
            setParameterValue(i, val);
            parameterChanged(i, val);
        }
    }

    void makeKnob(int index) {
        auto param = params[index];

        if (ImGuiKnobs::Knob(param->name, &paramValues[index], param->ranges.min, param->ranges.max, 0,
                             "%.1f" + (param->unit == "%"
                                           // using "%%" causes weird type errors for some reason
                                           ? param->unit + param->unit
                                           : " " + param->unit),
                             ImGuiKnobVariant_Wiper)) {
            if (ImGui::IsItemActivated())
                editParameter(index, true);
            setParameterValue(index, paramValues[index]);
        }
        if (ImGui::IsItemDeactivated())
            editParameter(index, false);
    }

    void makeSlider(int index, bool vertical = false) {
        auto param = params[index];
        int flags = 0;

        if (param->hints & kParameterIsLogarithmic)
            flags |= ImGuiSliderFlags_Logarithmic;

        bool slider = vertical
                          ? ImGui::VSliderFloat(param->name, ImVec2(40, 165), &paramValues[index], param->ranges.min,
                                                param->ranges.max, "%.1f\n" + param->unit, flags)
                          : ImGui::SliderFloat(param->name, &paramValues[index], param->ranges.min, param->ranges.max,
                                               "%.0f " + param->unit, flags);

        if (slider) {
            if (ImGui::IsItemActivated())
                editParameter(index, true);
            setParameterValue(index, paramValues[index]);
        }
        if (ImGui::IsItemDeactivated())
            editParameter(index, false);
    }

    DISTRHO_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MisstortionUI)
};

UI *createUI() { return new MisstortionUI; }

END_NAMESPACE_DISTRHO
