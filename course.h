#ifndef COURSE_H
#define COURSE_H

#include "stringhelper.h"
#include <map>
#include <string>
#include <vector>

/*class Meeting {
    public:
        std::vector<int> weeks; // ignore this for now
        uchar time[80];
        Meeting(std::vector<std::string> dataRow);
        void setTime(int start, int end);
        void print();
};*/

class Class {
private:
  void addTime(int start, int end);

public:
  uchar time[80];
  /* ignore RSV and CLN now... Since they're pretty useless */
  std::string code;      /* e.g. COMP1511 */
  std::string component; /* e.g. LEC, TUT */
  std::string section;   /* e.g. F09A */
  std::string instrMode; /* instruction mode, e.g. in person */
  int enrlTot;           /* how many students currently enrol */
  int enrlCap;           /* how many students it currently allows to enrol */
  int roomCap;           /* how many students it has room to enrol */
  void addTime(std::vector<std::string> dataRow);
  void print();
  Class(std::vector<std::string> dataRow);
};

class Course {
public:
  std::string code;
  std::string name;
  std::string career;
  std::map<std::string, std::vector<Class *>> classes; // components -> classes
};

typedef std::map<std::string, Course *> CourseMap;

#endif
