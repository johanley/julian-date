package jd;

import java.time.Month;
import java.time.Year;
import java.util.LinkedHashMap;
import java.util.Map;

/** 
 Convert between dates in the Gregorian calendar and Julian dates, and vice versa.
 
 There's no restriction on the Julian date being non-negative.
 
 <P>(I would have preferred using long instead of int for the year. 
 But Java's date-time packages use int for the year, so I'll go with that.)
*/
public final class JulianDateConvert {

  /**
   Convert a date in the Gregorian calendar to a Julian date. 
   
   With help from Robin O'Leary's algorithm: https://pdc.ro.nu/jd-code.html
   
   <P>I base the calculation on counting days from January 0, year 0.
   Then I simply re-base the result at the end, to reflect the usual 
   origin-day for Julian dates. This exploits the (near) symmetry of 
   the calendar cycles.
  */
  static double calToJd(int y, int m, double d) {
    //completed years: small asymmetry between positive and negative years
    int y_p = (y >= 0) ? (y - 1) : y;  //y_p = y-prime
    int num_366yrs = (y_p/4) - (y_p/100) + (y_p/CYCLE_YEARS); //Robin's clever trick
    if (y > 0) {
      num_366yrs += 1; //since year 0 is a leap year
    }
    int num_365yrs = y - num_366yrs;
    double res = num_365yrs * SHORT_YR + num_366yrs * LONG_YR;    
    
    //completed months
    res += DAYS_IN_PRECEDING_MONTHS.get(Month.of(m));   
    res += (isLeap(y) && (m - 1) >= 2 ? 1 : 0); //'correct' for leap years  
    
    res += d;  // the day of the month
    
    //rebase to the usual origin of Julian date
    res += JAN_0_YEAR_0;   
    return res;
  }
  
  /**
   Convert a Julian date to a date in the Gregorian calendar.
     
   Mental model: use a 'base', a point in time occurring once every 400 years, 
   at which the calendar cycle starts. Counting forward in time from such any 
   such base exploits the symmetry of the calendar's cycle. Let's take a base 
   as always falling on a N*400 years from January 1.0, year 0:
    
    <pre>JD of a base = 1_721_059.5 + N * 146_097  N = ...-2,-1,0,1,2,...</pre>
      
   There are 2 loops here, with a max number of 14 loop iterations (not much).
  */
  static DateStruct jdToCal(double jd) {
    //1. find the closest base that PRECEDES the given moment
    int num_cycles = (int)Math.floor((jd - JAN_1_YEAR_0)/CYCLE_DAYS); //rounds towards negative infinity: good!
    double base_jd = JAN_1_YEAR_0 + num_cycles * CYCLE_DAYS; //a January 1.0 in the years  ..., -800, -400, 0, 400, 800, ... 
    int year = num_cycles * CYCLE_YEARS; // ...,-400, 0, 400,... (the starting value)
    double jd_minus_base = jd - base_jd; //never neg
    double cursor = 0.0; //points to a Jan 1.0 initially; approaches jd_minus_base from below!
    
    //2. remainder-years: whole, completed years after the base 
    //one big chunk of years: calculate a MINIMUM number of full remainder-years, to reduce loop iterations later
    int approx_days = (int)Math.floor(jd_minus_base);
    int more_years = (approx_days / LONG_YR) - 1; // at least this many
    if (more_years > 0) {
      int m_p = more_years - 1;
      int more_days = more_years * SHORT_YR + (m_p/4) - (m_p/100) + (m_p/400) + 1;
      cursor += more_days; //still on a Jan 1.0!
      year += more_years;
    }
    //loop to find the rest of the remaining-years: at most 2 iterations here!
    int year_so_far = year; //for use in the loop 
    for(int more = 0; more < CYCLE_YEARS; ++more ) { 
      int year_length = isLeap(year_so_far + more) ? LONG_YR : SHORT_YR;
      if (cursor + year_length <= jd_minus_base) {
        cursor += year_length; // Jan 1.0 of the next year
        ++year;
      } else { break; }
    }
    
    //3. months and days
    int month = 0; //both a loop index AND a result-value
    double fractional_days = 0.0;
    for(month = 1; month <= 12; ++month) {
      int month_length = Month.of(month).length(isLeap(year));
      if (cursor + month_length <= jd_minus_base) {
        cursor += month_length; //1st day of the next month
      }
      else {
        fractional_days = jd_minus_base - cursor + 1.0; break;
      }
    }
    return new DateStruct(year, month, fractional_days);
  }

  /** 
   This little guy plays well with the algorithm, since it models the day-as-double.
   In practice, you would almost always move this info into Java's standard date-time classes. 
  */
  static final class DateStruct {
    DateStruct(int y, int m, double d){
      this.y = y; this.m = m; this.d = d;
    }
    int y, m; 
    double d;
  }

  /**
   The Julian date of December 31.0, in the year -1. 
   January 0 is an alias for December 31 of the previous year. 
  */
  static final double JAN_0_YEAR_0 = 1_721_058.5;
  
  private static boolean isLeap(int y) {
    return Year.of(y).isLeap();
  }

  private static final int SHORT_YR = 365;
  private static final int LONG_YR = 366;
  private static final int CYCLE_YEARS = 400;
  private static final double JAN_1_YEAR_0 = JAN_0_YEAR_0 + 1.0;
  private static final int CYCLE_DAYS = 
    SHORT_YR * CYCLE_YEARS 
    + CYCLE_YEARS/4 
    - CYCLE_YEARS/100 
    + CYCLE_YEARS/CYCLE_YEARS
  ; //146_097 days

  /** 
   For a non-leap year: Jan=0, Feb=31, Mar=59, ...
   I didn't want to hard-code these numbers, as I did in the C implementation.  
  */
  private static final Map<Month, Integer> DAYS_IN_PRECEDING_MONTHS = daysInPrecedingMonths();
  
  private static Map<Month, Integer> daysInPrecedingMonths() {
    int accumulator = 0;
    Map<Month, Integer> res = new LinkedHashMap<>();
    for(Month month : Month.values()) {
      res.put(month, accumulator);
      accumulator += month.length(false);
    }
    return res;
  }
}