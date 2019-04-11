#include "course.h"
#include "stringhelper.h"
#include <bitset>
#include <cstring>
#include <iostream>
#include <map>

void Class::addTime(int start, int end) {
  // std::cout << "setTime" << start << " " << end << std::endl;
  int ind = start / 8;
  uchar cbit = 1 << (start % 8);
  for (int i = start; i < end; i++) {
    time[ind] |= cbit;
    cbit <<= 1;
    if (cbit == 0) {
      cbit = 1;
      ind++;
    }
  }
}

void Class::addTime(std::vector<std::string> dataRow) {
  // e.g. MTG,Tue,12:30,13:30,13,All,,Z-26-G03,PEMS P25,Dr SA Rao,Prof X  Wang,
  std::vector<std::string>::iterator it = dataRow.begin();
  it++;
  std::vector<std::string> daysStr;
  split(*it, ' ', daysStr);
  // TODO(adamyi): parse weeks
  it++;
  std::string startTimeStr = *it;
  it++;
  std::string endTimeStr = *it;
  for (auto &dstr : daysStr) {
    int week = dayOfWeekToInt(dstr);
    this->addTime(timeToInt(week, startTimeStr), timeToInt(week, endTimeStr));
  }
  this->print();
}

void Class::print() {
  std::cout << "time: ";
  for (int i = 0; i < 80; i++) {
    std::cout << std::bitset<8>(time[i]) << '(' << (int)time[i] << ") ";
  }
  std::cout << std::endl;
}

Class::Class(std::vector<std::string> dataRow) {
  /*
   * CLS,5187,T2,COMP,6447,Security Assessment,
   * ,UGRD,COMPSC,CRS,12997,CR01,Enr,8881,GRD,A,No,P,KENS,KENSINGTON, ,
   * ,2018-07-23,2018-10-28,0,39,9999, ,0,0,0,65170,1
   * CLS,5187,T2,COMP,6447,Security Assessment,
   * ,UGRD,COMPSC,LAB,9517,H12A,Non,1,GRD,A,No,P,KENS,KENSINGTON, ,
   * ,2018-07-23,2018-10-28,11,20,20,1467/COMP6447-PGRD/19,1,0,0,65170,1
   */
  code = dataRow[3] + dataRow[4];
  component = std::string(dataRow[9]);
  section = dataRow[11];
  instrMode = dataRow[17];
  enrlTot = std::stoi(dataRow[24]);
  enrlCap = std::stoi(dataRow[25]);
  roomCap = std::stoi(dataRow[26]);
  std::fill_n(time, 80, 0);
}
