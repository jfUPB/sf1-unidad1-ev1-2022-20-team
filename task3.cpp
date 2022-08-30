#include <Arduino.h>
#include "task3.h"
#include "events.h"

static bool compareKeys(BUTTONS *pSecret, BUTTONS *pKey)
{
    bool correct = true;
    for (uint8_t i = 0; i < 7; i++)
    {
        if (pSecret[i] != pKey[i])
        {
            correct = false;
            break;
        }
    }

    return correct;
}

void task3()
{
    enum class TaskStates
    {
        INIT,
        WAIT,
        WAIT2,
        SLOW,
        MEDIUM,
        FAST,
        OFF,
        ON,

    };

    static TaskStates taskState = TaskStates::INIT;
    const uint8_t led = 25;
    static uint32_t lastTime;
    static uint8_t keyCounter;
    static bool ledStatus;
    static uint8_t variable;
    const uint32_t LEDINTERVAL = 1000U;
    const uint32_t LEDCOUNTERINTERVAL = 500U;

    static BUTTONS secret[5] = {BUTTONS::BTN_1, BUTTONS::BTN_1,
                                BUTTONS::BTN_2, BUTTONS::BTN_2,
                                BUTTONS::BTN_1};

    static BUTTONS disarmKey[5] = {BUTTONS::NONE};

    switch (taskState)
    {
    case TaskStates::INIT:
    {

        pinMode(led, OUTPUT);
        ledStatus = true;
        digitalWrite(led, ledStatus);
        lastTime = millis();
        keyCounter = 0;
        taskState = TaskStates::SLOW;
        break;
    }

    case TaskStates::SLOW:
    {
        uint32_t currentTime = millis();
        if ((currentTime - lastTime) >= 1000)
        {
            lastTime = currentTime;
            digitalWrite(led, ledStatus);
            ledStatus = !ledStatus;
        }

        if (buttonEvt.trigger == true)
        {
            buttonEvt.trigger == false;
            if (buttonEvt.whichButton == BUTTONS::BTN_1)
            {
                taskState = TaskStates::WAIT;
            }

            else if (buttonEvt.whichButton == BUTTONS::BTN_2)
            {

                ledStatus = true;
                digitalWrite(led, ledStatus);
                lastTime = currentTime;
                taskState = TaskStates::MEDIUM;
            }
        }

        break;
    }

    case TaskStates::MEDIUM:
    {

        uint32_t currentTime = millis();
        if ((currentTime - lastTime) >= 500)
        {
            lastTime = currentTime;
            digitalWrite(led, ledStatus);
            ledStatus = !ledStatus;
        }
        if (buttonEvt.trigger == true)
        {

            buttonEvt.trigger == false;
            if (buttonEvt.whichButton == BUTTONS::BTN_1)
            {
                taskState = TaskStates::WAIT2;
            }

            else if (buttonEvt.whichButton == BUTTONS::BTN_2)
            {

                ledStatus = true;
                digitalWrite(led, ledStatus);
                lastTime = currentTime;
                taskState = TaskStates::SLOW;
            }
        }

        break;
    }

    case TaskStates::FAST:
    {

        uint32_t currentTime = millis();
        if ((currentTime - lastTime) >= 250)
        {
            lastTime = currentTime;
            digitalWrite(led, ledStatus);
            ledStatus = !ledStatus;
        }

        if (buttonEvt.trigger == true)
        {

            buttonEvt.trigger = false;
            disarmKey[keyCounter] = buttonEvt.whichButton;
            keyCounter++;
            if (keyCounter == 5)
            {
                keyCounter = 0;
                if (compareKeys(secret, disarmKey) == true)
                {

                    // Me voy para OFF o me voy para ON?

                    if (variable == 1)
                    {
                        lastTime = currentTime;
                        ledStatus = true;
                        digitalWrite(led, ledStatus);
                        taskState = TaskStates::SLOW;
                    }
                    else
                    {
                        lastTime = currentTime;
                        ledStatus = true;
                        digitalWrite(led, ledStatus);
                        taskState = TaskStates::MEDIUM;
                    }
                }
            }
        }

        break;
    }

    case TaskStates::OFF:
    {
        if (buttonEvt.trigger == true)
        {
            buttonEvt.trigger = false;
            if (buttonEvt.whichButton == BUTTONS::BTN_1)
            {
                ledStatus = true;
                digitalWrite(led, ledStatus);
                lastTime = millis();
                taskState = TaskStates::SLOW;
            }
            else if (buttonEvt.whichButton == BUTTONS::BTN_2)
            {
                keyCounter = 0;
                ledStatus = true;
                digitalWrite(led, ledStatus);
                lastTime = millis();
                variable = 1;
                taskState = TaskStates::FAST;
            }
        }

        break;
    }

    case TaskStates::ON:
    {

        if (buttonEvt.trigger == true)
        {
            buttonEvt.trigger = false;
            if (buttonEvt.whichButton == BUTTONS::BTN_1)
            {
                ledStatus = true;
                digitalWrite(led, ledStatus);
                taskState = TaskStates::MEDIUM;
            }
            else if (buttonEvt.whichButton == BUTTONS::BTN_2)
            {
                ledStatus = true;
                digitalWrite(led, ledStatus);
                keyCounter = 0;
                variable = 2;
                taskState = TaskStates::FAST;
            }
        }

        break;
    }

    case TaskStates::WAIT:
    {
        uint32_t currentTime = millis();
        if ((currentTime - lastTime) > 1000)
        {
            ledStatus = false;
            digitalWrite(led, ledStatus);

            taskState = TaskStates::OFF;
        }

        break;
    }

    case TaskStates::WAIT2:
    {
        uint32_t currentTime = millis();
        if ((currentTime - lastTime) > 500)
        {
            ledStatus = false;
            digitalWrite(led, ledStatus);

            taskState = TaskStates::ON;
        }

        break;
    }
    default:
    {

        break;
    }
    }
}
