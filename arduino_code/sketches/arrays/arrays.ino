#define LENGTH 10

//Create a global variable array called x
int x[LENGTH];

void fill_array(int arr[], int len)
{
  int r;
  for (int i = 0; i < len; i++)
  {
    randomSeed(analogRead(0));
    r = random(0, 100);
    arr[i] = r;
  }
}

void print_array(int arr[], int len)
{
  for (int i = 0; i < len; i++)
  {
    Serial.print(arr[i]);
    Serial.print(" | ");
  }
  Serial.println();
}

void setup() {
  Serial.begin(9600);
}

void loop() {
  fill_array(x, LENGTH);
  print_array(x, LENGTH);
}
