#ifndef CCEXTRACTORGUI_H
#define CCEXTRACTORGUI_H

#ifndef NK_IMPLEMENTATION
#include "nuklear_lib/nuklear.h"
#endif // !NK_IMPLEMENTATION


struct main_tab
{
	enum {PORT, FILES} port_or_files;
	char port_num[8];
	int port_num_len;
	int is_check_common_extension;
	char **port_type;
	int port_select;
	char **filenames;
	int filename_count;
	nk_size progress_cursor;
};

void setup_main_settings(struct main_tab *main_settings);
//void truncate_path_string();
#endif //!CCEXTRACTORGUI_H