/*
    ------------------------------------------------------------------

    This file is part of the Open Ephys GUI
    Copyright (C) 2012 Open Ephys

    ------------------------------------------------------------------

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.

*/


#include "SignalGeneratorEditor.h"
#include "../SignalGenerator.h"
#include "../../UI/EditorViewport.h"
#include <stdio.h>


SignalGeneratorEditor::SignalGeneratorEditor (GenericProcessor* parentNode) 
	: GenericEditor(parentNode), amplitudeSlider(0), frequencySlider(0), phaseSlider(0)

{
	desiredWidth = 270;

	int buttonWidth = 30;
	int buttonHeight = 20;

	for (int i = 0; i < 5; i++)
	{
		WaveformSelector* ws = new WaveformSelector(i);
		ws->setBounds(15 + (buttonWidth+3)*i, 30, buttonWidth, buttonHeight);
		ws->addListener(this);
		waveformSelectors.add(ws);
		addAndMakeVisible(ws);
	}

	amplitudeSlider = new Slider ("Amplitude Slider");
	amplitudeSlider->setBounds(10,60,50,60);
	amplitudeSlider->setRange(0,1000,10);
	amplitudeSlider->addListener(this);
	amplitudeSlider->setSliderStyle(Slider::Rotary);
	amplitudeSlider->setTextBoxStyle(Slider::TextBoxBelow, false, 40, 20);
	addAndMakeVisible(amplitudeSlider);

	frequencySlider = new Slider ("Frequency Slider");
	frequencySlider->setBounds(70,60,50,60);
	frequencySlider->setRange(1,1000,1);
	frequencySlider->addListener(this);
	frequencySlider->setSliderStyle(Slider::Rotary);
	frequencySlider->setTextBoxStyle(Slider::TextBoxBelow, false, 40, 20);
	addAndMakeVisible(frequencySlider);

	phaseSlider = new Slider ("Phase Slider");
	phaseSlider->setBounds(130,60,50,60);
	phaseSlider->setRange(0, 360, 1);
	phaseSlider->addListener(this);
	phaseSlider->setSliderStyle(Slider::Rotary);
	phaseSlider->setTextBoxStyle(Slider::TextBoxBelow, false, 40, 20);
	addAndMakeVisible(phaseSlider);

	numChannelsLabel = new Label("Number of Channels","1");
    numChannelsLabel->setEditable(true);
    numChannelsLabel->addListener(this);
    numChannelsLabel->setBounds(200,50,25,20);
    addAndMakeVisible(numChannelsLabel);

    upButton = new TriangleButton(1);
    upButton->addListener(this);
    upButton->setBounds(200,30,20,15);
    addAndMakeVisible(upButton);

    downButton = new TriangleButton(2);
    downButton->addListener(this);
    downButton->setBounds(200,75,20,15);
    addAndMakeVisible(downButton);

}

SignalGeneratorEditor::~SignalGeneratorEditor()
{
	deleteAllChildren();
}

void SignalGeneratorEditor::sliderValueChanged (Slider* slider)
{

	Array<int> chans = getActiveChannels();
	
	//std::cout << chans.size() << " channels selected." << std::endl;

	GenericProcessor* p = (GenericProcessor*) getAudioProcessor();

	for (int n = 0; n < chans.size(); n++) {
		
		p->setCurrentChannel(chans[n]);

		if (slider == amplitudeSlider)
			p->setParameter(0,slider->getValue());
		else if (slider == frequencySlider)
			p->setParameter(1,slider->getValue());
		else if (slider == phaseSlider)
			p->setParameter(2,slider->getValue());

	}


}

void SignalGeneratorEditor::buttonEvent (Button* button)
{

	for (int i = 0; i < waveformSelectors.size(); i++)
	{
		if (button == waveformSelectors[i])
		{

			Array<int> chans = getActiveChannels();
	
			GenericProcessor* p = getProcessor();

			for (int n = 0; n < chans.size(); n++) {
		
				p->setCurrentChannel(chans[n]);
				p->setParameter(3,(float) i);

			}


		}
	}

	int num = numChannelsLabel->getText().getIntValue();

    if (button == upButton)
    {
        numChannelsLabel->setText(String(++num), true);

    } else if (button == downButton)
    {

        if (num > 1)
            numChannelsLabel->setText(String(--num), true);

    }
}


void SignalGeneratorEditor::labelTextChanged (Label* label)
{

	SignalGenerator* sg = (SignalGenerator*) getProcessor();
	sg->nOut = numChannelsLabel->getText().getIntValue();
	getEditorViewport()->makeEditorVisible(this);
}

WaveformSelector::WaveformSelector(int type) : Button("Waveform")
{

	setRadioGroupId(299);
	setClickingTogglesState(true);

	if (type == 0)
	 	icon = ImageCache::getFromMemory (BinaryData::sine_wave_png, 
	 								    BinaryData::sine_wave_pngSize);
	else if (type == 1)
		icon = ImageCache::getFromMemory (BinaryData::square_wave_png, 
	 								    BinaryData::square_wave_pngSize);
	else if (type == 2)
		icon = ImageCache::getFromMemory (BinaryData::saw_wave_png, 
	 								    BinaryData::saw_wave_pngSize);
	else if (type == 3)
		icon = ImageCache::getFromMemory (BinaryData::triangle_wave_png, 
	 								    BinaryData::triangle_wave_pngSize);
	else if (type == 4)
		icon = ImageCache::getFromMemory (BinaryData::noise_wave_png, 
	 								    BinaryData::noise_wave_pngSize);

}

void WaveformSelector::paintButton(Graphics& g, bool isMouseOver, bool isButtonDown)
{
 

    if (getToggleState())
    {
        g.setColour(Colours::orange);
    } else {
        g.setColour(Colours::grey);
    }

    g.fillAll();

    g.setColour(Colours::black);
    g.drawRect(0, 0, getWidth(), getHeight());

    g.setImageResamplingQuality(Graphics::highResamplingQuality);
     g.drawImage(icon, 2, 2, getWidth()-4, getHeight()-4,
     			0, 0, icon.getWidth(), icon.getHeight());
}