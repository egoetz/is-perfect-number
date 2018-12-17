#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <pthread.h>
#define INITIAL_ARRAY_VAL 10

struct shared_vars{
	unsigned long long *array;
	unsigned long long num_factors;
	unsigned long long capacity;
	unsigned long long num_of_threads;
	unsigned long long n;
	unsigned long long upper_bound;
	/*Every factor of an integer (excluding potential square roots) has a pair so that factor * pair
	* = integer where pair is greater than factor. The lowest_factor_pair is the smallest of all 
	* pair values for a given integer */
	unsigned long long lowest_factor_pair;
	pthread_mutex_t mutex;
};

struct thread_vars{
	struct shared_vars *shared;
	unsigned long long thread_num;
};

/*SUMMARY: A function to be run by all threads. Each thread checks multiples of its thread_num that
* are less than upper_bound to see if they are factors of n. factors are put in a shared array.
* PRECONDITION: thread_vars structure is created and a pointer is passed to check_nums. All
* values need to be initialized in the main function prior to calling check_nums.
* POSTCONDITION: A NULL pointer is returned.
* SIDE EFFECTS: my_vars->shared is mutated. Any factors that are found are placed in my_vars->
* shared->array and num_factors and capacity are increased accordingly.*/
void *check_nums(void *my_vars){
	struct thread_vars *my_vars_casted = (struct thread_vars *) my_vars;

	//Get first value to analyze
	unsigned long long current_num = my_vars_casted->thread_num;
	if(current_num == 0){
		current_num += my_vars_casted->shared->num_of_threads;
	}

	//While we need to check values
	while(current_num <= (my_vars_casted->shared->upper_bound)){
		//If we have a factor
		if(my_vars_casted->shared->n % current_num == 0){
			unsigned long long factor_buddy = my_vars_casted->shared->n / current_num;
			pthread_mutex_lock(&my_vars_casted->shared->mutex);

			//Make sure that the factor has not been found yet
			if(my_vars_casted->shared->n / current_num == 
			my_vars_casted->shared->lowest_factor_pair){
				//We have all of the factors
				pthread_mutex_unlock(&my_vars_casted->shared->mutex);
				return NULL;
			}

			//Check whether factor is part of a pair or if it is a perfect square
			if(current_num != 1 && current_num != factor_buddy){
				my_vars_casted->shared->num_factors += 2;
			}
			else{
				my_vars_casted->shared->num_factors++;
			}
			
			//Resize array if necessary
			if(my_vars_casted->shared->num_factors > my_vars_casted->shared->capacity){
				my_vars_casted->shared->array = realloc(my_vars_casted->shared->array, 
				sizeof(unsigned long long) * my_vars_casted->shared->capacity * 2);
				my_vars_casted->shared->capacity *= 2;
			}
			
			//Add factors to array
			if(current_num != 1 && current_num != factor_buddy){
				my_vars_casted->shared->array[my_vars_casted->shared->num_factors - 
				2] = current_num;
				my_vars_casted->shared->array[my_vars_casted->shared->num_factors - 
				1] = factor_buddy;
				if(factor_buddy < my_vars_casted->shared->lowest_factor_pair){
					my_vars_casted->shared->lowest_factor_pair = factor_buddy;
				}
			}
			else{
				my_vars_casted->shared->array[my_vars_casted->shared->num_factors - 
				1] = current_num;
				if(current_num < my_vars_casted->shared->lowest_factor_pair){
					my_vars_casted->shared->lowest_factor_pair = current_num;
				}
			}
			pthread_mutex_unlock(&my_vars_casted->shared->mutex);
		}

		current_num += my_vars_casted->shared->num_of_threads;
	}
	return NULL;
}

/*SUMMARY: A function run by the main thread. It creates the requested number of threads and uses.
* those threads to find out if the given number is a perfect number.
* PRECONDITION: argv must contain three arguments. The first must be the call to the compiled
* program. The second and third must be positive integers.
* POSTCONDITION: If successful, zero is returned.
* SIDE EFFECTS: All factors of the given number and its status as a perfect number is printed.*/
int main(int argc, char **argv){
	//Ensure that needed parameters are passed
	if(argc != 3){
		fprintf(stderr, "Passed incorrect number of args. Must supply potential perfect number \
		and desired number of threads.");
	}

	//Store parameter information
	unsigned long long n = strtoull(argv[1], NULL, 10);
	unsigned long long upper_bound = ceil(sqrt(n));
	unsigned long long num_threads = strtoull(argv[2], NULL, 10);
	if(num_threads > upper_bound){
		num_threads = upper_bound;
	}

	//Initialize shared variables for all threads
	pthread_t *threads = malloc(sizeof(pthread_t) * num_threads);
	struct shared_vars *shared = malloc(sizeof(struct shared_vars));
	shared->array = malloc(sizeof(unsigned long long) * INITIAL_ARRAY_VAL);
	shared->num_factors = 0;
	shared->capacity = INITIAL_ARRAY_VAL;
	shared->num_of_threads = num_threads;
	shared->upper_bound = upper_bound;
	shared->n = n;
	shared->lowest_factor_pair = n + 1;
	pthread_mutex_init(&shared->mutex, NULL);

	//Create array of thread specific structures, initialize the struct members, and create threads
	struct thread_vars thread_var_structs[num_threads];
	for(int i = 0; i < num_threads; i++){
		thread_var_structs[i].shared = shared;
		thread_var_structs[i].thread_num = i;
		pthread_create(&threads[i], NULL, check_nums, (void *)&thread_var_structs[i]); 
	}

	//Join threads
	for(int i = 0; i < num_threads; i++){
		pthread_join(threads[i], NULL);
	}

	//Get sum of all factors and print factors
	unsigned long long sum;
	if(shared->num_factors == 1){
		sum = 1;
		printf("%llu is a prime (its only factor is 1).\n", n);
	}
	else{
		printf("The factors of %llu are", n);
		sum = 0;
		for(int i = 0; i < shared->num_factors; i++){
			sum += shared->array[i];
			if(i + 1 < shared->num_factors){
				printf(" %llu,", shared->array[i]);
			}
			else{
				printf(" and %llu.\n", shared->array[i]);
			}
		}
	}

	//Print whether number is a perfect number
	if(sum == n){
		printf("%llu is a perfect number.\n", n);
	}
	else{
		printf("%llu is not a perfect number.\n", n);
	}

	return 0;
}
