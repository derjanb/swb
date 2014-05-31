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
	/* example - Exec follow_hints.js when page finished loading */
	{WEBKIT_LOAD_FINISHED, exec_js, CONFIG_PATH"scripts/follow_hints.js"}
	};
