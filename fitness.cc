#include "fitness.h"
#include "bitwisehelper.h"
#include "course.h"
#include "schedule.h"
#include "threads.h"
#include <algorithm>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>

/*
# Fitness Function

def get_hours_bound(comps, population):
    DAYS = ['Mon', 'Tue', 'Wed', 'Thu', 'Fri', 'Sat', 'Sun']
    ftp = {day: [] for day in DAYS}
    minimum = 0
    maximum = 7*24
    for dna in population:
        _, _, hrs, _, _ = get_fitness_vals(comps, dna, ftp)
        minimum = hrs if minimum > hrs else minimum
        maximum = hrs if maximum < hrs else maximum
    return minimum, maximum

def intervaltree_clash(tree, start, end):
    clash = 0
    for int in tree:
        if int.begin < start:
            if int.end > end:
                clash += end - start
            elif int.end > start:
                clash += int.end - start
        elif int.begin < end:
            if int.end > end:
                clash += end - int.begin
            elif int.end > start:
                clash += int.end - int.begin
    return clash

def get_fitness_vals(comps, dna, freetimepref):
  """
  Return course hour clash, total days, total hours, and
  hours clashed with user's preferenced free time (e.g. sleeping time,
  dating time, etc.)
  """
  DAYS = ['Mon', 'Tue', 'Wed', 'Thu', 'Fri', 'Sat', 'Sun']
  occupied = {day: [] for day in DAYS}
  hours = {day: 0 for day in DAYS}
  course_clash = 0
  ftp_clash = 0
  totalHours = 0
  st = timer.time()
  ints = []
  for i in xrange(len(comps)):
    for time in comps[i][dna[i]]['times']:
      if not time['hours']: #e.g. web lectures
        continue
      start = int(time['hours']['start'])
      end = int(time['hours']['end'])
      totalHours += end - start
      hours[time['day']] += end - start
      ints.append([start, end])

  for

  print("CALC TIME: %f" %(timer.time() - st))

  days = len([day for day in DAYS if hours[day] > 0])
  if numpy.count_nonzero(hours.values()) == 1:
    variance = 0
  else:
    avg = sum(hours.values())
    avg /= numpy.count_nonzero(hours.values())
    variance = sum([(hours[day] - avg) ** 2 if hours[day] > 0 else 0 for day in
DAYS]) / (numpy.count_nonzero(hours.values()) - 1)

  return course_clash, days, totalHours, ftp_clash, variance

# evaluate for each param
day_table = {0: 100, 1: 100, 2: 100, 3: 100, 4: 50, 5: 0}

# x should be # of min
clash_value = lambda x: max([-13.7*exp(-0.01*x)+113.6*exp(-0.02*x), 0])
day_value = lambda x: day_table[x]
hr_value = lambda x: (HOUR_UPPER-x)*1.0/(HOUR_UPPER - HOUR_LOWER) * 100
diff_value = lambda x: 0.25*x*x-10.25*x+100

def fitness(comps, dna):
  """
  Where the magic happens.

  The evaluation function of a class schedule (dna).
  The bigger the fitness value, the better the class schedule is.

  TODO: make this really fancy
  """
  DAYS = ['Mon', 'Tue', 'Wed', 'Thu', 'Fri', 'Sat', 'Sun']
  ftp = {day: [] for day in DAYS}

  course_clash, day, hr, ftp_clash, var = get_fitness_vals(comps, dna, ftp)
  return clash_value(course_clash) + day_value(day) + hr_value(hr) * 0.0 +
clash_value(ftp_clash) + diff_value(var)
 * */

inline int clash_duration(Schedule *schedule) {
  int cdur = 0;
  uchar time[80];
  uchar tmp_time[80];
  std::fill_n(time, 80, 0);
  for (auto _class : *schedule) {
    memcpy(tmp_time, _class->time, 80);
    for (int i = 0; i < 80; i++) {
      tmp_time[i] &= time[i];
      time[i] |= _class->time[i];
    }

    cdur += count_bits_asm(tmp_time, 80);
  }
  return std::max(-13.7 * exp(-0.01 * cdur) + 113.6 * exp(-0.02 * cdur), 0.0);
}

inline int total_duration(Schedule *schedule) {
  int tdur = 0;
  for (auto _class : *schedule) {
    tdur += count_bits_asm(_class->time, 80);
  }
  return tdur; // TODO(adamyi): upper lower boundary weight
}

inline int total_days(Schedule *schedule) {
  int ret[7] = {100, 100, 100, 100, 50, 0, 0};
  int tdays = 0;
  uchar time[80];
  std::fill_n(time, 80, 0);
  for (auto _class : *schedule) {
    for (int i = 0; i < 80; i++) {
      time[i] |= _class->time[i];
    }
  }
  for (int i = 0; i < 7; i++)
    if (timeInDay(time, i) > 0)
      tdays++;
  // printf("tdays %d\n", tdays);
  return ret[tdays];
}

inline int day_diff_duration(Schedule *schedule) {
  // TODO(adamyi)
  return 0;
}

int fitness(Schedule *schedule) {
  return clash_duration(schedule) + total_duration(schedule) +
         total_days(schedule) + day_diff_duration(schedule);
}
