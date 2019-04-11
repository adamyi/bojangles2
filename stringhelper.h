#ifndef STRING_HELPER_H
#define STRING_HELPER_H

#include <string>
#include <vector>

typedef unsigned char uchar;

void split(const std::string &s, char delim, std::vector<std::string> &elems);
int timeToInt(int day, std::string str);
int dayOfWeekToInt(std::string str);

#endif
