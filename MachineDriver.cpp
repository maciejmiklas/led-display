/*
 * Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.
 * The ASF licenses this file to You under the Apache License, Version 2.0
 * (the "License"); you may not use this file except in compliance with
 * the License.  You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
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

#if LOG_D
	log(F("MD sate to %d of %d"), state, statesCnt);
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
#if LOG_D
	log(F("MD RST"));
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

