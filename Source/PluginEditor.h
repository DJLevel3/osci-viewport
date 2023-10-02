#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "SettingsComponent.h"
#include "components/MainMenuBarModel.h"
#include "LookAndFeel.h"


class OscirenderAudioProcessorEditor : public juce::AudioProcessorEditor, private juce::CodeDocument::Listener, public juce::AsyncUpdater, public juce::ChangeListener {
public:
    OscirenderAudioProcessorEditor(OscirenderAudioProcessor&);
    ~OscirenderAudioProcessorEditor() override;

    void paint(juce::Graphics&) override;
    void resized() override;
    void handleAsyncUpdate() override;

    std::atomic<bool> editingPerspective = false;
    void changeListenerCallback(juce::ChangeBroadcaster* source) override;

    OscirenderLookAndFeel lookAndFeel;
private:
    OscirenderAudioProcessor& audioProcessor;
    
    juce::TabbedComponent tabs{juce::TabbedButtonBar::TabsAtTop};
    SettingsComponent settings{audioProcessor, *this};
    juce::CodeEditorComponent::ColourScheme colourScheme;
    juce::LuaTokeniser luaTokeniser;
    juce::XmlTokeniser xmlTokeniser;
	juce::ShapeButton collapseButton;


    void codeDocumentTextInserted(const juce::String& newText, int insertIndex) override;
    void codeDocumentTextDeleted(int startIndex, int endIndex) override;
    std::unique_ptr<juce::FileChooser> chooser;
    MainMenuBarModel menuBarModel{*this};
    juce::MenuBarComponent menuBar;

    std::atomic<bool> updatingDocumentsWithParserLock = false;

    bool keyPressed(const juce::KeyPress& key) override;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (OscirenderAudioProcessorEditor)
};
