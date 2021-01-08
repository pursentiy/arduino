#include "TM1637.h"

//Screen
int8_t DispMSG[4];
unsigned long last_time = 0;
#define CLK 3
#define DIO 2
TM1637 tm1637(CLK, DIO);
int _yearCounter = 1949;
int _yearLength;
int _numberPieces[4];
bool _finishYearcounter = false;

int GetNumberLength(int number)
{
  int length = 0;
  int n = number;
  while (n / 10 >= 0)
  {
    n /= 10;
    length++;
    if (n == 0)
      break;
  }

  return length;
}

void GetNumberPieces(int year, int numberYears)
{
  String substructNumber = "";
  int numberToCheck = 0;
  for (int yearSize = 0; yearSize < numberYears; yearSize++)
  {
    for (int i = 0; i <= 9; i++)
    {
      if (substructNumber == "")
      {
        numberToCheck = year - i;
      }
      else
      {
        numberToCheck = year - (String(i) + substructNumber).toInt();
      }

      int checkNumber = round(pow(10, yearSize + 1));

      if (numberToCheck % checkNumber == 0)
      {
        _numberPieces[numberYears - yearSize - 1] = i;
        substructNumber = i + substructNumber;
        break;
      }
    }
  }
}

void SetTimer()
{
  DispMSG[0] = _numberPieces[0]; //(int)(yearString[0]);
  DispMSG[1] = _numberPieces[1]; //(int)(yearString[1]);
  DispMSG[2] = _numberPieces[2]; //(int)(yearString[2]);
  DispMSG[3] = _numberPieces[3]; //(int)(yearString[3]);
  tm1637.display(DispMSG);
}

void IncrementTimer(int arraylength, int incrementNumber, int maxYear, int delay)
{
  bool ifSkip = false;
  String resultNumber = "";

  for (int i = arraylength - 1; i >= 0; i--)
  {

    if (ifSkip)
    {
      resultNumber = _numberPieces[i] + resultNumber;
      continue;
    }

    if (_numberPieces[i] + incrementNumber == 10)
    {
      _numberPieces[i] = 0;
      resultNumber = _numberPieces[i] + resultNumber;
    }
    else
    {
      _numberPieces[i] += incrementNumber;
      ifSkip = true;
      resultNumber = _numberPieces[i] + resultNumber;
    }
  }

  _yearCounter = resultNumber.toInt();
  Serial.println(resultNumber);

  if (_yearCounter >= maxYear)
  {
    _finishYearcounter = true;
  }
}


void setup()
{
  Serial.begin(115200);
  // tm1637.init();
  // tm1637.set(BRIGHT_TYPICAL);
  /*
     BRIGHT_TYPICAL = 2 Средний
     BRIGHT_DARKEST = 0 Тёмный
     BRIGHTEST = 7      Яркий
  */

  _yearLength = GetNumberLength(_yearCounter);

  // Serial.println(_numberPieces[0]);
  // Serial.println(_numberPieces[1]);
  // Serial.println(_numberPieces[2]);
  // Serial.println(_numberPieces[3]);

  // IncrementTimer(_yearCounter, 1, 2000, 1000);

  // Serial.println(_numberPieces[0]);
  // Serial.println(_numberPieces[1]);
  // Serial.println(_numberPieces[2]);
  // Serial.println(_numberPieces[3]);

}

void loop()
{
  GetNumberPieces(_yearCounter, _yearLength);
  IncrementTimer(_yearCounter, 1, 2000, 1000);

  // Serial.print(_numberPieces[0]);
  // Serial.print(_numberPieces[1]);
  // Serial.print(_numberPieces[2]);
  // Serial.print(_numberPieces[3]);
  delay(1000);
}
