/*
  ==============================================================================

   This file is part of the JUCE library.
   Copyright (c) 2013 - Raw Material Software Ltd.

   Permission is granted to use this software under the terms of either:
   a) the GPL v2 (or any later version)
   b) the Affero GPL v3

   Details of these licenses can be found at: www.gnu.org/licenses

   JUCE is distributed in the hope that it will be useful, but WITHOUT ANY
   WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
   A PARTICULAR PURPOSE.  See the GNU General Public License for more details.

   ------------------------------------------------------------------------------

   To release a closed-source product which uses JUCE, commercial licenses are
   available: visit www.juce.com for more information.

  ==============================================================================
*/

#ifndef __JUCER_APPEARANCESETTINGS_JUCEHEADER__
#define __JUCER_APPEARANCESETTINGS_JUCEHEADER__


class AppearanceSettings    : private ValueTree::Listener
{
public:
    AppearanceSettings (bool updateAppWhenChanged);

    bool readFromFile (const File& file);
    bool readFromXML (const XmlElement&);
    bool writeToFile (const File& file) const;

    void updateColourScheme();
    void applyToCodeEditor (CodeEditorComponent& editor) const;

    StringArray getColourNames() const;
    Value getColourValue (const String& colourName);
    bool getColour (const String& name, Colour& resultIfFound) const;

    Font getCodeFont() const;
    Value getCodeFontValue();

    ValueTree settings;

    static File getSchemesFolder();
    StringArray getPresetSchemes();
    void refreshPresetSchemeList();
    void selectPresetScheme (int index);

    static Font getDefaultCodeFont();

    static void showEditorWindow (ScopedPointer<Component>& ownerPointer);

    static const char* getSchemeFileSuffix()      { return ".scheme"; }
    static const char* getSchemeFileWildCard()    { return "*.scheme"; }

private:

    Array<File> presetSchemeFiles;

    static void writeDefaultSchemeFile (const String& xml, const String& name);

    void applyToLookAndFeel (LookAndFeel&) const;

    void valueTreePropertyChanged (ValueTree&, const Identifier&) override   { updateColourScheme(); }
    void valueTreeChildAdded (ValueTree&, ValueTree&) override               { updateColourScheme(); }
    void valueTreeChildRemoved (ValueTree&, ValueTree&) override             { updateColourScheme(); }
    void valueTreeChildOrderChanged (ValueTree&) override                    { updateColourScheme(); }
    void valueTreeParentChanged (ValueTree&) override                        { updateColourScheme(); }
    void valueTreeRedirected (ValueTree&) override                           { updateColourScheme(); }

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AppearanceSettings)
};

//==============================================================================
class IntrojucerLookAndFeel   : public LookAndFeel
{
public:
    IntrojucerLookAndFeel();

    void fillWithBackgroundTexture (Graphics&);
    static void fillWithBackgroundTexture (Component&, Graphics&);

    int getTabButtonOverlap (int tabDepth) override;
    int getTabButtonSpaceAroundImage() override;
    int getTabButtonBestWidth (TabBarButton&, int tabDepth) override;
    static Colour getTabBackgroundColour (TabBarButton&);
    void drawTabButton (TabBarButton& button, Graphics&, bool isMouseOver, bool isMouseDown) override;

    Rectangle<int> getTabButtonExtraComponentBounds (const TabBarButton&, Rectangle<int>& textArea, Component&) override;
    void drawTabAreaBehindFrontButton (TabbedButtonBar&, Graphics&, int, int) override {}

    void drawStretchableLayoutResizerBar (Graphics&, int w, int h, bool isVerticalBar, bool isMouseOver, bool isMouseDragging) override;
    Rectangle<int> getPropertyComponentContentPosition (PropertyComponent&) override;

    bool areScrollbarButtonsVisible() override   { return false; }

    void drawScrollbar (Graphics&, ScrollBar&, int x, int y, int width, int height, bool isScrollbarVertical,
                        int thumbStartPosition, int thumbSize, bool isMouseOver, bool isMouseDown) override;

    void drawConcertinaPanelHeader (Graphics&, const Rectangle<int>& area, bool isMouseOver, bool isMouseDown,
                                    ConcertinaPanel&, Component&) override;

    void drawButtonBackground (Graphics&, Button&, const Colour& backgroundColour,
                               bool isMouseOverButton, bool isButtonDown) override;

    void drawTableHeaderBackground (Graphics&, TableHeaderComponent&) override;

    static Colour getScrollbarColourForBackground (Colour background);

private:
    Image backgroundTexture;
    Colour backgroundTextureBaseColour;
};


#endif   // __JUCER_APPEARANCESETTINGS_JUCEHEADER__
