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
int adress = 0;
int channel = 1;

int enc0CC, enc1CC, enc2CC, enc3CC, enc4CC, enc5CC, enc6CC, enc7CC;
int keyboardStatusCC, keyboardCodeCC;

void handleNoteOn(byte inChannel, byte inNote, byte inVelocity) {
  if(inNote == adress) {
    analogWrite(PIN_PC5, inVelocity * 8);
  }
  }

void setup() {
  MIDI.begin(0);
  MIDI.setHandleNoteOn(handleNoteOn);
  keyboard.begin(DataPin, IRQPin);
  keyboard.setNoRepeat(1);
  keyboard.setNoBreak(1);
  pinMode(PIN_PD4, INPUT_PULLUP);
  pinMode(DIP0, INPUT_PULLUP);
  pinMode(DIP1, INPUT_PULLUP);
  pinMode(DIP2, INPUT_PULLUP);
  pinMode(PIN_PC5, OUTPUT);


  bitWrite(adress, 0, digitalRead(DIP0));
  bitWrite(adress, 1, digitalRead(DIP1));
  bitWrite(adress, 2, digitalRead(DIP2));

  enc0CC = 0 + (12 * adress);
  enc1CC = 1 + (12 * adress);
  enc2CC = 2 + (12 * adress);
  enc3CC = 3 + (12 * adress);
  enc4CC = 4 + (12 * adress);
  enc5CC = 5 + (12 * adress);
  enc6CC = 6 + (12 * adress);
  enc7CC = 7 + (12 * adress);

  keyboardStatusCC = 8 + (12 * adress);
  keyboardCodeCC = 9 + (12 * adress);
}

int result = 0;
void loop() {


  if (digitalRead(PIN_PD4)) {
    result = en0.readAndReset();
    if (result != 0) { MIDI.sendControlChange(enc0CC, result + 64, channel); }
    result = en1.readAndReset();
    if (result != 0) { MIDI.sendControlChange(enc1CC, result + 64, channel); }
    result = en2.readAndReset();
    if (result != 0) { MIDI.sendControlChange(enc2CC, result + 64, channel); }
    result = en3.readAndReset();
    if (result != 0) { MIDI.sendControlChange(enc3CC, result + 64, channel); }
  } else {
    result = en0.readAndReset();
    if (result != 0) { MIDI.sendControlChange(enc4CC, result + 64, channel); }
    result = en1.readAndReset();
    if (result != 0) { MIDI.sendControlChange(enc5CC, result + 64, channel); }
    result = en2.readAndReset();
    if (result != 0) { MIDI.sendControlChange(enc6CC, result + 64, channel); }
    result = en3.readAndReset();
    if (result != 0) { MIDI.sendControlChange(enc7CC, result + 64, channel); }
  }


  if (keyboard.available()) {
    code = keyboard.read();
    if (code > 0) {
      MIDI.sendControlChange(keyboardStatusCC, code >> 8, 1);
      MIDI.sendControlChange(keyboardCodeCC, code & 0xFF, 1);
    }
  }

 MIDI.read();
    
}
