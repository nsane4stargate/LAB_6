#pragma config(Sensor, S3,     colorSensor,    sensorEV3_Color)
#pragma config(Motor,  motorB,          LEFT,          tmotorEV3_Large, PIDControl, driveLeft, encoder)
#pragma config(Motor,  motorC,          RIGHT,         tmotorEV3_Large, PIDControl, driveRight, encoder)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//


int currentPower = 20, thresHold = 42, colorReflectiveValue, error, slewRate = 25;
float k = 2, nTurnRatio = 0,desiredNTurnRatio;


void slewNTurnRatioControl(){

		repeatUntil(nTurnRatio == desiredNTurnRatio){

			error = (float)(colorReflectiveValue - thresHold);

			if(desiredNTurnRatio < 0){
					 k = 2;
					 desiredNTurnRatio = -(k * error);
				}
				else{
						k = 1.85;
					 desiredNTurnRatio = -(k * error);
			}
				if(nTurnRatio < desiredNTurnRatio){
					if(nTurnRatio  + slewRate < desiredNTurnRatio)
							nTurnRatio  += slewRate;
					else
							nTurnRatio  = desiredNTurnRatio;
				}
				else if(nTurnRatio > desiredNTurnRatio){
					if(nTurnRatio - slewRate > desiredNTurnRatio)
						nTurnRatio  -= slewRate;
					else
					 nTurnRatio = desiredNTurnRatio;
			}
	}// END OF repeatUntil()
}// END OF slewRatecontrol()





task readColorReflective(){
	while(true){
		colorReflectiveValue = getColorReflected(colorSensor);
		displayBigTextLine(5,"CR: %i",colorReflectiveValue);
	}// END OF WHILE LOOP
}// END OF readColorReflective()

task controllerTask(){
	while(true){

		error = (float)(colorReflectiveValue - thresHold);

		if(colorReflectiveValue < thresHold || colorReflectiveValue > thresHold ){
				desiredNTurnRatio = -(k * error);
				slewNTurnRatioControl();
		}
	}// END OF WHILE LOOP
}// END OF readColorReflective()



task main()
{
	startTask(readColorReflective);
	startTask(controllerTask);
	while(true){
		setMotorSync(RIGHT,LEFT,nTurnRatio,currentPower);
	}
}
