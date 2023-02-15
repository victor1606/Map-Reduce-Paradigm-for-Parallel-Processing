#include <fstream>
#include <algorithm>

#include "tema1.h"

using namespace std;

void mapper_fct(thread_info *data) {
	int id = data->thread_id;

	while (data->file_sizes->size() > 0) {
		// Popping files must be inside mutex so that 2 threads don't try to
		// pop the same file at the same time
		pthread_mutex_lock(data->mutex); // Mutex locked

		string file_name =
			data->file_sizes->at(data->file_sizes->size() - 1).first;
		
		data->file_sizes->pop_back();

		pthread_mutex_unlock(data->mutex); // Mutex unlocked

		ifstream fin(file_name); // Opening file

		int count;
		fin >> count;

		for (int i = 0; i < count; ++i) {
			int nr;
			fin >> nr;
			if (nr == 1) { // If nr == 1, it is a perfect power for all exp
				for (int i = 0; i < data->reducer_ct; ++i) {
					data->mapper_matrix->at(id).partial_list[i].push_back(nr);
				}
			} else {
				// Vector that contains all exp that make nr a perfect power
				vector<int> exp_vector = perfect_power_fct(nr);
				for (long unsigned int i = 0; i < exp_vector.size(); ++i) {
					if (exp_vector[i] > 1) {
						// Add nr to the corresponding partial list
						data->mapper_matrix->at(id).
							partial_list[exp_vector[i] - 2].push_back(nr);
					}
				}
			}
		}
		fin.close();
	}

	// Barrier so that all mappers finish before reducers start
	pthread_barrier_wait(data->barrier);
}

void reducer_fct(thread_info *data) {
	// Same barrier as the one in the mapper function
	pthread_barrier_wait(data->barrier);

	int id = data->thread_id - data->mapper_ct;

	// Combines all partial lists
	for (int map = 0; map < data->mapper_ct; ++map) {
		data->reducer_matrix->at(id).insert(data->reducer_matrix->at(id).end(),
			data->mapper_matrix->at(map).partial_list[id].begin(),
			data->mapper_matrix->at(map).partial_list[id].end()); 
	}

	// Removes duplicates from lists
	sort(data->reducer_matrix->at(id).begin(), data->reducer_matrix->at(id).end());
	
	data->reducer_matrix->at(id).erase(unique(data->reducer_matrix->at(id).begin(),
		data->reducer_matrix->at(id).end()), data->reducer_matrix->at(id).end());

	// Opening output file
	ofstream fout("out" + to_string(id + 2) + ".txt");

	// Writing how many unique numbers are left in the final lists
	fout << data->reducer_matrix->at(id).size();
}

void *parallel(void *arg) {
	thread_info data = *(thread_info *)arg;
	
	if (data.thread_id < data.mapper_ct) {
		mapper_fct(&data);
	} 
	else {
		reducer_fct(&data);
	}

	return NULL;
}