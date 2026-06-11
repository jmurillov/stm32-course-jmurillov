/*
 * trafficLight.c
 *
 *  Created on: Jun 4, 2026
 *      Author: sebastian
 *      Brief: Implementation of the traffic light finite state machine.
 */

#include <stm32f4xx.h>
#include "trafficLight.h"

/* Type definitions and private constants */
typedef enum
{
	STATE_GREEN,
	STATE_ORANGE,
	STATE_RED
} TrafficLightState;

/* Static variables */
static volatile uint32_t traffic_light_counter = 0;
static TrafficLightState current_state = STATE_GREEN;

/* Private function prototypes */
static void trafficLight_SetLEDs(TrafficLightState state);

/* Public function implementations */

/*
 * @brief Initializes the traffic light GPIO pins and initial state.
 */
void trafficLight_Init(void)
{
	/* Activate GPIOA peripheral clock */
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;

	/* Configure PA5 as general purpose output (Green LED) */
	GPIOA->MODER &= ~(GPIO_MODER_MODE5);
	GPIOA->MODER |= GPIO_MODER_MODE5_0;
	GPIOA->OTYPER &= ~(GPIO_OTYPER_OT5);
	GPIOA->OSPEEDR &= ~(GPIO_OSPEEDR_OSPEED5);
	GPIOA->OSPEEDR |= GPIO_OSPEEDR_OSPEED5_1;

	/* Configure PA6 as general purpose output (Orange LED) */
	GPIOA->MODER &= ~(GPIO_MODER_MODE6);
	GPIOA->MODER |= GPIO_MODER_MODE6_0;
	GPIOA->OTYPER &= ~(GPIO_OTYPER_OT6);
	GPIOA->OSPEEDR &= ~(GPIO_OSPEEDR_OSPEED6);
	GPIOA->OSPEEDR |= GPIO_OSPEEDR_OSPEED6_1;

	/* Configure PA7 as general purpose output (Red LED) */
	GPIOA->MODER &= ~(GPIO_MODER_MODE7);
	GPIOA->MODER |= GPIO_MODER_MODE7_0;
	GPIOA->OTYPER &= ~(GPIO_OTYPER_OT7);
	GPIOA->OSPEEDR &= ~(GPIO_OSPEEDR_OSPEED7);
	GPIOA->OSPEEDR |= GPIO_OSPEEDR_OSPEED7_1;

	/* Initialize LEDs to match starting state */
	trafficLight_SetLEDs(STATE_GREEN);
}

/*
 * @brief Updates the traffic light state machine logic.
 */
void trafficLight_Update(void)
{
	switch (current_state)
	{
		case STATE_GREEN:
			if (traffic_light_counter >= 5)
			{
				traffic_light_counter = 0;
				current_state = STATE_ORANGE;
				trafficLight_SetLEDs(STATE_ORANGE);
			}
			break;

		case STATE_ORANGE:
			if (traffic_light_counter >= 2)
			{
				traffic_light_counter = 0;
				current_state = STATE_RED;
				trafficLight_SetLEDs(STATE_RED);
			}
			break;

		case STATE_RED:
			if (traffic_light_counter >= 5)
			{
				traffic_light_counter = 0;
				current_state = STATE_GREEN;
				trafficLight_SetLEDs(STATE_GREEN);
			}
			break;

		default:
			current_state = STATE_GREEN;
			trafficLight_SetLEDs(STATE_GREEN);
			break;
	}
}

/*
 * @brief Increments the internal timer counter.
 */
void trafficLight_Tick(void)
{
	traffic_light_counter++;
}

/*
 * @brief Handles the button press event, speeding up the state transition.
 */
void trafficLight_HandleButton(void)
{
	traffic_light_counter += 10;
}

/* Private function implementations */

/*
 * @brief Updates the GPIO outputs based on the current state using atomic BSRR writes.
 */
static void trafficLight_SetLEDs(TrafficLightState state)
{
	switch (state)
	{
		case STATE_GREEN:
			/* Turn on PA5 (Green), Turn off PA6 (Orange) and PA7 (Red) */
			GPIOA->BSRR = GPIO_BSRR_BS5 | GPIO_BSRR_BR6 | GPIO_BSRR_BR7;
			break;

		case STATE_ORANGE:
			/* Turn on PA6 (Orange), Turn off PA5 (Green) and PA7 (Red) */
			GPIOA->BSRR = GPIO_BSRR_BS6 | GPIO_BSRR_BR5 | GPIO_BSRR_BR7;
			break;

		case STATE_RED:
			/* Turn on PA7 (Red), Turn off PA5 (Green) and PA6 (Orange) */
			GPIOA->BSRR = GPIO_BSRR_BS7 | GPIO_BSRR_BR5 | GPIO_BSRR_BR6;
			break;

		default:
			break;
	}
}
