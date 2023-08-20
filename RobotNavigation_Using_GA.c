/* CEP: Autonomous Robot Navigation
-----------------------------------------------------------------------
The autonomous robot navigation is designed to find the most efficient
path to the goal, avoiding all obstacles.This problem is solved using
Search Heuristics "Genetic Algorithm".
-----------------------------------------------------------------------
Written by Abiam Asif Khalid (abiamasifkhalid77@gmail.com) on 23-Dec-2022.
IDE: Visual Studio Code 1.69.0
C Compiler: GCC (Rev1, Built by MSYS2 project) 12.2.0 */

#include <stdio.h>
#include "RobotNavigation_Using_GA.h"

int main()
{
    int iteration = 0, solnfound = 0;

    generate_population(population);
    generate_direction_orientation(direction, orientation);
    pathevaluator(population, direction, orientation, inf, turns, steps);
    fitnessfn(fitness, inf, turns, steps);

    while (solnfound == 0 && iteration < 6000)
    {
        sortfitness(fitness, population);
        iteration++;
        crossoverfn(population);
        mutation(population);

        generate_direction_orientation(direction, orientation);
        pathevaluator(population, orientation, direction, inf, turns, steps);
        sortfitness(fitness, population);
        printf("%4d\t\t", iteration);
        for (int i = 0; i < 1; i++)
        {
            for (int j = 0; j < GRIDSIZE - 2; j++)
            {
                printf("%2d ", population[i][j]);
            }
            printf("\t %d \t\t %d \t\t %f \t %0.0f \t %0.0f \t %0.0f \n", orientation[i], direction[i], fitness[i], inf[i], turns[i], steps[i]);
        }
        fitnessfn(fitness, inf, turns, steps);
        solnfound = solution(fitness, inf);
    }

    if (solnfound != 0)
    {
        printf("\nSolution Found in %d generation\n", iteration + 1);
        display_soln(population, solnfound);
    }
    else
    {
        printf("No solution found, most fit path:\n");
        display_soln(population, 0);
    }
    return 0;
}

// End of Program