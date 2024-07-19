#include <iostream>
#include <chrono>
#include <thread>
#include <ctime>
#include <numbers>
#include <resources/resources.h>
#include <math.h>

const time_t J2000_EPOCH = 946684800; // January 1, 2000, 12:00 noon UTC
const long double SECONDS_PER_DAY = 86400.0;

class Observer
{
	double latitude = latitudeObserver; //+ to N
	double longitude = longitudeObserver; //+ to E
	double height = heightObserver;

	void home(){
		while(!homeCheck()){
			// 
		}
	}
};

class Sun
{
	double declination{};
	double rightAscension{};
	double hourAngle{};
};

std::tm* UTCnow() {
    // Get the current time
    std::time_t t = std::time(nullptr);
    std::tm* now = std::gmtime(&t);
	 
	return now;
}

long double getJulianDate(std::tm* now) {
	// time is in UTC
    // Extract the year, month, and day
    int year = now->tm_year + 1900;
    int month = now->tm_mon + 1;
    int day = now->tm_mday;
    int hour = now->tm_hour;
    int minute = now->tm_min;
    int second = now->tm_sec;

    // If the month is January or February, subtract 1 from the year and add 12 to the month
    if (month <= 2) {
        year -= 1;
        month += 12;
    }

    // Calculate Julian Date
    int centuriesSinceEpoch = year / 100;
    int leapYearAdjustment = 2 - centuriesSinceEpoch + (centuriesSinceEpoch / 4);

    long double JD = static_cast<int>(365.25 * (year + 4716)) + static_cast<int>(30.6001 * (month + 1)) + day + leapYearAdjustment - 1524.5;
    JD += (hour + minute / 60.0 + second / 3600.0) / 24.0;

	// std::cout << std::format("{}", JD) << "\n";

    return JD;
}

long double getGMST(long double JD) {
	// source: https://www.omnicalculator.com/everyday-life/sidereal-time#how-to-calculate-sidereal-time-greenwich-sidereal-time-calculator-mean-and-apparent

    // Calculate Julian Century
	long double days_since_J2000 = JD - 2451545.0;
    long double JC = (days_since_J2000) / 36525.0;

	long double GMST_deg = 280.46061837 + 360.98564736629 * days_since_J2000 + (0.000387933 * JC*JC) - (JC*JC*JC / 38710000.0);
	GMST_deg = fmod(GMST_deg, 360.0);
	if (GMST_deg < 0) {
		GMST_deg += 360;
	}

	return GMST_deg;
}

void sunCoords(){
	using namespace std;
	// https://en.wikipedia.org/wiki/Position_of_the_Sun
	std::tm* now = UTCnow();
	long double JD = getJulianDate(now);

	// days = days since J2000
	long double days = JD - 2451545.0;
	// long double days = (difference / SECONDS_PER_DAY);
	cout << "Days since J2000: " << std::format("{}", days) << "\n";

	long double meanLongitude = 280.360L + 0.9856474L * days;
	meanLongitude = fmod(meanLongitude, 360);
	// cout << "Mean Longitude: " << meanLongitude << "\n";

	long double meanAnomaly = 357.528L + 0.9856003L * days;
	meanAnomaly = fmod(meanAnomaly, 360);
	long double meanAnomaly_rad = meanAnomaly * numbers::pi / 180.0L;
	// cout << "Mean Anomaly: " << meanAnomaly << "\n";

	long double eclipticLongitude = meanLongitude + 1.915L * sin(meanAnomaly_rad) + 0.2L * sin(2 * meanAnomaly_rad);
	long double eclipticLongitude_rad = eclipticLongitude * numbers::pi / 180.0L;
	// cout << "Ecliptic Longitude: " << eclipticLongitude << "\n";

	long double obliquity = 23.439L - 0.0000004L * days;
	long double obliquity_rad = obliquity * numbers::pi / 180.0L;
	// cout << "Obliquity: " << obliquity << "\n";

	long double rightAscension = atan2(cos(obliquity_rad) * sin(eclipticLongitude_rad), cos(eclipticLongitude_rad)) * 180.0 / numbers::pi;
	cout << "RightAscension: " << rightAscension << "\n";

	long double declination = asin(sin(obliquity_rad) * sin(eclipticLongitude_rad)) * 180.0 / numbers::pi;
	cout << "Declination: " << declination << "\n";


	long double gmst = getGMST(getJulianDate(now));
	cout << "GMST: " << gmst << " deg, or " << gmst / 15.0 << "h\n";
	long double lst = fmod(gmst + longitudeObserver, 360);
	cout << "LST: " << lst << "deg, or " << lst / 15.0 << "h\n";
	long double lha = lst - rightAscension;
	cout << "Hour Angle: " << lha << "deg \n";
}

int main()
{
	using namespace std::this_thread;
	using namespace std::chrono;
	
	// figure out where the sun is
	sunCoords();

	// figure out where the antenna is

	// track sun
}
