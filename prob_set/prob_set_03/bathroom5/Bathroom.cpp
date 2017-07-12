#include <iostream>
#include <queue>

#include <unistd.h>

#include "Bathroom.cpp"

using namespace std;

Bathroom::Bathroom(int _capacity) {
	capacity = _capacity;
	pthread_create(&resolver_thread, 0, bathroom_access, 0);
}


void Bathroom::m_use_bathroom() {
	while( !q.empty() ) 
		usleep(100);
	person* p = new person();
	p->thread = new pthread_t;
	p->gender = MALE;
	q.push(p);
}


void Bathroom::f_use_bathroom() {

	while( !q.empty() ) 
		usleep(100);
	person* p = new person();
	p->thread = new pthread_t;
	p->gender = FEMALE;
	q.push(p);
}

void* Bathroom::bathroom_access(void *args) {
	while( q.empty() ) {
		usleep(100);	
	}
	
	person p = q.pop();
	pthread_create(:w

}

