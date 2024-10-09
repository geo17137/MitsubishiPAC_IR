#ifndef IR_COMMAND_H
#define IR_COMMAND_H

#include <IRremoteESP8266.h>
#include <IRsend.h>
#include <ir_Mitsubishi.h>

#define N_PARAM 5

class IrCommand {

  private:
    String param;
    int onOff;
    int temp;
    int fan;
    int mode;
    int vanne;
    
    IRMitsubishiAC *ac; // Set the GPIO used for sending messages.

  public:
    IrCommand(const uint16_t kIrLed) {
      ac = new IRMitsubishiAC(kIrLed);
      ac->begin();
    }

    inline void off(){
      ac->off();
      ac->send();
    }

    inline void on() {
     ac->on();
     ac->send();
    }

    inline void setOnOff() {
      if (onOff)
        ac->on();
      else
        ac->off();
    }

    inline void setOnOff(int onOff) {
      if (onOff)
        ac->on();
      else
        ac->off();
      ac->send();
    }

    inline void setTemp(int temp) {
      ac->setTemp(temp);
      ac->send();
    }

    inline void setFan(int fan) {
      ac->setFan(fan);
      ac->send();
    }

    inline void setMode(int mode) {
      ac->setMode(mode);
      ac->send();
    }

    inline void setVanne(int vanne) {
      ac->setVane(vanne);
      ac->send();
    }

    inline void setTemp() {
      ac->setTemp(temp);
    }

    inline void setFan() {
      ac->setFan(fan);
    }

   inline void setMode() {
    ac->setMode(mode);
   }

   inline void setVanne() {
    ac->setVane(vanne);
   }

   inline void send() {
    ac->send();
   }
   
   inline String paramGet() {
    return param;
   }

   void setParam(String param);
   void printState();
  
   inline void apply() {
    setTemp();
    setFan();
    setMode();
    setVanne();
    setOnOff();
    ac->send();
   }
};



#endif