#ifndef Definitions_h
#define Definitions_h

/*
// ultrasonics
*/
#define SAFE_DISTANCE 30

const int trigger_pin = 22;
const int echo_pin =  23;

unsigned int frontWallDistance;

NewPing sonar(trigger_pin, echo_pin);


/*
//IRs
*/
const int front_ir_pin = A3;
const int rear_ir_pin = A4;

volatile  int frontDist;
volatile  int rearDist;
volatile int sideWallDistance;

const int frontIRXpos = 3;
const int rearIRXpos = 3;
const int frontIRYpos = 5.5;
const int rearIRYpos  = -5.5;

double sideWallAngle;

const int OpenIrValue = 40;
const int threshHold = 12;

DistanceGP2Y0A21YK frontIR;
DistanceGP2Y0A21YK rearIR;

int y1,y2;
/*
//light sensors
*/
const int light_sensor_pin = A0;
const int lightSensorVal = 550;
/*
//stepper
*/
const int stepper_step = 2;
const int stepper_dir = 3;

AccelStepper stepper(AccelStepper::DRIVER ,stepper_step, stepper_dir );

/*
//flame Sensor
*/
const int l_flame_sensor_pin = A5;
const int r_flame_sensor_pin = A6;
const int flameVal = 500;



/*
//encoders
*/
// const int l_encoder_pin1 = ;
// const int l_encoder_pin2 = ;
// const int r_encoder_pin1 = ;
// const int r_encoder_pin2 = ;
//long encoder_l, encoder_r = 0; 					//encoder ticks
long l, r, reference_l, reference_r = 0;		//angle turned in degrees, distance traveled in mm

//Encoder encl(l_encoder_pin1, l_encoder_pin2);
//Encoder encr(r_encoder_pin1, r_encoder_pin2);
 
/*
//time check
*/
int lasthb = 0;
int lastRightTurn = 0;
int lastEncoderSample = 0;
int lastPing = 0;
int lastir = 0;
int lastcc = 0;
int lastfc = 0;




//int irMap[2][11] ={{5,6,7,8,9,10,11,12,13,14,15},{399,340,287,260,240,212,189,175,165,153,146}};


/*
//state machine
*/
enum dirveStates{goStraight, turnLeft_90, turnRight_90, turnToOpenArea, brake} driveState;

bool facingCliff, nearFrontWall, rightIsOpen, atCliff, getReferencePos , stop_move, flameDetected = false;



/*
//motor controller
*/
// RegulatedMotor m1(&encoder1,7,6,10);
// RegulatedMotor m2(&encoder2,8,11,9);
MotorControllerMaster c;
// KinematicController kc(&m1,&m2,1,-1,225,75,64*50);
#endif