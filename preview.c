#ifndef NK_IMPLEMENTATION
#include "nuklear_lib/nuklear.h"
#endif
static int
preview(struct nk_context *ctx, int x, int y, int width, int height)
{
	if (nk_begin(ctx, "Preview", nk_rect(x, y, width, height), NK_WINDOW_TITLE))
	{
		nk_layout_row_dynamic(ctx, 20, 1);
		nk_label(ctx, "Preview info will come here!", NK_TEXT_LEFT);
	}
	nk_end(ctx);
	return !nk_window_is_closed(ctx, "Preview");
}