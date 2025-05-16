bool hasCompletedRotation() {
  static bool prevHigh = false;

  bool laserValue = readLaser();

  if (laserValue) {
    Serial.println("Laser hit!");
  }

  if (!prevHigh && laserValue) {
    prevHigh = true;
    Serial.println("Did rotation");
    return true;
  }

  prevHigh = readLaser();
  return false;
}
