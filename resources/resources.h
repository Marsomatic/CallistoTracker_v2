static const double latitudeObserver = 45.276055;
static const double longitudeObserver = 13.721878;
static const double heightObserver = 226;

static const int stepPin = 3;
static const int dirPin = 5;
static const int homePin = 7;

static const int stepsPerRev = 12800; // 200 * microsteps
static const int homingSpeed = 60;

void moveSteps();
bool homeCheck();
