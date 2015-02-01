/*
Sabertooth motor controller driver
Implements the packetized serial method of control
*/

#include "Sabertooth.h"

//Constructor
Sabertooth::Sabertooth()
{
  return;
}

Sabertooth::Sabertooth(unsigned char serialPort, int baud, unsigned char address)
{
  //Grab pointer for serial port, default to zero since it is always there
  switch (serialPort) {
    case 0:
    default:
      this->serialPort = &Serial;
      break;
    case 1:
      this->serialPort = &Serial1;
      break;
    case 2:
      this->serialPort = &Serial2;
      break;
    case 3:
      this->serialPort = &Serial3;
      break;
  }

  //Save address and baud
  this->address = address;
  this->baud = baud;

}

void Sabertooth::initSerialPort(void)
{
  this->serialPort->begin(this->baud,SERIAL_8N1);
}

void Sabertooth::sendSetupCommands(void)
{
  packet_t buf;

  buf.address = this->address;

  //Setup serial timeout of 0.5 seconds
  buf.command = PT_serialTimeout;
  buf.data = 5; //cmd in increments of 100ms
  CALC_CHECKSUM(buf);
  this->serialPort->write((unsigned char *)&buf, 4);

  //Setup zero deadband
  buf.command = PT_deadband;
  buf.data = 1; //Zero set deadband of 3, set it smaller
  CALC_CHECKSUM(buf);
  this->serialPort->write((unsigned char *)&buf, 4);

  //Setup ramping
  buf.command = PT_ramping;
  buf.data = 10; //Fastest ramp time
  CALC_CHECKSUM(buf);
  this->serialPort->write((unsigned char *)&buf, 4);

  //Max voltage - not needed for battery so leave at default

  //Min voltage - setup for 3.5v per cell (7v)
  buf.command = PT_serialTimeout;
  buf.data = 5; //Value = (desired volt - 6) * 5;
  CALC_CHECKSUM(buf);
  this->serialPort->write((unsigned char *)&buf, 4);
}

void Sabertooth::sendBaudCommand(baudrate_t baud)
{
  packet_t buf;

  buf.address = this->address;
  buf.command = PT_baudRate;
  buf.data = baud;
  CALC_CHECKSUM(buf);

  //Sweep through available baud rates and send the command twice with delay
  this->serialPort->begin(2400);
  delay(100);
  this->serialPort->write((unsigned char *)&buf, 4);
  this->serialPort->write((unsigned char *)&buf, 4);
  delay(200);

  this->serialPort->begin(9600);
  delay(100);
  this->serialPort->write((unsigned char *)&buf, 4);
  this->serialPort->write((unsigned char *)&buf, 4);
  delay(200);

  this->serialPort->begin(19200);
  delay(100);
  this->serialPort->write((unsigned char *)&buf, 4);
  this->serialPort->write((unsigned char *)&buf, 4);
  delay(200);

  this->serialPort->begin(38400);
  delay(100);
  this->serialPort->write((unsigned char *)&buf, 4);
  this->serialPort->write((unsigned char *)&buf, 4);
  delay(200);
}

void Sabertooth::updateMotor1(char speed)
{
  if (speed < 0) {
    this->speed1 = 255 + speed;
    this->direction1 = DD_BACK;
  }
  else {
    this->speed1 = speed;
    this->direction1 = DD_FWD;
  }
}

void Sabertooth::updateMotor2(char speed)
{
  if (speed < 0) {
    this->speed2 = 255 + speed;
    this->direction2 = DD_BACK;
  }
  else {
    this->speed2 = speed;
    this->direction2 = DD_FWD;
  }
}

void Sabertooth::updateBothMotors(char speed1, char speed2)
{
  if (speed1 < 0) {
    this->speed1 = 255 + speed1;
    this->direction1 = DD_BACK;
  }
  else {
    this->speed1 = speed1;
    this->direction1 = DD_FWD;
  }

  if (speed2 < 0) {
    this->speed2 = 255 + speed2;
    this->direction2 = DD_BACK;
  }
  else {
    this->speed2 = speed2;
    this->direction2 = DD_FWD;
  }
}

void Sabertooth::sendSpeedCommands(void)
{
  packet_t buf1;
  packet_t buf2;

  //Setup motor 1 packet
  buf1.address = this->address;
  buf1.command = PT_motorOneForward + this->direction1;
  buf1.data = this->speed1;
  CALC_CHECKSUM(buf1);

  //Setup motor 2 packet
  buf2.address = this->address;
  buf2.command = PT_motorTwoForward + this->direction2;
  buf2.data = this->speed2;
  CALC_CHECKSUM(buf2);

  //Write both packets to serial port
  this->serialPort->write((unsigned char *)&buf1, 4);
  this->serialPort->write((unsigned char *)&buf2, 4);
}

void Sabertooth::killMotors(void)
{
  this->speed1 = 0;
  this->speed2 = 0;
  this->direction1 = DD_FWD;
  this->direction2 = DD_FWD;
  this->sendSpeedCommands();
}


