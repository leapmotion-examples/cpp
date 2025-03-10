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

class ImageButtonHandler  : public ButtonHandler
{
public:
    enum ImageRole
    {
        normalImage = 0,
        overImage = 1,
        downImage = 2
    };

    //==============================================================================
    ImageButtonHandler()
        : ButtonHandler ("Image Button", "ImageButton", typeid (ImageButton), 150, 24)
    {
    }

    Component* createNewComponent (JucerDocument*)
    {
        return new ImageButton ("new button");
    }

    void getEditableProperties (Component* component, JucerDocument& document, Array <PropertyComponent*>& properties)
    {
        ButtonHandler::getEditableProperties (component, document, properties);

        addColourProperties (component, document, properties);

        ImageButton* const ib = (ImageButton*) component;

        ComponentLayout& layout = *document.getComponentLayout();

        properties.add (new ImageButtonProportionProperty (layout, ib));

        properties.add (new ImageButtonResourceProperty (layout, ib, normalImage, "normal image"));
        properties.add (new ImageButtonOpacityProperty (layout, ib, "opacity", normalImage));
        properties.add (new ImageButtonColourProperty (layout, ib, "overlay col.", normalImage));

        properties.add (new ImageButtonResourceProperty (layout, ib, overImage, "over image"));
        properties.add (new ImageButtonOpacityProperty (layout, ib, "opacity", overImage));
        properties.add (new ImageButtonColourProperty (layout, ib, "overlay col.", overImage));

        properties.add (new ImageButtonResourceProperty (layout, ib, downImage, "down image"));
        properties.add (new ImageButtonOpacityProperty (layout, ib, "opacity", downImage));
        properties.add (new ImageButtonColourProperty (layout, ib, "overlay col.", downImage));
    }

    XmlElement* createXmlFor (Component* comp, const ComponentLayout* layout)
    {
        XmlElement* e = ButtonHandler::createXmlFor (comp, layout);

        ImageButton* const ib = (ImageButton*) comp;

        e->setAttribute ("keepProportions", doesImageKeepProportions (ib));

        e->setAttribute ("resourceNormal", getImageResource (ib, normalImage));
        e->setAttribute ("opacityNormal", getImageOpacity (ib, normalImage));
        e->setAttribute ("colourNormal", getImageColour (ib, normalImage).toString());

        e->setAttribute ("resourceOver", getImageResource (ib, overImage));
        e->setAttribute ("opacityOver", getImageOpacity (ib, overImage));
        e->setAttribute ("colourOver", getImageColour (ib, overImage).toString());

        e->setAttribute ("resourceDown", getImageResource (ib, downImage));
        e->setAttribute ("opacityDown", getImageOpacity (ib, downImage));
        e->setAttribute ("colourDown", getImageColour (ib, downImage).toString());

        return e;
    }

    bool restoreFromXml (const XmlElement& xml, Component* comp, const ComponentLayout* layout)
    {
        if (! ButtonHandler::restoreFromXml (xml, comp, layout))
            return false;

        ImageButton* const ib = (ImageButton*) comp;
        ComponentLayout& l = const_cast <ComponentLayout&> (*layout);

        setImageKeepProportions (l, ib, xml.getBoolAttribute ("keepProportions", true), false);

        setImageResource (l, ib, normalImage, xml.getStringAttribute ("resourceNormal", String::empty), false);
        setImageOpacity (l, ib, normalImage, (float) xml.getDoubleAttribute ("opacityNormal", 1.0f), false);
        setImageColour (l, ib, normalImage, Colour::fromString (xml.getStringAttribute ("colourNormal", "0")), false);

        setImageResource (l, ib, overImage, xml.getStringAttribute ("resourceOver", String::empty), false);
        setImageOpacity (l, ib, overImage, (float) xml.getDoubleAttribute ("opacityOver", 1.0f), false);
        setImageColour (l, ib, overImage, Colour::fromString (xml.getStringAttribute ("colourOver", "0")), false);

        setImageResource (l, ib, downImage, xml.getStringAttribute ("resourceDown", String::empty), false);
        setImageOpacity (l, ib, downImage, (float) xml.getDoubleAttribute ("opacityDown", 1.0f), false);
        setImageColour (l, ib, downImage, Colour::fromString (xml.getStringAttribute ("colourDown", "0")), false);

        return true;
    }

    void fillInCreationCode (GeneratedCode& code, Component* component, const String& memberVariableName)
    {
        ButtonHandler::fillInCreationCode (code, component, memberVariableName);

        ImageButton* const ib = dynamic_cast <ImageButton*> (component);

        String s;

        s << getColourIntialisationCode (component, memberVariableName)
          << '\n';

        const String indent (String::repeatedString (" ", memberVariableName.length() + 13));

        s << memberVariableName << "->setImages (false, true, "
          << CodeHelpers::boolLiteral (doesImageKeepProportions (ib)) << ",\n"
          << indent
          << getImageCreationCode (ib, normalImage) << ", "
          << CodeHelpers::floatLiteral (getImageOpacity (ib, normalImage), 3) << ", "
          << CodeHelpers::colourToCode (getImageColour (ib, normalImage)) << ",\n"
          << indent
          << getImageCreationCode (ib, overImage) << ", "
          << CodeHelpers::floatLiteral (getImageOpacity (ib, overImage), 3) << ", "
          << CodeHelpers::colourToCode (getImageColour (ib, overImage)) << ",\n"
          << indent
          << getImageCreationCode (ib, downImage) << ", "
          << CodeHelpers::floatLiteral (getImageOpacity (ib, downImage), 3) << ", "
          << CodeHelpers::colourToCode (getImageColour (ib, downImage))
          << ");\n";

        code.constructorCode += s;
    }

    static String getImageCreationCode (ImageButton* ib, const ImageRole role)
    {
        const String resName (getImageResource (ib, role));

        if (resName.isEmpty())
            return "Image()";

        return "ImageCache::getFromMemory (" + resName + ", " + resName + "Size)";
    }

    //==============================================================================
    class ImageButtonResourceProperty    : public ImageResourceProperty <ImageButton>
    {
    public:
        ImageButtonResourceProperty (ComponentLayout& layout_, ImageButton* const owner_, const ImageRole role_, const String& name)
            : ImageResourceProperty <ImageButton> (*layout_.getDocument(), owner_, name, true),
              role (role_),
              layout (layout_)
        {
        }

        void setResource (const String& newName)
        {
            setImageResource (layout, element, role, newName, true);
        }

        String getResource() const
        {
            return getImageResource (element, role);
        }

    private:
        const ImageRole role;
        ComponentLayout& layout;
    };

    class SetImageResourceAction   : public ComponentUndoableAction <ImageButton>
    {
    public:
        SetImageResourceAction (ImageButton* const button,
                                ComponentLayout& layout_,
                                const ImageRole role_,
                                const String& newResource_)
            : ComponentUndoableAction <ImageButton> (button, layout_),
              newResource (newResource_),
              role (role_),
              layout (layout_)
        {
            oldResource = ImageButtonHandler::getImageResource (button, role_);
        }

        bool perform()
        {
            showCorrectTab();
            ImageButtonHandler::setImageResource (layout, getComponent(), role, newResource, false);
            return true;
        }

        bool undo()
        {
            showCorrectTab();
            ImageButtonHandler::setImageResource (layout, getComponent(), role, oldResource, false);
            return true;
        }

    private:
        String newResource, oldResource;
        const ImageRole role;
        ComponentLayout& layout;
    };

    //==============================================================================
    static void setImageResource (ComponentLayout& layout, ImageButton* button, const ImageRole role, const String& newName, const bool undoable)
    {
        jassert (role < 3);

        if (role < 3 && getImageResource (button, role) != newName)
        {
            if (undoable)
            {
                layout.getDocument()->perform (new SetImageResourceAction (button, layout, role, newName),
                                               "Change image resource");
            }
            else
            {
                button->getProperties().set ("resource" + String ((int) role), newName);
                updateButtonImages (*layout.getDocument(), button);
                layout.changed();
            }
        }
    }

    static String getImageResource (ImageButton* button, const ImageRole role)
    {
        jassert (role < 3);
        return button->getProperties() ["resource" + String ((int) role)].toString();
    }

    //==============================================================================
    class SetImageKeepsPropAction   : public ComponentUndoableAction <ImageButton>
    {
    public:
        SetImageKeepsPropAction (ImageButton* const button,
                                 ComponentLayout& layout_,
                                 const bool newState_)
            : ComponentUndoableAction <ImageButton> (button, layout_),
              newState (newState_),
              layout (layout_)
        {
            oldState = ImageButtonHandler::doesImageKeepProportions (button);
        }

        bool perform()
        {
            showCorrectTab();
            ImageButtonHandler::setImageKeepProportions (layout, getComponent(), newState, false);
            return true;
        }

        bool undo()
        {
            showCorrectTab();
            ImageButtonHandler::setImageKeepProportions (layout, getComponent(), oldState, false);
            return true;
        }

    private:
        bool newState, oldState;
        ComponentLayout& layout;
    };

    static bool doesImageKeepProportions (ImageButton* button)
    {
        return button->getProperties().getWithDefault ("keepImageProp", true);
    }

    static void setImageKeepProportions (ComponentLayout& layout, ImageButton* button, const bool newState, const bool undoable)
    {
        if (undoable)
        {
            layout.perform (new SetImageKeepsPropAction (button, layout, newState), "change imagebutton proportion mode");
        }
        else
        {
            button->getProperties().set ("keepImageProp", newState);
            updateButtonImages (*layout.getDocument(), button);
            layout.changed();
        }
    }

    class ImageButtonProportionProperty    : public ComponentBooleanProperty <ImageButton>
    {
    public:
        ImageButtonProportionProperty (ComponentLayout& layout_, ImageButton* const owner_)
            : ComponentBooleanProperty <ImageButton> ("proportional", "maintain image proportions", "scale to fit",
                                                      owner_, *layout_.getDocument()),
              layout (layout_)
        {
        }

        void setState (bool newState)
        {
            setImageKeepProportions (layout, component, newState, true);
        }

        bool getState() const
        {
            return doesImageKeepProportions (component);
        }

    private:
        ComponentLayout& layout;
    };

    //==============================================================================
    class SetImageOpacityAction   : public ComponentUndoableAction <ImageButton>
    {
    public:
        SetImageOpacityAction (ImageButton* const button,
                               ComponentLayout& layout_,
                               const ImageRole role_,
                               const float newState_)
            : ComponentUndoableAction <ImageButton> (button, layout_),
              role (role_),
              newState (newState_),
              layout (layout_)
        {
            oldState = ImageButtonHandler::getImageOpacity (button, role_);
        }

        bool perform()
        {
            showCorrectTab();
            ImageButtonHandler::setImageOpacity (layout, getComponent(), role, newState, false);
            return true;
        }

        bool undo()
        {
            showCorrectTab();
            ImageButtonHandler::setImageOpacity (layout, getComponent(), role, oldState, false);
            return true;
        }

    private:
        const ImageRole role;
        float newState, oldState;
        ComponentLayout& layout;
    };

    static float getImageOpacity (ImageButton* button, const ImageRole role)
    {
        return (float) button->getProperties().getWithDefault ("imageOpacity" + String ((int) role), 1.0f);
    }

    static void setImageOpacity (ComponentLayout& layout, ImageButton* button, const ImageRole role, const float opacity, const bool undoable)
    {
        if (undoable)
        {
            layout.perform (new SetImageOpacityAction (button, layout, role, opacity), "change imagebutton opacity");
        }
        else
        {
            button->getProperties().set ("imageOpacity" + String ((int) role), opacity);
            updateButtonImages (*layout.getDocument(), button);
            layout.changed();
        }
    }

    class ImageButtonOpacityProperty    : public SliderPropertyComponent
    {
    public:
        ImageButtonOpacityProperty (ComponentLayout& layout_, ImageButton* const owner_,
                                    const String& name, const ImageRole role_)
            : SliderPropertyComponent (name, 0.0, 1.0, 0.0),
              owner (owner_),
              layout (layout_),
              role (role_)
        {
        }

        void setValue (double newValue)
        {
            setImageOpacity (layout, owner, role, (float) newValue, true);
        }

        double getValue() const
        {
            return getImageOpacity (owner, role);
        }

    private:
        ImageButton* const owner;
        ComponentLayout& layout;
        const ImageRole role;
    };

    //==============================================================================
    class SetImageColourAction   : public ComponentUndoableAction <ImageButton>
    {
    public:
        SetImageColourAction (ImageButton* const button,
                              ComponentLayout& layout_,
                              const ImageRole role_,
                              Colour newState_)
            : ComponentUndoableAction<ImageButton> (button, layout_),
              role (role_),
              newState (newState_),
              layout (layout_)
        {
            oldState = ImageButtonHandler::getImageColour (button, role_);
        }

        bool perform()
        {
            showCorrectTab();
            ImageButtonHandler::setImageColour (layout, getComponent(), role, newState, false);
            return true;
        }

        bool undo()
        {
            showCorrectTab();
            ImageButtonHandler::setImageColour (layout, getComponent(), role, oldState, false);
            return true;
        }

    private:
        const ImageRole role;
        Colour newState, oldState;
        ComponentLayout& layout;
    };

    static Colour getImageColour (ImageButton* button, const ImageRole role)
    {
        return Colour::fromString (button->getProperties().getWithDefault ("imageColour" + String ((int) role), "0").toString());
    }

    static void setImageColour (ComponentLayout& layout, ImageButton* button,
                                const ImageRole role, Colour colour, const bool undoable)
    {
        if (undoable)
        {
            layout.perform (new SetImageColourAction (button, layout, role, colour), "change imagebutton colour");
        }
        else
        {
            button->getProperties().set ("imageColour" + String ((int) role), colour.toString());
            updateButtonImages (*layout.getDocument(), button);
            layout.changed();
        }
    }

    class ImageButtonColourProperty    : public JucerColourPropertyComponent,
                                         public ChangeListener
    {
    public:
        ImageButtonColourProperty (ComponentLayout& layout_, ImageButton* const owner_,
                                   const String& name, const ImageRole role_)
            : JucerColourPropertyComponent (name, false),
              owner (owner_),
              layout (layout_),
              role (role_)
        {
            layout_.getDocument()->addChangeListener (this);
        }

        ~ImageButtonColourProperty()
        {
            layout.getDocument()->removeChangeListener (this);
        }

        void setColour (Colour newColour)
        {
            setImageColour (layout, owner, role, newColour, true);
        }

        Colour getColour() const
        {
            return getImageColour (owner, role);
        }

        void resetToDefault() {}

        void changeListenerCallback (ChangeBroadcaster*)
        {
            refresh();
        }

    private:
        ImageButton* const owner;
        ComponentLayout& layout;
        const ImageRole role;
    };

    //==============================================================================
    static void updateButtonImages (JucerDocument& document, ImageButton* const ib)
    {
        Image norm = document.getResources().getImageFromCache (getImageResource (ib, normalImage));
        Image over = document.getResources().getImageFromCache (getImageResource (ib, overImage));
        Image down = document.getResources().getImageFromCache (getImageResource (ib, downImage));

        ib->setImages (false, true, doesImageKeepProportions (ib),
                       norm,
                       getImageOpacity (ib, normalImage),
                       getImageColour (ib, normalImage),
                       over,
                       getImageOpacity (ib, overImage),
                       getImageColour (ib, overImage),
                       down,
                       getImageOpacity (ib, downImage),
                       getImageColour (ib, downImage));
    }
};
