#ifndef STRINGS_H
#define STRINGS_H

/**
 * ============================================================================
 * PETOPIA - EDITABLE TEXT CONFIGURATION
 * ============================================================================
 * [EDITABLE]: Everything in this file is editable!
 * You can change any of the text values below to customize the game.
 * Simply edit the text inside the quotation marks, save, and recompile.
 * ============================================================================
 */

// [EDITABLE]: Window Title
#define TXT_WINDOW_TITLE        "PETOPIA"

// ----------------------------------------------------------------------------
// [EDITABLE]: Screen 1 - Welcome Screen Text
// ----------------------------------------------------------------------------
#define TXT_WELCOME_TITLE       "PETOPIA"                           // [EDITABLE]: Main title
#define TXT_WELCOME_SUBTITLE    "Rawat hewan peliharaan virtualmu!" // [EDITABLE]: Subtitle quote
#define TXT_BTN_START           "START"                             // [EDITABLE]: Start button label

// ----------------------------------------------------------------------------
// [EDITABLE]: Screen 2 - Pet Selection Screen Text
// ----------------------------------------------------------------------------
#define TXT_SELECT_TITLE        "Pilih Hewan Peliharaanmu"          // [EDITABLE]: Selection title
#define TXT_SELECT_SUBTITLE     "Pilih sahabat yang ingin dirawat"  // [EDITABLE]: Selection subtitle
#define TXT_PET_CAT             "Kucing"                            // [EDITABLE]: Pet 1 Name
#define TXT_PET_DOG             "Anjing"                            // [EDITABLE]: Pet 2 Name
#define TXT_PET_RABBIT          "Kelinci"                           // [EDITABLE]: Pet 3 Name
#define TXT_PET_NANDA           "NANDA"                             // [EDITABLE]: Pet 4 Name
#define TXT_BTN_SELECT          "PILIH"                             // [EDITABLE]: Choose button
#define TXT_BTN_SELECTED        "TERPILIH"                          // [EDITABLE]: Chosen button state
#define TXT_BTN_CONTINUE        "LANJUT"                            // [EDITABLE]: Continue button

// ----------------------------------------------------------------------------
// [EDITABLE]: Screen 3 - Name Your Pet Screen Text
// ----------------------------------------------------------------------------
#define TXT_NAME_TITLE          "Beri Nama Peliharaanmu"            // [EDITABLE]: Naming title
#define TXT_NAME_PLACEHOLDER    "Ketik nama pet..."                 // [EDITABLE]: Input prompt text
#define TXT_BTN_ENTER_HOUSE     "MASUK RUMAH"                       // [EDITABLE]: Confirm button

// ----------------------------------------------------------------------------
// [EDITABLE]: Screen 4 - Living Room (Central Hub) Text
// ----------------------------------------------------------------------------
#define TXT_ROOM_LIVING         "RUANG KELUARGA"                    // [EDITABLE]: Living room name
#define TXT_STAT_HUNGER         "Kenyang"                           // [EDITABLE]: Hunger stat label
#define TXT_STAT_HAPPINESS      "Senang"                            // [EDITABLE]: Happiness stat label
#define TXT_STAT_ENERGY         "Energi"                            // [EDITABLE]: Energy stat label
#define TXT_STAT_CLEANLINESS    "Kebersihan"                        // [EDITABLE]: Cleanliness stat label
#define TXT_STAT_HEALTH         "Kesehatan"                         // [EDITABLE]: Health stat label

#define TXT_NAV_KITCHEN         "KE DAPUR (FEED)"                   // [EDITABLE]: Navigation to kitchen
#define TXT_NAV_BACKYARD        "KE HALAMAN (PLAY)"                 // [EDITABLE]: Navigation to backyard
#define TXT_NAV_BEDROOM         "KE KAMAR TIDUR (REST)"             // [EDITABLE]: Navigation to bedroom
#define TXT_ACTION_BATH         "KE KAMAR MANDI (CLEAN)"            // [EDITABLE]: Bath button

// ----------------------------------------------------------------------------
// [EDITABLE]: Screen 5 - Kitchen (Feeding) Text
// ----------------------------------------------------------------------------
#define TXT_KITCHEN_TITLE       "DAPUR PETOPIA"                     // [EDITABLE]: Kitchen title
#define TXT_KITCHEN_SUBTITLE    "Pilih makanan lezat untuk petmu:"  // [EDITABLE]: Kitchen subtitle
#define TXT_FOOD_SNACK          "Beri Camilan (+10)"                // [EDITABLE]: Snack button
#define TXT_FOOD_MEAL           "Beri Makanan Berat (+30)"          // [EDITABLE]: Meal button
#define TXT_FOOD_DRINK          "Minum Air Segar (+15)"             // [EDITABLE]: Drink button

// ----------------------------------------------------------------------------
// [EDITABLE]: Screen 6 - Backyard (Playing) Text
// ----------------------------------------------------------------------------
#define TXT_BACKYARD_TITLE      "HALAMAN BELAKANG"                  // [EDITABLE]: Backyard title
#define TXT_BACKYARD_SUBTITLE   "Ajak petmu bermain di luar:"       // [EDITABLE]: Backyard subtitle
#define TXT_PLAY_BALL           "Lempar Bola (+20)"                 // [EDITABLE]: Ball activity
#define TXT_PLAY_FRISBEE        "Main Frisbee (+35)"                // [EDITABLE]: Frisbee activity
#define TXT_PLAY_WALK           "Jalan Santai (+15)"                // [EDITABLE]: Walk activity

// ----------------------------------------------------------------------------
// [EDITABLE]: Screen 7 - Bedroom (Resting) Text
// ----------------------------------------------------------------------------
#define TXT_BEDROOM_TITLE       "KAMAR TIDUR"                       // [EDITABLE]: Bedroom title
#define TXT_BEDROOM_SUBTITLE    "Biarkan petmu tidur dan istirahat" // [EDITABLE]: Bedroom subtitle
#define TXT_REST_NAP            "Tidur Siang (+25)"                 // [EDITABLE]: Nap button
#define TXT_REST_SLEEP          "Tidur Pulas (+60)"                 // [EDITABLE]: Deep sleep button
#define TXT_SLEEPING_ZZZ        "Z z z ..."                         // [EDITABLE]: Sleeping bubble

// ----------------------------------------------------------------------------
// [EDITABLE]: Common Navigation & Notification Messages
// ----------------------------------------------------------------------------
#define TXT_BTN_BACK            "< KEMBALI"                         // [EDITABLE]: Back button label
#define TXT_MSG_WELCOME         "Selamat datang di Petopia!"        // [EDITABLE]: Initial message
#define TXT_MSG_CLEANED         "dimandikan sampai bersih!"         // [EDITABLE]: Bath feedback
#define TXT_MSG_SHOWERED        "disiram air hangat, segar!"        // [EDITABLE]: Shower feedback
#define TXT_MSG_BRUSHED         "disikat bulunya, rapi!"            // [EDITABLE]: Brush feedback

// Bathroom Screen
#define TXT_BATHROOM_TITLE      "Kamar Mandi"                      // [EDITABLE]: Bathroom title
#define TXT_BATHROOM_SUBTITLE   "Bersihkan pet kesayanganmu!"      // [EDITABLE]: Bathroom subtitle
#define TXT_BATH_SHOWER         "Mandikan (Shower)"                 // [EDITABLE]: Shower option
#define TXT_BATH_BRUSH          "Sikat Bulu (Brush)"                // [EDITABLE]: Brush option
#define TXT_BATH_FULL           "Mandi Lengkap (Full Bath)"         // [EDITABLE]: Full bath option
#define TXT_MSG_SNACK           "menikmati camilan!"                // [EDITABLE]: Snack feedback
#define TXT_MSG_MEAL            "makan sampai kenyang!"             // [EDITABLE]: Meal feedback
#define TXT_MSG_DRINK           "minum air segar!"                  // [EDITABLE]: Drink feedback
#define TXT_MSG_BALL            "mengejar bola!"                    // [EDITABLE]: Ball feedback
#define TXT_MSG_FRISBEE         "menangkap frisbee!"                // [EDITABLE]: Frisbee feedback
#define TXT_MSG_WALK            "menikmati jalan santai!"           // [EDITABLE]: Walk feedback
#define TXT_MSG_NAP             "bangun segar dari tidur siang!"    // [EDITABLE]: Nap feedback
#define TXT_MSG_SLEEP           "tidur nyenyak sepanjang malam!"    // [EDITABLE]: Sleep feedback

#endif
