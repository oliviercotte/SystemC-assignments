#include <stdio.h>
#include <stdlib.h>
#include <time.h>

static int i = 0;

int producer(){
	int array[] = {1, 2 , 3, 4, 5};
	int x = array[i];
	i++;
	return x;
}
