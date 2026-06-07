#pragma once

#include <juce_gui_basics/juce_gui_basics.h>
#include "reflex_ext.h"

static constexpr Reflex::CString::View kVendorName = "Reflex++";
static constexpr Reflex::CString::View kAppName = "Reflex Juce Demo";
static constexpr Reflex::CString::View kAppSymbol = "ReflexJuceDemo";

class MainComponent final : public juce::Component
{
public:
    //==============================================================================
    MainComponent();

    //==============================================================================
    void parentHierarchyChanged() override;
    void paint (juce::Graphics&) override;
    void resized() override;


private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainComponent)

    Reflex::Reference <Reflex::System::Window> m_reflex_window;

    Reflex::Reference <Reflex::GLX::WindowClient> m_reflex_window_client;
};

extern Reflex::TRef <Reflex::GLX::Object> CreateReflexView(MainComponent & juce_component); //implement this and return your root GLX::Object
