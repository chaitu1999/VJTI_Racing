volatile byte REV;       //  VOLATILE DATA TYPE TO STORE REVOLUTIONS
 
unsigned long int rpm, maxRPM;  //  DEFINE RPM AND MAXIMUM RPM
 
unsigned long time;         //  DEFINE TIME TAKEN TO COVER ONE REVOLUTION
 
int RPMlen , prevRPM;  //  INTEGERS TO STORE LED VALUE AND CURRENT RPM AND PREVIOUS RPM
 
int flag = 0;             //  A VARIABLE TO DETERMINE WHETHER THE LCD NEEDS TO BE CLEARED OR NOT

long prevtime = 0;       //  STORE IDLE TIME TO TOGGLE MENU
    
 void setup()
 {
     Serial.begin(9600);   // GET VALUES USING SERIAL MONITOR
     
     attachInterrupt(digitalPinToInterrupt(2), RPMCount, RISING);     //  ADD A HIGH PRIORITY ACTION ( AN INTERRUPT)  WHEN THE SENSOR GOES FROM LOW TO HIGH
     
     REV = 0;      //  START ALL THE VARIABLES FROM 0
     
     rpm = 0;
     
     time = 0;
     Serial.println("TACHOMETER");           //   STARTUP TEXT
     delay(2000);
     
 }
 
 void loop()
 {
  long currtime = micros();                 // GET CURRENT TIME
  
  long idletime = currtime - prevtime;        //  CALCULATE IDLE TIME
    
    if(REV >= 5 )                  //  IT WILL UPDATE AFETR EVERY 5 READINGS
   {
     
             
     if(flag==0)                     //  CLEAR THE LCD TO AVOID ANY GARBAGE TEXT
     {
       Serial.println("SENSOR MEASURING");
       flag=1;                          //   AFTER FLAG = 1 , THE LOOP WILL NOT EXECUTE AGAIN
     }
     
     rpm = 30*1000000/(micros() - time)*REV;       //  CALCULATE  RPM USING REVOLUTIONS AND ELAPSED TIME
     
     if(rpm > maxRPM)
     maxRPM = rpm;                             //  GET THE MAX RPM THROUGHOUT THE RUN
    
     time = micros();                            
     
     REV = 0;
     
     /*int x= rpm;                                //  CALCULATE NUMBER OF DIGITS IN RPM
     while(x!=0)
     {
       x = x/10;
       RPMlen++;
     }       
          
     
     
     if(RPMlen!=prevRPM)                             // IF THE RPM FALLS TO A LOWER NUMBER WITH LESS DIGITS , THE LCD WILL GET CLEARED
     {
       lcd.clear();
       prevRPM = RPMlen;
       flag=0;
       lcd.print("SENSOR MEASURING");
     }*/
     
     Serial.print(rpm);
     Serial.println("RPM");
     delay(500);
     
     prevtime = currtime;                        // RESET IDLETIME
    
   }
   
   if(idletime > 5000 )                      //  IF THERE ARE NO READING FOR 5 SEC , THE SCREEN WILL SHOW MAX RPM
   {
     
     if(flag==1)                            // CLEAR THE LCD
     {
       flag=0;
     }
     
     Serial.println("MAXIMUM RPM");
     Serial.print(maxRPM);                     // DISPLAY MAX RPM
     Serial.println("   RPM");
     delay(2000);
     Serial.println("IDLE STATE");
     //Serial.println("READY TO MEASURE");
     delay(2000);
     prevtime = currtime;
   }
     
 }
 
 void RPMCount()                                // EVERYTIME WHEN THE SENSOR GOES FROM LOW TO HIGH , THIS FUNCTION WILL BE INVOKED 
 {
   REV++;
 }
//////////////////////////////////////////////////////////////  END OF THE PROGRAM  ///////////////////////////////////////////////////////////////////////
