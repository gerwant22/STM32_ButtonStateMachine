/*
 * button.c
 *
 *  Created on: Nov 8, 2022
 *      Author: Daniel
 */
#include "main.h"
#include "button.h"



//Button init

void ButtonInitKey(TButton* Key, GPIO_TypeDef* GPIO_Port, uint16_t GPIO_Pin, uint32_t TimerDebounce, uint32_t TimerLongPress, uint32_t TimerRepeat)
{
	Key-> State = IDLE;

	Key->GPIO_Port = GPIO_Port;
	Key->GPIO_Pin = GPIO_Pin;

	Key->TimerDebounce = TimerDebounce;
	Key->TimerLongPress = TimerLongPress;
	Key->TimerRepeat = TimerRepeat;

}

//Time setting function

void ButtonSetDebounceTime(TButton* Key, uint32_t Miliseconds)
{
	Key->TimerDebounce = Miliseconds;
}

void ButtonSetLongPressTime(TButton* Key, uint32_t Miliseconds)
{
	Key->TimerLongPress = Miliseconds;
}

void ButtonSetRepeatTime(TButton* Key, uint32_t Miliseconds)
{
	Key->TimerRepeat = Miliseconds;
}

// Register callback

void ButtonRegisterPressedCallback(TButton* Key, void *Callback)
{
	Key->ButtonPresed = Callback;
}

void ButtonRegisterLongPressCallback(TButton* Key, void *Callback)
{
	Key->ButtonLongPress = Callback;
}

void ButtonRegisterRepetCallback(TButton* Key, void *Callback)
{
	Key->ButtonRepeat= Callback;
}


//States routines

void ButtonIdleRoutine(TButton* Key)
{
	if(GPIO_PIN_RESET == HAL_GPIO_ReadPin(Key->GPIO_Port, Key->GPIO_Pin))
	{
		Key->State = DEBOUNCE;
		Key->LastTick = HAL_GetTick();
	}
}

void ButtonDebounceRoutine(TButton* Key)
{
	if((HAL_GetTick() - Key->LastTick) > Key->TimerDebounce)
	{
		if(GPIO_PIN_RESET == HAL_GPIO_ReadPin(Key->GPIO_Port, Key->GPIO_Pin))
		{
			Key->State = PRESSED;
			Key->LastTick = HAL_GetTick();

			if(Key->ButtonPresed != NULL)
			{
				Key->ButtonPresed();
			}
		}
		else
		{
			Key->State = IDLE;
		}
	}
}

void ButtonPressedRoutine(TButton* Key)
{


	if(GPIO_PIN_SET == HAL_GPIO_ReadPin(Key->GPIO_Port, Key->GPIO_Pin))
	{
		Key->State = IDLE;
	}
	else
	{
		if((HAL_GetTick() - Key->LastTick) > Key->TimerLongPress)
		{
			Key->State = REPEAT;
			Key->LastTick = HAL_GetTick();

			if(Key->ButtonLongPress!= NULL)
			{
				Key->ButtonLongPress();
			}

		}
	}
}

void ButtonRepeatRoutine(TButton* Key)
{
	if(GPIO_PIN_SET == HAL_GPIO_ReadPin(Key->GPIO_Port, Key->GPIO_Pin))
	{
		Key->State = IDLE;
	}
	else
	{
		if((HAL_GetTick() - Key->LastTick) > Key->TimerRepeat)
				{
					Key->LastTick = HAL_GetTick();

					if(Key->ButtonRepeat!= NULL)
					{
						Key->ButtonRepeat();
					}
				}
	}

}



//State machine
void ButtonTask(TButton* Key)
{
	switch(Key->State)
	{
	case IDLE:
		ButtonIdleRoutine(Key);
		break;

	case DEBOUNCE:
		ButtonDebounceRoutine(Key);
		break;

	case PRESSED:
		ButtonPressedRoutine(Key);
		break;
	case REPEAT:
		ButtonRepeatRoutine(Key);
		break;
	}
}

