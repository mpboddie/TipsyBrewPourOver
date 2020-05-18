// Don't change the pourCoord typedef
typedef struct {
  int angle;
  int distance;
} pourCoord;


double toRadians(double degree) {
  return (degree * PI / 180);
}

double toDegrees(double rad) {
  return (rad * 180 / PI);
}

double toServoSteps(double millimeters) {
  // Note: this is approximate and dependant on your exact setup
  // TODO: make customizable or eliminate the need in the future
  return (millimeters - 65.467) * 3.262;
}

double toMillimeters(double servoSteps) {
  // Note: this is approximate and dependant on your exact setup
  // TODO: make customizable or eliminate the need in the future
  return servoSteps / 3.262 + 65.467;
}

double reverseDistance(double distance) {
  // The servo is set up for a further distance to be a lower number sent to the servo
  // Reverse distance takes a servo value and flips it
  // The value used to do so is the mid-point of your servo settings
  // For instance, if your farthest point is servo value 20 and closest 180, then in the return statement below you will use ((180-20)/2)+20=100
  // continuing with the above example, the function if given 20 would return 180, 30 would return 170, etc.
  // TODO: eliminate the need for this is future revisions
  return 92.5 - (distance - 92.5);
}

double center(double distA, double distB) {
  double delta = distA - distB;
  return (abs(delta) / 2 + min(distA, distB));
}

double yCenter(double front, double back) {
  return center(front, back);
}

double xCenter(double left, double right) {
  return center(left, right);
}

double hypotenuse(double angle, double distance) {
  return (distance/cos(toRadians(angle)));
}

double SASdistance(double toCenter, double radius, double angle) {
  angle = toRadians(angle);
  return sqrt( sq(toCenter) + sq(radius) - (2*toCenter*radius*cos(angle)) );
}

double SSAngle(double radius, double centerAngle, double distance) {
  return toDegrees( asin( (radius*sin(toRadians(centerAngle))) / distance ) );
}

void drawCircle(double radius, int resolution, double brewDistance, int servoMid, pourCoord path[]) {
  float angleIncrement = 360/resolution;
  for (int i=0; i<resolution; i++) {
    double distance = SASdistance(brewDistance, radius, i*angleIncrement);
    double angle = SSAngle(radius, i*angleIncrement, distance) + servoMid;
    distance = reverseDistance(distance);
    path[i] = {(int)angle, (int)distance};
  }
}
