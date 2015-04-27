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
	
		//flame.scan();
		if(low  < flameVal){
			if(!flameDetected) stop_move = true;
			flameDetected = true;
	}
}

