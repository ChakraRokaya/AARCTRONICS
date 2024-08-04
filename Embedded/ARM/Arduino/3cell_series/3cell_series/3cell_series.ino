#include <stdio.h>
#include <string.h>
//This program is used to code the cell voltages with arduino program. The 3.4 V is divided into 3 dummy cells (C1,C2 and C3) with voltage divider. 
//The measured voltage from arduino program is exactly same as with the multimeter.


void setup() {
   Serial.begin(115200);
  // put your setup code here, to run once:

}

void loop() {
   
  // put your main code here, to run repeatedly:

float Read_input1 = analogRead(A0); //reading the Analog input1 cell voltage value
float Read_input2 = analogRead(A1); //reading the Analog input2 cell voltage value
float Read_input3 = analogRead(A2); //reading the Analog input3 cell voltage value
Serial.println("The analog to digital conversion value is");
Serial.println(Read_input1);//ADC value input 1 i.e. A0
 Serial.println(Read_input2); //ADC value input 2 i.e. A1
 Serial.println(Read_input3); //ADC value input 3 i.e. A2
float Valvoltage1 = (Read_input1*5)/1024; // as we are using 8 bit ADC and the maximum voltage at pin is 5V, // cell1 voltage with respect to gnd
float Valvoltage2 = (Read_input2*5)/1024; //cell2 voltage W.R.T to ground
float Valvoltage3 = (Read_input3*5)/1024; // cell3 voltage W.R.T to ground
float cell2_volt= (Valvoltage2-Valvoltage1);//subtracting cell 1 voltage from the the cell2 voltage  wrt ground value 
float cell3_volt= (Valvoltage3-Valvoltage2); //subtracting cell 2 voltage from the the cell3 voltage wrt ground value 
Serial.println("inputcell1 voltage = "); Serial.println (Valvoltage1);
Serial.println("inputcell2 voltage = "); Serial.println (cell2_volt);
Serial.println("inputcell3voltage = "); Serial.println (cell3_volt);
}
