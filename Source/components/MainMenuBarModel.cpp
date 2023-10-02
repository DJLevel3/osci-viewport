#include "MainMenuBarModel.h"
#include "../PluginEditor.h"

juce::StringArray MainMenuBarModel::getMenuBarNames() {
    return juce::StringArray("File");
}

juce::PopupMenu MainMenuBarModel::getMenuForIndex(int topLevelMenuIndex, const juce::String& menuName) {
    juce::PopupMenu menu;
    menu.addItem(1, "Open");
    menu.addItem(2, "Save");
    menu.addItem(3, "Save As");
    return menu;
}

void MainMenuBarModel::menuItemSelected(int menuItemID, int topLevelMenuIndex) {
    switch (menuItemID) {
        default:
            break;
    }
}

void MainMenuBarModel::menuBarActivated(bool isActive) {}
