#ifndef COMMAND_BUILDER_H
#define COMMAND_BUILDER_H

#include "ccextractorGUI.h"
#include "tabs.h"
#include "popups.h"

struct built_string
{
	char term_string[1000];
};

void command_builder(struct built_string *command, struct main_tab *main_settings, struct network_popup *network_settings, struct input_tab *input, struct output_tab *output);

#endif //!COMMAND_BUILDER_H