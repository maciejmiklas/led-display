#include "Util.h"

static uint32_t cycleMs = 0;

void util_setup() {
	util_cycle();
}

void util_cycle() {
	cycleMs = millis();
}

uint32_t ms() {
	return cycleMs;
}
