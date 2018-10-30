#ifndef STRING_HELPER_HPP
#define STRING_HELPER_HPP

#include <vector>
#include <string>

typedef unsigned char uchar;

void split(const std::string &s, char delim, std::vector<std::string> &elems);
int timeToInt(int day, std::string str);
int dayOfWeekToInt(std::string str);

#endif
