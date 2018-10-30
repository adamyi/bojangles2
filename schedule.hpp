#ifndef SCHEDULE_HPP
#define SCHEDULE_HPP

#include <vector>
#include <map>
#include <string>
#include "stringhelper.hpp"
#include "course.hpp"

class CoursePlan {
    public:
        std::vector<std::string> courseCodes;
        std::vector<std::vector<Class*>*> components;
        CoursePlan(CourseMap *cm, std::vector<std::string> courseCodes);
};

typedef std::vector<Class*> Schedule;

void printSchedule(Schedule *s);

#endif
