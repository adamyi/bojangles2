#ifndef COURSE_H
#define COURSE_H

#include <vector>
#include <map>
#include <string>
#include "stringhelper.h"

class Meeting {
    public:
    std::vector<int> weeks; // ignore this for now
    uchar time[80];
    Meeting(std::vector<std::string> dataRow);
    void setTime(int start, int end);
    void print();
};

class Class {
    public:
    std::vector<Meeting> mtgs; // times
    // ignore RSV and CLN now... Since they're pretty useless
    std::string component; // e.g. LEC, TUT
    std::string section; // e.g. F09A
    std::string instrMode; // instruction mode, e.g. in person
    int enrlTot; // how many students currently enrol
    int enrlCap; // how many students it currently allows to enrol
    int roomCap; // how many students it has room to enrol
    Class(std::vector<std::string> dataRow);
};

class Course {
    public:
    std::string code;
    std::string name;
    std::vector<Class> classes;
};

typedef std::map<std::string, Course> CourseMap;

#endif
