/*
 * button.h
 *
 *  Created on: Nov 8, 2022
 *      Author: Daniel
 */

#ifndef INC_BUTTON_H_
#define INC_BUTTON_H_

//states for state machine

typedef enum
{
	IDLE = 0,
	DEBOUNCE,
	PRESSED,
	REPEAT,
	RELEASE
} BUTTON_STATE;



//structure for button


typedef struct
{

	BUTTON_STATE State;

	GPIO_TypeDef* 	GPIO_Port;
	uint16_t 		GPIO_Pin;

	uint32_t 		LastTick;
	uint32_t 		TimerDebounce;

	uint32_t 		TimerLongPress;
	uint32_t		TimerRepeat;

	void(*ButtonPresed)(void);
	void(*ButtonLongPress)(void);
	void(*ButtonRepeat)(void);
	void(*ButtonRelease)(void);

}TButton;

//public functions

void ButtonTask(TButton* Key);

void ButtonSetDebounceTime(TButton* Key, uint32_t Miliseconds);
void ButtonSetLongPressTime(TButton* Key, uint32_t Miliseconds);
void ButtonSetRepeatTime(TButton* Key, uint32_t Miliseconds);

void ButtonInitKey(TButton* Key, GPIO_TypeDef* GPIO_Port, uint16_t GPIO_Pin, uint32_t TimerDebounce, uint32_t TimerLongPress, uint32_t RepeatTimier);

void ButtonRegisterPressedCallback(TButton* Key, void *Callback);
void ButtonRegisterLongPressCallback(TButton* Key, void *Callback);
void ButtonRegisterRepetCallback(TButton* Key, void *Callback);
void ButtonRegisterReleaseCallback(TButton* Key, void *Callback);


//public variables

#endif /* INC_BUTTON_H_ */
