#ifndef Sbus_h
#define Sbus_h

#include "Arduino.h"

class Sbus
{
  public:
	Sbus(HardwareSerial &bus);
	void begin();
	bool read(uint16_t *channels, uint8_t *failsafe, uint16_t *lostFrames);
	bool readCal(float *calChannels, uint8_t *failsafe, uint16_t *lostFrames);
	void write(uint16_t *channels);

  private:
	uint8_t _fpos;
	const uint16_t Sbus_TIMEOUT = 10000;
	const float _sbusScale = 0.00122025625f;
	const float _sbusBias = -1.2098840f;
	const uint8_t _sbusHeader = 0x0F;
	const uint8_t _sbusFooter = 0x00;
	const uint8_t _sbus2Footer = 0x04;
	const uint8_t _sbusLostFrame = 0x04;
	const uint8_t _sbusFailSafe = 0x08;
	static const uint8_t _payloadSize = 24;
	uint8_t _payload[_payloadSize];
	HardwareSerial *_bus;

	bool parse();
};

#endif
