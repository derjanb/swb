#pragma once
#include <webkit2/webkit2.h>
#include "handlers.h"
#include "navigation.h"
#include "helpers.h"

typedef struct {
	WebKitLoadEvent event;
	void (*func)(Browser *, char *);
	char *arg;
} Event;

Event events[] = {
	{WEBKIT_LOAD_COMMITTED, exec_js, CONFIG_PATH"scripts/noscript.js"}
};
