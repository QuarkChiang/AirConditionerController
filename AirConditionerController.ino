/*
 Name:		AirConditionerController.ino
 Created:	2018/5/16 下午 07:45:23
 Author:	UShi
*/

#include "IRremote.h"
#include "SoftwareSerial.h"

/// <summary>
/// 冷氣 26度
/// </summary>
unsigned int State1[] = { 3450, 3300, 900, 2450, 900, 2400, 900, 800, 900, 2450, 850, 2450, 900, 800, 900, 750, 900, 800, 900, 800, 900, 750, 900, 2450, 900, 800, 900, 750, 900, 2450, 900, 2400, 900, 2450, 900, 750, 950, 750, 900, 2450, 900, 750, 900, 800, 900, 800, 900, 2400, 900, 2450, 900, 2400, 900, 2450, 900, 750, 900, 2450, 900, 2450, 900, 2400, 900, 800, 900, 750, 3450, 3300, 900 };

/// <summary>
/// 冷氣 26度 睡眠
/// </summary>
unsigned int State2[] = { 3350, 3400, 800, 900, 800, 850, 850, 850, 800, 850, 850, 2500, 850, 850, 800, 850, 850, 2500, 800, 2500, 850, 2500, 800, 2500, 850, 2500, 850, 850, 800, 2500, 850, 2500, 800, 900, 800, 2500, 850, 2500, 800, 850, 850, 850, 850, 2500, 800, 850, 850, 2500, 800, 2500, 850, 850, 850, 850, 800, 2500, 850, 2500, 800, 900, 800, 2500, 850, 850, 800, 850, 3400, 3350, 850 };

/// <summary>
/// 冷氣 26度 取消睡眠
/// </summary>
unsigned int State3[] = { 3450, 3300, 900, 800, 900, 800, 850, 800, 900, 800, 900, 800, 850, 800, 900, 800, 900, 2450, 850, 2450, 900, 2450, 850, 2450, 900, 2450, 900, 2400, 950, 2400, 900, 2450, 850, 800, 900, 2450, 900, 2400, 900, 800, 900, 800, 900, 2400, 900, 800, 900, 2400, 900, 2450, 900, 800, 900, 750, 900, 2450, 900, 2400, 900, 800, 900, 2450, 900, 750, 900, 800, 3450, 3300, 900 };

/// <summary>
/// 冷氣 27度 睡眠
/// </summary>
unsigned int State4[] = { 3450, 3300, 950, 750, 900, 800, 900, 750, 900, 800, 900, 2450, 900, 750, 900, 800, 900, 2400, 950, 2400, 900, 2400, 950, 2400, 900, 2450, 900, 750, 900, 2450, 900, 2400, 900, 800, 900, 2450, 900, 2400, 900, 800, 900, 750, 950, 2400, 900, 800, 900, 2400, 900, 2450, 900, 750, 950, 750, 900, 2450, 900, 2400, 900, 800, 900, 2400, 900, 800, 900, 800, 3450, 3300, 900 };

/// <summary>
/// 送風 強
/// </summary>
unsigned int State5[] = { 3400, 3350, 850, 850, 800, 850, 850, 850, 850, 850, 800, 850, 850, 850, 850, 850, 800, 2500, 850, 2500, 800, 2500, 850, 2500, 800, 2500, 850, 2500, 850, 2500, 800, 2500, 850, 850, 800, 2500, 850, 850, 800, 2550, 800, 2500, 850, 850, 800, 850, 900, 2450, 850, 2500, 850, 800, 900, 2450, 850, 800, 900, 800, 900, 2450, 850, 2450, 900, 800, 850, 800, 3450, 3300, 900 };

IRsend irsend;
SoftwareSerial BT(10, 11); //PIN10及PIN11分別為RX及TX腳位
String GetDataPool;
const int Open_Button_Pin  = 6;
const int Timer_Button_Pin = 5;
const int Main_LED_Pin     = 13;
const uint64_t One_Hour    = 3600000;
uint64_t TimerCounter      = 0;
boolean TimerFlag          = false;
boolean Lock               = false;

void setup()
{
    Serial.begin(9600);
    Serial.println("System Start!");
    pinMode(Main_LED_Pin, OUTPUT);
    pinMode(Open_Button_Pin, INPUT);
    pinMode(Timer_Button_Pin, INPUT);
    BT.begin(9600);
    BT.println("BT is ready!");
    Serial.println("BT");
}

void loop()
{
    UseButtonOpen(digitalRead(Open_Button_Pin));
    TimerButton(digitalRead(Timer_Button_Pin));

    if (!Lock)
        Bluetooth();

    if (TimerFlag)
        CalculationTimer();
}

void UseButtonOpen(int readData)
{
    if (readData == HIGH)
    {
        digitalWrite(Main_LED_Pin, HIGH);
        delay(500);
        digitalWrite(Main_LED_Pin, LOW);
        delay(100);
        IRControl('1');
    }
    else
    {
        digitalWrite(Main_LED_Pin, LOW);
    }
}

void TimerButton(int readData)
{
    if (readData == HIGH)
    {
        TimerFlag = true;
        TimerCounter += One_Hour;
        Serial.println("TimerButton On");
    }
}

void CalculationTimer()
{
    TimerCounter -= 1;

    if (TimerCounter <= 0)
    {
        Serial.println("CalculationTimer Off");
        IRControl('1');
        TimerFlag = false;
        TimerCounter = 0;
    }
}

void CMDActivity()
{
    digitalWrite(Main_LED_Pin, HIGH);
    delay(1000);
    digitalWrite(Main_LED_Pin, LOW);
    Lock = false;
}

void IRControl(char inputData)
{
    Lock = true;
    delay(200);
    switch (inputData)
    {
    case '1':
        irsend.sendRaw(State1, 70, 38);
        BT.println("State1");
        break;
    case '2':
        irsend.sendRaw(State2, 70, 38);
        BT.println("State2");
        break;
    case '3':
        irsend.sendRaw(State3, 70, 38);
        BT.println("State3");
        break;
    case '4':
        irsend.sendRaw(State4, 70, 38);
        BT.println("State4");
        break;
    case '5':
        irsend.sendRaw(State5, 70, 38);
        BT.println("State5");
        break;
    default:
        Serial.println(inputData);
        Serial.println("IRControl Default");
        break;
    }
    CMDActivity();
}

void Bluetooth()
{
    if (BT.available() > 0)
    {
        delay(200); // 等資料
        GetDataPool = BT.readString();
        Serial.println(GetDataPool);
        IRControl(GetDataPool.charAt(0));
    }
}