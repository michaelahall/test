// localTime.c
// A program to test a function getLocalTime which determines the
// local time in a specified city, given the current UTC time.
//
// This program by Michaela Hall (z5117328) on 2017-08-24
//
// Program stub created by Andrew Bennett on 2017-08-01.
// Version 1.0.5: Fix typo in Lord Howe Daylight Time.
// Version 1.0.4: Fix incorrect Lord Howe Island unit test.
// Version 1.0.3: Add version numbers.
// Version 1.0.2: Fixed include order.
// Version 1.0.1: Fix `CITY_LORD_HOWE_IS` symbol name.
// Version 1.0.0: First release onto an unsuspecting class.

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#define TRUE  1
#define FALSE !TRUE

// ===========      Cities      ===========

// Australia
#define CITY_ADELAIDE       0
#define CITY_BRISBANE       1
#define CITY_BROKEN_HILL    2
#define CITY_CANBERRA       3
#define CITY_DARWIN         4
#define CITY_EUCLA          5
#define CITY_HOBART         6
#define CITY_LORD_HOWE_IS   7
#define CITY_MELBOURNE      8
#define CITY_PERTH          9
#define CITY_SYDNEY         10

// New Zealand
#define CITY_AUCKLAND       11
#define CITY_CHRISTCHURCH   12
#define CITY_WELLINGTON     13

// =========== Timezone Offsets ===========

// Australia
#define TIMEZONE_AWST  800 // Australian Western Standard Time

#define TIMEZONE_ACWST 845 // Australian Central Western Standard Time

#define TIMEZONE_ACST  930 // Australian Central Standard Time
#define TIMEZONE_ACDT 1030 // Australian Central Daylight Time

#define TIMEZONE_AEST 1000 // Australian Eastern Standard Time
#define TIMEZONE_AEDT 1100 // Australian Eastern Daylight Time

#define TIMEZONE_LHST 1030 // Lord Howe Standard Time
#define TIMEZONE_LHDT 1100 // Lord Howe Daylight Time

// New Zealand
#define TIMEZONE_NZST 1200 // NZ Standard Time
#define TIMEZONE_NZDT 1300 // NZ Daylight Time


// =========== Function Prototypes ===========
//       [DO NOT CHANGE THIS PROTOTYPE!]
int getLocalTime (int city, int day, int month, int timeUTC);

int getTimeZoneOffset (int city);

int checkIfNewZealand (int city);

int checkIfACST (int city);

int daylightSaving (int city, int day, int month, int rawLocalTime);

int daylightSavingLordHoweIs (int day, int month, int rawLocalTime);

int daylightSavingZone (int city, int timeZoneOffset);

int checkDayChange (int localTime);

int checkMonthChange (int day, int month);

int getDaysInMonth (int month);

int getLastTwoDigits (int stillRawLocalTime);

int addTime (int fourDigitTime, int fourDigitTimeDelta);

int main (int argc, char *argv[]) {

    // UTC time: 8:59am on 15th May, time in Christchurch: 8:59pm
    assert(getLocalTime(CITY_CHRISTCHURCH, 15, 5, 859) == 2059);

    // UTC time: midnight on 5th April, time in Sydney: 10am
    assert(getLocalTime(CITY_SYDNEY, 5, 4, 0) == 1000);

    // UTC time: 10:30am on 30th June, time in Adelaide: 8pm
    assert(getLocalTime(CITY_ADELAIDE, 30, 6, 1030) == 2000);

    // UTC time: 8:15pm on 17th August, time in Brisane: 6:15am
    assert(getLocalTime(CITY_BRISBANE, 17, 8, 2015) == 615);

    // UTC time: 8am on 27th April, time in Auckland: 8pm
    assert(getLocalTime(CITY_AUCKLAND, 27, 4, 800) == 2000);

    // UTC time: 11:11pm on 30th January, time in Perth: 7:11am on the 31st
    assert(getLocalTime(CITY_PERTH, 30, 1, 2311) == 711);

    // UTC time: 8pm on 5th May, time in Perth: 4am on the 6th
    assert(getLocalTime(CITY_PERTH, 2, 5, 2000) == 400);

    // UTC time: 10:30pm on 6th Oct, time in Christchurch: 11:30am on the 7th
    assert(getLocalTime(CITY_CHRISTCHURCH, 6, 10, 2230) == 1130);

    // UTC time: 2:28pm on 16th December, time in Canberra: 1:28am on the 17th
    assert(getLocalTime(CITY_CANBERRA, 16, 12, 1428) == 128);

    // UTC time: 3:39am on 1st November, time in Darwin: 1:09pm
    assert(getLocalTime(CITY_DARWIN, 1, 11, 339) == 1309);

    // UTC time: 1pm on 8th July, time in Hobart: 11pm
    assert(getLocalTime(CITY_HOBART, 8, 7, 1300) == 2300);

    // UTC time: 1:54pm on 27th September, time in Melbourne: 11:54pm
    assert(getLocalTime(CITY_MELBOURNE, 27, 9, 1354) == 2354);

    // UTC time: 9:24am on 5th January, time in Wellington: 10:24pm
    assert(getLocalTime(CITY_WELLINGTON, 5, 1, 924) == 2224);

    // UTC time: 2pm on 18th April, time in Brisbane: 12:00am on the 19th
    assert(getLocalTime(CITY_BRISBANE, 18, 4, 1400) == 0);

    // UTC time: 11:06am on 8th October, time in Brisbane: 9:06pm
    assert(getLocalTime(CITY_BRISBANE, 8, 10, 1106) == 2106);

    // UTC time: 12:01pm on 29th October, time in Sydney: 11:01pm
    assert(getLocalTime(CITY_SYDNEY, 29, 10, 1201) == 2301);

    // UTC time: 11:45pm on 17th February, time in Darwin: 9:15am on the 18th
    assert(getLocalTime(CITY_DARWIN, 17, 2, 2345) == 915);

    // UTC time: 11pm on 7th August, time in Auckland: 10am on the 8th
    assert(getLocalTime(CITY_AUCKLAND, 7, 8, 2200) == 1000);

    // UTC time: 10:15 am on 5th September, time in Eucla: 7pm
    assert(getLocalTime(CITY_EUCLA, 5, 9, 1015) == 1900);

    // UTC time: 10am on 1st January, time in Broken Hill: 8:30pm
    assert(getLocalTime(CITY_BROKEN_HILL, 1, 1, 1000) == 2030);

    // UTC time: 10:15pm on 1st October, time in Lord Howe Island: 9:15am on the 2nd
    assert(getLocalTime(CITY_LORD_HOWE_IS, 1, 10, 2215) == 915);

    // UTC time: 11:59pm on 6th February, time in Eucla: 8:44am on the 7th
    assert(getLocalTime(CITY_EUCLA, 6, 2, 2359) == 844);

    // UTC time: 1:17am on 17th July, time in Broken Hill: 10:47 am
    assert(getLocalTime(CITY_BROKEN_HILL, 17, 7, 117) == 1047);

    // Shared unit tests:
    // Add UTC time: 5:44am on 7th July, time in DARWIN: 4:14 pm
    assert(getLocalTime(CITY_DARWIN, 7, 7, 544) == 1514);

    // UTC time: 6:08pm on 22nd July, time in Canberra: 4:08am on the 23rd
    assert(getLocalTime(CITY_CANBERRA, 22, 7, 1808) == 408);

    // UTC time: 5:56pm on 17th August, time in Adelaide: 3:26am ont the 18th
    assert(getLocalTime(CITY_ADELAIDE, 17, 8, 1756) == 326);

    //UTC time: 7am on 25th January, time in Wellington: 8pm
    assert(getLocalTime(CITY_WELLINGTON, 25, 1, 700) == 2000);

    // UTC time: 9:45pm on 12th November, time in Sydney: 8:45am
    assert(getLocalTime(CITY_SYDNEY, 12, 11, 2145) == 845);

    //UTC time: 2:30am on 25th January, time in Darwin: 12:00pm
    assert(getLocalTime(CITY_DARWIN, 25,1,230) == 1200);

    // UTC time: 15:59 on 1st april, time in sydney:
    assert(getLocalTime(CITY_SYDNEY, 30, 9, 1559) == 159);

    // UTC time: 15:59 on 1st april, time in sydney:
    assert(getLocalTime(CITY_SYDNEY, 30, 9, 1601) == 301);

    // UTC time: 11:30am on 8th June, time in Brisbane: 9:30 am
    assert(getLocalTime(CITY_BRISBANE, 8, 6, 930) == 1930);

    // UTC time: 3:45pm on 30th September, time in Eucla: 12:30am
    assert(getLocalTime(CITY_EUCLA, 30, 9, 1554) == 39);

    // UTC time: 2:00pm on 5th July, time in Sydney: 12:00am
    assert(getLocalTime(CITY_SYDNEY, 4, 7, 1400) == 0);

    // UTC time: 5:15am on 4th January, time in Wellington: 6:15pm on the 4th
    assert(getLocalTime(CITY_WELLINGTON, 4, 1, 515) == 1815);

    // UTC time: 6:45pm on 18th December, time in Sydney: 5:45am on the 19th
    assert(getLocalTime(CITY_SYDNEY, 18, 12, 1845) == 545);

    // UTC time: 3:00pm on 1st April, time Sydney: 2:00 am on the 2nd
    assert(getLocalTime(CITY_SYDNEY, 1, 4, 1500) == 200);

    // UTC time: 4:00pm on 1st April, time Sydney: 2:00 am on the 2nd
    assert(getLocalTime(CITY_SYDNEY, 1, 4, 1600) == 200);

     // UTC time: 3:00pm on 30th September, time Melbourne: 1:00 am on the 1st October
     assert(getLocalTime(CITY_MELBOURNE, 30, 9, 1500) == 100);

     // UTC time: 4:00pm on 30th September, time Melbourne: 3:00 am on the 1st October
     assert(getLocalTime(CITY_MELBOURNE, 30, 9, 1600) == 300);

    //Own unit tests:
    // UTC time: 10:45pm on the 23rd of May, time in Lord Howe Island: 9:15am
    assert(getLocalTime(CITY_LORD_HOWE_IS, 23, 5, 2245) == 915);

    //UTC time: 3pm on the 1st of April, time in Sydney: 2am
    assert(getLocalTime(CITY_SYDNEY, 1, 4, 1500) == 200);

    //UTC time: 2pm on the 23rd of September, time in Christchurch: 3am
    assert(getLocalTime(CITY_CHRISTCHURCH, 23, 9, 1400) == 300);


    return EXIT_SUCCESS;

}


// Converts the time to local time
// [DO NOT CHANGE THIS PROTOTYPE!]
int getLocalTime (int city, int day, int month, int timeUTC) {

    //Get timezone offset no daylight savings
    int timeZoneOffset = getTimeZoneOffset (city);

    //add two times together to get a third time
    int rawLocalTime = addTime (timeZoneOffset, timeUTC);

    //check if the day has changed and subtract 24 hours if yes
    if (checkDayChange(rawLocalTime) == TRUE){
        rawLocalTime -= 2400;
        //change date if day has changed
        day++;
        //check if month changed and change month if yes
        if (checkMonthChange (day, month) == TRUE){
            month++;
            //reset days to the first
            day = 1;
        }
    }
    //Check daylight savings
    if (daylightSaving (city, day, month, rawLocalTime) == TRUE) {
        // Change timezone for the daylight savings
        timeZoneOffset = daylightSavingZone (city, timeZoneOffset);
    }
    // Add new time zone offset
    int localTime = addTime (timeZoneOffset, timeUTC);

    //Check if day is different in UTC than city
    if (checkDayChange (localTime) == TRUE){
        localTime -= 2400;
    }

    return localTime;
}
//Gets the timezone offset for the city without daylight savings
//Returns timezone offset
int getTimeZoneOffset (int city) {
    int timeZoneOffset;
    // Check if NZ
    if (checkIfNewZealand (city) == TRUE) {
        timeZoneOffset = TIMEZONE_NZST;
    } else if (city == CITY_PERTH) {
        timeZoneOffset = TIMEZONE_AWST;
    } else if (city == CITY_EUCLA) {
        timeZoneOffset = TIMEZONE_ACWST;
    } else if (city == CITY_LORD_HOWE_IS) {
        timeZoneOffset = TIMEZONE_LHST;
    //Check if Australian Central Standard Time
    } else if (checkIfACST (city) == TRUE) {
        timeZoneOffset = TIMEZONE_ACST;
    } else {
        timeZoneOffset = TIMEZONE_AEST;
    }

    return timeZoneOffset;
}

//Check if city in New Zealand and returns true or false
int checkIfNewZealand (int city) {
    int NZ = FALSE;
    if (city == CITY_AUCKLAND) {
        NZ = TRUE;
    } else if (city == CITY_WELLINGTON) {
        NZ = TRUE;
    } else if (city == CITY_CHRISTCHURCH) {
        NZ = TRUE;
    }

    return NZ;
}

//Check if city in Australian Central Standard Time Zone
//Returns true or false
int checkIfACST (int city) {
    //ACSTZ = Australian Central Standard Time Zone
    int ACSTZ;
    if (city == CITY_BROKEN_HILL) {
        ACSTZ = TRUE;
    } else if (city == CITY_DARWIN) {
        ACSTZ = TRUE;
    } else if (city == CITY_ADELAIDE) {
        ACSTZ = TRUE;
    }

    return ACSTZ;
}

//Determines if it is daylight savings and returns true or false
int daylightSaving (int city, int day, int month, int rawLocalTime) {
    int daylightSaving = FALSE;
    // Lord Howe Is daylight savings is different, must check separately
    if (city == CITY_LORD_HOWE_IS) {
        daylightSaving = daylightSavingLordHoweIs (day, month, rawLocalTime);
    //Check date for daylight savings at beginning of the year
    } else if (month < 4) {
        daylightSaving = TRUE;
    } else if (month == 4 && day < 2) {
        daylightSaving = TRUE;
    } else if (month == 4 && day == 2 && rawLocalTime <= 100) {
        daylightSaving = TRUE;
    // NZ daylight savings for end of year starts at different date
    } else if (checkIfNewZealand(city) == TRUE) {
        if (month > 9) {
            daylightSaving = TRUE;
        } else if (month == 9 && day > 24){
            daylightSaving = TRUE;
        } else if (month == 9 && day == 24 && rawLocalTime >= 200) {
            daylightSaving = TRUE;
        }
    // Check daylight savings for end of year
    } else if (month > 10) {
        daylightSaving = TRUE;
    } else if (month == 10 && day > 1) {
        daylightSaving = TRUE;
    } else if (month == 10 && day == 1 && rawLocalTime >= 200) {
        daylightSaving = TRUE;
    } else {
        daylightSaving = FALSE;
    }

    return daylightSaving;
}

//Check daylight saving Lord Howe Island returns true or false
int daylightSavingLordHoweIs (int day, int month, int rawLocalTime) {
    int daylightSaving;
    if (month < 4) {
        daylightSaving = TRUE;
    } else if ( month == 4 && day < 2 ) {
        daylightSaving = TRUE;
    } else if ( day == 2 && rawLocalTime <= 130) {
        daylightSaving = TRUE;
    } else if (month > 10) {
        daylightSaving = TRUE;
    } else if (month == 10 && day > 1) {
        daylightSaving = TRUE;
    } else if (month == 10 && day == 1 && rawLocalTime >= 200) {
        daylightSaving = TRUE;
    } else {
        daylightSaving = FALSE;
    }

     return daylightSaving;
}

//Changes timezone to daylight savings time zone
//Returns the new timezone offset
int daylightSavingZone (int city, int timeZoneOffset) {
    if (timeZoneOffset == TIMEZONE_ACST) {
        //Darwin doesn't follow daylight savings
        if (city != CITY_DARWIN) {
            timeZoneOffset = TIMEZONE_ACDT;
        }
    } else if (timeZoneOffset == TIMEZONE_NZST) {
        timeZoneOffset = TIMEZONE_NZDT;
    } else if (timeZoneOffset == TIMEZONE_AEST) {
        //Brisbane doesn't follow daylight savings
        if (city != CITY_BRISBANE) {
            timeZoneOffset = TIMEZONE_AEDT;
        }
    } else if (timeZoneOffset == TIMEZONE_LHST) {
            timeZoneOffset = TIMEZONE_LHDT;
    }

    return timeZoneOffset;
}

//Check if day has changed
int checkDayChange (int localTime) {
    int dayChange = FALSE;
    if (localTime >= 2400) {
        dayChange = TRUE;
    }
    return dayChange;
}

//Check if month has changed and returns true or false
int checkMonthChange (int day, int month) {
    int monthChange = FALSE;
    if (day > getDaysInMonth(month)) {
        monthChange = TRUE;
    }
    return monthChange;
}

//Checks how many days are in the month returns number of days in month
int getDaysInMonth (int month) {
    int daysInMonth;
    if ((month == 4) || (month == 6) || (month == 9) || (month == 11)) {
        daysInMonth = 30;
    } else if (month == 2) {
        daysInMonth = 28;
    } else {
        daysInMonth = 31;
    }

    return daysInMonth;
}

//Changes hour if local time is past 60 mins
int addTime (int fourDigitTime, int fourDigitTimeDelta) {
    int result = fourDigitTime + fourDigitTimeDelta;
    if (getLastTwoDigits (fourDigitTime) + getLastTwoDigits (fourDigitTimeDelta) >= 60) {
        result += 40;
    }
    return result;
}

//Returns last two digits of a number
int getLastTwoDigits (int stillRawLocalTime) {
    //Divide local time by 100 to separate last two digits by a decimal
    //Using an integer elimates last two digits
    int firstTwoDigits = stillRawLocalTime/ 100;;
    int lastTwoDigits = stillRawLocalTime - firstTwoDigits*100;

    return lastTwoDigits;
}
