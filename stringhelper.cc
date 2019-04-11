#include <boost/algorithm/string.hpp>
#include <map>
#include <sstream>
#include <string>
#include <vector>

// take from http://stackoverflow.com/a/236803/248823
void split(const std::string &s, char delim, std::vector<std::string> &elems) {
  std::stringstream ss;
  ss.str(s);
  std::string item;
  while (std::getline(ss, item, delim)) {
    boost::algorithm::trim(item);
    elems.push_back(item);
  }
}

// str: HH:MM
int timeToInt(int day, std::string str) {
  int hours = (str[0] - '0') * 10 + str[1] - '0';
  int minutes = (str[3] - '0') * 10 + str[4] - '0';
  return day * (22 - 7) * 6 + (hours - 7) * 6 + minutes / 10;
}

int dayOfWeekToInt(std::string str) {
  std::map<std::string, int> dti;
  dti["Sun"] = 0;
  dti["Mon"] = 1;
  dti["Tue"] = 2;
  dti["Wed"] = 3;
  dti["Thu"] = 4;
  dti["Fri"] = 5;
  dti["Sat"] = 6;
  return dti[str];
}
