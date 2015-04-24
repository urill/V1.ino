#include <MiniMotorControllerDefinitions.h>
#include <MotorControllerMaster.h>


#include <AccelStepper.h>
#include <NewPing.h>
//#include "MotorControllerMaster.h"
#include <Wire.h>
#include <math.h>
#include <DistanceGP2Y0A21YK.h>
#include <DistanceGP2Y0A21YK_LUTs.h>

#include "Definitions.h"
#include "flame.h"
#include "rangeFinder.h"
void setup()
{	
	Wire.begin();
	c.begin();

	frontIR.begin(front_ir_pin);
	rearIR.begin(rear_ir_pin);

	Serial.begin(9600);
	delay(1000);
	c.setAcceleration(400,600,400,600);
	c.brake();
	c.goVelocity(100, 0);
	delay(200);
	driveState = goStraight;
}

void loop()
{	
	
	sendHb();
	checkCliff();
	pingSonar();
	//checkFlame();
	getReferencePosition();
	Go();
	checkSideWall();
	 //Serial.print(frontDist);
	 //Serial.print(" ,");
	 //Serial.println(rearDist);
	//Serial.println(sideWallAngle);
	Serial.println( sideWallDistance - threshHold);


	if(facingCliff || nearFrontWall)
	{
		//turn left 90 degrees
		//Serial.println("set to turn state");
      if(stop_move){
            c.goVelocity(0,0);
			if(c.isStandby()){
				//Serial.println("standby");
				stop_move = false;
                driveState = turnLeft_90;
			}               
      }
              
		//flag set back to false in Go method!
	}
	if(rightIsOpen && !atCliff && !nearFrontWall)
	{
		//turn to open area
		//Serial.println("wrong");
		//flag set back to false in Go method!
		if(stop_move){
            c.goVelocity(0,0);
			if(c.isStandby()){
				//Serial.println("standby");
				stop_move = false;
				driveState = turnToOpenArea;
			}
		}
	}
	
	
	//if(!nearFrontWall && !rightIsOpen){
	//	Serial.println("follow wall");

	if(sideWallDistance - threshHold < -2)
	{
		Serial.println("turnl");
		// turn left
		c.goVelocity(100, 10);
        //c.goVelocity(100, map(sideWallDistance - threshHold, 0,200, 0,10));
	}
	
	else if (sideWallDistance - threshHold > 2)
	{	
		//turn right
		c.goVelocity(100, -10);
        //c.goVelocity(100,map(- sideWallDistance + threshHold, 0,200, -10 , 0));
	}
	else
	{
		//go straight
		driveState = goStraight;
	}
	//}

 // c.goVelocity(100,map(sideWallDistance - threshHold, -500,500, -50,50));

}

void pingSonar()
{
	if (millis() - lastPing > 100)
	{
		//frontWallDistance = sonar.convert_cm(echoTime);
		lastPing = millis();
		sonar.ping_timer(echoCheck);
	}		
}

void echoCheck()
{	
	if(sonar.check_timer())
	{
		frontWallDistance = sonar.ping_result / US_ROUNDTRIP_CM;
		
		if(frontWallDistance < SAFE_DISTANCE )//and if flame is not close by!! 
		{	
			if(nearFrontWall == false)
			{	
				stop_move = true;
				getReferencePos = true;
			}
			nearFrontWall = true;
		}	
	}	
}

//!!!!!!!!!!not complete
void checkCliff()
{
	if (millis() - lastcc > 20)
	{
		lastcc = millis();
		if(analogRead(light_sensor_pin) > lightSensorVal)
		{
			facingCliff = true;
			atCliff = true;
		}
	}
}

void sendHb()
{
	if (millis() - lasthb > 50){
				//Serial.println("inhb");
		c.heartbeat();
		lasthb = millis();	
	}
}

void getReferencePosition()
{
	if(getReferencePos)
	{
		//Serial.println("getref");
		c.getEncoder(&l, &r);
		reference_l = l;
		reference_r = r;
		getReferencePos = false;
	} 
}


void getCurrentPosition()
{
	//Serial.println("getcur");
	if(millis() - lastEncoderSample > 20 ) 
    {
		c.getEncoder(&l, &r);	
		lastEncoderSample = millis();	
	}
}

void Go()
{
	switch (driveState) {
	    case goStraight:
	    //Serial.println("straight");
		    c.goVelocity(100, 0);
	    	break;
	    
	    case turnLeft_90:
	    	//Serial.println("im turning");

	    	c.goVelocity(0, 20);          
        	getCurrentPosition();
			if (r - reference_r  > 80)
			{
			//Serial.println("complete turn");
				 c.goVelocity(0,0);
                 if(c.isStandby())
                 {
                 	driveState = goStraight;
					nearFrontWall = false;
					facingCliff = false;
                 }
				
			}	
	      	break;
	    
	    case turnRight_90:
	    	c.goVelocity(0, -90);          
        	getCurrentPosition();
			if (reference_r - r > 80)
			{
				driveState = goStraight;
				rightIsOpen = false;
			}
			break;

		case turnToOpenArea:
			c.goVelocity(100, 0);
			getCurrentPosition();
			if(l - reference_l > 100) //20cm
			{
				getReferencePos = true;
				driveState = turnRight_90;
			}

			break;
		
		case brake:
			c.brake();
			break;

	    default:
	    	c.brake();
	}
}
	
