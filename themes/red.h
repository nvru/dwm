// BlackArch
static const char normbordercolor[] = "#484848";
static const char normbgcolor[] = "#000000";
static const char normfgcolor[] = "#C40000";
static const char selbordercolor[] = "#C40000";
static const char selbgcolor[] = "#C40000";
static const char selfgcolor[] = "#FFFFFF";

static const char *colors[][3] = {
  /*               fg         bg         border   */
  [SchemeNorm] = { selfgcolor, normbgcolor, normbordercolor },
  [SchemeSel] = { selfgcolor, selbgcolor, selbordercolor },
};
