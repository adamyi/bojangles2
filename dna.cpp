#include "course.h"
#include "schedule.h"
#include "threads.h"
#include "clash.h"
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>

const int POP_SIZE = 5000;
const int GENERATIONS = 1000;
const int MUTATION_CHANCE = 150;

/*
class CoursePlan {
    public:
        std::vector<std::string> courseCodes;
        std::vector<std::vector<Class*>*> components;
        CoursePlan(CourseMap cm, std::vector<std::string> courseCodes);
};
*/

typedef std::vector<Class*> Schedule;

Schedule *randomPopulation(CoursePlan *cp) {
    Schedule *population = new Schedule[POP_SIZE];
    for (int i = 0; i < POP_SIZE; i++) {
        for (auto component: cp->components) {
            population[i].push_back(component->at(rand() % component->size()));
        }
    }
    return population;
}

int fitness(Schedule *schedule) {
    //TODO: make this similar to python fitness
    int fit = 0;
    uchar time[80];
    uchar tmp_time[80];
    std::fill_n(time, 80, 0);
    // printf("fs %d\n", schedule->size());
    for (auto _class: *schedule) {
        memcpy(tmp_time, _class->time, 80);
        for (int i = 0; i < 80; i++) {
            tmp_time[i] &= time[i];
            time[i] |= _class->time[i];
        }

        // fit += _class->enrlTot;
        fit += count_bits_asm(tmp_time, 80);
        // _class->print();
        // printf("fit %d\n", fit);
        // fit += _class->enrlTot;
    }
    return 100 - fit;
}

// calculate fitness of entire population
// return sum of fitness
int calcPopulationWeight(Schedule *population, int *weights) {
    int sum = 1; // so that we don't divide by zero
    #pragma omp parallel for reduction(+:sum)
    for (int i = 0; i < POP_SIZE; i++) {
        int fit = fitness(population + i);
        weights[i] = fit;
        sum += fit;
    }
    return sum;
}

/*
 * Choose a random dna by imitating the process of finding a girlfriend with
 * standards gradually lowered.
 *
 *  Disclaimer: this might not be a universal fact for finding your loved one.
 *  We are not responsible for using this algorithm to find a date. Use at your
 *  own risk ^_^
 */
Schedule rouletteSelect(Schedule *population, int *weights, int totalFit) {
    int criteria = rand() % totalFit;
    for (int i = 0; i < POP_SIZE; i++) {
        if (criteria < weights[i]) {
            // std::cout << "rselect " << i << ' ' << population[i].size() << std::endl;
            return population[i];
        }
        criteria -= weights[i];
    }
    // std::cout << "rselect " << POP_SIZE - 1 << ' ' << population[POP_SIZE - 1].size() << std::endl;
    return population[POP_SIZE - 1];
}

/*
 *
def crossover(dna1, dna2):
  """
  Slices two DNAs at a random index and swaps one part to generate two newborn
  DNAs.
  """
  pos = int(random.random() * len(dna1))
  return (dna1[:pos] + dna2[pos:], dna2[:pos] + dna1[pos:])

def mutate(comps, dna):
  """
  Copy a DNA while randomly mutating it with chance 1/MUTATION_CHANCE
  """
  dna_out = []
  for gl in xrange(len(comps)):
    if int(random.random() * MUTATION_CHANCE) == 1:
      dna_out.append(random_int(len(comps[gl])))
    else:
      dna_out.append(dna[gl])
  return dna_out
  */
void crossover(Schedule dna1, Schedule dna2, Schedule *newDna1, Schedule *newDna2) {
    int dna_size = dna1.size();
    int pos = rand() % dna_size;
    // printf("crossover at %d\n", pos);

    for (int i = 0; i < pos; i++) {
        newDna1->push_back(dna2.at(i));
        newDna2->push_back(dna1.at(i));
    }
    for (int i = pos; i < dna_size; i++) {
        newDna1->push_back(dna1.at(i));
        newDna2->push_back(dna2.at(i));
    }
}

void mutate(CoursePlan *cp, Schedule *schedule) {
    int dna_size = schedule->size();
    for (int i = 0; i < dna_size; i++) {
        if (rand() % MUTATION_CHANCE == 0) {
            std::vector<Class*> *component = cp->components.at(i);
            schedule->at(i) = component->at(rand() % component->size());
        }
    }
}

Schedule findOptimalSchedule(CoursePlan *cp) {
    // generate initial random population
    Schedule *population = randomPopulation(cp);
    int *weights = new int[POP_SIZE];

    // evolve
    for (int i = 0; i < GENERATIONS; i++) {
        int totalFit = calcPopulationWeight(population, weights);
        std::cout << "Generation #" << i << " " << totalFit << std::endl;
        Schedule *newPopulation = new Schedule[POP_SIZE];
        #pragma omp parallel for
        for (int j = 0; j < POP_SIZE; j += 2) {
            Schedule dna1 = rouletteSelect(population, weights, totalFit);
            Schedule dna2 = rouletteSelect(population, weights, totalFit);

            /*std::cout << "origin dna1: ";
            printSchedule(&dna1);
            std::cout << "origin dna2: ";
            printSchedule(&dna2);*/

            crossover(dna1, dna2, newPopulation + j, newPopulation + j + 1);

            /*std::cout << "cross dna1: ";
            printSchedule(newPopulation + j);
            std::cout << "cross dna2: ";
            printSchedule(newPopulation + j + 1);*/

            mutate(cp, newPopulation + j);
            mutate(cp, newPopulation + j + 1);

            /*std::cout << "mutat dna1: ";
            printSchedule(newPopulation + j);
            std::cout << "mutat dna2: ";
            printSchedule(newPopulation + j + 1);*/
        }
        delete[] population;
        population = newPopulation;
    }
    //TODO: return value
    delete[] population;
}


/*
# GA algorithm

def random_int(size):
  """
  Return a random int from 0 to size - 1
  """
  return random.randint(0,size - 1)

def random_population(comps):
  """
  Return the initial random population of DNAs by randomizing.
  """
  pop = []
  for i in xrange(POP_SIZE):
    dna = []
    for c in xrange(len(comps)):
      dna.append(random_int(len(comps[c])))
    pop.append(dna)
  return pop

def roulette_select(dnas):
  """
  Choose a random dna by imitating the process of finding a girlfriend with
  standards gradually lowered.

  Disclaimer: this might not be a universal fact for finding your loved one.
  We are not responsible for using this algorithm to find a date. Use at your
  own risk ^_^

  * This is actually an O(n) approach to the Fitness Proportionate Selection
  * problem.
  """
  fitness_total = sum((dna[1] for dna in dnas))
  criteria = random.uniform(0, fitness_total)
  for dna, fitness in dnas:
    if criteria < fitness:
      return dna
    criteria -= fitness

  return dna

def crossover(dna1, dna2):
  """
  Slices two DNAs at a random index and swaps one part to generate two newborn
  DNAs.
  """
  pos = int(random.random() * len(dna1))
  return (dna1[:pos] + dna2[pos:], dna2[:pos] + dna1[pos:])

def mutate(comps, dna):
  """
  Copy a DNA while randomly mutating it with chance 1/MUTATION_CHANCE
  """
  dna_out = []
  for gl in xrange(len(comps)):
    if int(random.random() * MUTATION_CHANCE) == 1:
      dna_out.append(random_int(len(comps[gl])))
    else:
      dna_out.append(dna[gl])
  return dna_out

if __name__ == "__main__":

  if (len(sys.argv) == 1):
    print("Usage: ./%s COMP1511 MATH1131......" % sys.argv[0])
    sys.exit(1)

  comps = read_data(sys.argv[1:])

  population = random_population(comps)
  HOUR_LOWER, HOUR_UPPER = get_hours_bound(comps, population)

  fits = []
  mean_fits = []

  DAYS = ['Mon', 'Tue', 'Wed', 'Thu', 'Fri', 'Sat', 'Sun']
  ftp = {day: [] for day in DAYS}

  for generation in tqdm(range(GENERATIONS)):
    try:
      max_fit = max(weighted_population, key=lambda pair: pair[1])
      fits.append(get_fitness_vals(comps, max_fit[0], ftp))
      mf = (0,0,0,0,0)
      for ind in weighted_population:
        mf = tuple(x + y for x, y in izip(mf, get_fitness_vals(comps, ind[0], ftp)))
      mean_fits.append(mf)
      # print "Generation %s: max_fit = '%s' fitness: %s" % \
      #     (generation, json.dumps(max_fit), fits[-1])
    except Exception as e:
        pass #print e


    weighted_population = [[dna, fitness(comps, dna)] for dna in population]
    population = []

    for _ in xrange(POP_SIZE / 2):
      # Selection
      dna1 = roulette_select(weighted_population)
      dna2 = roulette_select(weighted_population)

      # Crossover
      dna1, dna2 = crossover(dna1, dna2)

      # Mutate and add back into the population.
      population.append(mutate(comps, dna1))
      population.append(mutate(comps, dna2))

    HOUR_LOWER, HOUR_UPPER = get_hours_bound(comps, population)

  # weighted_population = [[dna, fitness(comps, dna)] for dna in population]
  # fittest_schedule = max(weighted_population, key=lambda pair: pair[1])

  # fits.append(fittest_schedule)
  # print "Fittest in all gens: %s" % json.dumps(max(fits, key=lambda pair: pair[1]))

  x = range(len(fits))
  pyplot.plot(x,[pair[0] for pair in mean_fits], label='course clash hour')
  pyplot.plot(x,[pair[1] for pair in mean_fits], label='number of day')
  pyplot.plot(x,[pair[2] for pair in mean_fits], label='total hours')
  pyplot.plot(x,[pair[3] for pair in mean_fits], label='ftp clash hour')
  pyplot.plot(x,[pair[4] for pair in mean_fits], label='variance')
  pyplot.legend()
  pyplot.show()
*/
