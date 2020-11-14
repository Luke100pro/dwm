// Settings
static unsigned int borderpx  = 3;    // border pixel of windows
static unsigned int snap      = 32;   // snap pixel
static int showbar            = 1;    // 0 means no bar
static int topbar             = 1;    // 0 means bottom bar
static int user_bh            = 24;   // 0 means that dwm will calculate bar height, >= 1 means dwm will user_bh as bar height
static int attachdirection    = 4;    // 0 default, 1 above, 2 aside, 3 below, 4 bottom, 5 top
static int hidevacanttags     = 1;    // 0 means to not hide empty tags

static unsigned int gappih    = 10;   // horiz inner gap between windows
static unsigned int gappiv    = 10;   // vert inner gap between windows
static unsigned int gappoh    = 10;   // horiz outer gap between windows and screen edge
static unsigned int gappov    = 10;   // vert outer gap between windows and screen edge
static int smartgaps          = 0;    // 1 means no outer gap when there is only one window

static unsigned int systraypinning  = 0;  // 0: sloppy systray follows selected monitor, >0: pin systray to monitor X
static unsigned int systrayspacing  = 2;  // systray spacing
static int systraypinningfailfirst  = 1;  // 1: if pinning fails, display systray on the first monitor, False: display systray on the last monitor
static int showsystray              = 1;  // 0 means no systray

static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };



// Colors
static char normbgcolor[]     = "#222222";
static char normbordercolor[] = "#444444";
static char normfgcolor[]     = "#bbbbbb";
static char selfgcolor[]      = "#eeeeee";
static char selbordercolor[]  = "#005577";
static char selbgcolor[]      = "#005577";
static char urgbordercolor[]  = "#ff0000";
static char *colors[][3] = {
    [SchemeNorm] = { normfgcolor, normbgcolor, normbordercolor },
    [SchemeSel]  = { selfgcolor,  selbgcolor,  selbordercolor  },
    [SchemeUrg]  = { selfgcolor,  selbgcolor,  urgbordercolor },
};



// Fonts
static const char *fonts[] = {
    "Inter:style=Medium:pixelsize=13:antialias=true:autohint=true",
    "Noto Color Emoji:pixelsize=11:antialias=true:autohint=true",
    "monospace:pixelsize=14:antialias=true:autohint=true"
};



// Window rules
static const Rule rules[] = {
    // class      instance    title       tags mask     isfloating   monitor
    { "Gimp",     NULL,       NULL,       0,            1,           -1 },
    { "Firefox",  NULL,       NULL,       1 << 8,       0,           -1 },
};



// Layouts
static float mfact     = 0.51; // factor of master area size [0.05..0.95]
static int nmaster     = 1;    // number of clients in master area
static int resizehints = 1;    // 1 means respect size hints in tiled resizals

#include "layouts.c"
static const Layout layouts[] = {
    { "[]=",  tile    },  // master'n'stack
    { "[\\]", dwindle },  // bspwm-like
//  { "[@]",  spiral  },  // fibonacci
    { "[M]",  monocle },  // single window visible
    { "><>",  NULL    },  // floating windows
    { NULL,   NULL    },  // needed by "cycle-layouts" patch
};



// Autostart
static const char *const autostart[] = {
    "st", "-e", "cd", "~/.local/src/dwm", NULL,
    "sh", "-c", "setbg",  NULL,
    "dwmblocks",          NULL,
    "unclutter",          NULL,
    "dunst",              NULL,
    "picom",              NULL,
    "mpd",                NULL,
     NULL
};



// Xresources
ResourcePref resources[] = {
    { "normbgcolor",        STRING,  &normbgcolor },
    { "normbordercolor",    STRING,  &normbordercolor },
    { "normfgcolor",        STRING,  &normfgcolor },
    { "selbgcolor",         STRING,  &selbgcolor },
    { "selbordercolor",     STRING,  &selbordercolor },
    { "selfgcolor",         STRING,  &selfgcolor },
    { "urgbordercolor",     STRING,  &urgbordercolor},

    { "attachdirection",    INTEGER, &attachdirection },
    { "hidevacanttags",     INTEGER, &hidevacanttags },
    { "barheight",          INTEGER, &user_bh },
    { "borderpx",           INTEGER, &borderpx },
    { "snap",               INTEGER, &snap },
    { "showbar",            INTEGER, &showbar },
    { "topbar",             INTEGER, &topbar },
    { "nmaster",            INTEGER, &nmaster },
    { "resizehints",        INTEGER, &resizehints },
    { "mfact",              FLOAT,   &mfact },

    { "gappov",             INTEGER, &gappov },
    { "gappoh",             INTEGER, &gappoh },
    { "gappiv",             INTEGER, &gappiv },
    { "gappih",             INTEGER, &gappih },
    { "smartgaps",          INTEGER, &smartgaps },

    { "systraypinning",     INTEGER, &systraypinning },
    { "systrayspacing",     INTEGER, &systrayspacing },
    { "showsystray",        INTEGER, &showsystray },
};



// Keybindings
#define TAGKEYS(KEY,TAG) \
    { KeyPress, Mod4Mask,                       KEY,  view,       {.ui = 1 << TAG} }, \
    { KeyPress, Mod4Mask|ControlMask,           KEY,  toggleview, {.ui = 1 << TAG} }, \
    { KeyPress, Mod4Mask|ShiftMask,             KEY,  tag,        {.ui = 1 << TAG} }, \
    { KeyPress, Mod4Mask|ControlMask|ShiftMask, KEY,  toggletag,  {.ui = 1 << TAG} },
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

static Key keys[] = {
    //type      modifier                    key             function        argument
    { KeyPress, Mod4Mask,                   XK_Return,      spawn,          SHCMD("$TERMINAL") },
    { KeyPress, Mod4Mask,                   XK_w,           spawn,          SHCMD("$BROWSER") },
    { KeyPress, Mod4Mask,                   XK_e,           spawn,          SHCMD("$EDITOR") },

    { KeyPress, Mod4Mask,                   XK_g,           spawn,          SHCMD("lutris") },
    { KeyPress, Mod4Mask,                   XK_z,           spawn,          SHCMD("thunar") },
    { KeyPress, 0,                          XK_Print,       spawn,          SHCMD("screengrab") },

    { KeyPress, Mod4Mask,                   XK_d,           spawn,          SHCMD("dmenurun") },
    { KeyPress, Mod4Mask,                   XK_Escape,      spawn,          SHCMD("dmenuexit") },
    { KeyPress, Mod4Mask,                   XK_m,           spawn,          SHCMD("mpdmenu") },
    { KeyPress, Mod4Mask,                   XK_F9,          spawn,          SHCMD("dmenumount") },
    { KeyPress, Mod4Mask,                   XK_F10,         spawn,          SHCMD("dmenuumount") },
    { KeyPress, Mod4Mask,                   XK_grave,       spawn,          SHCMD("dmenuunicode") },

    { KeyPress, Mod4Mask,                   XK_Page_Up,     spawn,          SHCMD("vmc up") },
    { KeyPress, Mod4Mask,                   XK_Page_Down,   spawn,          SHCMD("vmc down") },
    { KeyPress, Mod4Mask,                   XK_End,         spawn,          SHCMD("vmc next") },
    { KeyPress, Mod4Mask,                   XK_Begin,       spawn,          SHCMD("vmc prev") },
    { KeyPress, Mod4Mask,                   XK_Insert,      spawn,          SHCMD("vmc toggle") },
    { KeyPress, Mod4Mask,                   XK_Delete,      spawn,          SHCMD("vmc mute") },

    { KeyPress, Mod4Mask,                   XK_b,           togglebar,      {0} },
//  { KeyPress, Mod4Mask,                   XK_s,           togglesystray,  {0} },
    { KeyPress, Mod4Mask,                   XK_j,           focusstack,     {.i = +1 } },
    { KeyPress, Mod4Mask,                   XK_k,           focusstack,     {.i = -1 } },
    { KeyPress, Mod4Mask|ShiftMask,         XK_j,           pushdown,       {0} },
    { KeyPress, Mod4Mask|ShiftMask,         XK_k,           pushup,         {0} },
    { KeyPress, Mod4Mask,                   XK_i,           incnmaster,     {.i = +1 } },
    { KeyPress, Mod4Mask,                   XK_o,           incnmaster,     {.i = -1 } },
    { KeyPress, Mod4Mask,                   XK_h,           setmfact,       {.f = -0.05} },
    { KeyPress, Mod4Mask,                   XK_l,           setmfact,       {.f = +0.05} },
    { KeyPress, Mod4Mask|Mod1Mask,          XK_BackSpace,   togglegaps,     {0} },
    { KeyPress, Mod4Mask|Mod1Mask,          XK_0,           defaultgaps,    {0} },
    { KeyPress, Mod4Mask|Mod1Mask,          XK_equal,       incrgaps,       {.i = +5} },
    { KeyPress, Mod4Mask|Mod1Mask,          XK_minus,       incrgaps,       {.i = -5} },

    { KeyPress, Mod4Mask|ShiftMask,         XK_f,           zoom,           {0} },
    { KeyPress, Mod4Mask,                   XK_f,           focusmaster,    {0} },
    { KeyPress, Mod4Mask,                   XK_q,           killclient,     {0} },
    { KeyPress, Mod4Mask|ShiftMask,         XK_q,           quit,           {0} },

    { KeyPress, Mod4Mask,                   XK_space,       togglefloating, {0} },
    { KeyPress, Mod4Mask,                   XK_F11,         togglefullscr,  {0} },
    { KeyPress, Mod4Mask|Mod1Mask,          XK_comma,       cyclelayout,    {.i = -1 } },
    { KeyPress, Mod4Mask|Mod1Mask,          XK_period,      cyclelayout,    {.i = +1 } },

    { KeyPress, Mod4Mask,                   XK_comma,       focusmon,       {.i = -1 } },
    { KeyPress, Mod4Mask,                   XK_period,      focusmon,       {.i = +1 } },
    { KeyPress, Mod4Mask|ShiftMask,         XK_comma,       tagmon,         {.i = -1 } },
    { KeyPress, Mod4Mask|ShiftMask,         XK_period,      tagmon,         {.i = +1 } },

    { KeyPress, Mod4Mask,                   XK_Tab,         view,           {0} },
    { KeyPress, Mod4Mask,                   XK_0,           view,           {.ui = ~0 } },
    { KeyPress, Mod4Mask|ShiftMask,         XK_0,           tag,            {.ui = ~0 } },
    TAGKEYS(XK_1, 0)
    TAGKEYS(XK_2, 1)
    TAGKEYS(XK_3, 2)
    TAGKEYS(XK_4, 3)
    TAGKEYS(XK_5, 4)
    TAGKEYS(XK_6, 5)
    TAGKEYS(XK_7, 6)
    TAGKEYS(XK_8, 7)
    TAGKEYS(XK_9, 8)
};


// Mouse bindings
static Button buttons[] = {
    //click           event mask  button    function        argument
    { ClkLtSymbol,    0,          Button1,  setlayout,      {.v = &layouts[0]} },
    { ClkLtSymbol,    0,          Button4,  cyclelayout,    {.i = +1} },
    { ClkLtSymbol,    0,          Button5,  cyclelayout,    {.i = -1} },

    { ClkWinTitle,    0,          Button1,  zoom,           {0} },
    { ClkWinTitle,    0,          Button2,  killclient,     {0} },

    { ClkStatusText,  0,          Button1,  sigdwmblocks,   {.i = 1} },
    { ClkStatusText,  0,          Button2,  sigdwmblocks,   {.i = 2} },
    { ClkStatusText,  0,          Button3,  sigdwmblocks,   {.i = 3} },
    { ClkStatusText,  0,          Button4,  sigdwmblocks,   {.i = 4} },
    { ClkStatusText,  0,          Button5,  sigdwmblocks,   {.i = 5} },

    { ClkClientWin,   Mod4Mask,   Button1,  movemouse,      {0} },
    { ClkClientWin,   Mod4Mask,   Button2,  togglefloating, {0} },
    { ClkClientWin,   Mod4Mask,   Button3,  resizemouse,    {0} },

    { ClkTagBar,      0,          Button1,  view,           {0} },
    { ClkTagBar,      0,          Button3,  toggleview,     {0} },
    { ClkTagBar,      Mod4Mask,   Button1,  tag,            {0} },
    { ClkTagBar,      Mod4Mask,   Button3,  toggletag,      {0} },
};
