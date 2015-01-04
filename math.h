#ifndef MATH_H_LOCAL
#define MATH_H_LOCAL

//Replace the standard arduino map() function to use the div function instead
long fastMap(int x, int in_min, int in_max, int out_min, int out_max)
{
  return div( ((x - in_min) * (out_max - out_min)) , ((in_max - in_min) + out_min) ).quot;
}

#endif // MATH_H_LOCAL

