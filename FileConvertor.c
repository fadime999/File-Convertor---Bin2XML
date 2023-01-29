#include <fcntl.h>
#include <io.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

 struct record {
  char name[64]; //utf16
  char surname[32]; //utf8
  char gender;
  char email[32];
  char phone_number[16];
  char address[32];
  char level_of_education[8];
  unsigned int income_level; // given little-endian
  unsigned int expenditure; // given big-endian
  char currency_unit[16];
  char currentMood[32];
  float height;
  unsigned int weight;
};

typedef struct record myRecord;
unsigned int littleEndian(unsigned int);
unsigned int bigEndian(unsigned int);
long chartoInt(char);

int main(int argc, char *argv[]){
  FILE *fp,*xml;
  struct record rItem;
  if((argc) =! 3)
  {
      printf("Please enter 3 arguments\n");
      exit(1);
  }

  fp = fopen(argv[2],"rb"); //Binary file
  if (fp == NULL)
  {
      printf("Unable to open file!");
			exit(1);
  }

  xml = fopen(argv[3],"w");//XML file creation
  if(xml == NULL)
  {
     puts("Error opening file!");
     exit (1);
  }
   fprintf(xml,"<?xml version=\"1.0\" encoding=\"UTF-8\"?>");
   fprintf(xml,"\n<%s>",argv[2]);
  for (int i = 0; i <51; i++)
  {
    fread(&rItem, sizeof(myRecord), 1, fp);//Binary file reading
    //XML file writing
    fprintf(xml,"\n\t<row id=\"%d\">",i);
    fprintf(xml,"\n\t\t<name>%s</name>",rItem.name);
    fprintf(xml,"\n\t\t<surname>%s</surname>",rItem.surname);
    fprintf(xml,"\n\t\t<gender>%c</gender>",rItem.gender);
    fprintf(xml,"\n\t\t<email>%s</email>",rItem.email);
    fprintf(xml,"\n\t\t<phone_number>%s</phone_number>",rItem.phone_number);
    fprintf(xml,"\n\t\t<address>%s</address>",rItem.address);
    fprintf(xml,"\n\t\t<level_of_education>%s</level_of_education>",rItem.level_of_education);
    fprintf(xml,"\n\t\t<currency_unit>%s</currency_unit>",rItem.currency_unit);
    fprintf(xml,"\n\t\t<currentMood>%s</currentMood>",rItem.currentMood);
    fprintf(xml,"\n\t\t<height>%.2f</height>",rItem.height);
    fprintf(xml,"\n\t\t<weight>%d</weight>",rItem.weight);
    fprintf(xml,"\n\t</row>");
    
    printf("\nIncomeLevel -> id %d : %u",i,littleEndian(rItem.income_level));
    printf("\nExpenditure -> id %d : %u\n",i,bigEndian(rItem.expenditure));    
  }
 
   fprintf(xml,"\n</%s>",argv[2]);
   puts("\nRecord written...");
   fclose(xml);
   fclose(fp);
   return 0;
}

unsigned int littleEndian(unsigned int incomeLevel){
    char str[9];
    sprintf(str,"%x",incomeLevel);
    for (int i = strlen(str); i < 8; i++) 
      str[i] = '0';

    // str = 376b0000 -> 00006b37
    char firstPart[3] = {str[6],str[7],'\0'}; // firstPart = 00
    char secondPart[3]= {str[4],str[5],'\0'}; // secondPart = 00
    char thirdPart[3]=  {str[2],str[3],'\0'}; // thirdPart = 6b
    char fourthPart[3]= {str[0],str[1],'\0'}; // fourthPart = 37
   
    long result = 0;
    result = result + chartoInt(firstPart[0]) * pow(16,7); // 0 * 16^7
    result = result + chartoInt(firstPart[1]) * pow(16,6); // 0 * 16^6
   
    result = result + chartoInt(secondPart[0]) * pow(16,5); // 0 * 16^5
    result = result + chartoInt(secondPart[1]) * pow(16,4); // 0 * 16^4

    result = result + chartoInt(thirdPart[0]) * pow(16,3); // 6 * 16^3
    result = result + chartoInt(thirdPart[1]) * pow(16,2); // b * 16^2

    result = result + chartoInt(fourthPart[0]) * pow(16,1); // 3 * 16^1
    result = result + chartoInt(fourthPart[1]) * pow(16,0); // 7 * 16^0

    return result;
}

unsigned int bigEndian(unsigned int expenditure){
    char str[9];
    sprintf(str,"%x",expenditure);
    for (int i = strlen(str); i < 8; i++) 
      str[i] = '0';

    // str = E0090000
    char firstPart[3] = {str[6],str[7],'\0'}; // firstPart = 00
    char secondPart[3]= {str[4],str[5],'\0'}; // secondPart = 00
    char thirdPart[3]= {str[2],str[3],'\0'};  // thirdPart = 09
    char fourthPart[3]= {str[0],str[1],'\0'}; // fourthPart = E0

    unsigned int result = 0;

    result = result + chartoInt(firstPart[1]) * pow(16,0); // 0 * 16^0
    result = result + chartoInt(firstPart[0]) * pow(16,1);  // 0 * 16^1

    result = result + chartoInt(secondPart[1]) * pow(16,2); // 0 * 16^2
    result = result + chartoInt(secondPart[0]) * pow(16,3); // 0 * 16^3

    result = result + chartoInt(thirdPart[1]) * pow(16,4); // 9 * 16^4
    result = result + chartoInt(thirdPart[0]) * pow(16,5); // 0 * 16^5

    result = result + chartoInt(fourthPart[1]) * pow(16,6); // 0 * 16^6
    result = result + chartoInt(fourthPart[0]) * pow(16,7); // E * 16^7

    return result;
}

long chartoInt(char c){ //Values that will be used to convert hexaden to decimal
   if (c == 'a' || c=='A')
      return 10;
   else if(c == 'b' || c=='B')
      return 11;
   else if(c == 'c' || c=='C')
      return 12;
   else if(c == 'd' || c=='D')
      return 13;
   else if(c == 'e' || c=='E')
      return 14;
   else if(c == 'f' || c=='F')
      return 15;
   else
      return c-'0';
}