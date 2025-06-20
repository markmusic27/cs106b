/* File comments are usually used to describe the contents of the .cpp file for
 * someone reading them for the first time. In this case, though, it's a space
 * for you to write down answers to the three questions asked in this section.
 *
 * Q1: Story
 * Once upon a time,
 * Cherokee syllabary inventor Sequoyah
 * was recycling rare earth elements
 * when, suddenly,
 * Markiepoo
 * saved Rapa Nui National Park
 * from the storm of the century
 * for the second time in a week.
 * THE END!
 *
 * Q2: Yellow warning saying "All paths through this function will call itself"
 * Warning in compile output:
 * ../../DebuggerWarmups.cpp:258:38: warning: all paths through this function will call itself [-Winfinite-recursion]
  258 | void triggerStackOverflow(int value) {
      |                                      ^
1 warning generated.
 *
 * Q3:
 * Viewing the debugger, a variable named value takes certain values. The following are the values I found:
 * 65, 654, 553, 320, 196, 155, 717, 65... (repeats)
 *
 *
 *
 */
#include "DebuggerWarmups.h"
#include "hashcode.h"
#include "error.h"
#include "vector.h"
#include <string>
using namespace std;

/* TODO: Fill this in with your name before reading the story. Otherwise
 * you'll get the wrong story!
 */
const string MyName = "Markiepoo";

namespace {
    void theEnd() {
        string text = "THE END!";

        /* Set a breakpoint here. */
        (void) text;
    }

    void sayHow(int value) {
        string text;
        if (value % 4 == 0) {
            text = "through sheer force of personality.";
        } else if (value % 4 == 1) {
            text = "by setting up an elaborate diversion.";
        } else if (value % 4 == 2) {
            text = "without breaking a sweat.";
        } else {
            text = "for the second time in a week.";
        }

        theEnd();
    }

    void sayWhatFrom(int value) {
        string text = "from ";
        switch (value % 8) {
            case 0:
                text += "a black hole";
                break;
            case 1:
                text += "kleptomaniacal aliens";
                break;
            case 2:
                text += "a plague of locusts";
                break;
            case 3:
                text += "collective amnesia";
                break;
            case 4:
                text += "certain destruction";
                break;
            case 5:
                text += "a volcanic eruption";
                break;
            case 6:
                text += "the storm of the century";
                break;
            default:
                text += "overzealous ice cream enthusiasts";
                break;
        }

        sayHow(scramble(value));
    }

    void sayWhatTheyRescued(int value) {
        string text = "saved ";

        Vector<string> options = {
            "the original copy of 'Kubla Khan'",
            "the Palace of Darius the Great",
            "the Leshan Giant Buddha",
            "the Island of Meroe",
            "Mesa Verde National Park",
            "Itchan Kala",
            "Bagan",
            "Rapa Nui National Park"
        };

        text += options[value % options.size()];

        sayWhatFrom(scramble(value));
    }

    void ourHero(int value) {
        string text = MyName;
        sayWhatTheyRescued(scramble(value));
    }

    void expressSurprise(int value) {
        if (value % 2 == 0) {
            string text = "when, suddenly,";
            ourHero(scramble(value));
        } else {
            string text = "when, without warning,";
            ourHero(scramble(value));
        }
    }

    void sayWhatTheyWereDoing(int value) {
        string text;
        switch (value % 10) {
            case 0:
                text = "was polishing the mirrors on the James Webb Space Telescope";
                break;
            case 1:
                text = "was reciting poetry";
                break;
            case 2:
                text = "was climbing the Vinson Massif";
                break;
            case 3:
                text = "was officiating at a wedding";
                break;
            case 4:
                text = "was searching for the deep truths of life";
                break;
            case 5:
                text = "was defending the indigent in federal court";
                break;
            case 6:
                text = "was recycling rare earth elements";
                break;
            case 7:
                text = "was unraveling the mysteries of the universe";
                break;
            case 8:
                text = "was mediating at peace talks";
                break;
            default:
                text = "was running a high-altitude marathon";
                break;
        }
        expressSurprise(scramble(value));
    }

    void introduceProtagonist(int value) {
        const Vector<string> options = {
            "Cherokee syllabary inventor Sequoyah",
            "actress and inventor Hedy Lamarr",
            "the polymath Ismail al-Jazari",
            "virologist Francoise Barre-Sinoussi",
            "banjo maestro Bela Fleck",
            "singer/songwriter Carole King",
            "abstract expressionist painter Lee Krasner",
            "Olympic athlete Steven Bradbury",
            "poet Wislawa Szymborska",
            "mathematician Srinivasa Ramanujan",
            "the writer Jorge Luis Borges",
        };

        string text = options[value % options.size()];
        sayWhatTheyWereDoing(scramble(value));
    }

    void beginStory(int value) {
        string text;
        if (value % 3 == 0) {
            text = "Once upon a time, ";
        } else if (value % 3 == 1) {
            text = "The other day, ";
        } else {
            text = "It once came to pass that ";
        }

        introduceProtagonist(scramble(value));
    }
}

void tellStory() {
    /* Before we tell a story, let's make sure that you've changed
     * the MyName constant at the top of the file. If the debugger
     * pops up at the line below, it means that you forgot to edit
     * MyName.
     */
    if (MyName == "(Your Name Here)") {
        error("Please change the constant MyName defined in " __FILE__ " to your name.");
    }

    beginStory(hashCode(MyName));
}

namespace {
    /* The "Goto Table" for the Stack Overflow example. This is essentially a function
     * that given a number between 0 and 1023 gives back a number between 0 and 1023.
     * Our function uses this table to make it hard to predict what's going to happen
     * next. Use the debugger to find out!
     */
    int kGotoTable[1024] = {
        456,  841,  205,  326,  999,  935,   16,  497,  825,  218,  719,  204,  722,  792,  889,  779,  195, 1008,  776,  834,  587,  652,  498,  973,  428,  676,  876,  148,  126,  374,  190,   76,
        530,  994,  898,  806,   75,  865,   30,  863,   36,  167,    7,  414,  496,  391,  103,  111,  679,   35,  953,   65, 1013,   45,  263,  736,  760,  648,  983,   78,  626,   83,  571,  595,
         73,  877,  887,  666,  319,  275,  443,    9,  646,  257,  746,  526,  163,   53,    6,  726,   51,  485, 1014,  665,  738,  198,  493,  337,  761,  971,   31, 1009,   88,  325,  151,  177,
       1007,  466,  988,  897,  644,  696,  630,  998,  193,  924,  769,  459,  107,  710,  519,  281,  390,  934,  632,  297,  484,  554,  314,  949,   47,   87,  400,  900,  931,  856,  470,   81,
        191,  101,  780,  342,   82,  693,  528,  701,  499,  164,  671,  181,  395,   34,  481,  614,  547,  478,  588,   43,  673,  440,  258,  568,  196,  790,  479,  844,  728,  365,  896,  868,
        592,  283, 1000,  720,  685,  913,  387,  527,  796,  583,   79,   13,  334,  805,  929,  921,  610,  521,  364,  791,  253,  811,  773,  309,  637,  749,  421,  942,  729,  694,  940,  122,
        393,  525, 1012,   74,  559,  371,  869,   12,  581,   24, 1011,  327,  681,  893,  835,  704,   62,  492,  293,  389,  977,  429,  458,  357,  347,  129,  274,  222,   23,  851,  301,  578,
         70,  550,  946,  797,    2,  882,   40,  923, 1017,  885,  173,  462,  133,  329,  503,  997,  402,  359,   32,  132,  970,  185,  490,  225,  565,   28,  843,  510,  223,  707,  532,  338,
         64,  154,  627,  483,  700,  697,  477,  545,  937,  702,    5,  494,  705,  926,  596,   22,  143,  118,  202,  169,  854,  939,  867,  188,  212,  145, 1021,  830,  114,  833,  316,  925,
        649,  708,  838,  373,  216,  658,   77,  757,  340,  514,  339,  380,  593,  403,  566,  230,  981,  616,  174,  277,  706,   71,  640,  352,  576,  460,   14,  333,  574,   46,  793,  131,
        439,  692,  123,  376,   52,  457,   99,  549,   27,  713,  320,  752,  147,  775,   89,  961,  384,   38,  672,  142,  135,   97,  589,  157,  699,  430,  635,  287,  846,  817,  995,  422,
        758,  209,  991,  801,  300,  434,  562,  323,  546,  860,  450,  121,  812,  563,  905, 1006,  611,  508,  774,  310,  754,  990,  798, 1005,  271,  954,  770,  426,  500,  883,  660,  467,
         67,  920,  886,  161,  687,  584,  409,  141,  350,  711,  410,  698,  245,  482,  535,  351,  511,  986,  515,  659,  778,  955,  264,  331,  911,  267,  189,  231,  162,  594,  788,  782,
        794,  677,  747,  814,  399,  992,  468,   18,  850,  891,  144,  582,  343,  522,  613,   92,  839,  168,  909,   93,  933,  972,  247, 1004,  739,  115,  654,  454,  534,  906,  907,  695,
        819,  802,  591,  406,  227,   37,  290,  194,  404,   41,   20,  888,  918,  941,  601,  771,  480,  826,  488,   68,  952,  232,  255,  269,  871,  307,  762,  378,  600,  237,  899,  849,
        932,  413,  980,  186,  950,  651,  415,  645,  784,    0,  551,  725,  670,  661,  930,  721,  564,  620,  233,  317,  197,  976,  242,   44,  506,  433,    1,  140,  318,  328,   95,  653,
        464,  292,  249,  423,  146,  254,  299,   54,  213,  996,   21,  200,  948,  446,  149,  741,  866,  870,  878,  624,  285,  416,  507,  279,  647,  612,  455,  767,  602,   58,   42,  786,
        974,  447,  282,  354,  742,  137,  727,  407, 1002,  915,  348,  539,  385,  335,  236,  912,  577,  689,  813,  958,  628,  548,  124,  884,  179,   26,  605,  465,  951,  171,  441,  420,
        418,  502,  718,  987,  531,  716,  243,  211,   94,  822,  358,   56,  982,  541,  842,  452,  268,  353,  435,  408,  895,  730,   90,  816,  669,  759,  117,  240,  743,  288,  251,  638,
        542,  927,  824,  270,   80,  618,  379,  857, 1019,  967,  278,   39,   15,  853,  375,  308,  208,  349,  960,  158,  425,  524,  294,  703,  228,  733,  445, 1022,  709,  615,  787,  266,
        804,  680,  453,  262,  968,  295,  621,  712,  210,  803,  280,   98,  303,  105,  362,  682,  377,  401,  807,  370,   86,  489,  139,  963,  463,  643,  943,  155,  113,  772,  538,  127,
        599,  634,  633,  567,  715,   50,  663,  206,  427,   29,  112,  523,  272,  203,  845,  346,  580,  956,  229,  902,  777,    3,  109,  650,  381,  674,  872,  558,  723,  737,  472,  892,
        664,   10,  827,   17,  631,  286,  607,  265,  207,  336,  215,  572,  984,  182,  947,  916,  848,  609,  248,  914,  363,  486,  604,   33,  261,  684,  315, 1001,  569,  852,  432,  586,
        537,  516,   57,   60,  160,  120,  110,  556,  517,  656,  306,  451,  509,  298,  642,  828,  745,  629,   66,   96,   84,  361, 1003, 1020,  180,  276,  667,  166,   72,  394,  904,  219,
        192, 1010,  321,  688,  224,  259,  691,  405,  382,  178,  964,  544,  585,    4,  220,  879,  732,  832,  978,  606,  104,  910,  809,  100,  799,  862,  861,  431,  890,  199,  324,  360,
         49,  183,  471,  138,  617,  244,  847,  449,  529,   61,  768,  800,  969,  505,   11,  234,  102,  625,  836,  756,  678,  875,  573,  125, 1016,  683,  579,  312,  962,  473,  152,  686,
        273,  740,  150,  201,  136,  821,  355,  724,  859,  116,  461,  858,  397,  881,  570,   55,  134,  388,  755,  332,  356,  438,  221,   85,  386,  979,  831,  226,  873,  396,  789,  945,
        655,  763,  424,  330,  476,  170,  444,  748,  557,  561,  823,  657,  908,  553,  985,  735,  469,  619,  304, 1018,  130,  781,  874,   69,   48,  810,  598,  957,  217,  345,  623,  734,
         59,  936,  322,  106,  690,  829,  785,  305,  917,  383,  296,  989,  487,  555,  668,  820,  520,  837,  518,  491,   91,  714,  540,  543,  608,  903,   25,  284,  159,  501,  367,  765,
         19,  864,  959,  675,  536,  641,  241,  411,  214,  256,  419,  250,  344,  153,  176,  437,  764,  944,  880, 1015,  165,  840,  108,  533,  965,  235,  815,  442,  302,  291,  922,  560,
        753,  750,  260,  993,  475,  172,  639,  369,  590,  938,  855,  238,  504,  184,  119,  368,  448,  744,  975,  928,  603,  128,  795,  187,  436, 1023,  552,  808,  783,  398,  662,  622,
        311,  372,  392,  513,  289,  366,  495,  474,  597,  246,  417,  575,  894,  512,  717,  731,  751,  313,  252,  175,  966,  412,  636,  818,  239,  766,  341,   63,  901,  919,  156,    8,
    };
}

/* This is where the stack overflow will appear in the debugger.
 *
 * You may get a warning message here because the compiler thinks (correctly)
 * that this will trigger a stack overflow.
 */
void triggerStackOverflow(int value) {
    triggerStackOverflow(kGotoTable[value]);
}

/* Entry point for the code that triggers a stack overflow. It determines what the starting
 * number should be, validates that you've changed the MyName constant, and then makes the
 * initial call to triggerStackOverflow.
 */
void initiateStackOverflow() {
    /* Before we trigger the stack overflow, let's make sure that you've
     * changed the MyName constant at the top of the file. If the debugger
     * pops up at the line below, it means that you forgot to edit
     * MyName.
     */
    if (MyName == "(Your Name Here)") {
        error("Please change the constant MyName defined in " __FILE__ " to your name.");
    }

    /* Initiate the stack overflow by calling triggerStackOverflow. This line of code
     * also scrambles the numbers in the kGotoTable even more than they already are, and
     * does so in a way that will ensure you aren't going to spend too much time walking
     * the call stack looking for the loop.
     *
     * (A note: this scrambling of the goto table in principle could cause problems if
     * we called this function again, since the scramble would be different the second
     * time around. However, since the program always crashes after doing this, we don't
     * need to worry about that.)
     */
    triggerStackOverflow(shuffleValues(hashCode(MyName), kGotoTable, 1024, kGotoTable[0x2A]));
}
