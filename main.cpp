#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <vector>
#include <string>
#include <iostream>
#include <ctime>

#include "threads.hpp"
#include "bitwisehelper.hpp"
#include "parse.hpp"
#include "course.hpp"
#include "schedule.hpp"
#include "stringhelper.hpp"
#include "dna.hpp"

int main(int argc, const char *argv[]) {
    srand(time(0));
    printf("Threads: %d\n", num_threads());
    uchar a[10];
    memset(a, 1, sizeof(a));
    int res = count_bits_asm(a, 10);
    printf("%d\n", res);

    CourseMap cm = getClassesFromAdminData("classes");

    /*printf("%p\n", cm.at(std::string("COMP1511")));
    printf("%d\n", cm.at(std::string("COMP1511")).classes.size());
    printf("%p\n", cm.at(std::string("COMP1511")).classes.at(std::string("LEC")));
    printf("%d\n", cm[std::string("COMP1511")].classes[std::string("LEC")].size());
    printf("%p\n", cm[std::string("COMP1511")].classes[std::string("LEC")].back());

    std::cout << cm[std::string("COMP1511")].classes[std::string("LEC")][0]->enrlTot;*/

    std::vector<std::string> courseIds;

    for (int i = 1; i < argc; i++) {
        courseIds.push_back(std::string(argv[i]));
        std::cout << "add course: " << argv[i] << std::endl;
    }

    CoursePlan cp = CoursePlan(&cm, courseIds);
    findOptimalSchedule(&cp);

    //TODO: properly delete CourseMap and CoursePlan.
    return 0;
    
}
