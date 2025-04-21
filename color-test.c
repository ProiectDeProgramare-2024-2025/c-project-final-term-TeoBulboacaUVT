// color your text in Windows console mode
// colors are 0=black 1=blue 2=green and so on to 15=white  
// colorattribute = foreground + background * 16
// to get red text on yellow use 4 + 14*16 = 228
// light red on yellow would be 12 + 14*16 = 236
// a Dev-C++ tested console application by  vegaseat  07nov2004
#include <stdio.h>    // standard I/O header
#include <windows.h>   // WinApi header

int main()
{
  HANDLE  hConsole;
	int colorattribute = 0; // color attribute
	
  hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    while (TRUE)
    {
      scanf("%d", &colorattribute); // get color attribute from user
      if (colorattribute < 0 || colorattribute > 255) // check for valid input
      {
        printf("Please enter a number between 0 and 255.\n");
        continue; // skip to next iteration of the loop
      }
      if (colorattribute == 0) // exit on 0
        break; // exit the loop

        
    // pick the colorattribute k you want
    SetConsoleTextAttribute(hConsole, colorattribute);
    printf("This is color %d\n", colorattribute);

    //reset colorattribute to default
    SetConsoleTextAttribute(hConsole, 7); // default color is 7 (light gray on black)

    }
    


  system("pause"); // pause to see the last color
  return 0;
}