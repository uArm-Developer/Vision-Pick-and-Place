//adjust the parameters to make sure the grab position is in the center of object
#define x_rate_of_pixle_to_distance 0.75  //adjust the this parameter when there is offset in x axis
#define y_rate_of_pixle_to_distance 0.5  //adjust the this parameter when there is offset in y axis

int inByte = 0,//serial buf
    num = 0;//buf counter
int x_openmv=0, y_openmv=0;
int x_uarm=0, y_uarm=0;
unsigned long times;
char buf[20],
     flag=0;
unsigned char get_openmv_data();
void pick_and_palce();
void setup() {
  Serial.begin(115200);//usb or xbee
  Serial1.begin(115200);//openmv
  Serial2.begin(115200);//uarm
  
  Serial.write("START!\n");
  Serial2.write("M2400 S0\n");//set the mode of uarm

  pinMode(A3,OUTPUT);//orange led
  digitalWrite(A3,LOW);
  times = millis();
}

void loop() {
  if(flag == 0)
  {
    digitalWrite(A3,HIGH);
    Serial2.write("G0 X200 Y0 Z160 F10000\n");
    delay(5000);//wait for the uarm to finish the moving then start the vision tracking
    flag = 1;//vision start
    Serial1.write('S');//send vision start command
    Serial.write("status 0\n");//send vision start command
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
    x_openmv = 320 - x_openmv;
    y_openmv = 240 - y_openmv;
    x_uarm = 200 + (y_openmv - 20) * x_rate_of_pixle_to_distance; //0.6;//convert the x coordinates for uarm
    y_uarm = (x_openmv - 125) * y_rate_of_pixle_to_distance; //0.7;//convert the y coordinates for uarm
    Serial2.write("G0 X");
    Serial2.print(x_uarm,DEC);
    Serial2.write(" Y");
    Serial2.print(y_uarm,DEC);
    Serial2.write(" F10000\n");
Serial.write("x_uarm:");
Serial.println(x_uarm,DEC);
Serial.write("y_uarm:");
Serial.println(y_uarm,DEC);
    pick_and_palce();
    //while(1);
    
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
      Serial.write("status 2\n");
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
      flag = 0;//vision end
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
  Serial2.write("G0 Z29 F10000\n");
  Serial2.write("M2231 V1\n");
  Serial2.write("G0 Z100 F10000\n");
  delay(500);
  Serial2.write("G0 X150 Y-150 Z100 F10000\n");
  Serial2.write("G0 Z32 F10000\n");
  Serial2.write("M2231 V0\n");
  delay(8000);
}

