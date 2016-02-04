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
 #ifndef MASHINEDRIVER_H_
#define MASHINEDRIVER_H_

#include "StateMachine.h"
#include "ArdLog.h"

/**
 * Machine Driver orchestrates State Machines - it contains list of all possible states, executes and switches them.
 *
 * Each execution of particular state returns id of the next state to be executed, or #STATE_NOCHANGE if next iteration
 * should execute current state again.
 *
 * Each state has it's own ID and there can be up to 0-240 states - last 15 are reserved for predefined states. The are
 * defined in StateMachine#mstate_t.
 */
class MachineDriver {

public:

	/**
	 * Creates Machine Driver with number of states given by #states. The var-arg argument contains pointers to
	 * implementation of #StateMachine. There are few rules that you have to follow:
	 * 1) Amount of expected pointers is given by #states
	 * 2) States are numbered starting from 0 to #states. In order to change state, function #changeState(uint8_t)
	 *    takes an int argument and this is the state id determined by it's position in provided var-arg argument.
	 *    For example:
	 *    - if you have created the driver with 5 states, in order to change to last one execute #changeState(4)
	 *    - in order to switch to predefined noop-state execute: #change(STATE_NOOP)
	 * 3) State machine starts always from 0 state, also reseting switches to this state.
	 */
	MachineDriver(uint8_t states, ...);

	/** returns true if current state is not noop. */
	boolean isRunning();

	/**
	 * This method changes state and initializes it. In case of intermediate state, next state will be executed
	 * immediately.
	 */
	void changeState(uint8_t state);

	/**
	 * This method pushes forward out machine driver. Witch each call current state is being executed and this state
	 * becomes a chance to switch driver to follow up state.
	 *
	 * Each executed state returns id of next state to be executed. This method executes current state and calls
	 * #changeState(stateId) with #stateId returned be the execution. This can lead to new state, but it's also
	 * possible that method will return no-change. In this case current state can be executed again.
	 */
	void execute();

	/**
	 * This method resets whole work flow by:
	 * 1) calling reset() on all states
	 * 2) switching and initializing first state (technically speaking it's 0 index in states array).
	 *
	 * Reset  happens when state machine has to process completely new data. For example animating new sprite, or starts
	 * playing animation from the beginning.
	 */
	void reset();

	virtual ~MachineDriver();

private:
	const uint8_t statesCnt;

	/** Currently executed state. */
	StateMachine *current;

	/**
	 * Does nothing ;)
	 */
	class NoopState: public StateMachine {
	public:
		NoopState();
		virtual ~NoopState();
		virtual uint8_t execute();
		virtual void init();
		virtual void reset();
		virtual boolean isIntermediate();
	};
	NoopState noopState;

	/** Array of pointers to StateMachine obejcts.s */
	StateMachine ** const states;
};

#endif /* MASHINEDRIVER_H_ */
