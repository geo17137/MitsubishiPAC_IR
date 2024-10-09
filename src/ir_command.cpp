
#include "ir_command.h"
/*
  Construit un tableau d'ItemParam
*/
void IrCommand::setParam(String str) {
  param = str; 
  int index = -1;
  str = str.substring(index + 1);
  index = str.indexOf(':');
  onOff = str.substring(0, index) == "1";
  str = str.substring(index + 1);
  index = str.indexOf(':');
  temp = atoi(str.substring(0, index).c_str());
  str = str.substring(index + 1);
  index = str.indexOf(':');
  fan = atoi(str.substring(0, index).c_str());
  str = str.substring(index + 1);
  index = str.indexOf(':');
  mode = atoi(str.substring(0, index).c_str());
  str = str.substring(index + 1);
  index = str.indexOf(':');
  vanne = atoi(str.substring(0, index).c_str());
  // Serial.printf("temp:%d, fan:%d, mode:%d, vanne:%d\n", temp, fan, mode, vanne);
}

void IrCommand::printState() {
  // Display the settings.
  Serial.println("Mitsubishi A/C remote is in the following state:");
  Serial.printf("  %s\n", ac->toString().c_str());
  // Display the encoded IR sequence.
  unsigned char *ir_code = ac->getRaw();
  Serial.print("IR Code: 0x");
  for (uint8_t i = 0; i < kMitsubishiACStateLength; i++)
    Serial.printf("%02X", ir_code[i]);
  Serial.println();
}
