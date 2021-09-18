//Tom Gough
//thomas.darren.gough@gmail.com
//2021-9-19

#include "MIDIUSB.h"

void noteOn(byte channel, byte pitch, byte velocity) {
  midiEventPacket_t noteOn = {0x09, 0x90 | channel, pitch, velocity};
  MidiUSB.sendMIDI(noteOn);
}

void noteOff(byte channel, byte pitch, byte velocity) {
  midiEventPacket_t noteOff = {0x08, 0x80 | channel, pitch, velocity};
  MidiUSB.sendMIDI(noteOff);
}

//num pins
const int pinCount=9;

//start digital pin, change this to an array for non sequential pins
const int startPin=2;

//release delay to prevent bouncing
const int releaseDelay=64;

//fixed default velocity until analog is implemented
const int velocity = 64;
int pin_state[pinCount];

//4,7,3,5,1,2,6,9,8
//int noteMap[pinCount] = {49,55,52,59,53,51,56,54,57};
int noteMap[pinCount] = {49,55,52,59,53,51,58,56,57};
int noteVelo[pinCount] = {64,64,64,80,80,80,80,80,64};

void setup() {
  for (int i = 0; i<pinCount; i++)
  {
    pinMode(startPin+i,INPUT_PULLUP);
    pin_state[i]=0;                                                               
  }
}

void scanPins()
{
  for (int i = 0; i<pinCount; i++)
  {
    bool state = digitalRead(startPin+i);
    if(state==true)
    {
      //off
      if(pin_state[i]>0)
      {
        if(pin_state[i]>=releaseDelay)
        {
          noteOff(0,noteMap[i],noteVelo[i]);
          pin_state[i]=0;
        }else{
          pin_state[i]=pin_state[i]+1;
      }
    }
    }else{
      if(pin_state[i]>0)
      {
        if(pin_state[i]<releaseDelay)
        {
          pin_state[i]=pin_state[i]+1;
        }
      }else{
        noteOn(0,noteMap[i],velocity);
        pin_state[i]=1;
      }
    }
  }
}

void loop() {
  scanPins();
  MidiUSB.flush();
  delay(1);
}
