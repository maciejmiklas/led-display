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
 #ifndef STATEMASHINE_H_
#define STATEMASHINE_H_

#include "Arduino.h"

/**
 * State Machine is state full object that represents particular state. On each loop (MachineDriver#execute()) the
 * method #execute() will be called - until in returns next state. In this case the next State Machine will get
 * executed,... an so on.
 */
class StateMachine {

public:
	/**
	 * This function returns id of the state that will be executed as next,  or #STATE_NOCHANGE if state should not
	 * be changed.
	 */
	virtual uint8_t execute() = 0;

	/**
	 * This method will be called always before switching to this state machine from another one. This can happen
	 * multiple times during single workflow. For example: machine A switches to machine B, and after that B to A
	 */
	virtual void init() = 0;

	/**
	 * This method resets whole workflow - this happens when state machine has to process completely new data. For
	 * example animating new sprite.
	 */
	virtual void reset() = 0;

	/**
	 * The execution of each state returns ID of next state, or no change. If state changes, the #execute() method of
	 * new state will be executed by machine driver on next iteration (as next sprite frame). But some states have only
	 * organizing propose - they redirect to another state without doing any useful job. In this case such state can
	 * be marked as intermediate - when #execute() returns Id of new state, this state will be executed immediately and
	 * not as usual on next iteration.
	 */
	virtual boolean isIntermediate() = 0;
	virtual ~StateMachine() = 0;

	/**
	 * Predefined states.
	 */
	enum mstate_t {
		/** Resets all states and starts from first one */
		STATE_RESET = 253,

		/** Indicates that actual state should continue to execute. */
		STATE_NOCHANGE = 254,

		/** Select state that does nothing and never ends. */
		STATE_NOOP = 255
	};

protected:
	StateMachine();
};

#endif /* STATEMASHINE_H_ */
