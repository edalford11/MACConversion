//
//  MACConversion.cpp
//  MACConversion
//
//  Created by Eric Alford on 4/7/13.
//  Copyright (c) 2013 Eric Alford. All rights reserved.
//

#include <iostream>
#include <stdio.h>
#include <sstream>

using namespace std;

int indexx = 1;
unsigned int hexValue = -1;
char* filePath;
string months[] = {"January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December"};

void getFile(){
    unsigned char* buff;
    FILE *f;
    f = fopen(filePath, "r");
    if (f==NULL) {fputs ("File error\n",stderr); exit (1);}
    fseek (f , 0 , SEEK_END);
    long lSize = ftell(f);
    rewind (f);
    
    buff = (unsigned char*) malloc (sizeof(char)*lSize);
    if (buff == NULL) {fputs ("Memory error\n",stderr); exit (2);}
    
    size_t result = fread (buff,1,lSize,f);
    if (result != lSize) {fputs ("Reading error\n",stderr); exit (3);}
    fclose (f);
    
    //concatenate the individual numbers into a hex number.
    stringstream concat;
    concat << hex << buff[0] << buff[1] << buff[2] << buff[3] << buff[4] << buff[5];
    concat >> hexValue;
}

void convertDate(){
    int hex1 = (hexValue >> 8); //8 most significant bits.
    int hex2 = hexValue & 0xFF; //8 least significant bits.
    //since the format is little endian, we take the 8 least significant bits and shift them
    //to right right 1 place to get the 7 bits that represent the year.
    int year = (hex2 >> 1);
    year = 1980+year;
    //Here we take the 8 least significant bits and do a logical AND operation with the hex
    //value 0x01 in order to extract the very least significant bit. We then shift that bit
    //to the left 3 places. We take the 8 most significant bits and shift them to the right
    //5 places to get the 3 most significant bits. Doing a logical OR operation on these 2
    //values yields the 4 bits we need to get the month value.
    int month = ((hex2 & 0x01)<<3) | (hex1 >> 5);
    //We take the 8 most significant bits and do a logical AND operation with 0x1F, which
    //yields the 5 bits we need for the day.
    int day = (hex1 & 0x1F);
    cout << "Date: " << months[month-1] << " " << day << ", " << year << endl;
}

void convertTime(){
    //The format is in little endian so we will split the hex value between 8 bit values.
    int hex1 = (hexValue >> 8); //8 most significant bits.
    int hex2 = hexValue & 0xFF; //8 least significant bits.
    //Shift right 3 places to get the 5 bits that represent the hour.
    int hour = (hex2 >> 3);
    //Get the 3 least significant bits of hex2 and concantinate them with the 3 most significant
    //bits of hex1 to get the bits that represent the minutes.
    int minutes = ((hex2 & 0x07) << 3) | (hex1 >> 5);
    //Get the 5 least significant bits of hex1 to get the bits that represent the seconds.
    //The seconds are represented in 2 second intervals so we multiply by 2.
    int seconds = (hex1 & 0x1F)*2;
    if(hour>12){
        hour = hour-12;
        cout << "Time: " << hour << ":" << minutes << ":" << seconds << " PM" << endl;
    }
    else
        cout << "Time: " << hour << ":" << minutes << ":" << seconds << " AM" << endl;
}

int main(int argc, char* argv[]) {
    string cmd = argv[indexx];
    if(cmd == "-T"){
        indexx++;
        cmd = argv[indexx];
        if(cmd == "-f"){
            indexx++;
            filePath = argv[indexx];
            getFile();
            convertTime();
        }
        else if(cmd == "-h"){
            indexx++;
            hexValue = (unsigned int)strtol(argv[indexx], NULL, 0);
            convertTime();
        }
        else
            cout << "Incorrect Format. Command must be in the format MACConversion -T | -D [-f filename | -h hex value]" << endl;
    }
    else if(cmd == "-D"){
        indexx++;
        cmd = argv[indexx];
        if(cmd == "-f"){
            indexx++;
            filePath = argv[indexx];
            getFile();
            convertDate();
        }
        else if(cmd == "-h"){
            indexx++;
            hexValue = (unsigned int)strtol(argv[indexx], NULL, 0);
            convertDate();
        }
        else
            cout << "Incorrect Format. Command must be in the format MACConversion -T | -D [-f filename | -h hex value]" << endl;
    }
    else{
        cout << "Incorrect Format. Command must be in the format MACConversion -T | -D [-f filename | -h hex value]" << endl;
    }
}