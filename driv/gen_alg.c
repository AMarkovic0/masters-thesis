#include"gen_alg.h"

static const int32_t GENERATION_SIZE = 100;
static const int32_t REPRODUCTION_SIZE = 50;
static const int32_t SIMULATION_SIZE = 2000;

static const _parameters params = {0.0f, {0,50}, 5, 0.2f};

void* gen_alg_driv_exec( void* vargp )
{
	char* my_name = drivAPI_set_name( "GEN_ALG" );
	int* conSocket = drivAPI_init_driver( vargp , my_name );
	char read_buffer[ BUF_SIZE ] = { '\0' };
	char write_buffer[ BUF_SIZE ] = { '\0' };
	char wbuf_broker[ BUF_SIZE ] = { '\0' };
	char rbuf_broker[ BUF_SIZE ] = { '\0' };
	struct pollfd pfds = { *conSocket , POLLIN };

	_chromosome population[GENERATION_SIZE];
	_chromosome selected[REPRODUCTION_SIZE];

	_chromosome GLOBAL_BEST;

	while( 1 )
	{
		poll( &pfds, 1 , -1 );
		drivAPI_read_network( *conSocket , read_buffer );
		if( 0 == strcmp( read_buffer , EXECUTE_ALGORITHM ) )
		{
			printf( "%s Executing algorithmi\r\n" , my_name );

			clock_t t_begin = clock();
			srand((int32_t)time(0));

			GLOBAL_BEST = best_global_chromosome(population, selected, &calculate_fitness);

			printf("Fitness function of the best chromosome: %.8f \r\n", GLOBAL_BEST.fitness);
			for(int32_t j = 0; j < GENE_LEN; j++)
				printf("%f ", GLOBAL_BEST.genetic_code[j]);
			printf("\r\n");

			clock_t t_end = clock();
			double Time = (double)(t_end - t_begin) / CLOCKS_PER_SEC;
			printf("Execition time: %f[s]\n", Time);
		}
		else if( 0 == strcmp( read_buffer , SEND_RESULT ) )
		{
			printf( "%s Sending results \r\n" , my_name );

			sprintf( write_buffer , "<%f><%f><%f>" ,
					GLOBAL_BEST.genetic_code[ 0 ] ,
					GLOBAL_BEST.genetic_code[ 1 ] ,
					GLOBAL_BEST.genetic_code[ 2 ] );

			drivAPI_send_network( *conSocket , write_buffer );
			memset( write_buffer , 0 , sizeof( write_buffer ) );
		}
		else
		{
		}

		drivAPI_read_broker( rbuf_broker , my_name );
		if( 0 == strcmp( rbuf_broker , "RETURN80" ) )
		{
			printf( "D1 Received %s \r\n" , rbuf_broker );
			drivAPI_send_network( *conSocket , "+80" );
			memset( rbuf_broker , 0 , sizeof( rbuf_broker ) );
		}
		else if( 0 == strcmp( rbuf_broker , "TURN80" ) )
		{
			printf( "D1 Received %s \r\n" , rbuf_broker );
			drivAPI_send_network( *conSocket , "-80" );
			memset( rbuf_broker , 0 , sizeof( rbuf_broker ) );
		}
		else
		{
		}

		memset( read_buffer , 0 , sizeof( read_buffer ) );
		memset( wbuf_broker , 0 , sizeof( wbuf_broker ) );
	}

	return NULL;
}

void gen_alg_driv_handler( int sig )
{
	return;
}

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

		memcpy(
			tmp,
			selected_population[random_samples[0]].genetic_code,
			GENE_LEN*sizeof(float)
		);
		memcpy(
			child1,
			crossover(selected_population[random_samples[0]].genetic_code,
			selected_population[random_samples[1]].genetic_code,
			random_breaking_point),
			GENE_LEN*sizeof(float)
		);
		memcpy(
			child1,
			crossover(selected_population[random_samples[0]].genetic_code,
			tmp, random_breaking_point),
			GENE_LEN*sizeof(float)
		);

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
		counter++;
	}

	return GBC;
}

float calculate_fitness( float genetic_code[ GENE_LEN ] )
{
	float fitness_value = 0.0f;
	float dt = 0.0002f;

	float Vmax = 10.0f;
	float Vmin = -10.0f;
	float C1 = 0.1284f/0.0021f;
	float C2 = 0.084f/0.0021f;

	float omega_z = PI/2.0f;

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
		errX =	omega_z - omega[1];
		err_int = err_int + errX*dt;
		err_dot = (errX - (omega_z - omega[0]))/dt;

		if(i % 10 == 0 || i == 0)
		{
			Vp = Kp*(errX + 1/Ti*err_int + Td*err_dot);

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

			Udot = 1/(40*PI)*Vp - 1/(40*PI)*U;
			U = U + Udot*0.002;
			Vp = U;

		}

		theta_dot = C1*Vp - C2*theta;
		theta = theta + theta_dot*dt;
		omega[2] = omega[1] + theta*dt;

		omega[0] = omega[1];
		omega[1] = omega[2];
		omega[2] = 0;

		if(errX < 0)
			sum = sum - errX/10.0f;
		else
			sum = sum + errX*100.0f;
	}

	fitness_value = params.target - sum/SIMULATION_SIZE;
	if(fitness_value != fitness_value)
		fitness_value = -100000.0f;

	return fitness_value;
}

