#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define pop_size 400
#define GRIDSIZE 12

int population[pop_size][GRIDSIZE - 2];
int direction[pop_size], orientation[pop_size];
float inf[pop_size], turns[pop_size], steps[pop_size];
float fitness[pop_size];

int grid[GRIDSIZE][GRIDSIZE] = {{0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0},
                                {0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 1, 0},
                                {0, 1, 1, 0, 0, 0, 1, 0, 0, 1, 1, 0},
                                {0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0},
                                {0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0},
                                {0, 1, 0, 0, 1, 0, 0, 0, 0, 1, 1, 0},
                                {0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                                {0, 1, 1, 0, 0, 1, 1, 1, 0, 0, 1, 0},
                                {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                                {0, 0, 0, 0, 1, 0, 0, 0, 1, 1, 0, 0},
                                {1, 1, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0},
                                {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}};

void generate_population(int population[pop_size][GRIDSIZE - 2]);
void generate_direction_orientation(int direction[pop_size], int orientation[pop_size]);
void copymaker(int copyarray[GRIDSIZE], int chromosome[GRIDSIZE - 2]);
int pathfunc(int copy[GRIDSIZE], int path[GRIDSIZE * GRIDSIZE][2], int c);
void pathevaluator(int population[pop_size][GRIDSIZE - 2], int orientation[pop_size], int direction[pop_size], float inf[pop_size], float turns[pop_size], float steps[pop_size]);
void fitnessfn(float fitness[pop_size], float inf[pop_size], float turns[pop_size], float steps[pop_size]);
void sortfitness(float fitness[pop_size], int population[pop_size][GRIDSIZE - 2]);
void crossoverfn(int population[pop_size][GRIDSIZE - 2]);
void mutation(int population[pop_size][GRIDSIZE - 2]);
int solution(float fitness[pop_size], float inf[pop_size]);
void display_soln(int population[pop_size][GRIDSIZE - 2], int index);

void generate_population(int population[pop_size][GRIDSIZE - 2])
{
    srand(time(NULL));
    for (int i = 0; i < pop_size; i++)
    {
        for (int j = 0; j < GRIDSIZE - 2; j++)
        {
            population[i][j] = (rand() % GRIDSIZE);
        }
    }
}

void generate_direction_orientation(int direction[pop_size], int orientation[pop_size])
{
    for (int i = 0; i < pop_size; i++)
    {
        direction[i] = rand() % 2;
    }

    for (int i = 0; i < pop_size; i++)
    {
        orientation[i] = rand() % 2;
    }
}

void copymaker(int copyarray[GRIDSIZE], int chromosome[GRIDSIZE - 2])
{
    for (int i = 0; i < GRIDSIZE; i++)
    {
        if (i == 0)
        {
            copyarray[i] = 0;
        }
        else if (i == GRIDSIZE - 1)
        {
            copyarray[i] = GRIDSIZE - 1;
        }
        else
        {
            copyarray[i] = chromosome[i - 1];
        }
    }
}

int pathfunc(int copy[GRIDSIZE], int path[GRIDSIZE * GRIDSIZE][2], int c)
{
    int pathrow = 0, step = 0;

    if (direction[c] == 0 && orientation[c] == 0)
    {
        for (int a = 0; a < GRIDSIZE; a++)
        {
            int b = copy[a];
            if (a < (GRIDSIZE - 1))
            {
                pathrow = copy[a + 1];
            }
            else
            {
                b = (GRIDSIZE - 1);
                pathrow = (GRIDSIZE - 1);
            }
            if (pathrow - b >= 0)
            {
                while (b <= pathrow)
                {
                    path[step][0] = b;
                    path[step][1] = a;
                    step++;
                    b++;
                }
            }
            else
            {
                while (b >= pathrow)
                {
                    path[step][0] = b;
                    path[step][1] = a;
                    step++;
                    b--;
                }
            }
        }
    }
    else if (direction[c] == 1 && orientation[c] == 0)
    {
        path[step][0] = 0;
        path[step][1] = 0;
        step++;

        for (int a = 1; a < GRIDSIZE; a++)
        {
            int b = copy[a - 1];
            if (a <= GRIDSIZE - 1)
            {
                pathrow = copy[a];
            }
            else
            {
                b = (GRIDSIZE - 1);
                pathrow = (GRIDSIZE - 1);
            }
            if (pathrow - b >= 0)
            {
                while (b <= pathrow)
                {
                    path[step][0] = b;
                    path[step][1] = a;
                    step++;
                    b++;
                }
                continue;
            }
            else
            {
                while (b >= pathrow)
                {
                    path[step][0] = b;
                    path[step][1] = a;
                    step++;
                    b--;
                }
            }
        }
    }

    else if (direction[c] == 0 && orientation[c] == 1)
    {
        path[step][0] = 0;
        path[step][1] = 0;
        step++;

        for (int a = 1; a < GRIDSIZE; a++)
        {
            int b = copy[a - 1];
            if (a <= GRIDSIZE - 1)
            {
                pathrow = copy[a];
            }
            else
            {
                b = (GRIDSIZE - 1);
                pathrow = (GRIDSIZE - 1);
            }
            if (pathrow - b >= 0)
            {
                while (b <= pathrow)
                {
                    path[step][0] = a;
                    path[step][1] = b;
                    step++;
                    b++;
                }
                continue;
            }
            else
            {
                while (b >= pathrow)
                {
                    path[step][0] = a;
                    path[step][1] = b;
                    step++;
                    b--;
                }
            }
        }
    }

    else if (direction[c] == 1 && orientation[c] == 1)
    {
        for (int a = 0; a < GRIDSIZE; a++)
        {
            int b = copy[a];
            if (a < (GRIDSIZE - 1))
            {
                pathrow = copy[a + 1];
            }
            else
            {
                b = (GRIDSIZE - 1);
                pathrow = (GRIDSIZE - 1);
            }
            if (pathrow - b >= 0)
            {
                while (b <= pathrow)
                {
                    path[step][0] = a;
                    path[step][1] = b;
                    step++;
                    b++;
                }
            }
            else
            {
                while (b >= pathrow)
                {
                    path[step][0] = a;
                    path[step][1] = b;
                    step++;
                    b--;
                }
            }
        }
    }
    return step;
}

void pathevaluator(int population[pop_size][GRIDSIZE - 2], int orientation[pop_size], int direction[pop_size], float inf[pop_size], float turns[pop_size], float steps[pop_size])
{

    for (int c = 0; c < pop_size; c++)
    {
        int copyarray[GRIDSIZE];
        int path[GRIDSIZE * GRIDSIZE][2];
        int infeas = 0, turn = 0, step = 0;

        copymaker(copyarray, population[c]);
        step = pathfunc(copyarray, path, c);

        steps[c] = step - 1;

        for (int i = 0; i < GRIDSIZE - 1; i++)
        {
            if (copyarray[i] != copyarray[i + 1])
            {
                turn++;
            }
        }

        turns[c] = turn;

        for (int l = 0; l < step; l++)
        {
            int j = 0;
            if (grid[path[l][j]][path[l][j + 1]] == 1)
            {
                infeas++;
            }
        }

        inf[c] = infeas;
    }
}

void fitnessfn(float fitness[pop_size], float inf[pop_size], float turns[pop_size], float steps[pop_size])
{
    float maxinf = 0, mininf = 00000, maxturns = 0, minturns = 100000, maxsteps = 0, minsteps = 100000;
    for (int i = 0; i < pop_size; i++)
    {
        if (inf[i] > maxinf)
        {
            maxinf = inf[i];
        }
        if (turns[i] > maxturns)
        {
            maxturns = turns[i];
        }
        if (steps[i] > maxsteps)
        {
            maxsteps = steps[i];
        }

        // if (inf[i] < mininf)
        // {
        //     mininf = inf[i];
        // }

        if (turns[i] < minturns)
        {
            minturns = turns[i];
        }
        if (steps[i] < minsteps)
        {
            minsteps = steps[i];
        }
    }

    float fitval = 0, fturn = 0, flength = 0, finf = 0;
    for (int j = 0; j < pop_size; j++)
    {
        finf = 1 - ((inf[j] - mininf) / (maxinf - mininf));
        fturn = 1 - ((turns[j] - minturns) / (maxturns - minturns));
        flength = 1 - ((steps[j] - minsteps) / (maxsteps - minsteps));

        fitval = 3 * 100 * finf * ((2 * fturn + 2 * flength) / (2 + 2));
        fitness[j] = fitval;
    }
}

void sortfitness(float fitness[pop_size], int population[pop_size][GRIDSIZE - 2])
{
    int temp1 = 0, arr_swap[GRIDSIZE - 2];

    for (int i = 0; i < pop_size; i++)
    {
        for (int j = i + 1; j < pop_size; j++)
        {
            if (fitness[i] < fitness[j])
            {
                // fitness value swaping
                temp1 = fitness[i];
                fitness[i] = fitness[j];
                fitness[j] = temp1;

                temp1 = direction[i];
                direction[i] = direction[j];
                direction[j] = temp1;

                temp1 = orientation[i];
                orientation[i] = orientation[j];
                orientation[j] = temp1;

                temp1 = inf[i];
                inf[i] = inf[j];
                inf[j] = temp1;

                temp1 = turns[i];
                turns[i] = turns[j];
                turns[j] = temp1;

                temp1 = steps[i];
                steps[i] = steps[j];
                steps[j] = temp1;

                // chromosomes swaping
                for (int k = 0; k < GRIDSIZE - 2; k++)
                {
                    arr_swap[k] = population[i][k];
                }
                for (int k = 0; k < GRIDSIZE - 2; k++)
                {
                    population[i][k] = population[j][k];
                }
                for (int k = 0; k < GRIDSIZE - 2; k++)
                {
                    population[j][k] = arr_swap[k];
                }
            }
        }
    }
}

void crossoverfn(int population[pop_size][GRIDSIZE - 2])
{
    int parents = pop_size / 2;

    for (int i = 0; i < parents; i++)
    {
        for (int j = 0; j < (GRIDSIZE - 2) / 2; j++)
        {
            population[parents + i][j] = population[i][j];
            population[parents + i + 1][j] = population[i + 1][j];
        }
        for (int k = (GRIDSIZE - 2) / 2; k < (GRIDSIZE - 2); k++)
        {
            population[parents + i][k] = population[i + 1][k];
            population[parents + i + 1][k] = population[i][k];
        }
    }
}

void mutation(int population[pop_size][GRIDSIZE - 2])
{
    // srand(time(NULL));
    int index = 0, val = 0;

    for (int i = 0; i < pop_size; i++)
    {
        index = rand() % GRIDSIZE;
        val = rand() % GRIDSIZE;
        population[i][index] = val;
    }
}

int solution(float fitness[pop_size], float inf[pop_size])
{
    for (int i = 0; i < pop_size; i++)
    {
        if ((inf[i] == 0) && fitness[i] > 275)
        {
            return i;
        }
    }
    return 0;
}

void display_soln(int population[pop_size][GRIDSIZE - 2], int index)
{
    for (int j = 0; j < GRIDSIZE; j++)
    {
        if (j == 0)
        {
            printf("0 ");
        }
        else if (j == GRIDSIZE - 1)
        {
            printf("%2d", GRIDSIZE - 1);
        }
        else
        {
            printf("%2d ", population[index][j - 1]);
        }
    }
    printf("\n");
    printf("direction = %d, orientation = %d\n", direction[index], orientation[index]);
    printf("inf =  %f\n", inf[index]);

    int copy[GRIDSIZE];
    int pathrow = 0;
    int pathdisplay[GRIDSIZE * GRIDSIZE][2];

    int step = 0;
    copymaker(copy, population[index]);
    step = pathfunc(copy, pathdisplay, index);

    char sGrid[GRIDSIZE][GRIDSIZE];

    for (int i = 0; i < GRIDSIZE; i++)
    {
        for (int j = 0; j < GRIDSIZE; j++)
        {
            if (grid[i][j] == 0)
            {
                sGrid[i][j] = ' ';
            }
            else if (grid[i][j] == 1)
            {
                sGrid[i][j] = 178;
            }
        }
    }

    for (int i = 0; i < step; i++)
    {
        printf("(%d,%d)", pathdisplay[i][0], pathdisplay[i][1]);
        if (sGrid[pathdisplay[i][0]][pathdisplay[i][1]] == ' ')
            sGrid[pathdisplay[i][0]][pathdisplay[i][1]] = 46;
    }
    printf("\n");

    sGrid[0][0] = 219;
    sGrid[GRIDSIZE - 1][GRIDSIZE - 1] = 219;

    printf("---------------------------------");
    for (int i = 0; i < GRIDSIZE; i++)
    {
        printf("\n%2d|", i);
        for (int j = 0; j < GRIDSIZE; j++)
        {
            printf("%c", sGrid[i][j]);
            printf("|");
        }
    }
    printf("\n----------------------------------\n  ");
    for (int i = 0; i < GRIDSIZE; i++)
    {
        printf("%d ", i);
    }
    printf("\n");
}