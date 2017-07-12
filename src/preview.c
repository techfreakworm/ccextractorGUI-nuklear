#ifndef NK_IMPLEMENTATION
#include "nuklear_lib/nuklear.h"
#endif // !NK_IMPLEMENTATION

#include "preview.h"

preview(struct nk_context *ctx, int x, int y, int width, int height, struct main_tab *main_settings)
{
	if (nk_begin(ctx, "Preview", nk_rect(x, y, width, height), NK_WINDOW_TITLE))
	{
		nk_layout_row_dynamic(ctx, height - 60, 1);
		nk_text_wrap(ctx, main_settings->preview_string, main_settings->preview_string_len);
		//nk_edit_string(ctx, NK_EDIT_BOX|NK_EDIT_INACTIVE, main_settings->preview_string, &main_settings->preview_string_len, 65536, nk_filter_default);
	}
	nk_end(ctx);
	return !nk_window_is_closed(ctx, "Preview");
}
