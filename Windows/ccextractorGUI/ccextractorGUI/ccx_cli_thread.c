#include "ccx_cli_thread.h"
#include "ccextractorGUI.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define MAX_WAIT 10


void* extract_thread(void* extract_args)
{
	struct args_extract *params = extract_args;
	static char term_string[500];
	strcpy(term_string, params->command_string);
	strcat(term_string, " ");
	strcat(term_string, params->file_string);
	strcat(term_string, " >ccx.log 2>gui_report.log");
	printf("%s", term_string);
	system(term_string);

}

void* read_data_from_thread(void* read_args)
{
	/*UNIX specific*/
	/*struct timespec time;
	time.tv_sec = 0;
	time.tv_nsec = 500000000L;*/
	int wait = 0;
	struct main_tab *main_settings = read_args;
	static int unknown1, unknown2,progress_count;
	FILE *file;
	char line[128];
	file = fopen("gui_report.log", "r");

	while (file == NULL) {
		printf("Cannot open file! Trying again.\n");
		file = fopen("gui_report.log", "r");
		//nanosleep(&time, NULL);   ----> UNIX SPECIFIC
		_sleep(500);  //----> WINDOWS SPECIFIC
		wait++;
		if (wait >= MAX_WAIT) {
			printf("POPUP:Make sure the directory isn't write protected!");
			break;
		}

	}

	while(fgets(line, sizeof(line), file) != NULL)
	{
		sscanf(line, "###PROGRESS#%d#%d#%d", &progress_count, &unknown1, &unknown2);
		main_settings->progress_cursor = progress_count;
	}
	fclose(file);
}