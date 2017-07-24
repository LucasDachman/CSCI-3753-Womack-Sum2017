#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#define DEFAULT_ITERATIONS 1000000
#define RADIUS (RAND_MAX / 2)

#define USAGE "<iterations>"

/* function prototypes */
double dist(double x0, double y0, double x1, double y1){
    return sqrt(pow((x1-x0),2) + pow((y1-y0),2));
}

double zeroDist(double x, double y){
    return dist(0, 0, x, y);
}

int main(int argc, char * argv[]) {

	long i, iterations;
	double x, y;
    double inCircle = 0.0;
    double inSquare = 0.0;
    double pCircle = 0.0;
    double piCalc = 0.0; 

	// set iterations
	if (argc < 2 || atoi(argv[1]) == -1) {
		iterations = DEFAULT_ITERATIONS;
	    printf("Iterations value set to default: %ld\n", iterations );
	} else {	
			iterations = atol(argv[1]);
	    	printf("Iterations value set to : %ld\n", iterations );
	}


	/* Calculate pi using statistical methode across all iterations*/
    for(i=0; i<iterations; i++){
		x = (random() % (RADIUS * 2)) - RADIUS;
		y = (random() % (RADIUS * 2)) - RADIUS;
		if(zeroDist(x,y) < RADIUS){
			inCircle++;
		}
		inSquare++;
    }

    /* Finish calculation */
    pCircle = inCircle/inSquare;
    piCalc = pCircle * 4.0;

    /* Print result */
    fprintf(stdout, "pi = %f\n", piCalc);

	return 0;

}
