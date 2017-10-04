#ifndef Led_h
#define Led_h

class Led
{
  public:
    Led(int rPin, int gPin, int bPin);
    
    void setup();
    void light(int pin, int brightness);
    void lightR(int brightness);
    void lightG(int brightness);
    void lightB(int brightness);
    
  private:
    int rPin;
    int gPin;
    int bPin;
};

#endif