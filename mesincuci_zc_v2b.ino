#include <RBDdimmer.h>
#define outputPin  12 
#define zerocross  2 

//#define WASH_MOTOR 12
#define WASH_MOTOR_DIRECTION 11
#define INLET 10
#define DRAIN 9
#define CW 1
#define CCW 0
dimmerLamp dimmer(outputPin); 
int ke=0;
char data = 0;
int kecepatan=0;
int analogPin = 0;
int istirahat=0;
int highestspeed;
int tenaga=25;
int sambildrain=0;
int kecepatansense=70;
int myInts[30];
unsigned int  jml=0;
int jmlrata=0;
int isi=6;
int aduk=5;
int nyuci=55;//i change from 70 to 50 on 20250710
unsigned int lamaistirahat=60000;
unsigned int lamaistirahatantararah=5000;
int keepatancuci=47;
int lamaspin=32000;//30 menit

// the setup function runs once when you press reset or power the board
void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  //pinMode(WASH_MOTOR, OUTPUT);
  pinMode(WASH_MOTOR_DIRECTION, OUTPUT);
  pinMode(INLET, OUTPUT);
  pinMode(DRAIN, OUTPUT);
  dimmer.begin(NORMAL_MODE,OFF);  
  Serial.begin(9600);
}

// the loop function runs over and over again forever
void loop() {
  digitalWrite(LED_BUILTIN, HIGH);
  //digitalWrite(WASH_MOTOR, HIGH);
  digitalWrite(WASH_MOTOR_DIRECTION, CW);
  digitalWrite(INLET, HIGH);
  digitalWrite(DRAIN, HIGH);

  //TELL USER WE WANT TO START
  for (int i = 0; i <= 10; i++) {
  data=Serial.read();
  Serial.println("pilih!1sd2");
  Serial.println("1. Delicate");
  Serial.println("2. Quick");
  Serial.println("3. Heavy");

  if(data=='1'){//gentle
    Serial.println("under construcition");
  }
  if(data=='2'){
    Serial.println("quick mode selected");
    Serial.println("agitate 25x");
  }
    digitalWrite(LED_BUILTIN, HIGH);
    delay(500);
    digitalWrite(LED_BUILTIN, LOW);
    delay(500);    
  }
  digitalWrite(LED_BUILTIN, HIGH);
  delay(1000);    

  //add sense how many water actually we need
  //2024-oktober-12
  //the logic, we need to reach a speed, let say 100
  //the heavier the load, then we need more power to reach that number
  //then the power will equal with the heavy of load
  Serial.print("Ke ");
  Serial.print(ke);
  Serial.print("\n");
if(ke==0){
for(int lup1=0; lup1<=29; lup1++){
  dimmer.setPower(tenaga); // name.setPower(0%-100%)
  delay(25);
  dimmer.setState(ON);
  for(int lup = 0; lup <= 1000; lup++) {
        kecepatan = analogRead(analogPin);
        if(kecepatan < kecepatansense) tenaga++;
        if(kecepatan > kecepatansense) {
          for(int slowdown=tenaga;slowdown<=25;slowdown--){
            delay(100);
            dimmer.setPower(slowdown);
          }
          break;
        }
        delay(100);//500*20=10detik
        if(tenaga>49) tenaga=49;
        if(tenaga<25) tenaga=25;
        dimmer.setPower(tenaga); // name.setPower(0%-100%)     
  }
  //dimmer.setPower(0); // name.setPower(0%-100%)
  dimmer.setState(OFF);
  Serial.print("tenaga ");
  Serial.print(tenaga);
  myInts[lup1]=tenaga;
  tenaga=25;
  Serial.print("\n");
  delay(1500);
}  
Serial.print("sense selesai\n");
for(int a=0;a<=29;a++){
  Serial.print("a ke ");
  Serial.print(a);
  Serial.print(myInts[a]);
  Serial.print("\n");
  jml=jml+myInts[a];
}
jmlrata=jml/30;
}//end sense  heavy clothes load
Serial.print("jml rata2 ");
Serial.print(jmlrata);
Serial.print("\n");

//
//38 - 39 SEDIKIT ATAU KOSONG (1kg)
//40 - 41 3KG 2kg
//40        3k

//bibir 3 menit
//cocok untu 3kg di 5min

if(  ke==0 ){
  if(  jmlrata<=39 ){
    isi=3;
  } else if(jmlrata==40) {
    isi=4;
  } else {
    isi=5;
  }
} else {
  isi=6;
}
Serial.print("isi air ");
Serial.print(isi);
Serial.print("\n");

   //start water inTAKE
  for(int i = 0; i <= isi; i++) {  
    digitalWrite(INLET, LOW);
    delay(60000);// 1 menit
    Serial.print("menit air ");
    Serial.print(i);
    Serial.print("\n");
    digitalWrite(INLET, HIGH);//istirahat biar gak panas selenoid
    delay(1000);
  }


  //start wash
  if(ke==1) nyuci=20;
  if(ke==2) nyuci=10;
  for (int i = 0; i <= nyuci; i++) {
    //1 tentukan arah, cw or ccw, 2. putar, 3. nolkan semua, ganti arah dst
    digitalWrite(WASH_MOTOR_DIRECTION,CW);
    delay(1000 );//THIS TO MAKE RELAY DIRECTION CONTROL LAST LONG
    tenaga=25; 
    dimmer.setPower(tenaga); // name.setPower(0%-100%)
    delay(100);
    dimmer.setState(ON);
    //lama putar
    for(int genjot = 0; genjot <= 1000; genjot++) {
        kecepatan = analogRead(analogPin);
        if(kecepatan < keepatancuci) tenaga++;
        if(kecepatan > keepatancuci) tenaga--;
        delay(20);//500*20=10detik
        if(tenaga>49) tenaga=49;
        if(tenaga<25) tenaga=25;
        dimmer.setPower(tenaga); // name.setPower(0%-100%)     
    }
    dimmer.setPower(10);
    dimmer.setState(OFF);
    delay(5000);//16.1 detik untuk satu arah

    //ganti arah
    digitalWrite(WASH_MOTOR_DIRECTION,CCW);
    delay(1000 );       //THIS TO MAKE RELAY DIRECTION CONTROL LAST LONG
    tenaga=25; 
    dimmer.setPower(tenaga); // name.setPower(0%-100%)
    delay(100);
    dimmer.setState(ON);
    //lama putar
    for(int genjot = 0; genjot <= 1000; genjot++) {
        kecepatan = analogRead(analogPin);
        if(kecepatan < keepatancuci) tenaga++;
        if(kecepatan > keepatancuci) tenaga--;
        delay(20);//500*20=10detik
        if(tenaga>49) tenaga=49;
        if(tenaga<25) tenaga=25;
        dimmer.setPower(tenaga); // name.setPower(0%-100%)     
    }
    dimmer.setPower(10);
    dimmer.setState(OFF);
    delay(5000);//16.1
    //siklus end
    Serial.print("nyuco cycle ");
    Serial.print(i);
    Serial.print("\n");
    //add istirahat 
  istirahat++;
    if(istirahat==15) {
    //lamaistirahat2 5=5mins;
    for(int lamaistirahat2=0; lamaistirahat2<=6; lamaistirahat2++){
      delay(lamaistirahat);//1 min
    }
    istirahat=0;
    } 
  }
  //END WASH 
  //step 1 32.2detik*35 = 18.7menit, 
  //step 2 32.2*20=       10.7menit
  //step 3 32.2*10=        5.3 menit, 
  //                total 34.7

  delay(1000*30);//biarkan air turun dari pakaian
  ke++;

  //SUCIKAN
  digitalWrite(INLET, LOW);
  delay(30000);
  //putar sedikit
  dimmer.setPower(10);
  dimmer.setState(OFF);
  digitalWrite(WASH_MOTOR_DIRECTION,CW);
    delay(1000 );//THIS TO MAKE RELAY DIRECTION CONTROL LAST LONG
    tenaga=25; 
    dimmer.setPower(tenaga); // name.setPower(0%-100%)
    delay(100);
    dimmer.setState(ON);
    //lama putar
    for(int genjot = 0; genjot <= 1000; genjot++) {
        kecepatan = analogRead(analogPin);
        if(kecepatan < keepatancuci) tenaga++;
        if(kecepatan > keepatancuci) tenaga--;
        delay(20);//1000*20=20detik
        if(tenaga>49) tenaga=49;
        if(tenaga<25) tenaga=25;
        dimmer.setPower(tenaga); // name.setPower(0%-100%)     
    }
    dimmer.setPower(10);
    dimmer.setState(OFF);
    delay(5000);//16.1 detik untuk satu arah
  //end putar dikit
  digitalWrite(DRAIN, LOW);
  delay(60000);
  digitalWrite(DRAIN, HIGH);
  digitalWrite(INLET, HIGH);

  //DRAin
  for(int i = 0; i <= 2; i++) {  
    digitalWrite(DRAIN, LOW);
    delay(60000);
    digitalWrite(DRAIN, HIGH);//istirahat biar gak panas selenoid
    delay(10000);
  }


  //SPIN
  digitalWrite(WASH_MOTOR_DIRECTION,CW);
  delay(1000 );//THIS TO MAKE RELAY DIRECTION CONTROL LAST LONG
  tenaga=25; 
  dimmer.setPower(tenaga); // name.setPower(0%-100%)
  delay(1000);
  dimmer.setState(ON);
  //lama putar
  int spinstep[10]={50, 100, 150, 200, 250,300,350,400,500,600};
  for(int spinx=0; spinx<=9; spinx++){
    for(int genjot = 0; genjot <= 1000; genjot++) {
        kecepatan = analogRead(analogPin);
        if(kecepatan < spinstep[spinx]) tenaga++;
        if(kecepatan > spinstep[spinx]) tenaga--;
        delay(10);
        if(tenaga>51) tenaga=51;
        if(tenaga<25) tenaga=25;
        dimmer.setPower(tenaga); // name.setPower(0%-100%)     
    }
  }
   
  if(ke==1) lamaspin=30000;;
  if(ke==2) lamaspin=30000;;
  for(int genjot = 0; genjot <= lamaspin; genjot++) {
    kecepatan = analogRead(analogPin);
    if(kecepatan < spinstep[9]) tenaga++;
    if(kecepatan > spinstep[9]) tenaga--;
    delay(20);
    if(tenaga>51) tenaga=51;
    if(tenaga<25) tenaga=25;
    dimmer.setPower(tenaga); // name.setPower(0%-100%)
    sambildrain++;
    if(sambildrain==300){
      digitalWrite(DRAIN, LOW);
    }
    if(sambildrain==450){
      digitalWrite(DRAIN, HIGH);
      sambildrain=0;
    }     
  }
  dimmer.setPower(10);
  dimmer.setState(OFF);
  digitalWrite(DRAIN, HIGH);
  delay(10000);
  //END SPIN

  if(ke==3){
    digitalWrite(LED_BUILTIN, LOW);
    while(true){
      digitalWrite(LED_BUILTIN, HIGH);
      delay(500);
      digitalWrite(LED_BUILTIN, LOW);
      delay(1000);
    }
  }
}
      
