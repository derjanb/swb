#define HOME_PAGE "http://duckduckgo.com"

#define COOKIE_FILE "/home/simon/.config/swb/cookies"

#define STYLESHEET_FILE "/home/simon/.config/swb/style.css"

#define HISTORY_FILE "/home/simon/.config/swb/history"
#define READ_URL_CMD "sqlite3 " HISTORY_FILE " \"select url from history order by hits desc;\" | dmenu"

#define OPEN_KEY GDK_KEY_o
#define TABOPEN_KEY GDK_KEY_t
#define CLOSE_TAB_KEY GDK_KEY_q
#define FORWARD_KEY GDK_KEY_f
#define BACKWARD_KEY GDK_KEY_d
