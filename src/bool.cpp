#include <iostream>
using namespace std;


int main() {
   int time;
   cout << "Waht is the time now?";
   cin >> time;
if (time < 10) {
  cout << "Good morning.";
} else if (time <= 20) {
  cout << "Good day.";
} else {
  cout << "Good evening.";
}
}