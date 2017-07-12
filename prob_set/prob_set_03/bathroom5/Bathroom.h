#ifndef Bathroom_h
#define Bathroom_h
#include <pthread.h>

#define MALE 0
#define FEMALE 1

typedef struct person;

class Bathroom {
	
	public:
		// functions
		Bathroom();
		void m_request_bathroom();
		void m_use_bathroom();
		void f_request_bathroom();
		void f_use_bathroom();
		// resolver thread
		void* bathroom_access(void*);

		//vars
		std::queue<person*> q;
		int capacity;
		pthread_t resolver_thread;

		typedef struct person {
			pthread_t thread;
			bool gender;
		}

}

#endif
