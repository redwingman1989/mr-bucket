void readAD(uint8_t analogSignal) {
  
  switch (analogSignal) {
    case ANALOG_LEFT_IR:
      break;
    case ANALOG_RIGHT_IR:
      break;
    case ANALOG_SPARE:
      break;
    case ANALOG_RED:
      loadedBall.redValue = analogRead(ANALOG_RED);
      loadedBall.redValue >>= 2;
      break;
    case ANALOG_GREEN:
      loadedBall.greenValue = analogRead(ANALOG_GREEN);
      loadedBall.greenValue *= 1.25;
      loadedBall.greenValue >>= 2;
      break;
    case ANALOG_BLUE:
      loadedBall.blueValue = analogRead(ANALOG_BLUE);
      loadedBall.blueValue *= 1.75;
      loadedBall.blueValue >>= 2;
      break;
    default:
      break;
  }
}
