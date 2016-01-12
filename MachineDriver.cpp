#include "MachineDriver.h"
#include "StateMachine.h"

MachineDriver::MachineDriver(uint8_t statesCnt, ...) :
		statesCnt(statesCnt), noopState(), states(new StateMachine*[statesCnt]) {
	this->current = &noopState;

	va_list va;
	va_start(va, statesCnt);
	for (uint8_t sIdx = 0; sIdx < statesCnt; sIdx++) {
		states[sIdx] = va_arg(va, StateMachine*);
	}
}

MachineDriver::~MachineDriver() {
	delete (states);
}

void MachineDriver::changeState(uint8_t state) {
	if (state == StateMachine::STATE_NOCHANGE) {
		return;
	}

#if LOG
	log(F("Change state to %d of %d"), state, statesCnt);
#endif

	boolean init = true;
	switch (state) {
	case StateMachine::STATE_RESET:
		reset();
		init = false;
		break;

	case StateMachine::STATE_NOOP:
		current = &noopState;
		break;

	default:
		current = states[state];
		break;
	}

	if (init) {
		current->init();
	}

	if (current->isIntermediate()) {
		execute();
	}
}

void MachineDriver::reset() {
#if LOG
	log(F("Reseting Machine Driver"));
#endif

	for (uint8_t sIdx = 0; sIdx < statesCnt; sIdx++) {
		states[sIdx]->reset();
	}
	current = states[0];
	current->init();
}

boolean MachineDriver::isRunning() {
	return current != &noopState;
}

void MachineDriver::execute() {
	uint8_t retState = current->execute();
	changeState(retState);
}

// ################ NoopState ################
MachineDriver::NoopState::NoopState() {
}

MachineDriver::NoopState::~NoopState() {
}

void MachineDriver::NoopState::init() {
}

void MachineDriver::NoopState::reset() {
}

boolean MachineDriver::NoopState::isIntermediate() {
	return false;
}

uint8_t MachineDriver::NoopState::execute() {
	return StateMachine::STATE_NOCHANGE;
}

