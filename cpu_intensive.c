#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <unistd.h>

// Default values
#define DEFAULT_NUM_CALCULATIONS 10000000
#define DEFAULT_MAX_VALUE 1e22
#define DEFAULT_MIN_VALUE 1e9
#define DEFAULT_ITERATIONS_PER_NUMBER 1000

// Global variables to hold configuration
int NUM_CALCULATIONS;
double MAX_VALUE;
double MIN_VALUE;
int ITERATIONS_PER_NUMBER;

void load_config_from_env() {
    char *env_val;
    
    // Load NUM_CALCULATIONS
    env_val = getenv("NUM_CALCULATIONS");
    NUM_CALCULATIONS = env_val ? atoi(env_val) : DEFAULT_NUM_CALCULATIONS;
    if (NUM_CALCULATIONS <= 0) NUM_CALCULATIONS = DEFAULT_NUM_CALCULATIONS;
    
    // Load MAX_VALUE
    env_val = getenv("MAX_VALUE");
    MAX_VALUE = env_val ? atof(env_val) : DEFAULT_MAX_VALUE;
    if (MAX_VALUE <= 0) MAX_VALUE = DEFAULT_MAX_VALUE;
    
    // Load MIN_VALUE
    env_val = getenv("MIN_VALUE");
    MIN_VALUE = env_val ? atof(env_val) : DEFAULT_MIN_VALUE;
    if (MIN_VALUE <= 0) MIN_VALUE = DEFAULT_MIN_VALUE;
    
    // Load ITERATIONS_PER_NUMBER
    env_val = getenv("ITERATIONS_PER_NUMBER");
    ITERATIONS_PER_NUMBER = env_val ? atoi(env_val) : DEFAULT_ITERATIONS_PER_NUMBER;
    if (ITERATIONS_PER_NUMBER <= 0) ITERATIONS_PER_NUMBER = DEFAULT_ITERATIONS_PER_NUMBER;
    
    // Ensure MIN_VALUE < MAX_VALUE
    if (MIN_VALUE >= MAX_VALUE) {
        MIN_VALUE = DEFAULT_MIN_VALUE;
        MAX_VALUE = DEFAULT_MAX_VALUE;
    }
}

double cpu_intensive_sqrt(double x) {
    if (x <= 0) return 0;
    
    double guess = x / 2.0;
    double prev_guess;
    
    for (int i = 0; i < 100; i++) {
        prev_guess = guess;
        guess = (guess + x / guess) / 2.0;
        
        guess += sin(guess * 0.001) * 0.0001;
        guess = fabs(guess);
        
        if (fabs(guess - prev_guess) < 1e-15) break;
    }
    
    return guess;
}

double extreme_cpu_operation(double x) {
    double result = 0;
    
    for (int i = 0; i < ITERATIONS_PER_NUMBER; i++) {
        double temp = cpu_intensive_sqrt(x + i);
        temp += pow(x, 1.1 + (i % 10) * 0.01);
        temp += log(x + 1);
        temp += exp(temp * 0.0001);
        temp += sin(temp) * cos(temp);
        temp += atan2(temp, x + 1);
        
        if (i % 2 == 0) {
            temp *= 1.00001;
        } else {
            temp /= 1.00001;
        }
        
        result += temp;
    }
    
    return result / ITERATIONS_PER_NUMBER;
}

int main() {
    double *numbers;
    double *results;
    clock_t start, end;
    double cpu_time_used;
    
    // Load configuration from environment variables
    load_config_from_env();
    
    setvbuf(stdout, NULL, _IOLBF, 0);
    
    printf("EXTREME CPU INTENSIVE BENCHMARK\n");
    fflush(stdout);
    
    printf("Calculations: %d\n", NUM_CALCULATIONS);
    printf("Iterations per number: %d\n", ITERATIONS_PER_NUMBER);
    printf("Total operations: %lld\n", (long long)NUM_CALCULATIONS * ITERATIONS_PER_NUMBER);
    printf("Single threaded execution\n");
    printf("CPU cores detected: %d\n", (int)sysconf(_SC_NPROCESSORS_ONLN));
    fflush(stdout);
    
    numbers = (double*)malloc(NUM_CALCULATIONS * sizeof(double));
    results = (double*)malloc(NUM_CALCULATIONS * sizeof(double));
    
    if (numbers == NULL || results == NULL) {
        printf("Memory allocation failed\n");
        fflush(stdout);
        return 1;
    }
    
    srand((unsigned int)time(NULL) ^ getpid());
    
    printf("Generating %d random numbers between %.2e and %.2e\n", 
           NUM_CALCULATIONS, MIN_VALUE, MAX_VALUE);
    fflush(stdout);
    
    for (int i = 0; i < NUM_CALCULATIONS; i++) {
        double base = MIN_VALUE + ((double)rand() / RAND_MAX) * (MAX_VALUE - MIN_VALUE);
        numbers[i] = base + sin(base * 1e-10) * 1e6;
    }
    
    printf("Starting extreme CPU intensive calculations\n");
    fflush(stdout);
    
    start = clock();
    
    for (int i = 0; i < NUM_CALCULATIONS; i++) {
        results[i] = extreme_cpu_operation(numbers[i]);
        
        if (i % 1000000 == 0 && i > 0) {
            printf("Progress: %.1f%% complete\n", 100.0 * i / NUM_CALCULATIONS);
            fflush(stdout);
        }
    }
    
    end = clock();
    cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;
    
    printf("PERFORMANCE RESULTS\n");
    printf("Total calculations: %d\n", NUM_CALCULATIONS);
    printf("Operations per calculation: %d\n", ITERATIONS_PER_NUMBER);
    printf("Total floating point operations: %lld\n", (long long)NUM_CALCULATIONS * ITERATIONS_PER_NUMBER * 8);
    printf("Time taken: %.3f seconds\n", cpu_time_used);
    printf("Calculations per second: %.0f\n", NUM_CALCULATIONS / cpu_time_used);
    printf("FLOPS (approx): %.2e\n", (double)NUM_CALCULATIONS * ITERATIONS_PER_NUMBER * 8 / cpu_time_used);
    printf("SYSTEM STRESS TEST COMPLETE\n");
    fflush(stdout);
    
    free(numbers);
    free(results);
    return 0;
}
