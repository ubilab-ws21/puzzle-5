Code-breaking with NFC Tags
==============

## Key Components
- A portable NFC tags scanner
- NFC tags

## Objective
- Scan the tags in the correct sequence (the code)
- Possible consequence:
    - Open the emergency exit
    - Open the door to the control room
    - Unlock another pizzle
        - Display a passcode on the screen, which is the key for another pizzle

## The narrative

**A special key for the emergency exit:**

Despite the solar storm disabled the numpad of the door lock, a portable NFC tags scanner is still functional. The scanner was for the technician to check-in during regular equipment check-up. As a convenience, once the locations are checked in a correct sequence, the scanner will send a special radio command to open the door.

## What's the sequence?

**Melody as the sequence**

- Each tag can make the scanner's buzzer play a tone
- A melody is form by scanning the cards in order

## How to find the sequence?

**Option 1: Emergency radio**

The looping emergency announcment / noise contain the melody. Players should notice that.

**Option 2: Noise from the control room**

The control panel in the control room keeps playing some tones, which is the correct sequence.

## Location of the tags

**Option 1: Fixed on the equipment**

Scattered in the escape room. On different equipment.

**Option 2: Collect the NFC cards from other pizzles**

Players don't need to keep walking around

Need to be coherent with other pizzles (why cards would appear in some pizzles?)

## Penalty (optional)

If a tag out-of-sequence is being scanned, the battery level of the scanner will decrease.

**Game-over:** Battery level of the scanner drops to 0.

**Additional material:**

- LCD display or some LEDs on the scanner to indicate the battery level.

## A little bit more detailed BOM

- A portable NFC tag scanner, consist of:
    - MCU (ESP32)
    - NFC reader
    - buzzer (play a note when scanning a tag)
    - Battery / Power bank
    - Enclosure
    - (LCD display / LEDs)
    - Substitution: A mobile phone with NFC (but the development will be building an mobile APP -> not so fun)
- Some NFC tags
- Radio:
    - MP3 player + Speaker, or
    - Build our own audio player with:
        - ESP32
        - ES8388
        - SD card adapter
        - Speaker

