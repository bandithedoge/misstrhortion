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

        setGeometryConstraints(DISTRHO_UI_DEFAULT_WIDTH, DISTRHO_UI_DEFAULT_HEIGHT);
    };

  protected:
    void parameterChanged(uint32_t index, float value) override {
        paramValues[index] = value;
        repaint();
    };

    void onImGuiDisplay() override {
        const float width = getWidth();
        const float height = getHeight();

        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0);

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
                                  ImGuiTableFlags_SizingStretchProp |
                                      ImGuiTableFlags_NoBordersInBody |
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
                if (ImGui::Button(DISTRHO_PLUGIN_NAME " 1.3"))
                    resetParameters();
                if (ImGui::IsItemHovered()) {
                    ImGui::SetTooltip("Left-click to reset parameters, right-click to randomize");
                }
                if (ImGui::IsItemClicked(ImGuiMouseButton_Right))
                    randomizeParameters();
                ImGui::Text("nimble.tools");
                ImGui::Separator();
                ImGui::Text("DPF port");
                ImGui::Text("bandithedoge.com");
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
                const int mode = floor(paramValues[m_paramFilterMode]);
                const char *name = mode_names[mode];

                if (ImGui::SliderInt(param->name, (int *)&paramValues[m_paramFilterMode],
                                     param->ranges.min, param->ranges.max, name)) {
                    if (ImGui::IsItemActivated())
                        editParameter(m_paramFilterMode, true);
                    setParameterValue(m_paramFilterMode, paramValues[m_paramFilterMode]);
                }
                if (ImGui::IsItemDeactivated())
                    editParameter(m_paramFilterMode, false);
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
            if (i == m_paramGainIn || i == m_paramGainOut)
                continue;
            const float val = params[i]->ranges.min + (rand() % (int)params[i]->ranges.max);
            setParameterValue(i, val);
            parameterChanged(i, val);
        }
    }

    void makeKnob(int index) {
        auto param = params[index];

        if (ImGuiKnobs::Knob(param->name, &paramValues[index], param->ranges.min, param->ranges.max,
                             0,
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
                          ? ImGui::VSliderFloat(param->name, ImVec2(40, 165), &paramValues[index],
                                                param->ranges.min, param->ranges.max,
                                                "%.1f\n" + param->unit, flags)
                          : ImGui::SliderFloat(param->name, &paramValues[index], param->ranges.min,
                                               param->ranges.max, "%.0f " + param->unit, flags);

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