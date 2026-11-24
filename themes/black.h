static const char col_gray[] = "#888888";
static const char col_white[] = "#ffffff";
static const char col_soft[] = "#b0b0b0";
static const char col_black[] = "#000000";

static const char *colors[][3] = {
  /*               fg          bg          border */
  [SchemeNorm] = { col_gray, col_black, col_black },
  [SchemeSel] = { col_soft, col_black, col_white },

  [SchemeStatus] = { col_soft, col_black, col_black },
  [SchemeTagsSel] = { col_white, col_black, col_black },
  [SchemeTagsNorm] = { col_gray, col_black, col_black },
  [SchemeInfoSel] = { col_soft, col_black, col_black },
  [SchemeInfoNorm] = { col_gray, col_black, col_black },
};
