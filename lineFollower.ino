int IN1=11;
int IN2=9;
int IN3=5;//left motor
int IN4=3;
char str[24]={'R','R','L','R','L','L','R','L','R','R','F','R','R','L','F','L','R','L','L','F','L','R','F','F'};
int ndx=0;
//Motor Speed
int basicSpeed1 = 150;
int basicSpeed2 = 160;
int turnSpeed =60;
int forwardDelay=180;
int forwardSpeed =100;
//80,150,120
//PID Calibrations
float Kp=0.15f;// ideal Kp=0.25
float Ki=0.0f;
float Kd=0.35f;// ideal Kd=0.1

int baseValue = 1400;

//PID map value// decreasing increses speed
int pidMapVal = 300;
int lastErrorL = 0;
int lastErrorR = 0;
bool sfollower=false;
int white=500;
int black=700;
int t = 0;
int pidThreshold = 300;

//Initial Input values
int in1 = analogRead(A0);
int in2 = analogRead(A1);
int in3 = analogRead(A3);
int in4 = analogRead(A2);
int in5 = analogRead(A4);


void setup() {
  // put your setup code here, to run once:
  pinMode(IN1,OUTPUT);
  pinMode(IN2,OUTPUT);
  pinMode(IN3,OUTPUT);
  pinMode(IN4,OUTPUT);
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  if(sfollower)
  {t+=1;
  readInputs();
  
  if(in1<white||in5<white)
  {readInputs();
    if(in1<white||in5<white)
   { stop1(200);
    char ch=str[ndx++];
    if(ch=='R')
    {
      detectForward();
      takeRight();
    }
    else if(ch=='L')
    {
      detectForward();
      takeLeft();
    }
    else if(ch=='F')
    {
      detectForward();
    }
   }
  }
   else
   followLine();
  }
  else{
  t+=1;
  readInputs();
  //checkIRs();
  if (in1>black&&in2>black&&in3>black&&in4>black&&in5>black){
      //stop1(5000);
      takeLeft();
   }else if ((in1>black&&in5>black)&&(in2<white||in3<white||in4<white)){
      followLine();
    }else if(in1<white || in5<white){
      readInputs();
      if (in1<white || in5<white){
        stop1(200);
      //Serial.print(t);
        if (in1<white&&in5>black){
            //right turn
            detectForward();
            readInputs();
            if (in3<white&&in1<white&&in2<white&&in5>black){//winning condition
               sfollower=true;
               readInputs();
              detectForward();
              if (in5>black){
              takeLeft();}
              delay(10000);
              }else{
              takeRight();
              }
       }else if(in5<white&&in4<white&&in1>black){//left turn
            //stop1(5000);
            detectForward();
            readInputs();
            if(in2<white||in3<white||in4<white){
                 followLine();
              }else{ 
                takeLeft();
              }
       }else if((in2<white&&in3<white&&in4<white)){//T condition
            detectForward();
            takeRight();
       }
    }}
  
   }
}

void takeLeft(){
    readInputs();
    while(in5>black){
          //Serial.print("helloi");
            analogWrite(IN1,turnSpeed);
            analogWrite(IN2,0);
            analogWrite(IN3,0);
            analogWrite(IN4,turnSpeed);
            readInputs();
            //checkIRs();
     }
     readInputs();
   
     while(in4>black){
          //Serial.print("helloi");
            analogWrite(IN1,turnSpeed);
            analogWrite(IN2,0);
            analogWrite(IN3,0);
            analogWrite(IN4,turnSpeed);
            readInputs();
            //checkIRs();
     }
     stop1(200);
  
}


void takeRight(){
    readInputs();
    if (in5<white){
      while(in1<white){
            analogWrite(IN1,0);
            analogWrite(IN2,turnSpeed);
            analogWrite(IN3,turnSpeed);
            analogWrite(IN4,0);
            readInputs();
       }  
    }
    while(in1>black){
          //Serial.print("helloi");
            analogWrite(IN1,0);
            analogWrite(IN2,turnSpeed);
            analogWrite(IN3,turnSpeed);
            analogWrite(IN4,0);
            readInputs();
            //checkIRs();
     }
     
     while(in2>black){
          //Serial.print("helloi");
            analogWrite(IN1,0);
            analogWrite(IN2,turnSpeed);
            analogWrite(IN3,turnSpeed);
            analogWrite(IN4,0);
            readInputs();
            //checkIRs();
     }
     stop1(200);
}


void stop1(int t){
    analogWrite(IN1,0);
    analogWrite(IN2,0);
    analogWrite(IN3,0);
    analogWrite(IN4,0);
    delay(t);
}

void readInputs(){
  in1 = analogRead(A0);
  in2 = analogRead(A1);
  in3 = analogRead(A3);
  in4 = analogRead(A2);
  in5 = analogRead(A4);

  
  // print out the value you read:

in1=map(in1,350,924,0,900);
  in2=map(in2,725,924,0,900);
  in3=map(in3,350,924,0,900);
  in4=map(in4,500,924,0,900);
  in5=map(in5,600,924,0,900);
  
  if (in1<0){in1=0;};
  if (in2<0){in2=0;};
  if (in3<0){in3=0;};
  if (in4<0){in4=0;};
  if (in5<0){in5=0;}
  delay(20);
}

void detectForward(){
    //Serial.print("detect forward");
    analogWrite(IN1,forwardSpeed);
    analogWrite(IN3,forwardSpeed);
    delay(forwardDelay);
    analogWrite(IN1,0);
    analogWrite(IN3,0);
    readInputs();
}

void checkIRs(){
  Serial.print(in1);
  Serial.print("     ");
  Serial.print(in2);
  Serial.print("     ");
  Serial.print(in3);
  Serial.print("     ");
  Serial.print(in4);
  Serial.print("     ");
  Serial.println(in5);
}

void followLine(){
  readInputs();
  int diff = in2>in4 ? in2-in4 : in4-in2;
  if (diff<pidThreshold){
      //Serial.println("forward");
      forward();
  }else{
    int mean=2*in2+3*in3+2.2*in4;  
    int d = mean-0;
    if (d<0){d=0;}

    d= map(d,0,4000,0,500);
    
    if (in4>in2){
      
         turnLeft(d);
    }else{
       turnRight(d); 
    }
  }
 //Serial.println(",");
 }

void forward(){
    //Serial.println("forward");
    analogWrite(IN1,basicSpeed1);
    analogWrite(IN3,basicSpeed2);
}


void turnLeft(int diff){

    //Serial.println("left");
    float P = diff;
    float I = I + diff;
    float D = diff - lastErrorL;
    lastErrorL = diff;
    float speedChange=(float)((P*Kp)+(I*Ki)+(D*Kd));
    int change = map(speedChange,0,pidMapVal,0,75);
    //Serial.print("change-left");
    //Serial.print(change);
    //Serial.print(',');
    analogWrite(IN1,(basicSpeed1-change));
    if(basicSpeed2+change<250){
      analogWrite(IN3,basicSpeed2+change);
    }else{
      analogWrite(IN3,(250));
    }
    //string++'R';
  }

void turnRight(int diff){
    //Serial.println("Right");
    float P = diff;
    float I = I + diff;
    float D = diff - lastErrorR;
    lastErrorR = diff;
    float speedChange=(float)((P*Kp+0.2)+(I*Ki)+(D*Kd));
    int change = map(speedChange,0,pidMapVal,0,75);
    //Serial.print("change-right");
    //Serial.print(change);
    //Serial.print(',');
    analogWrite(IN3,(basicSpeed2-change));
    if(basicSpeed1+change<250){
      analogWrite(IN1,basicSpeed1+change);
    }else{
      analogWrite(IN1,(250));
    }
  }