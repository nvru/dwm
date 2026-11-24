static const char col_bg[]     = "#1a1b26";  // background
static const char col_fg[]     = "#a9b1d6";  // foreground
static const char col_blu[]    = "#7aa2f7";  // blue
static const char col_mag[]    = "#ad8ee6";  // magenta

static const char *colors[][3] = {
    /*               fg       bg       border */
    [SchemeNorm]     = { col_fg,  col_bg, col_fg },
    [SchemeSel]      = { col_blu, col_bg, col_mag },

    /* Statusbar right */
    [SchemeStatus]   = { col_fg,  col_bg, col_bg    },

    /* Tagbar left */
    [SchemeTagsSel]  = { col_blu,  col_bg, col_bg    },
    [SchemeTagsNorm] = { col_fg,  col_bg, col_bg    },

    /* Infobar middle */
    [SchemeInfoSel]  = { col_fg,  col_bg, col_bg    },
    [SchemeInfoNorm] = { col_fg,  col_bg, col_bg    },
};
