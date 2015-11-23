/*
 * Name: Tiny Logtail v1.0
 * Author: Peez Gloriousintel
 * Last modified: 23 Nov 2015
 * Description:
 * 		Install:
 * 						make
 * 		Usage:
 * 						tiny-logtail <log_file> <offset_file>
*/

#include "tiny_logtail.h"

/* Checking if file exists or not */
int file_exist(const char *filename) {
	struct stat file;
	return (stat(filename, &file) == 0)? 1:0;
}

/* Read offset in offset file */
unsigned long get_offset(char * offset_file) {
	char s_offset [64];	// 64-byte offset
	FILE * save_pos = fopen(offset_file,"r+");
	if (save_pos == NULL)
		return -1;
	fseek(save_pos, 0, SEEK_SET);
	fread(s_offset, sizeof(s_offset), 1, save_pos);
	fclose(save_pos);
	return (unsigned long) atoi(s_offset);
}

/* Save offset to offset file */
int set_offset(char * offset_file, fpos_t pos) {
	FILE * save_pos = fopen(offset_file,"w");
	if (save_pos == NULL)
		return -1;
	fprintf(save_pos, "%lld\n", pos);
	fclose(save_pos);
	return 0;
}

int main (int argc, char **argv) {
	char logfile [BUFFER_SIZE];
	char offset_file [BUFFER_SIZE];
	char buffer [BUFFER_SIZE];
	unsigned long last_offset = 0;
	fpos_t pos;

	/* Detemine offset file from input arguments */
	if (argc > 2){
		strcpy(offset_file, argv[2]);
	}
	else if (argc < 2) {
		fprintf(stderr, "No input file.\n");
		fprintf(stderr, "Usage: tiny-logtail <log_file> <offset_file>\n");
		exit(1);
	}
	else {
		strcat(offset_file, argv[1]);
		strcat(offset_file, ".offset");
	}

	strcpy(logfile, argv[1]);

	if (!file_exist((const char*)logfile)) {
		fprintf(stderr, "Input file not found.\n");
		exit(2);
	}

	if (file_exist((const char*)offset_file)) {
		last_offset = get_offset(offset_file);
	}

	/* Get unread log lines */
	FILE * fd = fopen(logfile, "r");
	fseek(fd, last_offset, SEEK_SET);
	while(fgets(buffer, sizeof(buffer), fd) != NULL) {
		printf("%s", buffer);
		memset(buffer, 0, sizeof buffer);
	}
	fgetpos(fd, &pos);
	fclose(fd);

	if(set_offset(offset_file,	pos) < 0) {
		fprintf(stderr, "Error while saving offset file.");
		exit(3);
	}

	return 0;
}
