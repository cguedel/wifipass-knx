#include <FlashAsEEPROM.h>

byte readMemory(int index) {
    Debug.println(F("FLASH read on index %d"),index);
    return EEPROM.read(index);
}

void writeMemory(int index, byte val) {
    Debug.println(F("FLASH write value %d on index %d"),val, index);
    EEPROM.write(index, val);
}

void updateMemory(int index, byte val) {
    Debug.println(F("FLASH update"));
    if (EEPROM.read(index) != val) {
        EEPROM.write(index, val);
    }
}

void commitMemory() {
    Debug.println(F("FLASH commit"));
    EEPROM.commit();
}