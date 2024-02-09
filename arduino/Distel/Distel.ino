// Pins
// PD0 MIDI RX
// PD1 MIDI TX
// PD2 PS/2 Data
// PD3 PS/2 CLOCK
// PD5 Enc0 A
// PD6 Enc0 B
// PD7 Enc1 A
// PB0 Enc1 B
// PB1 Enc2 A
// PB2 Enc2 B
// PC0 Enc3 A
// PC1 Enc3 B
// PD4 Switch

#define DIP0 PIN_PC2
#define DIP1 PIN_PC3
#define DIP2 PIN_PC4


// PC2 DIP0
// PC3 DIP1
// PC4 DIP2
// PC5 Vibration Motor

#define ENCODER_DO_NOT_USE_INTERRUPTS
#include <Encoder.h>
#include <PS2KeyAdvanced.h>
#include <MIDI.h>


#define DataPin PIN_PD2
#define IRQPin PIN_PD3


PS2KeyAdvanced keyboard;
MIDI_CREATE_DEFAULT_INSTANCE();

Encoder en0(PIN_PD5, PIN_PD6);
Encoder en1(PIN_PD7, PIN_PB0);
Encoder en2(PIN_PB1, PIN_PB2);
Encoder en3(PIN_PC0, PIN_PC1);

uint16_t code;
byte adress;

int enc0CC, enc1CC, enc2CC, enc3CC, enc4CC, enc5CC, enc6CC, enc7CC;
int keyboardStatusCC, keyboardCodeCC;

void handleNoteOn(byte inChannel, byte inNote, byte inVelocity) {
    analogWrite(PIN_PC5, inVelocity * 8);
  }
void handleNoteOff(byte inChannel, byte inNote, byte inVelocity) {
    digitalWrite(PIN_PC5, LOW);
  }

void setup() {
  pinMode(PIN_PD4, INPUT_PULLUP);
  pinMode(DIP0, INPUT_PULLUP);
  pinMode(DIP1, INPUT_PULLUP);
  pinMode(DIP2, INPUT_PULLUP);
  pinMode(PIN_PC5, OUTPUT);
  bitWrite(adress, 0, digitalRead(DIP0));
  bitWrite(adress, 1, digitalRead(DIP1));
  bitWrite(adress, 2, digitalRead(DIP2));
  bitWrite(adress, 3, LOW);
  bitWrite(adress, 4, LOW);
  bitWrite(adress, 5, LOW);
  bitWrite(adress, 6, LOW);
  bitWrite(adress, 7, LOW);

  MIDI.begin(adress);
  MIDI.setThruFilterMode(midi::Thru::DifferentChannel); // Different Channel
  MIDI.setHandleNoteOn(handleNoteOn);
  MIDI.setHandleNoteOff(handleNoteOff);
  keyboard.begin(DataPin, IRQPin);
  keyboard.setNoRepeat(1);
  keyboard.setNoBreak(1);
  keyboardStatusCC = 8;
  keyboardCodeCC = 9;
}

int result = 0;
void loop() {


  if (digitalRead(PIN_PD4)) {
    result = en0.readAndReset();
    if (result != 0) { MIDI.sendControlChange(0, 64 - result, adress); }
    result = en1.readAndReset();
    if (result != 0) { MIDI.sendControlChange(1, 64 - result, adress); }
    result = en2.readAndReset();
    if (result != 0) { MIDI.sendControlChange(2, 64 - result, adress); }
    result = en3.readAndReset();
    if (result != 0) { MIDI.sendControlChange(3, 64 - result, adress); }
  } else {
    result = en0.readAndReset();
    if (result != 0) { MIDI.sendControlChange(4, 64 - result, adress); }
    result = en1.readAndReset();
    if (result != 0) { MIDI.sendControlChange(5, 64 - result, adress); }
    result = en2.readAndReset();
    if (result != 0) { MIDI.sendControlChange(6, 64 - result, adress); }
    result = en3.readAndReset();
    if (result != 0) { MIDI.sendControlChange(7, 64 - result, adress); }
  }


  if (keyboard.available()) {
    code = keyboard.read();
    if (code > 0) {
     // MIDI.sendNoteOn(code >> 8, code & 0xFF, adress);
     MIDI.sendControlChange(keyboardStatusCC, code >> 8, adress);
      MIDI.sendControlChange(keyboardCodeCC, code & 0xFF, adress);
    }
  }

 MIDI.read();
    
}
