#include "MainComponent.h"
#include "parser/FileParser.h"
#include "parser/FrameProducer.h"
#include "PluginEditor.h"

MainComponent::MainComponent(OscirenderAudioProcessor& p, OscirenderAudioProcessorEditor& editor) : audioProcessor(p), pluginEditor(editor) {
	setText("Main Settings");

    addAndMakeVisible(fileButton);
    fileButton.setButtonText("Choose File(s)");
    
	fileButton.onClick = [this] {
		chooser = std::make_unique<juce::FileChooser>("Open", juce::File::getSpecialLocation(juce::File::userHomeDirectory), "*.obj;*.svg;*.lua;*.txt");
		auto flags = juce::FileBrowserComponent::openMode | juce::FileBrowserComponent::canSelectMultipleItems;

		chooser->launchAsync(flags, [this](const juce::FileChooser& chooser) {
			for (auto& url : chooser.getURLResults()) {
				if (url.isLocalFile()) {
					auto file = url.getLocalFile();
					audioProcessor.addFile(file);
				}
			}
			pluginEditor.updateCodeEditor();
			updateFileLabel();
		});
	};

	addAndMakeVisible(closeFileButton);
	closeFileButton.setButtonText("Close File");
	
	closeFileButton.onClick = [this] {
		audioProcessor.removeFile(audioProcessor.getCurrentFileIndex());
		pluginEditor.updateCodeEditor();
		updateFileLabel();
	};

	addAndMakeVisible(fileLabel);
	updateFileLabel();
}

MainComponent::~MainComponent() {
}

void MainComponent::updateFileLabel() {
	if (audioProcessor.getCurrentFileIndex() == -1) {
		fileLabel.setText("No file open", juce::dontSendNotification);
		return;
	}
	
	fileLabel.setText(audioProcessor.getCurrentFile().getFileName(), juce::dontSendNotification);
}

void MainComponent::resized() {
	auto baseYPadding = 10;
	auto xPadding = 10;
	auto yPadding = 10;
	auto buttonWidth = 120;
	auto buttonHeight = 40;
    fileButton.setBounds(xPadding, baseYPadding + yPadding, buttonWidth, buttonHeight);
	closeFileButton.setBounds(xPadding, baseYPadding + yPadding + buttonHeight + yPadding, buttonWidth, buttonHeight);
	fileLabel.setBounds(xPadding + buttonWidth + xPadding, baseYPadding + yPadding, getWidth() - xPadding - buttonWidth - xPadding, buttonHeight);
}
