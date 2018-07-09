#include <stdio.h>
#include <stdlib.h>

char load_file(const char *filename, char **result) {
	int size = 0;
	FILE *handle = fopen(filename, "rb");

	if (handle == NULL) { 
		*result = NULL;
		return -1;
	} 

	fseek(handle, 0, SEEK_END);
	size = ftell(handle);
	fseek(handle, 0, SEEK_SET);
	*result = (char *)malloc(size+1);

	if (size != fread(*result, sizeof(char), size, handle)) { 
		free(*result);
		return -2; 
	} 

	fclose(handle);
	(*result)[size] = 0;

	return size;
}