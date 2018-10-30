#include "course.hpp"
#include "schedule.hpp"
#include "threads.hpp"
#include "bitwisehelper.hpp"
#include "fitness.hpp"
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>

const int POP_SIZE = 1000;
const int GENERATIONS = 200;
const int MUTATION_CHANCE = 150;

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


// calculate fitness of entire population
// return sum of fitness
int calcPopulationWeight(Schedule *population, int *weights, Schedule **optimalSchedule, int *optimalWeight) {
    int sum = 1; // so that we don't divide by zero
    #pragma omp parallel for reduction(+:sum)
    for (int i = 0; i < POP_SIZE; i++) {
        int fit = fitness(population + i);
        weights[i] = fit;
        sum += fit;
        if (fit > *optimalWeight) {
            *optimalWeight = fit;
            *optimalSchedule = population + i;
        }
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

    Schedule *optimalSchedule = NULL;
    int optimalWeight = -1;

    // evolve
    for (int i = 0; i < GENERATIONS; i++) {
        int totalFit = calcPopulationWeight(population, weights, &optimalSchedule, &optimalWeight);
        std::cout << "Generation #" << i << " " << totalFit << " " << optimalWeight << std::endl;
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

    Schedule ret = *optimalSchedule;

    delete[] population;

    return ret;
}

