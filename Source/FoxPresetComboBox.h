/*
  ==============================================================================

    FoxPresetComboBox.h
    Created: 2 May 2025 8:55:52pm
    Author:  juyeon hong

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "FoxPresetManager.h"

//==============================================================================
/*
*/
class FoxPresetComboBox  : public juce::ComboBox
{
    public:
        FoxPresetComboBox(FoxPresetManager& inPresetManager);
        ~FoxPresetComboBox() override;

    private:
        FoxPresetManager& mPresetManager;
        void resetByFactoryPreset() noexcept;
    
        std::unique_ptr<juce::FileChooser> mFileChooser;
        void resetByXmlPreset() noexcept;
        enum IdComboBoxPreset
        {
            Save = 1,
            Load = 2
        };
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FoxPresetComboBox)
};
