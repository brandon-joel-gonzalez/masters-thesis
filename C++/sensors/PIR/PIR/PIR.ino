#define PIR 2
#define PIR_THRESH 600
 
void setup()
{
  Serial.begin(115200);
  pinMode(PIR, INPUT);

  Serial.println("Sensor Initializing.....");
  delay(10000);
  Serial.println("Setup Completed");
  delay(3000);  
}

void loop() 
{
  int duration;
  duration = pulseIn(PIR,HIGH);

  if(duration>PIR_THRESH)
    Serial.println("Motion Detected");
  else
    Serial.println("No Motion");

  delay(1000);
}
