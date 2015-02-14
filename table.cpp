/*
Because the size of the table is dynamic, this functino is required to reallocate the array sizes
Note that this may clear some of the existing values of the table
*/
#include "table.h"

void table2D_setSize(struct table2D* targetTable, byte newSize)
{
  //2D tables can contain either bytes or ints, depending on the value of the valueSize field
  if(targetTable->valueSize == SIZE_BYTE)
  {
    targetTable->values = (byte *)realloc(targetTable->values, newSize * sizeof(byte));
    targetTable->axisX = (byte *)realloc(targetTable->axisX, newSize * sizeof(byte));
    targetTable->xSize = newSize;
  }
  else
  {
    targetTable->values16 = (int *)realloc(targetTable->values16, newSize * sizeof(int));
    targetTable->axisX16 = (int *)realloc(targetTable->axisX16, newSize * sizeof(int));
    targetTable->xSize = newSize;
  }
}

/*
This function simply pulls a 1D linear interpolated (ie averaged) value from a 2D table
ie: Given a value on the X axis, it returns a Y value that coresponds to the point on the curve between the nearest two defined X values

This function must take into account whether a table contains 8-bit or 16-bit values. 
Unfortunately this means many of the lines are duplicated depending on this
*/
int table2D_getValue(struct table2D fromTable, int X)
{

    int xMinValue, xMaxValue;
    if (fromTable.valueSize == SIZE_BYTE)
    {
      //Byte version
      xMinValue = fromTable.axisX[0];
      xMaxValue = fromTable.axisX[fromTable.xSize-1];
    }
    else
    {
      //int version
      xMinValue = fromTable.axisX16[0];
      xMaxValue = fromTable.axisX16[fromTable.xSize-1];
    }
    int xMin = 0;
    int xMax = 0;
    
    //If the requested X value is greater/small than the maximum/minimum bin, reset X to be that value
    if(X > xMaxValue) { X = xMaxValue; }
    if(X < xMinValue) { X = xMinValue; }  

    for (int x = fromTable.xSize-1; x >= 0; x--)
    {
       if (fromTable.valueSize == SIZE_BYTE)
       {
          //Byte version
          //Checks the case where the X value is exactly what was requested
          if ( (X == fromTable.axisX[x]) || (x == 0) )
          {
            return fromTable.values[x]; //Simply return the coresponding value
          }
          //Normal case
          if ( (X <= fromTable.axisX[x]) && (X > fromTable.axisX[x-1]) )
          {
            xMaxValue = fromTable.axisX[x];
            xMinValue = fromTable.axisX[x-1];
            xMax = x;
            xMin = x-1;
            break;
          }
       }
       else
       {
         //int version
         if ( (X == fromTable.axisX16[x]) || (x == 0) )
          {
            return fromTable.values16[x]; //Simply return the coresponding value
          }
          //Normal case
          if ( (X <= fromTable.axisX16[x]) && (X > fromTable.axisX16[x-1]) )
          {
            xMaxValue = fromTable.axisX16[x];
            xMinValue = fromTable.axisX16[x-1];
            xMax = x;
            xMin = x-1;
            break;
          }
       }
    }
    
    unsigned int m = X - xMinValue;
    unsigned int n = xMaxValue - xMinValue;
    
    //Float version
    /*
    int yVal = (m / n) * (abs(fromTable.values[xMax] - fromTable.values[xMin]));
    */
    
    //Non-Float version
    int yVal;
    if (fromTable.valueSize == SIZE_BYTE)
    {
       //Byte version
       yVal = ((long)(m << 6) / n) * (abs(fromTable.values[xMax] - fromTable.values[xMin]));
       yVal = (yVal >> 6);
        
       if (fromTable.values[xMax] > fromTable.values[xMin]) { yVal = fromTable.values[xMin] + yVal; }
       else { yVal = fromTable.values[xMin] - yVal; }
    }
    else
    {
       //int version
       yVal = ((long)(m << 6) / n) * (abs(fromTable.values16[xMax] - fromTable.values16[xMin]));
       yVal = (yVal >> 6);
        
       if (fromTable.values[xMax] > fromTable.values16[xMin]) { yVal = fromTable.values16[xMin] + yVal; }
       else { yVal = fromTable.values16[xMin] - yVal; }
    }
    
    return yVal;
}




//This function pulls a value from a 3D table given a target for X and Y coordinates.
//It performs a 2D linear interpolation as descibred in: http://www.megamanual.com/v22manual/ve_tuner.pdf
int get3DTableValue(struct table3D fromTable, int Y, int X)
  {
    //Loop through the X axis bins for the min/max pair
    //Note: For the X axis specifically, rather than looping from tableAxisX[0] up to tableAxisX[max], we start at tableAxisX[Max] and go down. 
    //      This is because the important tables (fuel and injection) will have the highest RPM at the top of the X axis, so starting there will mean the best case occurs when the RPM is highest (And hence the CPU is needed most)
    int xMinValue = fromTable.axisX[0];
    int xMaxValue = fromTable.axisX[fromTable.xSize-1];
    int xMin = 0;
    int xMax = 0;
    
    //If the requested X value is greater/small than the maximum/minimum bin, reset X to be that value
    if(X > xMaxValue) { X = xMaxValue; }
    if(X < xMinValue) { X = xMinValue; }
    
    for (int x = fromTable.xSize-1; x >= 0; x--)
    {
      //Checks the case where the X value is exactly what was requested
      if ( (X == fromTable.axisX[x]) || (x == 0) )
      {
        xMaxValue = fromTable.axisX[x];
        xMinValue = fromTable.axisX[x];
        xMax = x;
        xMin = x;
        break;
      }
      //Normal case
      if ( (X <= fromTable.axisX[x]) && (X > fromTable.axisX[x-1]) )
      {
        xMaxValue = fromTable.axisX[x];
        xMinValue = fromTable.axisX[x-1];
        xMax = x;
        xMin = x-1;
        break;
      }   
    }
    
    //Loop through the Y axis bins for the min/max pair
    int yMaxValue = fromTable.axisY[0];
    int yMinValue = fromTable.axisY[fromTable.ySize-1];
    int yMin = 0;
    int yMax = 0;
    
    //If the requested Y value is greater/small than the maximum/minimum bin, reset Y to be that value
    if(Y > yMaxValue) { Y = yMaxValue; }
    if(Y < yMinValue) { Y = yMinValue; }
    
    for (int y = fromTable.ySize-1; y >= 0; y--)
    {
      //Checks the case where the Y value is exactly what was requested
      if ( (Y == fromTable.axisY[y]) || (y==0) )
      {
        yMaxValue = fromTable.axisY[y];
        yMinValue = fromTable.axisY[y];
        yMax = y;
        yMin = y;
        break;
      }
      //Normal case
      if ( (Y >= fromTable.axisY[y]) && (Y < fromTable.axisY[y-1]) )
      {
        yMaxValue = fromTable.axisY[y];
        yMinValue = fromTable.axisY[y-1];
        yMax = y;
        yMin = y-1;
        break;
      }   
    }
        
    
    /* 
    At this point we have the 4 corners of the map where the interpolated value will fall in
    Eg: (yMin,xMin)  (yMin,xMax)
    
        (yMax,xMin)  (yMax,xMax)
        
    In the following calculation the table values are referred to by the following variables:
              A          B
              
              C          D
    
    */
    int A = fromTable.values[yMin][xMin];
    int B = fromTable.values[yMin][xMax];
    int C = fromTable.values[yMax][xMin];
    int D = fromTable.values[yMax][xMax];
    
    //Create some normalised position values
    //These are essentially percentages (between 0 and 1) of where the desired value falls between the nearest bins on each axis
    
    // Float version
    /*
    float p, q;
    if (xMaxValue == xMinValue)
    { p = (float)(X-xMinValue); }
    else { p = ((float)(X - xMinValue)) / (float)(xMaxValue - xMinValue); }
    
    if (yMaxValue == yMinValue)
    { q = (float)(Y - yMinValue); }
    else { q = ((float)(Y - yMaxValue)) / (float)(yMinValue - yMaxValue); }
    
    float m = (1.0-p) * (1.0-q);
    float n = p * (1-q);
    float o = (1-p) * q;
    float r = p * q;
    
    
    return ( (A * m) + (B * n) + (C * o) + (D * r) ); 
    */
    
    // Non-Float version:
    //Initial check incase the values were hit straight on
    long p;
    if (xMaxValue == xMinValue)
    { p = ((long)(X - xMinValue) << 8); } //This only occurs if the requested X value was equal to one of the X axis bins
    else 
    { 
      p = ((long)(X - xMinValue) << 8) / (xMaxValue - xMinValue); //This is the standard case
    } 
    
    long q;
    if (yMaxValue == yMinValue)
    { q = ((long)(Y - yMinValue) << 8); }
    else
    { q = ((long)(Y - yMaxValue) << 8) / (yMinValue - yMaxValue); }
      
    int m = ((257-p) * (257-q)) >> 8;
    int n = (p * (257-q)) >> 8;
    int o = ((257-p) * q) >> 8;
    int r = (p * q) >> 8;

    return ( (A * m) + (B * n) + (C * o) + (D * r) ) >> 8;
  }
