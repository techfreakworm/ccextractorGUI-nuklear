
static int
preview(struct nk_context *ctx, int x, int y, int width, int height)
{
	if (nk_begin(ctx, "Preview", nk_rect(x, y, width, height), 0))
	{
		nk_layout_row_dynamic(ctx, 20, 1);
		nk_label(ctx, "Preview info will come here!", 0);
	}
	nk_end(ctx);
	return !nk_window_is_closed(ctx, "Preview");
}