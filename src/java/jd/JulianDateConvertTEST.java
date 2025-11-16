package jd;

import org.junit.Test;
import static org.junit.Assert.*;

import java.time.LocalDate;
import java.time.Year;

/** Unit tests. Uses JUnit 4. */
public final class JulianDateConvertTEST {

  @Test public void smallYears() {
    testSmallYears();
  }
  
  @Test public void testLeapDayInCenturyYears() {
    //Explanatory Supplement 1961, p.437
    testDate(1500, 3, 1, 2268923 + 0.5 + 59); 
    testDate(1600, 3, 1, 2305447 + 0.5 + 60); //March 1 is after Feb 29; only this one is a leap year
    testDate(1700, 3, 1, 2341972 + 0.5 + 59);
    testDate(1800, 3, 1, 2378496 + 0.5 + 59);
    testDate(1900, 3, 1, 2415020 + 0.5 + 59);
  }  
  
  @Test public void specificCases() {
    //Meeus 1991
    testDate(1957, 10, 4.81, 2436116.31);
    
    //From Vondrak, Wallace, Capitaine 2011
    // -1374 May 3, at 13:52:19.2 TT 
    testDate(-1374, 5, 3.578, 1219339.078);    
    
    //https://legacy-www.math.harvard.edu/computing/javascript/Calendar/index.html
    testDate(-8, 1, 1.5, 1718138.0);
    testDate(-101, 1, 1.5, 1684171.0);
    testDate(-799, 1, 1.5, 1429232.0);
    testDate(-800, 1, 1.5, 1428866.0);
    testDate(-801, 1, 1.5, 1428501.0);
    testDate(99, 12,31.5, 1757584.0);
    testDate(100,1,1.5, 1757584.0 + 1.0);
    testDate(100,1,31.5, 1757584.0 + 31.0);
    testDate(100,2,1.5, 1757584.0 + 31.0 + 1.0);
    testDate(100,2,28.5, 1757584.0 + 31.0 + 28.0); //100 is not a leap year
    testDate(100,3,1.5, 1757584.0 + 31.0 + 28.0 + 1.0);
    testDate(3000, 1, 1.5, 2816788);
    testDate(30000, 1, 1.5, 12678335.0);
        
    testDate(100,1,1.5, 1757585.0);
    testDate(101,1,1.5, 1757950.0); 
    testDate(200,1,1.5, 1794109.0); 
    testDate(300,1,1.5, 1830633.0); 
    testDate(400,1,1.5, 1867157); 
    testDate(700,1,1.5, 1976730);  
    testDate(800,1,1.5, 2013254);
    
    //Explanatory Supplement 1961, p 438
    testYear(1920, 2422324 - 0.5);
    testYear(1960, 2436934 - 0.5);
    testYear(1964, 2438395 - 0.5);
    testYear(1980, 2444239 - 0.5);
    testYear(1990, 2447892 - 0.5);
  }
  
  @Test public void deepPast() {
    testDate(-4712, 1, 1.5, 38.0);
    testDate(-4713, 12, 31.5, 37.0);
    testDate(-4713, 12, 1.5, 7.0);
    testDate(-4713, 11, 30.5, 6.0);
    testDate(-4713, 11, 24.5, 0.0); //the JD=0 date
    testDate(-4713, 11, 24.0, -0.5);
    testDate(-4713, 11, 23.0, -1.5);
    testDate(-4713, 1, 1.5, -327.0);
    testDate(-4714, 1, 1.5, -327.0 - 365 * 1.0);
    testDate(-4715, 1, 1.5, -327.0 - 365 * 2.0);
    testDate(-4716, 1, 1.5, -327.0 - 365 * 2.0 - 366 * 1.0);
    testDate(-4717, 1, 1.5, -327.0 - 365 * 3.0 - 366 * 1.0);
    testDate(-4718, 1, 1.5, -327.0 - 365 * 4.0 - 366 * 1.0);
    testDate(-4719, 1, 1.5, -327.0 - 365 * 5.0 - 366 * 1.0);
    testDate(-4720, 1, 1.5, -327.0 - 365 * 5.0 - 366 * 2.0);
    testDate(-4721, 1, 1.5, -327.0 - 365 * 6.0 - 366 * 2.0);
    
    testDate(-4800, 1, 1.5, -327.0 - 365 * 65.0 - 366 * 22.0); //leap century year
    testDate(-4801, 1, 1.5, -327.0 - 365 * 66.0 - 366 * 22.0);
    
    testDate(-4900, 1, 1.5, -327.0 - 365 * (75.0 + 66.0) - 366 * (24.0 + 22.0)); //not a leap year
  }
  
  /** It's easy to compute the JD manually for small years .*/
  private void testSmallYears() {
    double base = JulianDateConvert.JAN_0_YEAR_0;
    testYear(-9, base - 2*366 - 7*365);
    testYear(-8, base - 2*366 - 6*365);
    testYear(-7, base - 1*366 - 6*365);
    testYear(-6, base - 1*366 - 5*365);
    testYear(-5, base - 1*366 - 4*365);
    testYear(-4, base - 1*366 - 3*365);
    testYear(-3, base - 0*366 - 3*365);
    testYear(-2, base - 0*366 - 2*365);
    testYear(-1, base - 0*366 - 1*365);
    testYear(0, base + 0*366 + 0*365);
    testYear(1, base + 1*366 + 0*365);
    testYear(2, base + 1*366 + 1*365);
    testYear(3, base + 1*366 + 2*365);
    testYear(4, base + 1*366 + 3*365);
    testYear(5, base + 2*366 + 3*365);
    testYear(6, base + 2*366 + 4*365);
    testYear(7, base + 2*366 + 5*365);
    testYear(8, base + 2*366 + 6*365);
    testYear(9, base + 3*366 + 6*365);
    testYear(10, base + 3*366 + 7*365);
    testYear(11, base + 3*366 + 8*365);
    testYear(12, base + 3*366 + 9*365);
  }
  
  private void testYear(int year, double jan_0_in_given_year) {
    testDateToJdForYear(year, jan_0_in_given_year);
    testJdToDateForYear(year, jan_0_in_given_year);
  }
  
  private void testDate(int year, int month, double day, double jd) {
    testCalToJd(year, month, day, jd);
    testJdToCal(year,  month, day, jd);
  }

  private void testDateToJdForYear(int year, double jan_0_given_year) {
    LocalDate day = LocalDate.of(year, 1, 1);
    for(int i = 0; i < Year.of(year).length(); ++i) {
      testCalToJd(year, day.getMonthValue(), day.getDayOfMonth(), jan_0_given_year + 1 + i);
      day = day.plusDays(1);
    }
  }

  private void testJdToDateForYear(int year, double jan_0_in_given_year) {
    LocalDate day = LocalDate.of(year, 1, 1);
    for(int i = 1; i <= Year.of(year).length(); ++i) {
      testJdToCal(year, day.getMonthValue(), day.getDayOfMonth(), jan_0_in_given_year + i);
      day = day.plusDays(1);
    }
  }
  
  private void testCalToJd(int year, int month, double day, Double jd_expected) {
    Double jd = JulianDateConvert.calToJd(year, month, day);
    assertEquals(jd_expected, jd);
  }
  
  private void testJdToCal(int year, int month, double day, Double jd) {
    JulianDateConvert.DateStruct date = JulianDateConvert.jdToCal(jd);
    assertEquals(year, date.y);
    assertEquals(month, date.m);
    assertEquals(day, date.d, 0.000000001);
  }
}
