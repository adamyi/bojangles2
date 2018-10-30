#include "schedule.hpp"
#include "course.hpp"
#include <vector>
#include <string>
#include <cstdio>
#include <iostream>

CoursePlan::CoursePlan(CourseMap *cm, std::vector<std::string> courseCodes) {
    this->courseCodes = courseCodes;
    for (auto& ccode: courseCodes) {
        //FIXME: consider PGRD
        std::cout << cm->at(ccode)->career << std::endl;
        if (cm->at(ccode)->career == std::string("PGRD"))
            continue;
        for (auto& class_pair: cm->at(ccode)->classes) {
            std::cout << class_pair.first << " "<< class_pair.second.size() << std::endl;
            //TODO: remove ones without any time.

            components.push_back(&class_pair.second);
            std::cout << components.size() << std::endl;

        }
    }
}

void printSchedule(Schedule *s) {
    std::cout << "Schedule{";
    for (auto &curr: *s) {
        std::cout << curr->section<< ", ";
    }
    std::cout << "}" << std::endl;
}
