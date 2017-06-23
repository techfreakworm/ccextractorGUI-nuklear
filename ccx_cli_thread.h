#ifndef CCX_CLI_THREAD_H
#define CCX_CLI_THREAD_H
#define HAVE_STRUCT_TIMESPEC
#include "ccextractorGUI.h"
#include "popups.h"
#include "command_builder.h"
#include "pthread.h"
struct args_extract {
	char *file_string;
	char *command_string;
};

pthread_t tid[9];

void* read_data_from_thread(void* read_args);
void* extract_thread(void* extract_args);

#endif //!CCX_CLI_THREAD_H