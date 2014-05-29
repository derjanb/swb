#pragma once

#define BUF_LEN 10000

#define HOME_PAGE "http://duckduckgo.com"

#define USER_AGENT "Mozilla/5.0 (Macintosh; Intel Mac OS X 10_9_2) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/36.0.1944.0 Safari/537.36"


#define CONFIG_PATH "/home/simon/.config/swb/"

#define COOKIE_FILE CONFIG_PATH"cookies"

#define STYLESHEET_FILE CONFIG_PATH"style.css"
#define BROWSER_STYLE CONFIG_PATH"browser.css"


#define HISTORY_FILE CONFIG_PATH"history"
#define READ_URL_CMD(CUR_URL, PROMPT) g_strconcat("sh -c \"echo \\\"", CUR_URL, "\\\" $(sqlite3 ", HISTORY_FILE, " \\\"select url from history order by hits desc;\\\") | tr -s ' ' '\n' | dmenu -p \\\"", PROMPT,"\\\"\"",  (char*)0)
#define READ_ANY_CMD(PROMPT) g_strconcat("sh -c \"dmenu -p \\\"", PROMPT, "\\\"", " <&-\"", (char *)0)
