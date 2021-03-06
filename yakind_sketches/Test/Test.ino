#include "Arduino.h"
#include <avr/sleep.h>

#include "src/LightCtrl.h"
//#include "src-gen/LightCtrl_required.h"
#include "src/sc_timer_service.h"

#define BUTTON_PIN 2
#define LED_PIN 6

#define MAX_TIMERS 20 //number of timers our timer service can use
#define CYCLE_PERIOD 10 //number of milliseconds that pass between each statechart cycle

static unsigned long cycle_count = 0L; //number of passed cycles
static unsigned long last_cycle_time = 0L; //timestamp of last cycle

static LightCtrl lightctrl;

static sc_timer_service_t timer_service;

static sc_timer_t timers[MAX_TIMERS];

//! callback implementation for the setting up time events
void lightCtrl_set_timer(LightCtrl* handle, const sc_eventid evid, const sc_integer time_ms, const sc_boolean periodic){
	sc_timer_start(&timer_service, (void*) handle, evid, time_ms, periodic);
}

//! callback implementation for canceling time events.
void lightCtrl_unset_timer(LightCtrl* handle, const sc_eventid evid) {
	sc_timer_cancel(&timer_service, evid);
}

void lightCtrl_setLight(const LightCtrl* handle, const sc_boolean lightOn) {
  
	if(lightOn)
		digitalWrite(LED_PIN, HIGH);
	else
		digitalWrite(LED_PIN, LOW);
}

//The setup function is called once at startup of the sketch
void setup()
{
  Serial.begin(9600);
	pinMode(BUTTON_PIN, INPUT);
	pinMode(LED_PIN, OUTPUT);

	sc_timer_service_init(
			&timer_service,
			timers,
			MAX_TIMERS,
			(sc_raise_time_event_fp) &lightctrl.raiseTimeEvent
			);

	lightctrl; //initialize statechart

	lightctrl.enter(); //enter the statechart
}

// The loop function is called in an endless loop
void loop()
{
	unsigned long current_millies = millis();

	if(digitalRead(BUTTON_PIN))
 {
		&lightctrl.raiseButton;
    
 }

	if ( cycle_count == 0L || (current_millies >= last_cycle_time + CYCLE_PERIOD) ) {

			sc_timer_service_proceed(&timer_service, current_millies - last_cycle_time);

			&lightctrl.runCycle;
//      return an integer to figure out why I'm not getting LED output like I expect

			last_cycle_time = current_millies;
			cycle_count++;
		}
  int temp = digitalRead(BUTTON_PIN);
//  int temp2 = digitalRead(LED_PIN);
  Serial.print("Button is ");
  Serial.println(temp);
//  Serial.println(&lightctrl.button_raised);
//  Serial.print("LED is ");
//  Serial.println(temp2);
}
