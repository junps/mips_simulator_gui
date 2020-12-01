#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <string>
#include <iostream>

using namespace std;

int main() {
  string file_name = "/home/junps/Desktop/min-caml/shootout/mandelbrot_instr.mem";
  int n = file_name.size();
  cout << "str len : " << n  << endl;
  string base_name = file_name.substr(0, n - 9);
  cout << "base_name : " << base_name << endl;
  string data_name = base_name + "data.mem";
  string debug_name = base_name + "debug.mem";
  cout << "data : " << data_name << endl;
  cout << "debug : " << debug_name << endl;
}
