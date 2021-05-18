#include <stdlib.h>
#include "Energia.h"
#include "Encoder.h"

/* Variables used in the ISR should be volatile */
volatile int32_t enc_left_cnt = 0;
volatile int32_t enc_right_cnt = 0;
volatile uint8_t enc_left_wheel_dir = 0;
volatile uint8_t enc_right_wheel_dir = 0;

/* Pins used by the encoder */
uint8_t _era_pin;
uint8_t _erb_pin;
uint8_t _ela_pin;
uint8_t _elb_pin;


void triggerLeftEncoder() {
	/* The wheel direction can be determined by the second encoder pin */
	enc_left_wheel_dir = digitalRead(_ela_pin);
	//because return of 0 means wheels are going backwards
	if (getLeftWheelDir()) enc_left_cnt--;
	else enc_left_cnt++;
	//enc_left_cnt++;
	//enc_left_wheel_dir = digitalRead(_ela_pin);
	//Serial.println(getLeftWheelDir());
}

void triggerRightEncoder() {
	/* The wheel direction can be determined by the second encoder pin */
	enc_right_wheel_dir = digitalRead(_era_pin);
	if (getRightWheelDir()) enc_right_cnt--;
	else enc_right_cnt++;
	//enc_right_cnt++;
	//enc_right_wheel_dir = digitalRead(_era_pin);
	//Serial.println(getRightWheelDir());
}

void setupEncoder(uint8_t ela_pin, uint8_t elb_pin, uint8_t era_pin, uint8_t erb_pin) {
	_era_pin = era_pin;
	_erb_pin = erb_pin;
	_ela_pin = ela_pin;
	_elb_pin = elb_pin;

	/* Encoder drives pins high during pulse so by default they should be pulled low */
	pinMode(_ela_pin, INPUT_PULLDOWN );
	pinMode(_elb_pin, INPUT_PULLDOWN );
	pinMode(_era_pin, INPUT_PULLDOWN );
	pinMode(_erb_pin, INPUT_PULLDOWN );

	/* Only count the rising edge of the encoder */
	attachInterrupt(digitalPinToInterrupt(erb_pin),triggerRightEncoder,RISING);
	attachInterrupt(digitalPinToInterrupt(elb_pin),triggerLeftEncoder,RISING);
}

int32_t getEncoderRightCnt() {
	return enc_right_cnt;
}

int32_t getEncoderLeftCnt() {
	return enc_left_cnt;
}

void resetLeftEncoderCnt() {
	enc_left_cnt = 0;
}

void resetRightEncoderCnt() {
	enc_right_cnt = 0;
}

uint8_t getLeftWheelDir() {
	return enc_left_wheel_dir;
}
uint8_t getRightWheelDir() {
	return enc_right_wheel_dir;
}