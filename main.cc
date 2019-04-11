#include <chrono>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <iostream>
#include <string>
#include <vector>

#include "bitwisehelper.h"
#include "course.h"
#include "dna.h"
#include "parse.h"
#include "schedule.h"
#include "stringhelper.h"
#include "threads.h"

inline int currentTime() {
  std::chrono::milliseconds ms =
      std::chrono::duration_cast<std::chrono::milliseconds>(
          std::chrono::system_clock::now().time_since_epoch());
  return ms.count();
}

int main(int argc, const char *argv[]) {
  int ms = currentTime();
  srand(ms);
  printf("Threads: %d\n", num_threads());
  uchar a[10];
  memset(a, 1, sizeof(a));
  int res = count_bits_asm(a, 10);
  printf("%d\n", res);

  CourseMap cm = getClassesFromAdminData("classes");

  /*printf("%p\n", cm.at(std::string("COMP1511")));
  printf("%d\n", cm.at(std::string("COMP1511"))->classes.size());
  printf("%p\n",
  cm.at(std::string("COMP1511"))->classes.at(std::string("LEC")));
  printf("%d\n",
  cm[std::string("COMP1511")]->classes[std::string("LEC")].size());
  printf("%p\n",
  cm[std::string("COMP1511")]->classes[std::string("LEC")].back());

  std::cout <<
  cm[std::string("COMP1511")]->classes[std::string("LEC")][0]->enrlTot;*/

  std::vector<std::string> courseIds;

  for (int i = 1; i < argc; i++) {
    courseIds.push_back(std::string(argv[i]));
    std::cout << "add course: " << argv[i] << std::endl;
  }

  std::cout << "Used " << currentTime() - ms << " milliseconds to initialize."
            << std::endl;

  ms = currentTime();
  CoursePlan cp = CoursePlan(&cm, courseIds);
  Schedule optimal = findOptimalSchedule(&cp);
  std::cout << "Used " << currentTime() - ms << " milliseconds to calculate."
            << std::endl;
  printSchedule(&optimal);

  // TODO(adamyi): properly delete CourseMap and CoursePlan.
  return 0;
}
