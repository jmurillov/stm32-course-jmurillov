/*
 * trafficLight.h
 *
 *  Created on: Jun 4, 2026
 *      Author: sebastian
 *      Brief: Header file for the traffic light FSM module.
 */

#ifndef _TRAFFICLIGHT_H_
#define _TRAFFICLIGHT_H_

#include <stdint.h>

/* Public function prototypes */

/*
 * @brief Initializes the traffic light GPIO pins and initial state.
 */
void trafficLight_Init(void);

/*
 * @brief Updates the traffic light state machine logic.
 *        Should be called continuously in the main application loop.
 */
void trafficLight_Update(void);

/*
 * @brief Increments the internal timer counter.
 *        Should be called from the timer interrupt service routine.
 */
void trafficLight_Tick(void);

/*
 * @brief Handles the button press event, speeding up the state transition.
 */
void trafficLight_HandleButton(void);

#endif /* _TRAFFICLIGHT_H_ */
