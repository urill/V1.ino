// void getSideAngle() 
// {
// 	sideWallAngle = atan((frontDist - rearDist + frontIRXpos -rearIRXpos)/(rearDist*sin(0)- frontDist*sin(0) + rearIRYpos - frontIRYpos));
// }

void getSideDistance()

	{
		//getSideAngle();
	// 	 y1 = frontDist*cos(-sideWallAngle) + frontIRXpos*cos(sideWallAngle) + frontIRYpos*sin(sideWallAngle);
	// 	 y2 = rearDist*cos(-sideWallAngle) + rearIRXpos*cos(sideWallAngle) + rearIRYpos*sin(sideWallAngle);
		
	// 	sideWallDistance = (y1 + y2 )/2;
		if(frontDist != rearDist)
		{
			sideWallAngle = atan (11 / abs(frontDist - rearDist));
			sideWallDistance = (sin(sideWallAngle) * frontDist + sin(sideWallAngle) * rearDist) / 2; 
		}
		else
			sideWallDistance = frontDist;
	}




void checkSideWall() {
	
	if(millis() - lastir > 20)
	{
		//Serial.println("readIR");
		lastir = millis();
			
		frontDist = frontIR.getDistanceCentimeter();
		rearDist = rearIR.getDistanceCentimeter();
		getSideDistance();
	
		if(frontDist > OpenIrValue && rearDist > OpenIrValue)
		{
			if(!rightIsOpen)
			{
				getReferencePos = true;
				stop_move = true;
			}
			rightIsOpen = true;
		}
		else
			atCliff = false;

	}

}