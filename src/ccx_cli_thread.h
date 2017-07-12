#ifndef CCX_CLI_THREAD_H
#define CCX_CLI_THREAD_H
#define HAVE_STRUCT_TIMESPEC
#include "ccextractorGUI.h"
#include "popups.h"
#include "command_builder.h"
#include "pthread.h"
struct args_extract {
	struct main_tab *main_threadsettings;
	struct built_string *threadcommand;
	char *file_string;
	char *command_string;
};


static struct args_extract extract_args;
pthread_t tid_launch;
pthread_attr_t attr_launch;

void* read_data_from_thread(void* read_args);
void* extract_thread(void* extract_args);
void* feed_files_for_extraction(void* file_args);
void setup_and_create_thread(struct main_tab *main_settings, struct built_string *command);

#endif //!CCX_CLI_THREAD_H
