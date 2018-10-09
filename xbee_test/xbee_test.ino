// setup() 函數只會於電源啟動時執行1次
void setup() 
{
  Serial.begin(9600); 

  pinMode(2, OUTPUT);  // channel A  
  pinMode(3, OUTPUT);  // channel B   
  pinMode(4, OUTPUT);  // channel C
   pinMode(13, OUTPUT);   
}

// loop() 函數會不斷的被重複執行
void loop()
{
  if (Serial.available() > 16) 
  {
    if (Serial.read() == 0x7E) 
    {
      byte HightByte = Serial.read();
      byte LowByte = Serial.read();
      int NumberOfByte = HightByte * 256 + LowByte;
      int DataLength = NumberOfByte-12;

      for(int i = 0; i<12;i++)
        byte discardByte = Serial.read();
         
      Serial.println("Data=");
      for(int i = 0; i<DataLength;i++)
      {
        delay(100);
        byte dataByte = Serial.read();
        Serial.write(dataByte);

        digitalWrite(13, HIGH);   // turn the EL channel on
        delay(500);              // wait for 1/10 second
        digitalWrite(13, LOW);    // turn the EL channel off
        delay(500); 

          switch (dataByte)
          { 
            case 0x61:  
              digitalWrite(2, HIGH);
              break; 
            case 0x62:  
              digitalWrite(3, HIGH);
              break; 
            case 0x63:  
              digitalWrite(4, HIGH);
              break; 
            default: 
              digitalWrite(2, LOW);
              digitalWrite(3, LOW);
              digitalWrite(4, LOW);
              break;
          }
        
      }
    }
  }
}
