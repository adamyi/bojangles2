#ifndef SCHEDULE_H 
#define SCHEDULE_H

#include <vector>
#include <map>
#include <string>
#include "stringhelper.h"
#include "course.h"

class CoursePlan {
    public:
        std::vector<std::string> courseCodes;
        std::vector<std::vector<Class*>*> components;
        CoursePlan(CourseMap *cm, std::vector<std::string> courseCodes);
};

typedef std::vector<Class*> Schedule;

void printSchedule(Schedule *s);

#endif
