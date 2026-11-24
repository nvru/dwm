/* See LICENSE file for copyright and license details. */
#include <X11/XF86keysym.h>

/* appearance */
static const unsigned int borderpx  = 0;        /* border pixel of windows */
static const unsigned int snap      = 32;       /* snap pixel */
static const unsigned int gappih    = 5;       /* horiz inner gap between windows */
static const unsigned int gappiv    = 5;       /* vert inner gap between windows */
static const unsigned int gappoh    = 5;       /* horiz outer gap between windows and screen edge */
static const unsigned int gappov    = 5;       /* vert outer gap between windows and screen edge */
static       int smartgaps          = 0;        /* 1 means no outer gap when there is only one window */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 1;        /* 0 means bottom bar */
static const int usealtbar          = 1;        /* 1 means use non-dwm status bar */
static const char *altbarclass      = "Polybar"; /* Alternate bar class name */
static const char *altbarcmd        = "$HOME/.config/suckless/dwm/scripts/extrabar"; /* Alternate bar launch command */
static const char *fonts[] = {
	"JetBrainsMono Nerd Font Propo:style=Bold:size=10",
	"Adwaita Mono:style=Bold:pixelsize=13",
	"monospace:style=Bold:pixelsize=13",
	"Iosevka Nerd Font Propo:style=Bold:size=10",
	"Ubuntu:weight=bold:size=11:antialias=true:hinting=true",
	"Hack:size=11:antialias=true:autohint=true",
	"DejaVu Sans:style=Bold:size=11",
	"JoyPixels:size=10:antialias=true:autohint=true"};
static const char dmenufont[]       = "monospace:size=10";
static const char col_gray1[]       = "#222222";
static const char col_gray2[]       = "#444444";
static const char col_gray3[]       = "#bbbbbb";
static const char col_gray4[]       = "#eeeeee";
static const char col_cyan[]        = "#005577";
// static const char *colors[][3]      = {
// 	/*               fg         bg         border   */
// 	[SchemeNorm] = { col_gray3, col_gray1, col_gray2 },
// 	[SchemeSel]  = { col_gray4, col_cyan,  col_cyan  },
// };

#include "themes/toykyonight.h"

static const char *const autostart[] = {
       "sh", "-c", "~/.config/suckless/dwm/scripts/autostart", NULL,
	NULL /* terminate */
};

/* tagging */
static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9", "10" };

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class      instance    title       tags mask     isfloating   isfullscreen   monitor */
	{ "Gimp",     NULL,       NULL,       0,            1,           0,             -1 },
	{ "Firefox",  NULL,       NULL,       1 << 8,       0,           0,             -1 },
};

/* layout(s) */
static const float mfact     = 0.55; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 1;    /* 1 means respect size hints in tiled resizals */
static const int lockfullscreen = 1; /* 1 will force focus on the fullscreen window */
static const int refreshrate = 120;  /* refresh rate (per second) for client move/resize */

#define FORCE_VSPLIT 1  /* nrowgrid layout: force two clients to always split vertically */
#include "vanitygaps.c"

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "󰙀",      tile },    /* first entry is default */
	// { "[]=",      tile },    /* first entry is default */
	{ "[M]",      monocle },
	{ "[@]",      spiral },
	{ "[\\]",     dwindle },
	{ "H[]",      deck },
	{ "TTT",      bstack },
	{ "===",      bstackhoriz },
	{ "HHH",      grid },
	{ "###",      nrowgrid },
	{ "---",      horizgrid },
	{ ":::",      gaplessgrid },
	{ "|M|",      centeredmaster },
	{ ">M>",      centeredfloatingmaster },
	{ "><>",      NULL },    /* no layout function means floating behavior */
	{ NULL,       NULL },
};

/* key definitions */
#define MODKEY Mod4Mask
#define ALT Mod1Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
static char dmenumon[8]; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "dmenu_run", "-m", dmenumon, "-fn", dmenufont, "-nb", col_gray1, "-nf", col_gray3, "-sb", col_gray3, "-sf", col_gray1, NULL };
#define TERM     "alacritty"
#define TERM_ALT "st"
static const char *termcmd[] = {
	"sh",
	"-c",
	"command -v \"$1\" >/dev/null 2>&1 && exec \"$1\"; exec \"$2\"",
	"sh",
	TERM,
	TERM_ALT,
	NULL};

static const char *volup[] = {
	"sh", "-c",
	"if command -v pactl >/dev/null 2>&1 && pactl info 2>/dev/null | grep "
	"-qi "
	"'PulseAudio'; then "
	"pactl set-sink-volume @DEFAULT_SINK@ +5% >/dev/null && "
	"VOL=$(pactl get-sink-volume @DEFAULT_SINK@ 2>/dev/null | grep -oP "
	"'[0-9]+(?=%)' | head -n1); "
	"[ -z \"$VOL\" ] && VOL=$(pactl get-sink-volume @DEFAULT_SINK@ "
	"2>/dev/null "
	"| sed -n 's/.*[[:space:]]\\([0-9]\\{1,3\\}\\)%.*/\\1/p' | head -n1); "
	"dunstify -h string:x-dunst-stack-tag:volume \"Volume: ${VOL}%\"; "
	"elif command -v wpctl >/dev/null 2>&1; then "
	"wpctl set-volume -l 1.5 @DEFAULT_AUDIO_SINK@ 5%+ >/dev/null && "
	"VOL=$(wpctl get-volume @DEFAULT_AUDIO_SINK@ | awk '{printf \"%d\", "
	"$2*100}'); "
	"dunstify -h string:x-dunst-stack-tag:volume \"Volume: ${VOL}%\"; "
	"else "
	"pactl set-sink-volume @DEFAULT_SINK@ +5% >/dev/null && "
	"VOL=$(pactl get-sink-volume @DEFAULT_SINK@ 2>/dev/null | grep -oP "
	"'[0-9]+(?=%)' | head -n1); "
	"[ -z \"$VOL\" ] && VOL=$(pactl get-sink-volume @DEFAULT_SINK@ "
	"2>/dev/null "
	"| sed -n 's/.*[[:space:]]\\([0-9]\\{1,3\\}\\)%.*/\\1/p' | head -n1); "
	"dunstify -h string:x-dunst-stack-tag:volume \"Volume: ${VOL}%\"; "
	"fi",
	NULL};
static const char *voldown[] = {
	"sh", "-c",
	"if command -v pactl >/dev/null 2>&1 && pactl info 2>/dev/null | grep "
	"-qi "
	"'PulseAudio'; then "
	"pactl set-sink-volume @DEFAULT_SINK@ -5% >/dev/null && "
	"VOL=$(pactl get-sink-volume @DEFAULT_SINK@ 2>/dev/null | grep -oP "
	"'[0-9]+(?=%)' | head -n1); "
	"[ -z \"$VOL\" ] && VOL=$(pactl get-sink-volume @DEFAULT_SINK@ "
	"2>/dev/null "
	"| sed -n 's/.*[[:space:]]\\([0-9]\\{1,3\\}\\)%.*/\\1/p' | head -n1); "
	"dunstify -h string:x-dunst-stack-tag:volume \"Volume: ${VOL}%\"; "
	"elif command -v wpctl >/dev/null 2>&1; then "
	"wpctl set-volume @DEFAULT_AUDIO_SINK@ 5%- >/dev/null && "
	"VOL=$(wpctl get-volume @DEFAULT_AUDIO_SINK@ | awk '{printf \"%d\", "
	"$2*100}'); "
	"dunstify -h string:x-dunst-stack-tag:volume \"Volume: ${VOL}%\"; "
	"else "
	"pactl set-sink-volume @DEFAULT_SINK@ -5% >/dev/null && "
	"VOL=$(pactl get-sink-volume @DEFAULT_SINK@ 2>/dev/null | grep -oP "
	"'[0-9]+(?=%)' | head -n1); "
	"[ -z \"$VOL\" ] && VOL=$(pactl get-sink-volume @DEFAULT_SINK@ "
	"2>/dev/null "
	"| sed -n 's/.*[[:space:]]\\([0-9]\\{1,3\\}\\)%.*/\\1/p' | head -n1); "
	"dunstify -h string:x-dunst-stack-tag:volume \"Volume: ${VOL}%\"; "
	"fi",
	NULL};
static const char *volmute[] = {
	"sh", "-c",
	"if command -v pactl >/dev/null 2>&1 && pactl info 2>/dev/null | grep "
	"-qi 'PulseAudio'; then "
	"pactl set-sink-mute @DEFAULT_SINK@ toggle >/dev/null && "
	"if pactl get-sink-mute @DEFAULT_SINK@ | grep -q 'Mute: yes'; then "
	"MSG=Muted; else MSG=Unmuted; fi; "
	"dunstify -h string:x-dunst-stack-tag:volume \"$MSG\"; "
	"elif command -v wpctl >/dev/null 2>&1; then "
	"wpctl set-mute @DEFAULT_AUDIO_SINK@ toggle >/dev/null && "
	"if wpctl get-volume @DEFAULT_AUDIO_SINK@ | grep -q MUTED; then "
	"MSG=Muted; else MSG=Unmuted; fi; "
	"dunstify -h string:x-dunst-stack-tag:volume \"$MSG\"; "
	"else "
	"pactl set-sink-mute @DEFAULT_SINK@ toggle >/dev/null && "
	"if pactl get-sink-mute @DEFAULT_SINK@ | grep -q 'Mute: yes'; then "
	"MSG=Muted; else MSG=Unmuted; fi; "
	"dunstify -h string:x-dunst-stack-tag:volume \"$MSG\"; "
	"fi",
	NULL};
static const char *brightup[] = {
	"sh", "-c",
	"brightnessctl set +5% && dunstify -h "
	"string:x-dunst-stack-tag:brightness \"Brightness: $(brightnessctl -m "
	"| cut -d, -f4)\"",
	NULL};
static const char *brightdown[] = {
	"sh", "-c",
	"brightnessctl set 5%- && dunstify -h "
	"string:x-dunst-stack-tag:brightness \"Brightness: $(brightnessctl -m "
	"| cut -d, -f4)\"",
	NULL};
static const char *flameshot[] = {"flameshot", "gui", NULL};
static const char *flameshotfull[] = {
    "sh", "-c",
    "flameshot full --path ~/Pictures/Screenshots",
    NULL
};
static const char *slock[] = {"slock", NULL};
static const char *rofi[] = {"rofi", "-show", "drun", NULL};

#include "movestack.c"
static const Key keys[] = {
	/* modifier                     key        function        argument */

	/* launch */
	{ MODKEY,                       XK_space,  spawn,          {.v = dmenucmd } },
	{ MODKEY,                       XK_d,      spawn,          {.v = rofi} },
	{ MODKEY|ShiftMask,             XK_l,      spawn,          {.v = slock} },
	{ 0,                            XK_Print,  spawn,          {.v = flameshot} },
	{ ShiftMask,                    XK_Print,  spawn,          {.v = flameshotfull} },
	{ MODKEY,                       XK_Return, spawn,          {.v = termcmd } },

	/* media */
	{ 0,                            XF86XK_AudioRaiseVolume,   spawn, {.v = volup } },
	{ 0,                            XF86XK_AudioLowerVolume,   spawn, {.v = voldown } },
	{ 0,                            XF86XK_AudioMute,          spawn, {.v = volmute } },
	{ 0,                            XF86XK_MonBrightnessUp,    spawn, {.v = brightup } },
	{ 0,                            XF86XK_MonBrightnessDown,  spawn, {.v = brightdown } },

	/* windows */
	{ MODKEY,                       XK_b,      togglebar,      {0} },
	{ MODKEY,                       XK_k,      focusstack,     {.i = +1 } },
	{ MODKEY,                       XK_j,      focusstack,     {.i = -1 } },
	{ MODKEY,                       XK_i,      incnmaster,     {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_i,      incnmaster,     {.i = -1 } },
	{ MODKEY,                       XK_h,      setmfact,       {.f = -0.05} },
	{ MODKEY,                       XK_l,      setmfact,       {.f = +0.05} },
	{ MODKEY|ShiftMask,             XK_j,      movestack,      {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_k,      movestack,      {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_h,      setcfact,       {.f = +0.25} },
	{ MODKEY|ShiftMask,             XK_l,      setcfact,       {.f = -0.25} },
	{ MODKEY|ShiftMask,             XK_o,      setcfact,       {.f =  0.00} },
	{ MODKEY,                       XK_Return, zoom,           {0} },

	/* gaps */
	{ MODKEY,                       XK_z,      incrgaps,       {.i = +1 } },
	{ MODKEY,                       XK_x,      incrgaps,       {.i = -1 } },
	{ MODKEY|ALT|ShiftMask,         XK_u,      incrgaps,       {.i = -1 } },
	{ MODKEY|ALT,                   XK_y,      incrigaps,      {.i = +1 } },
	{ MODKEY|ALT|ShiftMask,         XK_y,      incrigaps,      {.i = -1 } },
	{ MODKEY|ALT,                   XK_p,      incrogaps,      {.i = +1 } },
	{ MODKEY|ALT|ShiftMask,         XK_p,      incrogaps,      {.i = -1 } },
	{ MODKEY|ALT,                   XK_6,      incrihgaps,     {.i = +1 } },
	{ MODKEY|ALT|ShiftMask,         XK_6,      incrihgaps,     {.i = -1 } },
	{ MODKEY|ALT,                   XK_7,      incrivgaps,     {.i = +1 } },
	{ MODKEY|ALT|ShiftMask,         XK_7,      incrivgaps,     {.i = -1 } },
	{ MODKEY|ALT,                   XK_8,      incrohgaps,     {.i = +1 } },
	{ MODKEY|ALT|ShiftMask,         XK_8,      incrohgaps,     {.i = -1 } },
	{ MODKEY|ALT,                   XK_9,      incrovgaps,     {.i = +1 } },
	{ MODKEY|ALT|ShiftMask,         XK_9,      incrovgaps,     {.i = -1 } },
	{ MODKEY,                       XK_a,      togglegaps,     {0} },
	{ MODKEY|ShiftMask,             XK_a,      defaultgaps,    {0} },

	/* navigation */
	{ MODKEY,                       XK_Tab,    view,           {0} },
	{ MODKEY,                       XK_q,      killclient,     {0} },
	{ MODKEY,                       XK_o,      view,           {.ui = ~0 } },

	/* layouts */
	{ MODKEY,                       XK_t,      setlayout,      {.v = &layouts[0]} },
	{ MODKEY|ControlMask,           XK_f,      setlayout,      {.v = &layouts[14]} },
	{ MODKEY,                       XK_m,      setlayout,      {.v = &layouts[1]} },
	{ MODKEY|ControlMask,           XK_d,      setlayout,      {.v = &layouts[3]} },
	{ MODKEY|ShiftMask,             XK_space,  togglefloating, {0} },
	{ MODKEY,                       XK_f,      togglefullscr,  {0} },

	/* tags and monitors */
	{ MODKEY|ShiftMask,             XK_0,      tag,            {.ui = ~0 } },
	{ MODKEY,                       XK_comma,  focusmon,       {.i = -1 } },
	{ MODKEY,                       XK_period, focusmon,       {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_comma,  tagmon,         {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_period, tagmon,         {.i = +1 } },

	TAGKEYS(                        XK_1,                      0)
	TAGKEYS(                        XK_2,                      1)
	TAGKEYS(                        XK_3,                      2)
	TAGKEYS(                        XK_4,                      3)
	TAGKEYS(                        XK_5,                      4)
	TAGKEYS(                        XK_6,                      5)
	TAGKEYS(                        XK_7,                      6)
	TAGKEYS(                        XK_8,                      7)
	TAGKEYS(                        XK_9,                      8)
	TAGKEYS(                        XK_0,                      9)

	/* session */
	{ MODKEY|ShiftMask,             XK_q,      quit,           {0} },
	{ MODKEY|ShiftMask,             XK_r,      quit,           {1} },
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static const Button buttons[] = {
	/* click                event mask      button          function        argument */
	{ ClkLtSymbol,          0,              Button1,        setlayout,      {0} },
	{ ClkLtSymbol,          0,              Button3,        setlayout,      {.v = &layouts[2]} },
	{ ClkWinTitle,          0,              Button2,        zoom,           {0} },
	{ ClkStatusText,        0,              Button2,        spawn,          {.v = termcmd } },
	{ ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
	{ ClkClientWin,         MODKEY,         Button2,        togglefloating, {0} },
	{ ClkClientWin,         ShiftMask,      Button1,        resizemouse,    {0} },
	{ ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
	{ ClkTagBar,            0,              Button1,        view,           {0} },
	{ ClkTagBar,            0,              Button3,        toggleview,     {0} },
	{ ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
};

