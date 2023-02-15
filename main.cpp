#include <iostream>
#include <fstream>
#include <algorithm>
#include <cmath>

#include "tema1.h"

using namespace std;

int main(int argc, char* argv[]) {
	// Checks for invalid number of CLI args
	if (argc != 4) {
		exit(-1);
	}
	
	// Extract CLI arguments
	int mapper_ct = atoi(argv[1]);
	int reducer_ct = atoi(argv[2]);
	string input_file = argv[3];

	ifstream fin(input_file);
	if (!fin.is_open()) {
		cout << "File not found!";
		exit(-1);
	}

	// Gets file count
	string line;
	getline(fin, line);
	int files_ct = stoi(line);
	
	// Vector of <file_name, file_size> pairs
	vector< pair<string, int> > file_sizes;

	for (int i = 0; i < files_ct; ++i) {
		getline(fin, line);

		ifstream test_file(line);

		// Computes file size
		test_file.seekg(0, ios::end);
		streampos fileSize = test_file.tellg();

		file_sizes.push_back(make_pair(line, fileSize));
		test_file.close();
	}

	// Sorts vector of pairs ascending based on file size
	sort(file_sizes.begin(), file_sizes.end(), comparator);

	vector<pthread_t> threads(mapper_ct + reducer_ct);

	pthread_mutex_t mutex;
	pthread_mutex_init(&mutex, NULL);

	pthread_barrier_t barrier;
	pthread_barrier_init(&barrier, NULL, mapper_ct + reducer_ct);

	vector<mapper_list> mapper_matrix(mapper_ct);
	vector< vector<int> > reducer_matrix(reducer_ct);

	// Vector of "thread_info" type structures, one for every thread
	thread_info data_vect[mapper_ct + reducer_ct];

	// Populates each "thread_info" type element
	for (int i = 0; i < mapper_ct + reducer_ct; ++i) {
		data_vect[i].thread_id = i;
		data_vect[i].reducer_ct = reducer_ct;
		data_vect[i].mapper_ct = mapper_ct;
		data_vect[i].mutex = &mutex;
		data_vect[i].barrier = &barrier;
		data_vect[i].file_sizes = &file_sizes;
		data_vect[i].mapper_matrix = &mapper_matrix;
		data_vect[i].reducer_matrix = &reducer_matrix;
	}

	// Creates all threads
	for (long unsigned int id = 0; id < threads.size(); ++id) {
		int result =
			pthread_create(&threads[id], NULL, parallel,
				(void *) &(data_vect[id]));

        if (result) {
			cout << "Eroare la crearea thread-ului " << id << endl;
            exit(-1);
        }
	}

	// Joins all threads
	for (long unsigned int id = 0; id < threads.size(); ++id) {
        int result = pthread_join(threads[id], NULL);
 
        if (result) {
            cout << "Eroare la asteptarea thread-ului " << id << endl;
            exit(-1);
        }
    }

	// Destroys barrier and mutex
	pthread_barrier_destroy(&barrier);
	pthread_mutex_destroy(&mutex);

	return 0;
}