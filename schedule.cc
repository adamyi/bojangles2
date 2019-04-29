#include "schedule.h"
#include "course.h"
#include <cstdio>
#include <iostream>
#include <string>
#include <vector>

CoursePlan::CoursePlan(CourseMap *cm, std::vector<std::string> courseCodes) {
  this->courseCodes = courseCodes;
  for (auto &ccode : courseCodes) {
    // TODO(adamyi): consider PGRD
    CourseMap::iterator i = cm->find(ccode);
    if (i == cm->end()) {
      std::cout << "ERROR: " << ccode << " does not exist" << std::endl;
      continue;
    }
    Course *course = i->second;
    std::cout << course->career << std::endl;
    if (course->career == std::string("PGRD"))
      continue;
    for (auto &class_pair : course->classes) {
      std::cout << class_pair.first << " " << class_pair.second.size()
                << std::endl;
      for (auto &clss : class_pair.second) {
        std::cout << clss->code << "_" << clss->section << "; ";
      }
      std::cout << std::endl;
      // TODO(adamyi): remove ones without any time.

      components.push_back(&class_pair.second);
      std::cout << components.size() << std::endl;
    }
  }
}

void printSchedule(Schedule *s) {
  std::cout << "Schedule{";
  bool prefix = false;
  for (auto &curr : *s) {
    if (prefix)
      std::cout << ", ";
    else
      prefix = true;
    std::cout << curr->code << "_";
    std::cout << curr->section;
  }
  std::cout << "}" << std::endl;
}
