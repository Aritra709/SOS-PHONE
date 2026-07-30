#include <SoftwareSerial.h>
#include <String.h>
#include  <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Adafruit_PCF8574.h>
#define RfidPin 6
#include "uRTCLib.h"
char daysOfTheWeek[7][12] = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};

uRTCLib rtc;
Adafruit_PCF8574 pcf;

LiquidCrystal_I2C lcd(0x27,16,2);
SoftwareSerial mySerial(3,2);  

String number= ""; 
String action= "WT";
 
// Receive sms Strings
String Response ="";
String sms="";
String Type;
String Caller_id;
String Text;
String name1;
String name2;
String SP_name="";

// REPLACE THESE
String dialNumber1 = "+91"; // SOS Number 1 to dial
String dialNumber2 = "+91"; // SOS Number 2 to dial
String password = "709"; // CHANGE THE PASSWORD

 
char character;
char quote= 0x22;
 
// Global Flags
bool print1st = false;
bool print2nd = true;
bool Send_m=false;
bool sms_Receive_mode_off=true;
bool Receive_mode=false;
bool msg_Receive=false;
bool time_registered=false;
bool msg_fetched=false;
bool on_call=false;
bool start_Receive=false;
bool flag=true;
//bool pass=true;
bool pass=false;
bool pass_void=true;
bool pass_loop=false;
bool waitForPassword = false;
String enteredPassword = "";
bool passwordEntered = false;
 
int sec,minutes; // Clock variables
long c_start;
long c_time;
 
int i=0;
int indexOfQuotes[10];
 
double time_start;
double time_current;
double operational_time;
 
/********* Keypad Variables**********/

char value;
char num1;



const int rows[] = {4, 5, 6, 7};
const int cols[] = {0, 1, 2, 3}; 

char keypad[4][4] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};

unsigned long previousMillisBlink = 0;
const long intervalBlink = 3000;

void setup()
{
  pinMode(RfidPin,INPUT);
  pinMode(5,OUTPUT);
  pinMode(10,OUTPUT);
  digitalWrite(5,0);
  pinMode(A2, INPUT);
   

Serial.begin(9600);
lcd.backlight();
lcd.init();
print_00("    AB PHONE ");
delay(5000);
print_00("POWERED BY:");
print_01("AB OS v7.5");
delay(3000);
/////Serial.println("GSM startrd");
mySerial.begin(9600);
mySerial.setTimeout(2000);
Serial.setTimeout(2000);
initilise();
get_SP();
mySerial.println ("AT+CSMP=17,167,0,0\r");

}
 
 
void loop()
{

/////Serial.println("Action: "+ action); //Reports it current mode of working
 
while(action=="WT") // Its wait for SMS and Calls in this loop
{
if(sms_Receive_mode_off) //So, This turns on the SMS recieve mode
{ delay(1000);
On_sms_Receive_mode();
}
 
if(flag)
{
/////Serial.println("Receive_ready");
flag=false;
print_00("Sim~"); // Service provide name is printed on LCd
print_50(SP_name);
clear_Serial();
}
 
 
if(Receiving_on()) // FINALLY, the module is set to receive, Receive_on will beocome true in case msg or call arrives
{
Extract_type();
}
 
else
{ // In case of no reciving, update the current signal strength
int sensorValue = analogRead(A2);
  
if (sensorValue>1020){
    Serial.println("btn 1");
    digitalWrite(10,1);
    number = dialNumber1;
    pass=true;
    action = "SC";  // Set action to "SC" (send call)
    send_call(dialNumber1);
    delay(1000);
    digitalWrite(10,0);
  }
  if (sensorValue >= 500 && sensorValue <= 620){
    Serial.println("btn 2");
    digitalWrite(10,1);  
    pass=true;
     number = dialNumber2;
     send_sms(dialNumber2);
    delay(1000);
    digitalWrite(10,0);
   }
  if (sensorValue >= 370 && sensorValue <= 420){
    Serial.println("btn 3");
    digitalWrite(10,1);
    delay(1000);
    digitalWrite(10,0);
  }
update_signal_strength();
get_request(); // Or, check if user pressed any button for callling or SMS
}
  

}
 
while(action=="SM") // Sending Msg action
{
  get_password () ;
  if (pass) {
/////Serial.println("Enter number to message");
print_00("Send SMS to");
number= Take_input(); // Take input through swith matrix
//LCD print for Send message
bool success = send_sms(number);
if(success) // If sucessful go to wait state otherwise send again
{
action="WT";

}
flag=true;
}
}
 
while(action=="SC") //Sending call action, similar process as above
{ get_password();
  if (pass){
print_00("Enter Call num");
/////Serial.println("Enter number to call");
 
number = Take_input();
 
if (valid_number()) // Check number is 10 digit long
{
  
    Serial.println(number+ "(@); (Outgoing call)" + rtc.hour() +";" + rtc.minute() + " " +rtc.day() +"/" + rtc.month() );
    Serial.println(" ");

  //  Serial.print(rtc.hour());
  //  Serial.print(':');
  //  Serial.print(rtc.minute());
  //  Serial.print(" ");
   // Serial.print(rtc.day());
  //  Serial.print('/');
 //   Serial.print(rtc.month());

print_00("Calling");
print_01(number);
delay(1000);
 
send_call(number);
 
print_00("On line with");
print_01(number);
delay(1000);
clear_Serial();
if(on_call)
{terminate_call();

} // Waits here till the user is on call
}
 
action="WT";
flag=true;
}
}
 
while(action=="RC") // Recive call action
{
/////Serial.println("Press * to pick up or # to terminate");
    
   

  //  Serial.println(Caller_id + "(@);");
 //   Serial.print(" ");
 //   Serial.print(name1);
 //   Serial.print("; (Received call)");
 //   Serial.print(rtc.hour());
//    Serial.print(':');
//    Serial.print(rtc.minute());
//    Serial.print(" ");
 //   Serial.print(rtc.day());
 //   Serial.print('/');
//    Serial.print(rtc.month());
print_00("Call from");
print_01(Caller_id);
print_100(name1);
 Serial.println(Caller_id+ "(@); (Recived Call)("+ name1+")" + rtc.hour() +";" + rtc.minute() + " " +rtc.day() +"/" + rtc.month() );
Serial.println(" ");
clear_Serial();
WaitForPickup();
//incall
if(on_call) // Waits here till the user is on call
{terminate_call();}
/////Serial.println("Call response Recieved");
action="WT";
flag=true;
}
 
while(action=="RM") // Recieve SMS action
{
Show_sms();
action="WT";
flag=true;
}
}
 
/*
* Function to get the service provider(SP) name
* Sets a Global varible: SP_name
*/
 
void get_SP (void)
{ bool got_it=false;
delay(1000);
mySerial.println("AT");
delay(500);
print_00("Searching");
while(!( SP_name.indexOf('"')>0))
{ if(GSM_operational())
{
mySerial.println("AT+CSPN?"); //AT command for getting serivce provider name
mySerial.println();
}
 
delay(1000);
 
while(mySerial.available())
{
char character=mySerial.read();
SP_name.concat(character);
}
 
}
// Extracton process
SP_name= (SP_name.substring(SP_name.indexOf('"')+1,SP_name.lastIndexOf('"')));
/////Serial.println("Sim~: "+ SP_name);
 
}
 
// Fuciton to print current signal strength on lcd
 
void update_signal_strength (void)
{ unsigned long currentMillis = millis(); // Get the current time

  
  if (currentMillis - previousMillisBlink >= intervalBlink) {
    previousMillisBlink = currentMillis;
   
  
  String Network;
long Strength;
mySerial.println("AT+CSQ");
mySerial.println();
 
delay(500);
while(mySerial.available())
{
char character=mySerial.read();
Network.concat(character);
}
 
Network=Network.substring(Network.indexOf(':')+2,Network.indexOf(','));
Strength= Network.toInt(); // Strength Int value here
 
Strength=(Strength*100/31); // MAX strength= 31
lcd.setCursor(13,0);
lcd.print(int(Strength));
lcd.print('%');



rtc.refresh();
    /////Serial.print(rtc.day());
    /////Serial.print('/');
    /////Serial.print(rtc.month());
///////Serial.print(" (");
   // /////Serial.print(daysOfTheWeek[rtc.dayOfWeek()]);
  //  /////Serial.print(") ");
    /////Serial.print(rtc.hour());
    /////Serial.print(':');
    /////Serial.println(rtc.minute());
    
    lcd.setCursor(0,1);
    lcd.print(rtc.hour());
    lcd.print(':');
    lcd.print(rtc.minute());
    lcd.print(' ');
    lcd.print(' ');
    lcd.print(daysOfTheWeek[rtc.dayOfWeek()]);
    lcd.print(' ');
    lcd.print(rtc.day());
    lcd.print('/');
    lcd.print(rtc.month()); 
  }
   
}
 
//It recives a the char value of key pressed and stores it into
void get_request (void)
{
value=Return_Keypad_Values();
event(value);
}
 
// Select the apt mode as per the input
void event(char func)
{
switch (func)
{
 
case 'A':
action="SC"; //Send call
break;
case 'B':
action="SM"; // Send Message
break;
case 'C':
action="RC"; // Receive Call
break;
default:
action="WT"; // Wait for response
break;
}
}
 
/*
* Input: (string:num,)
* Output bool( t=sent f=unsent)
* Function to send sms to number
*/
bool send_sms (String number)
{
delay(00);
mySerial.println("AT");
delay(500);
if(GSM_operational())
{
mySerial.println("AT+CMGF=1");
delay(500);
mySerial.print ("AT+CSMP=17,167,0,0\r");
delay(500);
}
 
if(GSM_operational())
{ digitalWrite(5, HIGH); 
delay(100);
  digitalWrite(5, LOW);
   
  if (Serial.available() > 0) {
    Serial.println(Serial.read());
   String receivedData = Serial.readStringUntil('E'); // Read until 'E' (end marker)
  Serial.println(Serial.read());
  // Extract latitude and longitude from received data
  int separatorIndex1 = receivedData.indexOf(':');
  int separatorIndex2 = receivedData.indexOf(':', separatorIndex1 + 1);
  
  if (separatorIndex1 != -1 && separatorIndex2 != -1) {
    String latitudeStr = receivedData.substring(separatorIndex1 + 1, separatorIndex2 - 1); // Exclude the last character
    String longitudeStr = receivedData.substring(separatorIndex2 + 1, receivedData.length() - 1); // Exclude the last character
    
    // Generate Google Maps link with corrected strings
    String googleMapsLink = "https://maps.google.com/maps?q=loc:" + latitudeStr + "," + longitudeStr;
    delay(100);

  
 

//  Serial.println(number+ + "(@);");
//  Serial.print("(SMS Sent)" + googleMapsLink);
 // delay(100);
  //  Serial.print(" | ");
 //   Serial.print(rtc.hour());
 //   Serial.print(':');
  //  Serial.println(rtc.minute());
 //   Serial.print(" ");
 //   Serial.print(rtc.day());
 //   Serial.print('/');
  //  Serial.print(rtc.month());
delay(100); 
mySerial.print("AT+CMGS=\""); // Send the SMS number
delay(100);
mySerial.print(number);
delay(100);
mySerial.println("\"");
 
delay(1000);
mySerial.print(googleMapsLink); // SMS BODY here in case u want change
// mySerial.print(i);
delay(500);
 
mySerial.write(0x1A);
delay(100);
mySerial.write(0x0D);
delay(100);
mySerial.write(0x0A);
delay(100);
Serial.println("SMS Sent:" + googleMapsLink);
 delay(1000);
  Serial.println(" ");
   Serial.print( number + "(@);(Outgoing SMS)/" );
   delay(100);
   Serial.print(googleMapsLink);
      delay(500);

   Serial.print("/");
   Serial.print(rtc.hour());
   Serial.print(":" );
   Serial.print(rtc.minute() );
   Serial.print(";" );
   Serial.print(rtc.day() + "/" + rtc.month());
   Serial.println(" ");
 
print_00("SMS Send to");
print_01(number);
delay(2000);
 
return(true); //SMS sent succussfuly
pass=false;
  }
  
}

}

return(true); // Failed attempt
}
 
/*
* Input: (string:num,)
* Output bool( t=sent f=failed)
* Function to send call to number
*/
bool send_call (String number)
{
 
mySerial.println("AT");
delay(500);
 
if(GSM_operational())
{
//Number dialing
print_00("Calling to");
print_01(number);
mySerial.println("ATD"+ number +";"); // AT command for dialing up the number
mySerial.println();
on_call=true;
delay(100);
Serial.println(number+ "(@); (Outgoing Call)("+ name1+")" + rtc.hour() +":" + rtc.minute() + " " +rtc.day() +"/" + rtc.month() );
delay(1000);
Serial.println(" ");
return(true);
}
return(false);
}
 
// This is to switch on the messaging mode of Gsm
void On_sms_Receive_mode (void)
{
mySerial.print("ATE0");
delay(500);
 
if(GSM_operational())
mySerial.print("AT");
delay(500);
 
if(GSM_operational())
mySerial.print("AT+CMGF=1"); // Setup in msging mode
delay(500);
 
if (GSM_operational())
{
mySerial.print("AT+CNMI=2,2,0,0,0\r" ); //Prints the msg on serial as soon as it arrives
delay(500);
 
while(mySerial.available())
{
char response = mySerial.read();
Serial.println(response);
}
 
Serial.println("Receive mode On");
sms_Receive_mode_off=false; //turn it on off
}
}
 
/*
* Input: none
* Output: True: A response( call or sms) incoming, Or false
*
*/
bool Receiving_on (void)
{
bool Response_available=false;
 
if(mySerial.available())
{
while(!halt_fetch()) //In case of incoming recieve until halt_fetch() gives true
{
while(mySerial.available())
{
if(!time_registered) //Capture the time of start of message receiving
{
time_start=millis();
time_registered=true;
}
char character=mySerial.read();
Response.concat(character);
Serial.print(character); // Store as a string
}
 
}
 
/////Serial.println("Response Received"); //Looks like we got something
Response_available=true;
msg_fetched=false;
flag=true;
 
}
return (Response_available);
}
 
/*
*The function is created to halt or to indicate the end of receiving
*It does that by a timeout of 3sec or Response Text limit of 500 characters
*Input: none
*Output: Boolean, T= halt fetching F= Wait for message
 
*/
bool halt_fetch (void)
{
bool halt=false;
 
if(time_registered)
{
time_current=millis();
operational_time=time_current-time_start;
}
 
if(operational_time>3000 || Response.length()==200 ) // Halt condition
{
halt=true;
operational_time=0;
}
return halt;
}
 
/*
* It extracts the Response and caller id
* It does that by quotes position.
* Caller id is between first and second quotes
* While, Text message is after last quotes
*/
 
void Extract_type (void)
{
if(valid_input())
{
/////Serial.println("Valid respone");
extract();
 
 
/////Serial.println(Response); //In case u want to see everything incoming
/////Serial.println("Type: ");
/////Serial.print(Type);
/////Serial.println("Caller id : ");
/////Serial.println(Caller_id);
/////Serial.println("Text: ");
/////Serial.println(Text);
/////Serial.println("Name: ");
/////Serial.println(name1);
/////Serial.println(name2);
callORsms();
/////Serial.print(Caller_id);
}
 
time_registered=false;
 
Response=""; //Clear string for refresh incoming
 
}
 
/*
* Checks the validity condition,
* True: Its call or msg Resonse
* False: it is some junk
*/
bool valid_input (void)
{
bool validity;
 
validity=(( Response.indexOf('+') > 0) && (Response.indexOf('"')>0 )); //If the reponse has these two thing that means it is a 'real' response
 
if(!validity)
{
/////Serial.println("invalid input");
 
}
 
return (validity);
 
}
 
// Find the indexes of all the quotes in the stirng and sets them up in gloablevariable: indexOfQuotes[index]
void extract(void)
{
int Length,i,index=0;
 
Length=Response.length();
for(i=0;i<=Length;i++)
{
if(Response.charAt(i)==quote)
{
indexOfQuotes[index]=i;
index++;
}
}

Type=Response.substring(1,indexOfQuotes[0]);
Caller_id=Response.substring(indexOfQuotes[0]+1,indexOfQuotes[1]);
name1 = Response.substring(indexOfQuotes[4] + 1, indexOfQuotes[5]);
Text=Response.substring(indexOfQuotes[5]+3);
/////Serial.println("Extracted TEXT :" +Text);
/////Serial.println("Extracted");

}
 
// Determine weather the response is of call or sms
void callORsms (void)
{
if( Type.indexOf("LIP")>0) //Call string consist this( +CLIP)
{ action="RC";
//////Serial.println("Call from: ");
}
else if(Type.indexOf("MT")>0 ) // Msg stirng consist (+CMT)
{ action="RM";
name2 = Response.substring(indexOfQuotes[2] +1, indexOfQuotes[3]);
/////Serial.println("Message from: ");
}
}
 
// Waits till customer press * or #
void WaitForPickup (void)
{
char key;
bool user_wait = true; //default state
while(user_wait)
{ user_wait=check_termination();
 
key=Return_Keypad_Values();
if(key=='*') //picking up reponse
{
mySerial.println("ATA");
mySerial.println();
/////Serial.println("Call picked up");
print_00("Call picked up");
print_01(Caller_id);
delay(1000);
user_wait=false;
on_call=true;
}
 
if(key=='#') //Termination action
{
mySerial.println("ATH");
mySerial.println();
/////Serial.println("Call Terminated");
print_00("Call Terminated");
delay(1000);
print_01(Caller_id);
on_call=false;
user_wait=false;
pass=false;
}
 
}
 
}
 
/*
* This function is used after two user get connected on a call
* It waits '#' to terminate or 'NO CARRIER' on serial monitor
* It updates clock untill waiting
* */
 
void terminate_call (void)
{
char key;
bool user_wait = true; //default state
start_clock();
while(user_wait)
{
 
user_wait=check_termination();
 
key=Return_Keypad_Values();
 
if(key=='#')
{
mySerial.println("ATH"); //Termination action
mySerial.println();
/////Serial.println("Call Terminated");
print_00("Call Terminated");
delay(1000);
print_01(Caller_id);
user_wait=false;
pass=false;
}
 
else
{
update_clock();
}
 
}
on_call=false;
pass=false;
}
 
// Function to start a clock
void start_clock (void)
{
lcd.clear();
c_start=millis();
sec=0;
minutes=0;
lcd.print("On call");
}
 
// Function to update the value as arduino internal clock
 
void update_clock (void)
{
long current= millis();
 
if(current-c_start>1000)
{
sec++;
c_start=current;
}
 
if(sec>59)
{
minutes++;
sec=-0;
}
 
lcd.setCursor(0,1);
 
if(minutes<10)
{lcd.print('0');}
lcd.print(minutes);
lcd.print(':');
if(sec<10)
{lcd.print('0');}
lcd.print(sec);
}
 
// Fuction to Show sms on a LCD
void Show_sms() {

  

   
     
    print_00("SMS from:");
    print_01(Caller_id);
    print_110(name2);
//  Serial.println(Caller_id+ "(@); (Received SMS)("+name2  +") " + rtc.hour() +";" + rtc.minute() + " " +rtc.day() +"/" + rtc.month() +" || Text: "+ Text );
    
    Serial.println(" ");
    Serial.print(Caller_id + "(@);");
    Serial.print(name2);
    Serial.print("; (Received SMS)");
    Serial.print(rtc.hour());
    Serial.print(':');
    Serial.print(rtc.minute());
    Serial.print(" ");
    Serial.print(rtc.day());
    Serial.print('/');
    Serial.print(rtc.month());
    Serial.print(" || TEXT: ");
    Serial.print(Text);
   



    bool firstPartDisplayed = false;
    int startIdx = 0;
    int endIdx = 16;
    bool user_wait = true;
    
    while (user_wait) {
        char key = Return_Keypad_Values();
        
        if (key == '*') {
            print_00(Text.substring(startIdx, endIdx));
            print_01(Text.substring(endIdx, endIdx + 16));
            firstPartDisplayed = true;
        } else if (key == '#') {
            print_00("OK");
            Serial.println("MSG Terminated");
            delay(500);
            user_wait = false;
        } else if (firstPartDisplayed && key == '6') {
            if (endIdx + 16 < Text.length()) {
                startIdx += 16;
                endIdx += 16;
                print_00(Text.substring(startIdx, endIdx));
                print_01(Text.substring(endIdx, endIdx + 16));
            }
        } else if (firstPartDisplayed && key == '4') {
            if (startIdx - 16 >= 0) {
                endIdx -= 16;
                startIdx -= 16;
                print_00(Text.substring(startIdx, endIdx));
                print_01(Text.substring(endIdx, endIdx + 16));
            }
        }
    }
}


 
//True if starkey is pressed
bool Starkey_pressed (void)
{ char key;
key=Return_Keypad_Values();
return (key=='*');
}
 
//True if Hashkey is pressed
bool Hashkey_pressed (void)
{ char key;
key=Return_Keypad_Values();
return (key=='#');
}
 
//Check if 'NO CARRIER' is printer on Serial monitor
 
bool check_termination (void)
{
bool check=true;
String listen_no="";
 
while(mySerial.available())
{
char data= mySerial.read();
Serial.print(data);
listen_no.concat(data);
}
 
if(listen_no.indexOf("CAR")>0) // I check for only CAR
{ check=false; 
pass=false;}
 
return check;
}
 
// A Fuciton to check the lenth of number calling should be 10 + ('+91' country code) =13
bool valid_number (void)
{
bool valid=false;
if(number.length()==13) // condition here
{valid=true;}
else
{ print_00("Invalid input");
delay(1000);
}
return valid;
}
 
//Essential command to determine the state of GSM module
bool GSM_operational(void)
{
int count =0;
bool Status=false;
mySerial.println();
while(1)
{
if (mySerial.available()>0)
{
char data = mySerial.read();
if( data == 'O' || data == 'K') //Its working properly
{
/////Serial.println("OK");
Status=true;
break;
}
if( data == 'E' || data == 'R' || data== 'O') // Working yet busy with some thing else
{
/////Serial.println("GSM not functional");
Status=false;
break;
}
}
count++;
delay(10);
if (count == 100)
{
/////Serial.println("GSM not connected"); // No reponse for AT commands
Status=false;
break;
}
}
return Status;
}
 
void clear_Serial (void)
{
while(mySerial.available())
{
char character=mySerial.read();
Serial.print(character);
}
 
}
 
/*************************************************************
* Keypad Firmware Ahead *
*************************************************************/
 
/*
* input: none
* Output: A 13 digit number
* Waits till user enter a ten Digit number
*/
String Take_input (void)
{ String num="+91";
int len=0;
int len2;
while (len <= 13)
{
len=num.length();
num1=Return_Keypad_Values();
if ((num1!='A')&&(num1!='B')&&(num1!='C')&&(num1!='a'))
{
if ((num1!='#') && (num1!='*') && (num1!='D'))
{num+=String(num1);
print_01(num);
Serial.println(num);
}
 
else if (num1=='*')
{
num.setCharAt(len-1,'*');
print_01(num);
num.remove(len-1);
}
else if (num1=='#')
{
Serial.println(num);
break;
}
else if(num1=='D')
{
break;
}
}
 
}
return num;
}
 
 
void initilise()
{
Wire.begin();
  pcf.begin(0x20); // Change the address if it's different

  for (int i = 0; i < 4; i++) {
    pcf.pinMode(rows[i], OUTPUT);
    pcf.digitalWrite(rows[i], HIGH); // Set initial state to HIGH for rows
    pcf.pinMode(cols[i], INPUT_PULLUP); // Enable internal pull-ups for columns
  }

  Serial.begin(9600);
}

char Return_Keypad_Values(void)
{
char keyPressed = 'a'; // Default return value
  
  for (int i = 0; i < 4; i++) {
    pcf.digitalWrite(rows[i], LOW); // Activate a row

    for (int j = 0; j < 4; j++) {
      if (pcf.digitalRead(cols[j]) == LOW) { // Check for a button press in the column
        keyPressed = keypad[i][j];
        Serial.println(keyPressed); // Print the pressed key
        delay(50); // Debounce delay
        while (pcf.digitalRead(cols[j]) == LOW) {} // Wait for button release
      }
    }

    pcf.digitalWrite(rows[i], HIGH); // Deactivate the row
  }

  return keyPressed;
}
 
 void get_password () {
  if (pass_void){
  lcd.clear(); 
  lcd.setCursor(0, 0);
  lcd.print("Enter password:");
  pass_void = false ;
   
  }
  
   
    if (digitalRead(RfidPin) == HIGH) {
       pass = true;
        }
  
 char key = Return_Keypad_Values();
  
  if (key != 'a') {
    
    if (digitalRead(RfidPin) == HIGH) {
       pass = true;
        }
    if (key == '*') {
      if (enteredPassword.length() > 0) {
        enteredPassword.remove(enteredPassword.length() - 1);
        lcd.setCursor(0, 1);
        lcd.print(enteredPassword + " ");
      }
    } else if (key == '#') {
      
      passwordEntered = true;
    } else if (!passwordEntered) {

    
      enteredPassword += key;
      lcd.setCursor(0, 1);
      lcd.print(enteredPassword + " ");

      // Check for password match without # button
      if (enteredPassword == "709") {
        passwordEntered = true;
      }
        

    }
    
    if (passwordEntered && enteredPassword.length() > 0) {
      if (enteredPassword == password) {
        lcd.clear();
        lcd.print("Password matched!");
        // Perform action when correct password is entered
        delay(2000);  // Display message for 2 seconds
        pass = true;
        enteredPassword = "";  // Reset entered password

      } else {
        lcd.clear();
        lcd.print("Wrong password!");
        delay(2000);  // Display wrong password message for 2 seconds
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Enter password:");
        enteredPassword = "";  // Reset entered password
      }
      passwordEntered = false; // Reset flag for password entry
      lcd.setCursor(0, 1);
      lcd.print("                "); // Clear entered password from LCD
    }
  }
 }

 

/*************************************************************
* LCD functions Ahead *
*************************************************************/
 
//Print out the Heading On lCD
void print_00 (String str)
{ lcd.clear();
lcd.setCursor(0,0);
lcd.print(str);
}
 
//Print secondary content on LCD
void print_01 (String str)
{
lcd.setCursor(0,1);
lcd.print(str);
}
void print_50 (String str)
{
lcd.setCursor(4,0);
lcd.print(str);
}
void print_110 (String str)
{
lcd.setCursor(9,0);
lcd.print(str);
}
void print_100 (String str)
{
lcd.setCursor(10,0);
lcd.print(str);
} 
