Code-breaking with NFC Tags
==============

## Key Components
- A portable NFC tags scanner
- NFC tags

## Objective
- Scan the tags in the correct sequence (the code) to unlock a door

## The narrative

Despite the solar storm disabled the numpad of the door lock, a portable NFC tags scanner is still functional. The scanner was for the technician to check-in during regular equipment check-up. As a convenience, once the locations are checked in a correct sequence, the scanner will send a special radio command to open the door.

## Finding the sequence

### By melody

- Each tag can make the scanner play a musical note
- Hint: The correct melody is played from the noisy radio
  - the melody may as simple as from the lowest pitch to the highest pitch so that players don't need to rely on the hint
- Scan the tags in sequence 

### Location of the tags

Scattered in the escape room. On different equipment.

### Penalty (optional)

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

