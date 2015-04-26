#include <MiniMotorControllerDefinitions.h>
#include <MotorControllerMaster.h>


#include <AccelStepper.h>
#include <NewPing.h>
#include <Wire.h>
#include <math.h>
#include <DistanceGP2Y0A21YK.h>
#include <DistanceGP2Y0A21YK_LUTs.h>
#include <LiquidCrystal.h>


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
	
	c.setAcceleration(1000,1000,1000,1000);
	c.brake();
	//c.goVelocity(100, 0);
	//delay(200);
	lcd.begin(16,2);
		driveState = goStraight;

	
}

void loop()
{	
	getCoordinate();
	sendHb();
	checkCliff();
	pingSonar();
	//checkFlame();
	getReferencePosition();
	getCurrentPosition();
	Go();
	checkSideWall();
	 lcd.setCursor(0,1);
	 lcd.print(x);
	 lcd.print(" ,");
	 lcd.print(y);
	 lcd.print(" ,");
	 lcd.println(r);
	 // lcd.setCursor(0,1);
	 //  lcd.println(analogRead(light_sensor_pin) > lightSensorVal);
	//Serial.println(sideWallAngle);
	Serial.println( sideWallDistance - sideLimit );
	//c.goVelocity(-100,0);

	if(facingCliff || nearFrontWall)
	{
		//turn left 90 degrees
		//Serial.println("set to turn state");
        if(stop_move){
        	driveState = brake;
			if(c.isStandby())
			{
				stop_move = false;
			}
		}

		else if(backUp)
		{	
			lcd.setCursor(0,0);
			lcd.println("backUp");
			if(reference_l - l < 50)//5cm
				driveState = backup;	
			else
			{ 
				backUp = false;
				stop_move = true;
				//getReferencePos = true;
			}
		}
		else
		    driveState = turnLeft_90;


      }
              
		//flag set back to false in Go method!
	

	if( rightIsOpen && !facingCliff && !atCliff && !nearFrontWall)
	{
		//turn to open area
		//flag set back to false in Go method!
		if(stop_move){
				driveState = brake;			
				if(c.isStandby())
				{
					stop_move = false;
					driveState = turnToOpenArea;
				}
		}
	}
	
	
	if(!facingCliff && !atCliff && !nearFrontWall && !rightIsOpen && driveState != alignWall){
		if(abs(sideWallDistance - sideLimit) > 1.5 )
		{
			if(driveState != followWall) getReferencePos = true;
		//Serial.println("follow wall");

			driveState = followWall;
		}
		// else if (sideWallDistance - sideLimit > 30)
		// 	driveState = turnRight_90;
		
		//else driveState = goStraight;
	}
}

void pingSonar()
{
	if (millis() - lastPing > 100)
	{
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
			if(facingCliff == false) 
			{
				stop_move = true;
				getReferencePos = true;
				backUp = true;
			}
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
	    lcd.setCursor(0,0);
			lcd.println("goStraight");
		    c.goVelocity(100, 0);
	    	break;
	    
	    case turnLeft_90:
	    	//Serial.println("im turning");
	    	lcd.setCursor(0,0);
			lcd.println("turnLeft");
	    	c.goVelocity(0, 20);          
        	getCurrentPosition();
			if (r - reference_r  > 80)
			{
			//Serial.println("complete turn");
				 c.goVelocity(0,0);
                 if(c.isStandby())
                 {
                 	driveState = goStraight;

                 	if(facingCliff) atCliff =true;
					nearFrontWall = false;
					facingCliff = false;
                 }
				
			}	
	      	break;
	    
	    case turnRight_90:
	    lcd.setCursor(0,0);
			lcd.println("turnRight");
	    	c.goVelocity(0, -20);          
			if (reference_r - r > 80)
			{
				c.goVelocity(0,0);
                 if(c.isStandby())
                 {
                 	driveState = goStraight;
					rightIsOpen = false;
                 }
			}
			break;

		case turnToOpenArea:
		lcd.setCursor(0,0);
			lcd.println("turnToOpenArea");
			c.goVelocity(100, 0);
			if(l - reference_l > 50) //5cm
			{
				getReferencePos = true;
				driveState = turnRight_90;
			}

			break;
		
		case brake:
			c.goVelocity(0,0);
			break;
		
		case followWall:
			lcd.setCursor(0,0);
			lcd.println("followWall");
			if(abs(reference_r - r) > 5)
			{
				driveState = alignWall;

			}
			else 
			{
				if(sideWallDistance - sideLimit < -1.5)
			 		c.goVelocity(90, map(sideWallDistance - sideLimit, -10,-1, 20,10));
			 
				else if(sideWallDistance - sideLimit > 1.5)
			 		c.goVelocity(90, map(sideWallDistance - sideLimit, 10,1, -20, -10));
			}
			 	


			break;

		case alignWall:
			lcd.setCursor(0,0);
			lcd.println("alignWall");
			if(frontDist - rearDist <= -1 )
			 	c.goVelocity(90, map(frontDist - rearDist,-8, -1, 20,10));
			 
			else if(frontDist - rearDist >= 1 )
			 	c.goVelocity(90, map(frontDist - rearDist, 8, 1, -20, -10));
			else
			driveState = goStraight;
			 
			break;
		case backup:
			c.goVelocity(-100,0);
			break;


	    default:
	    	c.brake();
	}
}
	
