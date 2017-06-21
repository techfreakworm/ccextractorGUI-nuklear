#ifndef CCEXTRACTORGUI_H
#define CCEXTRACTORGUI_H

struct main
{
	enum {PORT, FILES} port_or_files;
	char port_num[8];
	int port_num_len;
	int is_check_common_extension;
	char **port_type;
	int port_select;
};

void setup_main_settings(struct main *main_settings);
#endif //!CCEXTRACTORGUI_H