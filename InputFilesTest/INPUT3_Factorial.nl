/* 
* NIVLang Example:
* Testing numbers (calculating factorial)
*/

fun main() {
    int i = 1;
    int n = 0;
    int f = 1;
    
    print("Write a number:");
    n = input();
    
    while (i < n) {
        f = f * i;
        i = i + 1;
    }
    
    print("Factorial:"+f);
}