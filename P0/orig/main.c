#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int producer();
void consumer(int n);

int main(){

	int producedNumber, consumedNumber;
	int i;
	for(i = 0; i < 5; i++){
		producedNumber = producer();
		printf("Producer produced %i\n", producedNumber);
		
		consumer(producedNumber);
		printf("\t \t \t \t \t Consumer consumed %i\n", producedNumber);
	}
	return 0;
}
