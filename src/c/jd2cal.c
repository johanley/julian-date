#include <float.h>
#include <stdio.h>
#include <math.h>

/*
 Algorithm for converting a Julian date to a date in the Gregorian calendar (C99).

 This algorithm doesn't fail for dates in the deep past.
 
 This implementation is self-contained; some of the static const data is repeated 
 in the implementation of the inverse function.
*/

static const int SHORT_YR = 365;
static const int LONG_YR = 366;
static const double JAN_1_YEAR_0 = 1721058.5 + 1.0;
static const int CYCLE_YEARS = 400;
static const int CYCLE_DAYS = SHORT_YR*CYCLE_YEARS + CYCLE_YEARS/4 - CYCLE_YEARS/100 + CYCLE_YEARS/CYCLE_YEARS; //146_097 days
static const int MONTH_LEN[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

static int is_leap(int y) {
  return (y % 100 == 0) ? (y % 400 == 0) : (y % 4 == 0);
}

/* 
 The length of the given month in days. The month-index is 1-based. 
 This function is non-static, since it's used in the unit-test file.
*/
int the_month_len(int y, int m){
  int length = MONTH_LEN[m - 1];
  int has_leap_day = is_leap(y) && (m == 2);
  return has_leap_day ? length + 1 : length;
}

/*
  Mental model: use a 'base', a point in time occurring once every 400 years, 
  at which the calendar cycle starts. Counting forward in time from such any 
  such 'base' exploits the symmetry of the calendar's cycle.

  Choose a base falling on a day N*400 years from January 1.0, year 0:
    JD of a base = 1_721_059.5 + N * 146_097, with  N = ...-2,-1,0,1,2,...

  There are 2 loops in this implementation, with a max number of 14 loop iterations.
*/
void jd_to_cal(double jd, int *y, int *m, double *d) {
  //1. find the closest 'base' that PRECEDES the given moment
  int num_cycles = (int)floor((jd - JAN_1_YEAR_0)/CYCLE_DAYS); //rounds towards negative infinity: good!
  double base_jd = JAN_1_YEAR_0 + num_cycles * CYCLE_DAYS; //a January 1.0 in the years  .., -400, 0, 400, ..
  int year = num_cycles * CYCLE_YEARS; // ..,-400, 0, 400,.. (the starting value)
  double jd_minus_base = jd - base_jd; //never neg

  //THE GAME: is to move this cursor forward from our base Jan 1.0 taken
  //as the zero-point, to the target jd_minus_base
  double cursor = 0.0; 
    
  //2. remainder-years: whole, completed years after the base 
  //one big chunk of years: calculate a MINIMUM number of full remainder-years, to reduce loop iterations below
  int approx_days = (int)floor(jd_minus_base);
  int more_years = (approx_days / LONG_YR) - 1; // at least this many
  if (more_years > 0) {
    int m_p = more_years - 1;
    int more_days = more_years * SHORT_YR + (m_p/4) - (m_p/100) + (m_p/400) + 1;
    cursor += more_days; //still on a Jan 1.0!
    year += more_years;
  }
  //loop to find the rest of the remainder-years: at most 2 iterations here!
  int year_so_far = year; //for use in the loop 
  for(int more = 0; more < CYCLE_YEARS; ++more ) { 
    int year_length = is_leap(year_so_far + more) ? LONG_YR : SHORT_YR;
    if (cursor + year_length <= jd_minus_base) {
      cursor += year_length; // Jan 1.0 of the next year
      ++year;
    } else { break; }
  }
    
  //3. months and days
  int month = 0; //both a loop index AND a result-value
  double fractional_days = 0.0;
  for(month = 1; month <= 12; ++month) {
    int month_length = the_month_len(year, month);
    if (cursor + month_length <= jd_minus_base) {
      cursor += month_length; //1st day of the next month
    }
    else {
      fractional_days = jd_minus_base - cursor + 1.0; break;
    }
  }
  *y = year;
  *m = month;
  *d = fractional_days;
}