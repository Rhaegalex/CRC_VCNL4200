#if ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include <Wire.h>
#include "CRC_VCNL_4200.h"

CRC_VCNL4200::CRC_VCNL4200() { }


boolean CRC_VCNL4200::begin(uint8_t addr) {
	_i2caddr = addr;
	Wire.begin();

	uint8_t rev = read8(VCNL4200_PRODUCTID);
	//uint16_t rev = read16(VCNL4200_PRODUCTID);
	Serial.print("ProductID: ");
	Serial.println(rev, HEX);
	Serial.println(rev);
	if ((rev & 0xF0) != 0x20) {
		return false;
	}

	setLEDcurrent(20);
	setFrequency(VCNL4200_390K625);

	write8(VCNL4200_INTCONTROL, 0x08);
	return true;
}

void CRC_VCNL4200::setLEDcurrent(uint8_t c) {
	if (c > 20) c = 20;
	write8(VCNL4200_IRLED, c);
}

//uint8_t CRC_VCNL4200::getLEDcurrent(void) {
//	return read8(VCNL4200_IRLED);
//}

//void CRC_VCNL4200::setFrequency(VCNL4200_freq f) {
//	uint8_t r = read8(VCNL4200_MODTIMING);
//	r &= ~(0b00011000);
//	r |= f << 3;
//	write8(VCNL4200_MODTIMING, r);
//}

//uint16_t  CRC_VCNL4200::readProximity(void) {
//	uint8_t i = read8(VCNL4200_INTSTAT);
//	i &= ~0x80;
//	write8(VCNL4200_INTSTAT, i);
//
//	write8(VCNL4200_COMMAND, VCNL4200_MEASUREPROXIMITY);
//	while (1) {
//		//Serial.println(read8(VCNL4200_INTSTAT), HEX);
//		uint8_t result = read8(VCNL4200_COMMAND);
//		//Serial.print("Ready = 0x"); Serial.println(result, HEX);
//		if (result & VCNL4200_PROXIMITYREADY) {
//			return read16(VCNL4200_PROXIMITYDATA);
//		}
//		delay(1);
//	}
//}

//uint16_t  CRC_VCNL4200::readAmbient(void) {
//	uint8_t i = read8(VCNL4200_INTSTAT);
//	i &= ~0x40;
//	write8(VCNL4200_INTSTAT, i);
//
//
//	write8(VCNL4200_COMMAND, VCNL4200_MEASUREAMBIENT);
//	while (1) {
//		//Serial.println(read8(VCNL4200_INTSTAT), HEX);
//		uint8_t result = read8(VCNL4200_COMMAND);
//		//Serial.print("Ready = 0x"); Serial.println(result, HEX);
//		if (result & VCNL4200_AMBIENTREADY) {
//			return read16(VCNL4200_AMBIENTDATA);
//		}
//		delay(1);
//	}
//}

uint16_t CRC_VCNL4200::readData(uint8_t command_code)
{
	uint16_t data;

	Wire.beginTransmission(_i2caddr);
	Wire.write(command_code);
	Wire.endTransmission(false);
	Wire.requestFrom(_i2caddr, (uint8_t)2);
	while (!Wire.available());
	data = Wire.read();
	data <<= 8;
	while (!Wire.available());
	data |= Wire.read();
	return data;

	//int reg = 0x8; //Proximity Sensor (PS) output
	//Wire.beginTransmission(SENSOR_ADDR);
	//Wire.write(reg);
	//Wire.endTransmission(false);
	////Read data
	//byte prox[2] = { 0, 0 };  //The sensor provides the output over 2 bytes.
	//Wire.requestFrom(SENSOR_ADDR, 2);
	//prox[0] = Wire.read();
	//prox[1] = Wire.read();
	//int value = int(prox[1]) * 256 + int(prox[0]); //Combines bytes
	//Serial.print("Proximity: ");
	//Serial.println(value);
}

// write 1 byte
void CRC_VCNL4200::write8(uint8_t address, uint8_t data) //Original
{
	Wire.beginTransmission(_i2caddr);
	Wire.write(address);
	Wire.write(data);
	Wire.endTransmission();
}

uint8_t CRC_VCNL4200::write16_LowHigh(uint8_t address, uint8_t low, uint8_t high)
{
	Wire.beginTransmission(_i2caddr);
	Wire.write(address);
	Wire.write(low);
	Wire.write(high);
	Wire.endTransmission();
}
