const int LASER_PIN = 4;

void setupLaser() {
  pinMode(LASER_PIN, INPUT_PULLUP);
// do some setup
}

bool readLaser() {
  if (digitalRead(LASER_PIN) == LOW) {
    return true;
  } else {
    return false;
  }
}