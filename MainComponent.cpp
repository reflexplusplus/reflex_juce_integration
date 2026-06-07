#include "MainComponent.h"



//==============================================================================
MainComponent::MainComponent()
{
    setSize (600, 400);
}

//==============================================================================
void MainComponent::parentHierarchyChanged()
{
	using namespace Reflex;

	m_reflex_window.Clear();

	if (auto peer = getPeer())
	{
		if (auto system_window = peer->getNativeHandle())
		{
			//start GLX, create System::Window, embed into juce managed window, attach view
			//as we instantiated Bootstrap::Global in GuiAppApplication, we can use Detail::CreateWindowClient which also instantiates the IDE window and hot-reloading

			m_reflex_window = System::Window::Create(0, false, system_window);

			auto config = GetProperty<ObjectOf<System::Renderer::Config>>(Bootstrap::global->prefs, Bootstrap::Detail::kViewGraphicsConfig);	//restore view setting

			auto glx = GLX::Start(Bootstrap::global->resourcepool, config->value);

			GLX::Core::Context ctx;

			GLX::AnimationScope scope(File::UnpackResource<bool>("Bootstrap", "view.allow_init_animations", true));

			auto view = CreateReflexView(*this);

			m_reflex_window_client = Bootstrap::Detail::CreateWindowClient(m_reflex_window, view, File::PersistentPropertySet::null);	//the Bootstrap window client will try to store/restore its size from the propertyset, but as we are embedded, we dont need it anyway, so pass the null instance

			resized();	// juce can recreate the peer without sending resized()

			GLX::FocusBranch(view);
		}
	}
}

void MainComponent::resized()
{
	if (auto* peer = getPeer())
	{
		//adjust for discrepancy between how JUCE and Reflex compute backing density

		const auto area = peer->getAreaCoveredBy(*this);
		const float factor = float(peer->getPlatformScaleFactor()) / m_reflex_window_client->GetPixelDensity();

		m_reflex_window->SetRect({ {}, { Reflex::Truncate(area.getWidth() * factor), Reflex::Truncate(area.getHeight() * factor) } });
	}
}

void MainComponent::paint(juce::Graphics& g)
{
    //nothing to do here as Reflex window has its own refresh mechanism
}
