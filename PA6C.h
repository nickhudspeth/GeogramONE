#include <avr/pgmspace.h>
#include <EEPROM.h>

#ifndef PA6C_h
#define PA6C_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#define GPRMC   			"GPRMC"
#define GPGGA   			"GPGGA"
#define GPGSA   			"GPGSA"
#define GPVTG   			"GPVTG"
#define GPGSV   			"GPGSV"
#define PMTKLOG 			"PMTKLOG"
#define PMTK001 			"PMTK001"
#define PMTK869 			"PMTK869"
#define PMTKLOX 			"PMTKLOX"

#define USEHDOP				1
#define USEVDOP				1
#define USEPDOP				1
#define USEALTITUDE			1
#define USESPEEDKNOTS		1
#define USECOURSE			1
#define USEMODE2			1
#define USESATINVIEW		0
#define USESATELLITESUSED	1
#define USEPOSITIONFIXIND	1
#define USEGSAMODE1			1
#define USEVTGMODE			1
#define USERMCMODE			1
#define USERMCSTATUS		1

#define GPSTIMEOUT			1100

#define KNOTSTOMPH			1.15078
#define KNOTSTOKPH			1.852
#define METERSTOFEET		3.2808

typedef const char prog_char;

struct geoFence
{
	uint8_t inOut;
	uint8_t reset;
	long latitude;
	long longitude;
	unsigned long radius;
};

struct configVar
{
	bool engMetric;
	uint8_t timeFormat;
	int8_t timeZone;
};


struct time
{
	uint8_t hour;
	uint8_t minute;
	uint8_t seconds;
	char amPM;
};
struct date
{
	uint8_t month;
	uint8_t day;
	uint8_t year;
};


struct gpsData
{
	uint8_t hour;
	uint8_t minute;
	uint8_t seconds;
	char amPM;
	uint8_t month;
	uint8_t day;
	uint16_t year;
	long latitude;
	long longitude;
#if USEALTITUDE
	float altitude;
#endif
#if USECOURSE
	uint16_t course;
	char courseDirection[3];
#endif
#if USESPEEDKNOTS	
	uint16_t speedKnots;
#endif
#if USEHDOP
	uint16_t hdop;
#endif
#if USEVDOP	
	uint16_t vdop;
#endif
#if USEPDOP
	uint16_t pdop;
#endif
#if USEMODE2
	uint8_t mode2;
#endif
#if USESATINVIEW
	uint8_t satInView;
#endif
#if USESATELLITESUSED
    uint8_t satellitesUsed;
#endif
#if USEPOSITIONFIXIND
	uint8_t positionFixInd;
#endif
#if USEGSAMODE1
	char gsaMode1;
#endif
#if USEVTGMODE
	char vtgMode;
#endif
#if USESPEEDKMR
	float speedKmr;
#endif
#if USERMCMODE
	char rmcMode;
#endif
#if USERMCSTATUS
	char rmcStatus;
#endif
};

class PA6C
{
	public:
		PA6C(HardwareSerial *ser);
		configVar settings;
		uint8_t getTheData(gpsData *);
		uint8_t init(unsigned long);
		uint8_t sleepGPS();
		uint8_t wakeUpGPS();
		uint8_t geoFenceDistance(gpsData *, geoFence *);
	private:
		struct GPS
		{
			char field[15];
			uint8_t checksum;
			uint8_t index;
			uint8_t returnStatus;
			unsigned long timeOut;
			uint8_t dataCollected;
		} gps;
		struct pmtkCommand
		{
			uint8_t p001Cmd;
			uint8_t p001Flag;
		} pmtk001;
		uint8_t getGPRMC(GPS *, gpsData *);
		uint8_t getGPGSV(GPS *, gpsData *);
		uint8_t getGPGGA(GPS *, gpsData *);
		uint8_t getGPGSA(GPS *, gpsData *);
		uint8_t getPMTK001(GPS *);
		uint8_t nextField(GPS *);
		uint8_t verifyChecksum(GPS *);
		uint8_t saveCoordinates(gpsData *);
		void lookForDollarSign(GPS *);
		void getSentenceId(GPS *, char *);
	#if USECOURSE
		void directionOfTravel(gpsData *);
	#endif
		HardwareSerial *gpsSerial;
};

#endif