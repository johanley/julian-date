#include <float.h>

/*
 Algorithm for converting a date in the Gregorian calendar to a Julian date (C99).

 This algorithm doesn't fail for dates in the deep past.
 
 This implementation is self-contained; some of the static const data is repeated 
 in the implementation of the inverse function.
*/

static const double JAN_0_YEAR_0 = 1721058.5; //January 0.0 of year 0 = Dec 31.0 of year -1. 
static const int CYCLE_YEARS = 400;
static const int SHORT_YR = 365;
static const int LONG_YR = 366;

 //Explanatory Supplement 1961, page 434: 
static const int DAYS_IN_PRECEDING_MONTHS[] = {
  0 /*Jan*/, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334 /*Dec*/
};

/*
 Convert a date in the Gregorian calendar to a Julian date.
 There's no restriction on the date.

 I base the calculation on counting days from January 0, year 0.
 Then I simply re-base the result at the end, to reflect the usual 
 origin-day for Julian dates. This exploits the (near) symmetry 
 of the calendar cycles.

 I'm using a nice trick in Robin O'Leary's algorithm:  
 https://pdc.ro.nu/jd-code.html
*/
double cal_to_jd(int y, int m, double d)  {
  //completed years: small asymmetry between positive and negative years
  int y_p = (y >= 0) ? (y - 1) : y;  //y_p = y-prime
  int num_366yrs = (y_p/4) - (y_p/100) + (y_p/CYCLE_YEARS); //Robin's clever trick
  if (y > 0) {
    num_366yrs += 1; //since year 0 is a leap year
  }
  int num_365yrs = y - num_366yrs;
  double res = num_365yrs * SHORT_YR + num_366yrs * LONG_YR;    
    
  //completed months
  res += DAYS_IN_PRECEDING_MONTHS[m-1];   
  int is_leap = (y % 100 == 0) ? (y % 400 == 0) : (y % 4 == 0);
  res += (is_leap && (m - 1) >= 2 ? 1 : 0); //'correct' for leap years  
    
  res += d;  // the day of the month
    
  //rebase to the usual origin of Julian date
  res += JAN_0_YEAR_0;   
  return res;
}