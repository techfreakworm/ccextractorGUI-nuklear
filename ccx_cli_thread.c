#include "ccx_cli_thread.h"
#include "ccextractorGUI.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define MAX_WAIT 10
#define PROGRESS_COMPLETE 100

void* extract_thread(void* extract_args)
{
	struct args_extract *params = (struct args_extract*)extract_args;
	static char term_string[500];
	strcpy(term_string, params->command_string);
	strcat(term_string, " ");
	strcat(term_string, params->file_string);
	strcat(term_string, " >ccx.log 2>gui_report.log");
	printf("%s", term_string);
	system(term_string);
	pthread_exit(0);
}

void* read_data_from_thread(void* read_args)
{
	/*UNIX specific*/
	struct timespec time;
	time.tv_sec = 0;
	time.tv_nsec = 10000000L;
	int wait = 0;
	struct args_extract *read_params = (struct args_extract*)read_args;
	int unknown1 = 0, unknown2 = 0,progress_count = 0;
	FILE *file;
	char line[1000];
	file = fopen("gui_report.log", "r");

	while (file == NULL) {
		printf("Cannot open file! Trying again.\n");
		file = fopen("gui_report.log", "r");
		nanosleep(&time, NULL);   //----> UNIX SPECIFIC
		//_sleep(10);  //----> WINDOWS SPECIFIC
		wait++;
		if (wait >= MAX_WAIT) {
			printf("POPUP:Make sure the directory isn't write protected!");
			break;
		}

	}

	while( progress_count != PROGRESS_COMPLETE)
	{
		fgets(line, sizeof(line), file);
		sscanf(line, "###PROGRESS#%d#%d#%d", &progress_count, &unknown1, &unknown2);
		read_params->main_threadsettings->progress_cursor = progress_count;
	}
	printf("profress count:%d\n", progress_count);
	fclose(file);
	printf("File closed\n");
	pthread_exit(0);
}

void* feed_files_for_extraction(void* file_args)
{
	printf("Inside feeder\n");
	
	struct args_extract *extract_params = (struct args_extract *)file_args;
	printf("count:%d\n", extract_params->main_threadsettings->filename_count);
	extract_params->command_string = extract_params->threadcommand->term_string;
	int count = extract_params->main_threadsettings->filename_count;
	pthread_t tid_extract, tid_read;
	pthread_attr_t attr_extract, attr_read;


	for(int i = 0; count != 0; i++, count--)
	{
		pthread_t tid_extract, tid_read;
		pthread_attr_t attr_extract, attr_read;

		pthread_attr_init(&attr_extract);
		pthread_attr_init(&attr_read);
		
		extract_params->main_threadsettings->is_file_selected[i] = nk_true;
		extract_args.file_string = extract_params->main_threadsettings->filenames[i];
		int err1 = pthread_create(&tid_extract, &attr_extract, extract_thread, extract_params);
		int err2 = pthread_create(&tid_read, &attr_read, read_data_from_thread, extract_params);
		if (!err1)
			printf("Extraction Thread Complete:%d\n", i);
		if (!err2)
			printf("Read Thread Complete:%d\n", i);

		pthread_join(tid_extract, NULL);
		printf("Extract thread joined\n");
		pthread_join(tid_read, NULL);
		printf("Read thread joined\n");

		extract_params->main_threadsettings->is_file_selected[i] = nk_false;

		remove("gui_report.log");
		remove("ccx.log");

	}
	printf("File feeding over\n");
}

void setup_and_create_thread(struct main_tab *main_settings, struct built_string *command)
{
	extract_args.main_threadsettings = (struct main_tab*)main_settings;
	extract_args.threadcommand = (struct built_string*)command;

	pthread_attr_init(&attr_launch);
	int err = pthread_create(&tid_launch, &attr_launch, feed_files_for_extraction, &extract_args);
	if (!err)
		printf("Feeder created!\n");
}
