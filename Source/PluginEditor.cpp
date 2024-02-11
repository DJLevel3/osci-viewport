#include "PluginProcessor.h"
#include "PluginEditor.h"

OscirenderAudioProcessorEditor::OscirenderAudioProcessorEditor(OscirenderAudioProcessor& p)
	: AudioProcessorEditor(&p), audioProcessor(p), collapseButton("Collapse", juce::Colours::white, juce::Colours::white, juce::Colours::white)
{
    addAndMakeVisible(settings);

	juce::Path path;
    path.addTriangle(0.0f, 0.5f, 1.0f, 1.0f, 1.0f, 0.0f);
	collapseButton.setShape(path, false, true, true);

    colourScheme = lookAndFeel.getDefaultColourScheme();


    {
        juce::MessageManagerLock lock;
        audioProcessor.broadcaster.addChangeListener(this);
    }

    if (audioProcessor.wrapperType == juce::AudioProcessor::WrapperType::wrapperType_Standalone) {
        if (juce::TopLevelWindow::getNumTopLevelWindows() == 1) {
            juce::TopLevelWindow* w = juce::TopLevelWindow::getTopLevelWindow(0);
            w->setColour(juce::ResizableWindow::backgroundColourId, Colours::veryDark);
        }
    }

    juce::Desktop::getInstance().setDefaultLookAndFeel(&lookAndFeel);
    setLookAndFeel(&lookAndFeel);

    setSize(500, 500);
    setResizable(true, true);
    setResizeLimits(100, 100, 1000, 1000);
    getConstrainer()->setFixedAspectRatio(1.f);
}

OscirenderAudioProcessorEditor::~OscirenderAudioProcessorEditor() {
    setLookAndFeel(nullptr);
    juce::Desktop::getInstance().setDefaultLookAndFeel(nullptr);
    juce::MessageManagerLock lock;
    audioProcessor.broadcaster.removeChangeListener(this);
}

void OscirenderAudioProcessorEditor::paint(juce::Graphics& g) {
    g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));

    g.setColour(juce::Colours::white);
    g.setFont(15.0f);
}

void OscirenderAudioProcessorEditor::resized() {
    auto area = getLocalBounds();
    auto sections = 1;

    juce::Path path;
    path.addTriangle(0.0f, 0.5f, 1.0f, 1.0f, 1.0f, 0.0f);
    collapseButton.setShape(path, false, true, true);
    
    settings.sections = sections;
    settings.setBounds(area);

    repaint();
}

void OscirenderAudioProcessorEditor::handleAsyncUpdate() {
    resized();
}

bool OscirenderAudioProcessorEditor::keyPressed(const juce::KeyPress& key) {
    return false;
}

void OscirenderAudioProcessorEditor::changeListenerCallback(juce::ChangeBroadcaster* source) {
    if (source == &audioProcessor.broadcaster) {
    }
    else if (source == &audioProcessor.fileChangeBroadcaster) {
        // triggered when the audioProcessor changes the current file (e.g. to Blender)
    }
}

// parsersLock AND effectsLock must be locked before calling this function
void OscirenderAudioProcessorEditor::codeDocumentTextInserted(const juce::String& newText, int insertIndex) {
    if (updatingDocumentsWithParserLock) {
    }
    else {
    }
}

// parsersLock AND effectsLock must be locked before calling this function
void OscirenderAudioProcessorEditor::codeDocumentTextDeleted(int startIndex, int endIndex) {
    if (updatingDocumentsWithParserLock) {
    }
    else {
    }
}