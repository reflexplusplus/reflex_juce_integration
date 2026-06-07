#include "MainComponent.h"




using namespace Reflex;

struct View : public GLX::Object
{
	View(MainComponent & juce_component)
		: m_textarea(true)
	{
		GLX::AddFloat(*this, m_textarea, GLX::kAlignmentCenter);

		Bootstrap::SetStyleSheet(*this, ToWString(Join(":res:", kAppSymbol, "/styles.glx")));
	}

	void OnSetStyle(const GLX::Style & style) override
	{
		m_textarea.SetStyle(style["TextArea"]);
	}


	GLX::TextArea m_textarea;
};

TRef <GLX::Object> CreateReflexView(MainComponent & juce_component)
{
	return New<View>(juce_component);
}
