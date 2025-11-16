#include <stdio.h>
#include <math.h>
#include <time.h>
#include "julian_date.h"

/*
 UNIT TESTS FOR cal_to_jd and jd_to_cal (C99).

 An example of running the tests in PowerShell on Windows, in the current directory:
   gcc $(Get-ChildItem -Path *.c -Name) -std=c99 -Wall -Werror -Wpedantic -o run_tests.exe
   ./run_tests.exe 
*/

static int num_errors = 0;
static int num_successful = 0;

static const char *SUCCESS = "OK";
static const char *FAILURE = " X";

static const int REPORT = 1;
static const int SILENT = 0;

/* Output the result of checking expected-result versus actual-result. */
static void check_date_to_jd(double expected, double result, const int report){
  const char* message = (expected == result) ? SUCCESS : FAILURE;
  short ok = expected == result;
  !ok ? ++num_errors : ++num_successful;
  if (report){
    printf("%s Expected: %f Result: %f\n", message, expected, result);
  }
}

/* Output the result of checking expected-result versus actual-result. */
static void check_jd_to_date(
  int expected_y, int expected_m, double expected_d,
  int result_y, int result_m, double result_d,
  const int report
){
  short ok_d = abs(expected_d - result_d) < __FLT_EPSILON__;
  short ok = (expected_y == result_y) &&  (expected_m == result_m) && ok_d;
  const char* message_a = ok ? SUCCESS : FAILURE;
  !ok ? ++num_errors : ++num_successful;
  if (report){
    printf("%s Expected: %d-%d-%f Result: %d-%d-%f\n", \
      message_a, 
      expected_y, expected_m, expected_d, \
      result_y, result_m, result_d
    );
  }
}

/* Test converting a date in the Gregorian calendar to a Julian date. */
static void test_cal_to_jd(
  int y, int m, double d, double expected_jd, 
  double (*func)(int, int, double), 
  int report
){
  double jd = func(y, m, d); 
  check_date_to_jd(expected_jd, jd, report);
}

/** Test converting a Julian date to a date in the Gregorian calendar. */
static void test_jd_to_cal(
  double jd,
  int expected_y, int expected_m, double expected_d, 
  void (*func)(double, int*, int*, double*), 
  int report
){
  int result_y, result_m;
  double result_d;
  func(jd, &result_y, &result_m, &result_d); 
  check_jd_to_date(expected_y, expected_m, expected_d, result_y, result_m, result_d, report);
}

/* Test the conversions in both directions. */ 
static void test_both_directions(int y, int m, double d, double jd){
  test_jd_to_cal(jd, y, m, d, jd_to_cal, REPORT);
  test_cal_to_jd(y, m, d, jd, cal_to_jd, REPORT);
  printf("\n");
}

/* These tests aren't reported in detail. Only the count of success-fail is reported for these. */ 
static void test_entire_year(int y, double jd_jan_0){
  printf("Testing every day in the year: %d\n", y);
  int day_num = 0; //1..(365|366)
  for(int m = 1; m <= 12; ++m){
    int num_days_in_month = the_month_len(y, m);
    for(int d = 1; d <= num_days_in_month; ++d){
      ++day_num;
      double jd = jd_jan_0 + day_num;
      test_jd_to_cal(jd, y, m, d, jd_to_cal, SILENT);
      test_cal_to_jd(y, m, d, jd, cal_to_jd, SILENT);
    }
  }
}

/* Test every day of the year for years near the year 0. (These cases are easy to calculate manually.) */ 
static void test_small_years(void) {
    double jan0_year0 = 1721058.5; //alias for Dec 31, year -1
    test_entire_year(-9, jan0_year0 - 2*366 - 7*365);
    test_entire_year(-8, jan0_year0 - 2*366 - 6*365);
    test_entire_year(-7, jan0_year0 - 1*366 - 6*365);
    test_entire_year(-6, jan0_year0 - 1*366 - 5*365);
    test_entire_year(-5, jan0_year0 - 1*366 - 4*365);
    test_entire_year(-4, jan0_year0 - 1*366 - 3*365);
    test_entire_year(-3, jan0_year0 - 0*366 - 3*365);
    test_entire_year(-2, jan0_year0 - 0*366 - 2*365);
    test_entire_year(-1, jan0_year0 - 0*366 - 1*365);
    test_entire_year(0, jan0_year0 + 0*366 + 0*365);
    test_entire_year(1, jan0_year0 + 1*366 + 0*365);
    test_entire_year(2, jan0_year0 + 1*366 + 1*365);
    test_entire_year(3, jan0_year0 + 1*366 + 2*365);
    test_entire_year(4, jan0_year0 + 1*366 + 3*365);
    test_entire_year(5, jan0_year0 + 2*366 + 3*365);
    test_entire_year(6, jan0_year0 + 2*366 + 4*365);
    test_entire_year(7, jan0_year0 + 2*366 + 5*365);
    test_entire_year(8, jan0_year0 + 2*366 + 6*365);
    test_entire_year(9, jan0_year0 + 3*366 + 6*365);
    test_entire_year(10, jan0_year0 + 3*366 + 7*365);
    test_entire_year(11, jan0_year0 + 3*366 + 8*365);
    test_entire_year(12, jan0_year0 + 3*366 + 9*365);
  }

  
/*  Run all tests for conversions from calendar-date to Julian date, and vice versa. */
static void run_all_tests(void){
    printf("SOFA's tests.\n");
    test_both_directions(2003, 6, 1.0, 2400000.5 + 52791.0);

    //a modification of SOFA's test, in order to use whole days
    test_both_directions(1996, 2, 11, 2400000.5 +  50124.0);  

    printf("\nExplanatory Supplement, 1961, page  437.\n");
    test_both_directions(1500, 1, 1, 2268923.5);
    test_both_directions(1600, 1, 1, 2305447.5);
    test_both_directions(1700, 1, 1, 2341972.5);
    test_both_directions(1800, 1, 1, 2378496.5);
    test_both_directions(1900, 1, 1, 2415020.5);

    test_both_directions(1500, 3, 1, 2268923 + 0.5 + 59); 
    test_both_directions(1600, 3, 1, 2305447 + 0.5 + 60); //March 1 is after Feb 29; only this one is a leap year
    test_both_directions(1700, 3, 1, 2341972 + 0.5 + 59);
    test_both_directions(1800, 3, 1, 2378496 + 0.5 + 59);
    test_both_directions(1900, 3, 1, 2415020 + 0.5 + 59);

    printf("\nGuide de Donnees Astronomiques 2017, Bureau des longitudes, page 8.\n");
    test_both_directions(1950, 1, 1.5, 2433283.0);
    test_both_directions(2000, 1, 1.5, 2451545.0);
    test_both_directions(2050, 1, 1.5, 2469808.0);
    test_both_directions(2090, 1, 1.5, 2484418.0);

    // -1374 May 3, at 13:52:19.2 TT 
    printf("\nFrom Vondrak, Wallace, Capitaine 2011.\n");
    test_both_directions(-1374, 5, 3.578, 1219339.078);    

    printf("\nObserver's Handbook, RASC, 2024, page 47.\n");
    test_both_directions(2024, 1, 1, 2460310.5);    
    test_both_directions(2024, 3, 1, 2460370.5);    

    printf("\nAstronomical Algorithms, Meeus 1991, page 61ff.\n");
    test_both_directions(1957, 10, 4.81, 2436116.31);
    test_both_directions(1987, 6, 19.5, 2446966.0);

    printf("\nFrom https://legacy-www.math.harvard.edu/computing/javascript/Calendar/index.html\n");
    test_both_directions(-8, 1, 1.5, 1718138.0);
    test_both_directions(-101, 1, 1.5, 1684171.0);
    test_both_directions(-799, 1, 1.5, 1429232.0);
    test_both_directions(-800, 1, 1.5, 1428866.0);
    test_both_directions(-801, 1, 1.5, 1428501.0);
    test_both_directions(99, 12, 31.5, 1757584.0);
    test_both_directions(100, 1 , 1.5, 1757584.0 + 1.0);
    test_both_directions(100, 1, 31.5, 1757584.0 + 31.0);
    test_both_directions(100, 2, 1.5, 1757584.0 + 31.0 + 1.0);
    test_both_directions(100, 2, 28.5, 1757584.0 + 31.0 + 28.0); //100 is not a leap year
    test_both_directions(100, 3, 1.5, 1757584.0 + 31.0 + 28.0 + 1.0);
    test_both_directions(3000, 1, 1.5, 2816788);
    test_both_directions(30000, 1, 1.5, 12678335.0);
    test_both_directions(100, 1, 1.5, 1757585.0);
    test_both_directions(101, 1, 1.5, 1757950.0); 
    test_both_directions(200, 1, 1.5, 1794109.0); 
    test_both_directions(300, 1, 1.5, 1830633.0); 
    test_both_directions(400, 1, 1.5, 1867157); 
    test_both_directions(700, 1, 1.5, 1976730);  
    test_both_directions(800, 1, 1.5, 2013254);

    printf("\nThe origin of the Julian date is -4712-01-01 12h, in the Julian calendar.\n");
    printf("That date is -4713-11-24 in the Gregorian calendar.\n");
    test_both_directions(-4713, 11, 24.5, 0.0);

    printf("\nThe first date supported by the SOFA algorithm: -4799-01-01.\n");
    test_both_directions(-4799, 1, 1.0, -31738.5);

    printf("\nTest dates near JD = 0.\n");
    test_both_directions(-4712, 1, 1.5, 38.0);
    test_both_directions(-4712, 1, 1.5, 38.0);
    test_both_directions(-4713, 12, 31.5, 37.0);
    test_both_directions(-4713, 12, 1.5, 7.0);
    test_both_directions(-4713, 11, 30.5, 6.0);
    test_both_directions(-4713, 11, 24.5, 0.0); //the JD=0 date
    test_both_directions(-4713, 11, 24.0, -0.5);
    test_both_directions(-4713, 11, 23.0, -1.5);
    test_both_directions(-4713, 1, 1.5, -327.0);
    test_both_directions(-4714, 1, 1.5, -327.0 - 365 * 1.0);
    test_both_directions(-4715, 1, 1.5, -327.0 - 365 * 2.0);
    test_both_directions(-4716, 1, 1.5, -327.0 - 365 * 2.0 - 366 * 1.0);
    test_both_directions(-4717, 1, 1.5, -327.0 - 365 * 3.0 - 366 * 1.0);
    test_both_directions(-4718, 1, 1.5, -327.0 - 365 * 4.0 - 366 * 1.0);
    test_both_directions(-4719, 1, 1.5, -327.0 - 365 * 5.0 - 366 * 1.0);
    test_both_directions(-4720, 1, 1.5, -327.0 - 365 * 5.0 - 366 * 2.0);
    test_both_directions(-4721, 1, 1.5, -327.0 - 365 * 6.0 - 366 * 2.0);
    test_both_directions(-4800, 1, 1.5, -327.0 - 365 * 65.0 - 366 * 22.0); //leap century year
    test_both_directions(-4801, 1, 1.5, -327.0 - 365 * 66.0 - 366 * 22.0);
    test_both_directions(-4900, 1, 1.5, -327.0 - 365 * (75.0 + 66.0) - 366 * (24.0 + 22.0)); //not a leap year

    printf("\nNum failed tests: %d\n", num_errors);
    printf("Num successful tests: %d\n", num_successful);

    printf("\nTest entire years near the year 0.\n");
    printf("There's no detailed reporting in these cases.\n");
    test_small_years();

    printf("\nNum failed tests: %d\n", num_errors);
    printf("Num successful tests: %d\n", num_successful);
}

/* Wrap the execution of a void function with simple timing operations. */
static void add_timing (void (*func)(void)){
  clock_t start = clock();
  func();
  clock_t end = clock();

  double elapsed = 1000.0 * (end - start) / CLOCKS_PER_SEC;
  printf("\nElapsed time: %d milliseconds.\n", (int)elapsed);
}

/* I have renamed the 'main' function found in t_sofa_c.c, in order to replace it with this 'main'.  */
int main(void){
  add_timing(run_all_tests);
  return 0;
}