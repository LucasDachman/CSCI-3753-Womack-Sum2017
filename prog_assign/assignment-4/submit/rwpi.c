#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <math.h>
#include <fcntl.h>
#include <sys/stat.h>

#define DEFAULT_ITERATIONS 1000000
#define RADIUS (RAND_MAX / 2)

/* Local Defines */
#define MAXFILENAMELENGTH 80
#define DEFAULT_INPUTFILENAME "/dev/urandom"
#define DEFAULT_OUTPUTFILENAMEBASE "rwoutput"
#define DEFAULT_OUTPUTFILENAME "/dev/null"
#define DEFAULT_BLOCKSIZE 1024
#define DEFAULT_TRANSFERSIZE 1024*100
#define USAGE "<outputfilenameBase> <iterations>"


/* function prototypes */
double calc_pi(long iterations);

double dist(double x0, double y0, double x1, double y1){
    return sqrt(pow((x1-x0),2) + pow((y1-y0),2));
}

double zeroDist(double x, double y){
    return dist(0, 0, x, y);
}

/* Global Variables */
int main(int argc, char *argv[])
{

	char outputFilename[MAXFILENAMELENGTH];
	ssize_t buffersize;
	ssize_t blocksize = 0;
	ssize_t transfersize = 0;
	char *transferBuffer = NULL;
	long iterations;
	double pi;

	int outputFD;

	ssize_t bytesWritten = 0;
	ssize_t totalBytesWritten = 0;
	int totalWrites = 0;

	/* Process program arguments to select run-time parameters */
	/* check params */
	if( argc > 1 && strcmp(argv[1], "-h") == 0 ) {
		fprintf(stdout,"Use -1 to set a parameter to its default.\n"
					   "defaults:\n"
					   "transfersize: %d\n"
					   "blocksize: %d\n"
					   "inputfilename: %s\n"
					   "outputfilename: %s\n",
					   DEFAULT_TRANSFERSIZE, DEFAULT_BLOCKSIZE, DEFAULT_INPUTFILENAME,
					   DEFAULT_OUTPUTFILENAME);

	}
	if (argc < 3) {
		printf("USAGE: %s %s\n", argv[0], USAGE);
		printf("%s -h for hep\n", argv[0]);
		exit(EXIT_FAILURE);
	}

	/* Set supplied transfer size or default if not supplied */
	transfersize = DEFAULT_TRANSFERSIZE;
	
	blocksize = DEFAULT_BLOCKSIZE;
	
	/* Set supplied output filename base or default if not supplied */
	if (strnlen(argv[1], MAXFILENAMELENGTH) >= MAXFILENAMELENGTH) {
		fprintf(stderr, "Output filename base is too long\n");
		exit(EXIT_FAILURE);
	}
	strncpy(outputFilename, argv[1], MAXFILENAMELENGTH);
	if (strcmp(outputFilename, "-1") == 0) {
		strncpy(outputFilename, DEFAULT_OUTPUTFILENAME,MAXFILENAMELENGTH);
		printf("Output Filename set to default: %s\n", outputFilename);
	}

	// set iterations
	if (atoi(argv[2]) == -1) {
		iterations = DEFAULT_ITERATIONS;
	    printf("Iterations value set to default: %ld\n", iterations );
	} else {	
			iterations = atol(argv[1]);
	    	printf("Iterations value set to : %ld\n", iterations );
	}

	/* Confirm blocksize is multiple of and less than transfersize */
	if (blocksize > transfersize) {
		fprintf(stderr, "blocksize can not exceed transfersize\n");
		exit(EXIT_FAILURE);
	}
	if (transfersize % blocksize) {
		fprintf(stderr, "blocksize must be multiple of transfersize\n");
		exit(EXIT_FAILURE);
	}

	/* Allocate buffer space */
	buffersize = blocksize;
	if (!(transferBuffer = malloc(buffersize * sizeof(*transferBuffer)))) {
		perror("Failed to allocate transfer buffer");
		exit(EXIT_FAILURE);
	}

	/* Open Output File Descriptor in Write Only mode with standard permissions */
	if ((outputFD =
		 open(outputFilename,
			  O_WRONLY | O_CREAT | O_TRUNC | O_SYNC,
			  S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH)) < 0) {
		perror("Failed to open output file");
		exit(EXIT_FAILURE);
	}

	/* Print Status */
	fprintf(stdout, "writing to %s\n", outputFilename);

	pi = calc_pi(iterations);	
	/* Read from input file and write to output file */
	do {
		bytesWritten = dprintf(outputFD, "%e", pi);
		if (bytesWritten < 0) {
			perror("Error writing output file");
			exit(EXIT_FAILURE);
		} else {
			totalBytesWritten += bytesWritten;
			totalWrites++;
		}
	}
	while (totalBytesWritten < transfersize);

	/* Output some possibly helpfull info to make it seem like we were doing stuff */
	fprintf(stdout, "\n");
	fprintf(stdout, "outputfile:\t%s\n", outputFilename);
	fprintf(stdout, "Written:\t%zd bytes in %d writes\n",
			totalBytesWritten, totalWrites);

	/* Free Buffer */
	free(transferBuffer);

	/* Close Output File Descriptor */
	if (close(outputFD)) {
		perror("Failed to close output file");
		exit(EXIT_FAILURE);
	}
	
	return EXIT_SUCCESS;
}



double calc_pi(long iterations) {

	long i;
	double x, y;
    double inCircle = 0.0;
    double inSquare = 0.0;
    double pCircle = 0.0;
    double piCalc = 0.0; 


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

	return piCalc;


}
