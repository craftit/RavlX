// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
////////////////////////////////////////
//! rcsid="$Id$"
//! lib=RavlOS
//! file="Ravl/OS/Time/testDate.cc"
//! author="Charles Galambos"
//! docentry="Ravl.API.OS.Time"
//! userlevel=Develop

#include "Ravl/OS/Date.hh"
#include "Ravl/OS/DateTZ.hh"
#include "Ravl/String.hh"
#include "Ravl/Stream.hh"
#include "Ravl/UnitTest.hh"
#include "Ravl/SysLog.hh"

using namespace RavlN;

int CheckConsistant();
int CheckIO();
int CheckSleep();
int CheckLocalTime();
int CheckODBC();
int CheckTime();
int CheckTimeMath();
int CheckDateTZ();
int CheckISO8601();

int main() 
{
  SysLogOpen("testDate",false,true,false,-1,true);

#if 0
  RAVL_RUN_TEST(CheckTimeMath());
  RAVL_RUN_TEST(CheckConsistant());
  RAVL_RUN_TEST(CheckIO());
  RAVL_RUN_TEST(CheckSleep());
  RAVL_RUN_TEST(CheckLocalTime());
  RAVL_RUN_TEST(CheckODBC());
  RAVL_RUN_TEST(CheckTime());
  RAVL_RUN_TEST(CheckDateTZ());
#endif
  RAVL_RUN_TEST(CheckISO8601());

  RavlInfo("Test passed.");
  return 0;
}


int CheckLocalTime() {
  IntT year = 2004;
  IntT month = 7;
  IntT day = 19;
  IntT hour = 14;
  IntT min = 46;
  IntT sec = 12;
  IntT usec = 0;
  
  DateC aTime(year,month,day,hour,min,sec,usec,false);
  
  if(aTime.Year() != year) return __LINE__;
  if(aTime.Month() != month) return __LINE__;
  if(aTime.DayInMonth() != day) return __LINE__;
  if(aTime.Minute() != min) return __LINE__;
  if(aTime.Hour() != hour) return __LINE__;
  if(aTime.Seconds() != sec) return __LINE__;

  DateC aTime2(year,month,day,hour,min,sec,usec,true);
  
  if(aTime2.Year(true) != year) return __LINE__;
  if(aTime2.Month(true) != month) return __LINE__;
  if(aTime2.DayInMonth(true) != day) return __LINE__;
  if(aTime2.Minute(true) != min) return __LINE__;
  if(aTime2.Hour(true) != hour) return __LINE__;
  if(aTime2.Seconds(true) != sec) return __LINE__;
  
  return 0;
}

int CheckConsistant()
{
  int days = 0;
  int i;
  for(i = 1970;i < 2037;i++) {
    if(days != DateC::YearToDaysSince1970(i)) {
      cerr << "Failed year " << i << " Days:" << days << " Calc:" << DateC::YearToDaysSince1970(i) << " Leap:" << DateC::IsLeapYear(i) << endl;
      return __LINE__;
    }
    if(DateC::IsLeapYear(i))
      days += 366;
    else
      days += 365;
    for(int m = 1;m <= 12;m++) {
      DateC tmp(i,m,1); 
      if(tmp.Year() != i) {
	cerr << "Failed at Month=" << m << " Year=" << i << " Date=" << tmp.CTime() << " Month()=" << tmp.Month() << " Year()=" << tmp.Year() << "\n";
	return __LINE__;
      }
      if(tmp.Month() != m) {
	cerr << "Failed at Month=" << m << " Year=" << i << " Month()=" << tmp.Month() << " CTime()=" << tmp.CTime() << "\n";
	return __LINE__;
      }
      DateC previousMonth(i,m-1,1);
      if(m == 1) {
        if(previousMonth.Year() != i-1) {
          cerr << "Previous month failed. \n";
          return __LINE__;
        }
      } else {
        if(previousMonth.Month() != m-1) {
          cerr << "Previous month failed. \n";
          return __LINE__;
        }
      }
      
      DateC nextMonth(i,m+1,1);
      if(m == 12) {
        if(nextMonth.Year() != i+1) {
          cerr << "Next month failed. Next=" << nextMonth.ODBC() << " From=" << tmp.ODBC() << " \n";
          return __LINE__;
        }
      } else {
        if(nextMonth.Month() != m+1) {
          cerr << "Next month failed. \n";
          return __LINE__;
        }
      }
      
    }
  }
  return 0;
}

int CheckIO()
{
  // Check string conversions.
  DateC nowish(true);
  StringC str = nowish.Text();
  cerr << "Str:" << str << "\n";
  DateC t(str);
  cerr << "Rec:" << t.Text() << "\n";
  if(t != nowish)
    return __LINE__;
  return 0;
}

int CheckSleep() {
  for(int i = 0;i < 5;i++) {
    DateC before(true);
    Sleep(2);
    DateC after(true);
    RealT del = (after - before).Double();
    cerr << "Delay=" << del << "\n";
    if(del < 1.9 || del > 2.5)
      return __LINE__;
  }
  return 0;
}

int CheckODBC() {
  for(int i = 0;i < 20;i++) {
    DateC dateOrg((long) 1234 + i * 100000000,(long) 0);
    StringC odbcStr = dateOrg.ODBC();
    DateC dateOut;
    if(!dateOut.SetODBC(odbcStr)) {
      cerr << "Failed to decode odbc string. Text=" << odbcStr << "\n";
      return __LINE__;
    }
    //cerr << " Org=" << dateOrg.ODBC() << " New=" << dateOut.ODBC() << "\n";
    if(dateOut != dateOrg) {
      cerr << " Org=" << dateOrg << " New=" << dateOut << "\n";
      return __LINE__;
    }
  }
  return 0;
}

int CheckTime() {
  DateC timeNow = DateC::NowLocal();
  DateC timeUTC = DateC::NowUTC();
  cerr << "Time local=" << timeNow.ODBC() << "\n";
  cerr << "Time UTC  =" << timeUTC.ODBC() << "\n";
  cerr << "Offset    =" << DateC::TimeZoneOffset().Double() << "\n";
  if(Abs(((timeNow - timeUTC) + DateC::TimeZoneOffset()).Double()) > 0.0001) return __LINE__;
  return 0;
}

int CheckTimeMath() {
  DateC a(2.0);
  a += 2.4;
  if(Abs(a.Double() - 4.4) > 0.000001) return __LINE__;
  a += -0.3;
  if(Abs(a.Double() - 4.1) > 0.000001) return __LINE__;
  a += -0.4;
  if(Abs(a.Double() - 3.7) > 0.000001) return __LINE__;
  a = 4.0;
  a -= 1.5;
  if(Abs(a.Double() - 2.5) > 0.000001) return __LINE__;
  a -= -0.6;
  if(Abs(a.Double() - 3.1) > 0.000001) return __LINE__;

  DateC now = DateC::NowUTC();
  RealT dbl = now.Double();
  if((DateC(dbl) - now).Double() > 0.1) return __LINE__;

  DateC tv1,tv2;
  tv1.SetODBC("2010-04-15 09:09:10.1450190000");
  tv2.SetODBC("2010-04-15 09:09:10.4650180000");
  if((const DateC &)tv1 > (const DateC &)tv2) return __LINE__;

  return 0;
}

int CheckDateTZ()
{
  // Should do more testing...

  DateTZC theDateTZ = DateTZC::Now();

  StringC ds = theDateTZ.ISO8601();
  //RavlDebug("Current time:%s ",ds.c_str());
  //RavlDebug("Current time ODBC:%s ",theDateTZ.LocalTime().ODBC(false,true).c_str());
  DateTZC dateRestored = DateTZC::FromISO8601String(ds);
  //RavlDebug("Restored time:%s ",dateRestored.ISO8601().c_str());
  RAVL_TEST_EQUALS(ds,dateRestored.ISO8601());
  RAVL_TEST_TRUE(theDateTZ.IsValid());

  DateTZC invalid = DateTZC::InvalidTime();
  RAVL_TEST_EQUALS(invalid.ISO8601(),"undefined");
  RAVL_TEST_FALSE(invalid.IsValid());

  DateTZC dateRestored2 = DateTZC::FromISO8601String("undefined");
  RAVL_TEST_FALSE(dateRestored2.IsValid());

  return 0;
}

int CheckISO8601()
{
  DateC theDate = DateC::NowUTC();
  StringC ds = theDate.ISO8601();
  DateC dateRestored = DateC::FromISO8601String(ds);
  //RavlDebug("Restored time:%s ",dateRestored.ISO8601().c_str());
  RAVL_TEST_EQUALS(ds,dateRestored.ISO8601());

  DateC invalid = DateC::InvalidTime();
  RAVL_TEST_EQUALS(invalid.ISO8601(),"undefined");
  RAVL_TEST_FALSE(invalid.IsValid());

  DateC dateRestored2 = DateC::FromISO8601String("undefined");
  RAVL_TEST_FALSE(dateRestored2.IsValid());

  DateC testDate1 = DateC::FromISO8601String("2014-12-05T12:25:20");
  RavlDebug("Date:%s ",testDate1.ISO8601(false).c_str());
  DateC testDate2 = DateC::FromISO8601String("2014-12-07");
  RavlDebug("Date:%s ",testDate2.ISO8601(false).c_str());
  DateC testDate3 = DateC::FromISO8601String("2014-12-07T10:32:48+00:00");
  RavlDebug("Date:%s ",testDate3.ISO8601(false).c_str());
  DateC testDate4 = DateC::FromISO8601String("2014-12-07T10:32:48Z");
  RavlDebug("Date:%s ",testDate4.ISO8601(false).c_str());
  DateC testDate5 = DateC::FromISO8601String("2014-12-07T10:32:48");
  RavlDebug("Date:%s ",testDate5.ISO8601(false).c_str());

  DateTZC testDate6 = DateTZC::FromISO8601String("2014-09-29 18:50:31.296940+01:01");
  RavlDebug("Date:%s ",testDate6.ISO8601().c_str());
  DateTZC testDate7 = DateTZC::FromISO8601String("2014-09-29 18:50:31.296940+11:01");
  RavlDebug("Date:%s ",testDate7.ISO8601().c_str());
  DateTZC testDate8 = DateTZC::FromISO8601String("2014-09-29");
  RavlDebug("Date:%s ",testDate8.ISO8601().c_str());

  DateTZC testDate9 = DateTZC::FromISO8601String("1910-09-29");
  RavlDebug("Date:%s ",testDate9.ISO8601().c_str());

  DateC testDate10 = DateC::FromISO8601String("1910-09-29");
  RavlDebug("Date:%s ",testDate10.ISO8601().c_str());

  return 0;
}


