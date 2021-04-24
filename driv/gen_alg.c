#include"gen_alg.h"

const int32_t GENERATION_SIZE = 100;
const int32_t REPRODUCTION_SIZE = 50;
const int32_t SIMULATION_SIZE = 2000;

const _parameters params = {0.0f, {0,50}, 5, 0.2f};

/* **************************************************************************************************************************
 *
 * FUNCTION DEFINITION PART
 *
 * ************************************************************************************************************************* */

int32_t randomX(int32_t lower, int32_t upper)
{
        return (rand() % (upper - lower + 1)) + lower;
}

void swap(_chromosome* a, _chromosome* b)
{
        _chromosome tmp = *a;
        *a = *b;
        *b = tmp;

        return;
}

int32_t Qsort_partition(_chromosome* arr, int32_t low, int32_t high)
{
        float pivot = arr[high].fitness;
        int32_t i = low-1;

        int32_t j = low;
        for(j; j <= high-1;j++)
        {
                if(arr[j].fitness > pivot)
                {
                        i++;
                        swap(&arr[i], &arr[j]);
                }
        }
        swap(&arr[i+1], &arr[j]);

        return(i+1);
}

void Qsort(_chromosome* arr, int32_t low, int32_t high)
{
        int32_t partitioning_index = 0;
        if(low < high)
        {
                partitioning_index = Qsort_partition(arr, low, high);

                Qsort(arr, low, partitioning_index - 1);
                Qsort(arr, partitioning_index + 1, high);
        }

        return;
}

/*****************************************************************************************************
 *
 * ALGORITHM FUNCTION DEFINITION PART
 *
 * ************************************************************************************************** */

void initial_population(_chromosome* init_pop, float (*fitness_function)(float*))
{
        float selected_value = 0;
        float fitness = 0;

        for(int32_t i = 0; i < GENERATION_SIZE; i++)
        {
                for(int32_t j = 0; j < GENE_LEN; j++)
                {
                        selected_value = (float)random()/RAND_MAX*params.possible_gene_values[1];
                        init_pop[i].genetic_code[j] = selected_value;
                }
                fitness = (*fitness_function)(init_pop[i].genetic_code);
                init_pop[i].fitness = fitness;
        }

        return;
}

void selection(_chromosome* population, _chromosome* selected)
{
        Qsort(population, 0, GENERATION_SIZE-1);
        for(int32_t i = 0; i < REPRODUCTION_SIZE; i++)
        {
                selected[i] = population[i];
        }

        return;
}

float* crossover(float parent1[3], float parent2[3], int32_t break_point)
{
        for(int32_t i = 0; i < break_point; i++)
        {
                parent1[i] = parent2[i];
        }

        return parent1;
}

void create_generation(_chromosome* selected_population, _chromosome* generation, float (*fitness_function)(float*))
{
        int32_t random_samples[2] = {0};
        int32_t random_breaking_point = 0;
        float child1[GENE_LEN] = {0.0f};
        float child2[GENE_LEN] = {0.0f};

        float tmp[GENE_LEN] = {0.0f};

        for(int32_t i = 0; i < GENERATION_SIZE; i++)
        {
                random_samples[0] = randomX(0, REPRODUCTION_SIZE-1);
                do
                {
                        random_samples[1] = randomX(0, REPRODUCTION_SIZE-1);

                }while(random_samples[0] == random_samples[1]);
                random_breaking_point = randomX(0, GENE_LEN - 1);

                //Copy result of crossover between two selected genes in child1 and child2, first crossover places part of parent2 into parent1
                //and second crossover places part of parent1 into parent2 (because working with pointers, we need tmp to save parrent1 values for second crossover):
                memcpy(tmp, selected_population[random_samples[0]].genetic_code, GENE_LEN*sizeof(float));
                memcpy(child1, crossover(selected_population[random_samples[0]].genetic_code,selected_population[random_samples[1]].genetic_code, random_breaking_point),GENE_LEN*sizeof(float));
                memcpy(child1, crossover(selected_population[random_samples[0]].genetic_code, tmp, random_breaking_point),GENE_LEN*sizeof(float));

                memcpy(generation[i].genetic_code, child1, GENE_LEN*sizeof(float));
                generation[i].fitness = (*fitness_function)(child1);

                if(i != GENERATION_SIZE-1)
                {
                memcpy(generation[i+1].genetic_code, child1, GENE_LEN*sizeof(float));
                generation[i+1].fitness = (*fitness_function)(child2);
                }
        }

}

_chromosome best_global_chromosome(_chromosome* population, _chromosome* selected, float (*fitness_function)(float*))
{
        initial_population(population, fitness_function);

        _chromosome GBC;
        GBC.fitness = -10000;
        memset(GBC.genetic_code, 0, GENE_LEN*sizeof(float));

        int32_t counter = 0;
        while(counter < params.max_iterations)
        {
                selection(population, selected);
                if(selected[0].fitness > GBC.fitness)
                {
                        GBC.fitness = selected[0].fitness;
                        memcpy(GBC.genetic_code, selected[0].genetic_code, GENE_LEN*sizeof(float));
                }

                create_generation(selected, population, fitness_function);

                //Unclomment to print selected chromosomes for each iteration
                /*
                for(int32_t m = 0; m < REPRODUCTION_SIZE; m++)
                {
                        printf("Chromosome: %.8f\n", selected[m].fitness);
                        for(int32_t j = 0; j < GENE_LEN; j++)
                                printf("%f ", selected[m].genetic_code[j]);
                        printf("\n");
                }
                printf("\n\n\n\n\n");
                */

                counter++;
        }

        return GBC;
}

float calculate_fitness( float genetic_code[ GENE_LEN ] )
{
	float fitness_value = 0.0f;
        float dt = 0.0002f; // dt value IMPORTANT VARIABLE 0.0002

        // System parameters:
        float Vmax = 10.0f;
        float Vmin = -10.0f;
        float C1 = 0.1284f/0.0021f;
        float C2 = 0.084f/0.0021f;

        float omega_z = PI/2.0f;

        // System variables:
        float errX = omega_z;
        float err_int = 0.0f;
        float err_dot = 0.0f;
        float Kp = genetic_code[0];
        float Ti = genetic_code[1];
        float Td = genetic_code[2];

        float Vp = 0.0f;
        float omega[3] = {0.0f};
        float theta = 0.0f;
        float theta_dot = 0.0f;

        float Udot = 0.0f;
        float U = 0.0f;

        float sum = 0.0f;
        for(int32_t i = 0; i < SIMULATION_SIZE-1; i++)
        {
                // Error calculations:
                errX =  omega_z - omega[1];
                err_int = err_int + errX*dt;
                err_dot = (errX - (omega_z - omega[0]))/dt;

                if(i % 10 == 0 || i == 0)
                {
                        // Calculate control (PID):
                        Vp = Kp*(errX + 1/Ti*err_int + Td*err_dot);

                        // Anti-windup implementation
                        if(Vp > Vmax)
                        {
                                err_int = err_int + (-Vp + Vmax);
                                Vp = Kp*(errX + 1/Ti*err_int + Td*err_dot);
                        }
                        if(Vp < Vmin)
                        {
                                err_int = err_int + (-Vp + Vmin);
                                Vp = Kp*(errX + 1/Ti*err_int + Td*err_dot);
                        }

                        // Low-pass filter:
                        Udot = 1/(40*PI)*Vp - 1/(40*PI)*U;
                        U = U + Udot*0.002;
                        Vp = U;

                }

                // Solve equations:
                theta_dot = C1*Vp - C2*theta;
                theta = theta + theta_dot*dt;
                omega[2] = omega[1] + theta*dt;

                // Shift time:
                omega[0] = omega[1];
                omega[1] = omega[2];
                omega[2] = 0;

                if(errX < 0)
                        sum = sum - errX/10.0f;
                else
                        sum = sum + errX*100.0f;
        }

        // Calculate fitness:
        fitness_value = params.target - sum/SIMULATION_SIZE;
        // If NaN, discard
        if(fitness_value != fitness_value)
                fitness_value = -100000.0f;

        return fitness_value;
}

void* gen_alg_driv_exec( void* vargp )
{
	char msgq[ 255 ] = { '\0' };
        // Random seed
        clock_t t_begin = clock();
        srand((int32_t)time(0));

        // Initialize population and selected chromosome arrays
        _chromosome population[GENERATION_SIZE];
        _chromosome selected[REPRODUCTION_SIZE];

        // Initialize global best chromosome
        _chromosome GLOBAL_BEST;

        // Perform algorithm and find global best chromosome
        GLOBAL_BEST = best_global_chromosome(population, selected, &calculate_fitness);

        // Print Chromosome values and fitness function
        printf("Fitness function of the best chromosome: %.8f\n", GLOBAL_BEST.fitness);
        for(int32_t j = 0; j < GENE_LEN; j++)
                printf("%f ", GLOBAL_BEST.genetic_code[j]);
        printf("\n");

        // Stop timer and print executing time
        clock_t t_end = clock();
        double Time = (double)(t_end - t_begin) / CLOCKS_PER_SEC;
        printf("Time taken: %f[s]\n", Time);

	sprintf( msgq , "Values: %f %f %f \r\n" ,
			GLOBAL_BEST.genetic_code[ 0 ] ,
			GLOBAL_BEST.genetic_code[ 1 ] ,
			GLOBAL_BEST.genetic_code[ 2 ] );
	brokerQ_push( 1 , 1 ,  msgq );

        // Execute genetic algorithm
        return NULL;
}

