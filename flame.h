//#include "Definitions.h"

void stepperSetup() {

	pinMode(stepper_dir,OUTPUT);
  	pinMode(stepper_step,OUTPUT);
  	stepper.setMaxSpeed(1000);
  	stepper.setAcceleration(1000);
  	stepper.moveTo(16);
}

void GoStepper() {
	if(stepper.distanceToGo() == 0){
			stepper.move(16);
	}

}

void checkFlame() {
	if(millis() - lastfc ){
		int r = analogRead(r_flame_sensor_pin);
		int l = analogRead(l_flame_sensor_pin);
		
		if(r < flameVal || l < flameVal)
			flameDetected = true;
	}
}

