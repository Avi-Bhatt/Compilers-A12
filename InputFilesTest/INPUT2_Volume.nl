/* 
* NIVLang Example:
* Volume of a sphere 
*/

fun main() {
    float PI = 3.14;
    int r = 0;
    float Vol = 0;
    
    print("Enter radius: ");
    r=input();
    Vol = 4 / 3 * PI * (r * r * r);
    
    print("Volume of sphere: " + Vol);
}