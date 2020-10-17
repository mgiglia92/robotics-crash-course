// Array Task 1

void setup() {
  Serial.begin(9600);

  // Creat an array of type int and size 10:
  int int_arr[10];

  // Using a while loop, fill the array with multiples of 3 between 3 and 30 (inclusive):
                // Remember the syntax?
                // while (condition) {
                //       code...
                // }
  int i=0;
  while(i<10){
    int_arr[i] = (i*3)+3;
    i++;
    }
 



  // Rewrite the code above with a for loop:
                // Remember the syntax?
                // for (initialization; condition; update variable) {
                //       code...
                // }
  for (int j=0; j<10; j++)
  {
    int_arr[i] = (i*3)+3;

  }



  // Write a loop that will go through the array 
  //     and print any values that are a multiple of 9:
                // Hint: Use the % (modulus) operator.
for(int k = 0; k<10; k++)
{
  if(int_arr[k]%9==0){Serial.println(int_arr[k]);}
}

}

void loop() {
  // put your main code here, to run repeatedly:

}
