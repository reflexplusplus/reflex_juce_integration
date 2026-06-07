#include "MainComponent.h"

//==============================================================================
class GuiAppApplication final : public juce::JUCEApplication
{
public:
    //==============================================================================
    GuiAppApplication() 
    {
        //Start Reflex, and for convenience use Bootstrap::Global, which instantiates a virtual filesystem, resourcepool, and IDE for hot-reloading
        //this will result in a cache file being written in AppData/VendorName/AppName, so a more thorough implementation would extract these steps and avoid using Reflex::Bootstrap

        using namespace Reflex;

        root_module.Init();     //start Reflex and all sub-modules
        
        auto main_path = File::CorrectStrokes(ToWString(ToView(__FILE__))); //we 

        auto project_dir = File::SplitFilename(main_path).a;    //Bootstrap::Global assumes resources/ is here, allows hot-reloading

        m_reflex_bootstrap_global = Bootstrap::Global::Acquire(kVendorName, kAppName, project_dir, kAppSymbol);
    }

    ~GuiAppApplication()
    {
        m_reflex_bootstrap_global.Clear();

        Reflex::root_module.Deinit();
    }

    const juce::String getApplicationName() override       { return kAppName.data; }
    const juce::String getApplicationVersion() override    { return JUCE_APPLICATION_VERSION_STRING; }
    bool moreThanOneInstanceAllowed() override             { return true; }

    //==============================================================================
    void initialise (const juce::String& commandLine) override
    {
        juce::ignoreUnused (commandLine);

        mainWindow.reset (new MainWindow (getApplicationName()));
    }

    void shutdown() override
    {
        mainWindow = nullptr; // (deletes our window)
    }

    //==============================================================================
    void systemRequestedQuit() override
    {
        quit();
    }

    void anotherInstanceStarted (const juce::String& commandLine) override
    {
        juce::ignoreUnused (commandLine);
    }

    class MainWindow final : public juce::DocumentWindow
    {
    public:
        explicit MainWindow (juce::String name)
            : DocumentWindow (name,
                              juce::Desktop::getInstance().getDefaultLookAndFeel()
                                                          .findColour (backgroundColourId),
                              allButtons)
        {
            setUsingNativeTitleBar (true);
            setContentOwned (new MainComponent(), true);

           #if JUCE_IOS || JUCE_ANDROID
            setFullScreen (true);
           #else
            setResizable (true, true);
            centreWithSize (getWidth(), getHeight());
           #endif

            setVisible (true);
        }

        void closeButtonPressed() override
        {
            getInstance()->systemRequestedQuit();
        }

    private:
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainWindow)
    };

private:

    Reflex::Reference <Reflex::Object> m_reflex_bootstrap_global;

    std::unique_ptr<MainWindow> mainWindow;
};

START_JUCE_APPLICATION (GuiAppApplication)
