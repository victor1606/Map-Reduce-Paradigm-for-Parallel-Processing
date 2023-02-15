#ifndef TEMA_1_H
#define TEMA_1_H

#include <vector>
#include <pthread.h>
#include <cstring>

#define MAX_MAPPERS 32
#define MAX_REDUCERS 32
#define MAX_INT_EXPONENT 31
using namespace std;

typedef struct mapper_list{
    vector< vector<int> > partial_list = vector< vector<int> >(MAX_REDUCERS);
} mapper_list;

typedef struct thread_info{
    int thread_id;
    int reducer_ct;
    int mapper_ct;
    pthread_mutex_t *mutex;
    pthread_barrier_t *barrier;
	vector< pair<string, int> > *file_sizes;
    vector<mapper_list> *mapper_matrix;
    vector< vector<int> > *reducer_matrix;
} thread_info;

int get_file_size(ifstream fin);
bool comparator(pair<string, int> &x, pair<string, int> &y);
vector<int> perfect_power_fct(int n);
void mapper_fct(thread_info *data);
void reducer_fct(thread_info *data);
void *parallel(void *arg);

inline double fastPow(double a, double b);

#endif
