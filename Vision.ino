//After finishing the wiring, press the D5 button to run the code

int inByte = 0,//serial buf
    num = 0;//buf counter
int x_openmv=0, y_openmv=0;
int x_uarm=0, y_uarm=0;
unsigned long times;
char buf[20],
     flag=0;
char color_sel=1;// 0:yellow   1:red   2:green
     
unsigned char get_openmv_data();
void pick_and_palce();

void wait_for_finish_moving()
{
  inByte=0;//clear the buffer
  while(inByte!='@'){
     if (Serial2.available() > 0) {
        inByte = Serial2.read();
     }
  }
}

void setup() {
  pinMode(5,INPUT);//button
  pinMode(A3,OUTPUT);//orange led
  digitalWrite(A3,LOW);
  Serial.begin(115200);//usb or xbee
  Serial1.begin(115200);//openmv
  Serial2.begin(115200);//uarm
  
  Serial2.write("G0 X200 Y0 Z160 F10000\n");
//if button is pressed, then start the program
  while(digitalRead(5)==HIGH);
  digitalWrite(A3,HIGH);
  
  Serial.write("V2 START!\n");
  Serial2.write("M2400 S0\n");//set the mode of uarm
  delay(4000);
  Serial2.write("M2400 S0\n");//set the mode of uarm
  Serial2.write("M2122 V1\n");//report when finish the movemnet


  times = millis();
}

void loop() {
  if(flag == 0)
  {
    digitalWrite(A3,HIGH);
    Serial2.write("G0 X200 Y0 Z159 F10000\n");// in order to trig the report of finish movement '@'
    wait_for_finish_moving();
    //Serial2.write("G2202 N0 V90\n");
    //wait_for_finish_moving();
    Serial2.write("G0 X200 Y0 Z160 F10000\n");

    delay(100);//wait for the uarm to finish the moving then start the vision tracking
    wait_for_finish_moving();
    
    flag = 1;//vision start
    switch(color_sel){
      case 0: Serial1.write('y');break;
      case 1: Serial1.write('r');break;
      case 2: Serial1.write('g');break;
      default: break;
    }
    Serial1.write('S');//send vision start command
    Serial.write("vision start for finding the cube\n");//send vision start command
    times = millis();
    
  }
  digitalWrite(A3,LOW);
  //get commands from pc
  if (Serial.available() > 0) 
  {
    inByte = Serial.read();

    Serial2.write(inByte);
  }
  
  //get object coordinates from openmv
  if(get_openmv_data()==1)
  {
    flag = 0;//vision end
    Serial.write("move\n");//confirm the openmv data

//new algorithm
    x_uarm = y_openmv*(-0.7035)-3.635 + 88 + 70 + 200;
    y_uarm = x_openmv*(-0.7488)+12.391 + 107.5 + 15 +0;
    
    String commands="G0 X"; 
    commands.concat(x_uarm);
    commands+=" Y";
    commands.concat(y_uarm);
    commands+=" Z100 F10000\n";
    Serial2.print(commands);
    
    Serial.print(commands);

    pick_and_palce();    
  }
}

//get object coordinates from openmv
unsigned char get_openmv_data()
{
  if (Serial1.available() > 0) 
  {
    inByte = Serial1.read();
    buf[num++] = inByte;
    Serial.write(inByte);
    if((inByte=='\n')&&(buf[0]=='x'))
    {
      Serial.write("get openmv data\n");
      int counters=1;//jump the letter x
      x_openmv=0;
      do{
        x_openmv = x_openmv*10;
        x_openmv += buf[counters++] - 48;
      }while((buf[counters]>=0x30)&&(buf[counters]<=0x39));
      
      y_openmv=0;
      counters++;//jump the letter y
      do{
        y_openmv = y_openmv*10;
        y_openmv += buf[counters++] - 48;
      }while(counters+1<num);

      num = 0;
      return 1;
    }
    //Serial.println(x_openmv,DEC);
    //Serial.println(y_openmv,DEC);

  }
  if((millis()-times>10000)&&(flag==1))//if no object detected, reset the flag every 10s
  {
    //clear the uart buffers
    while(Serial1.available() > 0)
    {
      inByte = Serial1.read();
    }
    //reset the count of uart
    num = 0;
     times = millis();
     flag = 0;
     Serial.write("status 1\n");//NO OBJECT IN CAMERA
  }
  return 0;
}
//move the detected object to the fixed position
void pick_and_palce()
{
  Serial2.write("G0 Z23 F10000\n");
  Serial2.write("M2231 V1\n");
  Serial2.write("G0 Z120 F10000\n");
  delay(500);
  Serial2.write("G2202 N0 V15\n");
  Serial2.write("G0 Z50 F10000\n");
  Serial2.write("M2231 V0\n");
  Serial2.write("G0 Z80 F10000\n");
  Serial2.write("G2202 N0 V90\n");
  delay(8000);
  //change the color of tracking
  //color_sel++;
  //color_sel = color_sel%3;
}

