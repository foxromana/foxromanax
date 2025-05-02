/*
  ==============================================================================

    FoxPresetComboBox.h
    Created: 2 May 2025 8:55:52pm
    Author:  juyeon hong

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
class FoxPresetComboBox  : public juce::ComboBox
{
    public:
        FoxPresetComboBox();
        ~FoxPresetComboBox() override;

    private:
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FoxPresetComboBox)
};
