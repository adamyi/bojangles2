#ifndef SCHEDULE_H
#define SCHEDULE_H

#include "course.h"
#include "stringhelper.h"
#include <map>
#include <string>
#include <vector>

class CoursePlan {
public:
  std::vector<std::string> courseCodes;
  std::vector<std::vector<Class *> *> components;
  int min_time;
  int max_time;
  CoursePlan(CourseMap *cm, std::vector<std::string> courseCodes);
};

typedef std::vector<Class *> Schedule;

void printSchedule(Schedule *s);

#endif
