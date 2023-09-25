#include <SoftwareSerial.h>

SoftwareSerial mySerial(4, 3); // RX, TX


//dog size must larger than the char length of the GPGGA msg
#define DOG_SIZE 62
#define PI 3.1415926



typedef struct GPS
{

    //extra slot for '\0' when tokenlizing
    char watch_dog[DOG_SIZE+1];

    float UTC_h;
    float UTC_m;
    float UTC_s;

    float Lat;
    float Long;
    float Alt;

} GPS;

void print_dog (GPS *myGPS){

    for(int i =0; i<(DOG_SIZE);i++){
      Serial.print(myGPS->watch_dog[i]);  
    }

    Serial.print("\n");
}
// GPGGA,031434.000,4226.63801,N,07630.10c90,W,0,00,0.0,105.7,M,0
void buffer_shift(GPS *myGPS, char new_char)
{
    char temp_dog[DOG_SIZE];
    for (int i=0;i<DOG_SIZE-1;i++){
        temp_dog[i]=myGPS->watch_dog[i+1];
    }
    temp_dog[DOG_SIZE-1]=new_char;
    
    //full copy
    for (int i=0;i<DOG_SIZE;i++){
        myGPS->watch_dog[i]=temp_dog[i];
    }

    myGPS->watch_dog[DOG_SIZE]='\0';    

}

int check_GPGGA(GPS *myGPS)
{
    char c0 = myGPS->watch_dog[0];
    char c1 = myGPS->watch_dog[1];
    char c2 = myGPS->watch_dog[2];
    char c3 = myGPS->watch_dog[3];
    char c4 = myGPS->watch_dog[4];

    if (c0 == 'G' && c1 == 'P' && c2 == 'G' && c3 == 'G' && c4 == 'A'){
        return 1;
    }else{
        return 0;
    }
}

//precision = 0 , 1 , 2, for hour min sec
float token_to_time(char* token, int precision){
//HHMMSS
    char hh[3];
    char mm[3];
    char ss_sss[6];

    for (int i = 0; i < 2; i++)
    {
        hh[i] = token[i];
        mm[i] = token[i+2];
    }

    for (int i=0;i<5;i++){
        ss_sss[i] = token[i+4];
    }

    hh[2] = '\0';
    mm[2] = '\0';
    ss_sss[5] = '\0';
    
  switch(precision){
    case 0: //return hour
      return atof(hh);
    case 1: //return min
      return atof(mm);
    case 2: //return sec
      return atof(ss_sss);
  }



}

//convert token for either Lat or Long to degree
float token_to_degree(char* token, bool is_lat)
{
  float d,m;
  
  if(is_lat){
    char Lat_dd[2+1];
    char Lat_mm_mmmmm[8+1];

    for (int i = 0; i < 2; i++){
      Lat_dd[i] = token[i];
    }

    for (int i = 0; i < 8; i++)
    {
      Lat_mm_mmmmm[i] = token[2+i];
    }

    Lat_dd[2]='\0';
    Lat_mm_mmmmm[8]='\0';

    d= atof(Lat_dd);
    m=atof(Lat_mm_mmmmm);

    
  }else{
    char Long_ddd[3+1];
    char Long_mm_mmmmm[8+1];


    for (int i = 0; i < 3; i++)
    {
        Long_ddd[i] =  token[i];
    }

    for (int i = 0; i < 8; i++)
    {
        Long_mm_mmmmm[i] =  token[3+i];
    }

    Long_ddd[3]='\0';
    Long_mm_mmmmm[8]='\0';   

    d= atof(Long_ddd);
    m= atof(Long_mm_mmmmm); 
  }
  
    return d + m / 60.0;
}



void parse_gps(GPS *myGPS)
{
    int token_number=0;
    char *token = strtok(myGPS->watch_dog, ",");

    while (token != NULL) {
      switch (token_number) {
        case 1: //UTC time
          myGPS->UTC_h  = token_to_time(token,0);
          myGPS->UTC_m  = token_to_time(token,1);
          myGPS->UTC_s  = token_to_time(token,2);
        case 2: // Latitude
          myGPS->Lat= token_to_degree(token,true);
          break;
        case 3: // Latitude N/S indicator
          if (token[0] == 'S') {
            myGPS->Lat = -myGPS->Lat;
          }
          break;
        case 4: // Longitude
          myGPS->Long = token_to_degree(token,false);
          break;
        case 5: // Longitude E/W indicator
          if (token[0] == 'W') {
            myGPS->Long = -myGPS->Long;
          }
          break;
        case 9: // Altitude
          myGPS->Alt = atof(token);
          break;
      }
    token = strtok(NULL, ",");   
    token_number++;
    }
}

void fetch_GPS(GPS *myGPS){
 //Check to see if anything is available in the serial receive buffer
 while (mySerial.available() > 0)
 {
   //Read the next available byte in the serial receive buffer
   char new_char = mySerial.read();
   buffer_shift(myGPS, new_char);   
   //if valid, update the fields
   if(check_GPGGA(myGPS)==1){
     parse_gps(myGPS);
     
     //for testing purpose
     Serial.print("NEW MSG UPDATE!: ");     
     print_dog(myGPS);
   }
 }  
}
 uint8_t SetNMEA[] = {
  0xA0, 0xA1, 0x00, 0x03, 0x09, 0x01, 0x01, 0x09, 0x0D, 0x0A};

 uint8_t SetPosRate[] = {
  0xA0, 0xA1, 0x00, 0x03, 0x0E, 0x01, 0x01, 0x0E, 0x0D, 0x0A};  

static GPS myGPS;

void setup() {
 Serial.begin(9600);
 mySerial.begin(115200);
 delay(1000);


 mySerial.write((uint8_t *)&SetNMEA, sizeof(SetNMEA));
 delay(500);
 mySerial.write((uint8_t *)&SetNMEA, sizeof(SetNMEA));
 delay(500);
 mySerial.write((uint8_t *)&SetNMEA, sizeof(SetNMEA));
 delay(1000);
}

void loop() {
  
  fetch_GPS(&myGPS);

  Serial.print("UTC hour: ");
  Serial.println(myGPS.UTC_h);

  Serial.print("UTC min: ");
  Serial.println(myGPS.UTC_m);
  
  Serial.print("UTC sec: ");
  Serial.println(myGPS.UTC_s);

  Serial.print("Latitude(degree): ");
  Serial.println(myGPS.Lat);

  Serial.print("Longitude(degree): ");
  Serial.println(myGPS.Long);

    Serial.print("Altitude(M): ");
  Serial.println(myGPS.Alt);


  Serial.println("------------------------------------------------------------------------");

  delay(1000);


}
