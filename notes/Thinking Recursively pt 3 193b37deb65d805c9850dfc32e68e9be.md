# Thinking Recursively pt. 3

Type: Note
Description: Lecture notes for 01/24/2025
Last Edited: February 8, 2025 8:53 PM

## Iterations + Recursion

- It’s totally fine to incorporate for loops with recursion.
- If you want to make several recursive calls, you can do so with a for loop and call the recursive function within the for loop. View *Sierpinski Diagram code example*
- Your recursive function can also return something like the amount of times that it is called

### Example

```cpp
int drawCarpet(double x, double y, double sideLength, int order) {
    // Base case: If order is 0, draw a square
    if (order == 0) {
        drawSquare(x, y, sideLength);
        return 1;
    }

    // Recursive case: Draw eight smaller Sierpinski carpets
    int total = 0;
    double newLength = sideLength / 3;

    for (int row = 0; row < 3; row++) {
        for (int col = 0; col < 3; col++) {
            // Skip the center square
            if (row != 1 || col != 1) {
                total += drawCarpet(x + col * newLength, 
                                    y + row * newLength, 
                                    newLength, 
                                    order-1);
            }
        }
    }

    return total;
}
```

### Wrapper Functions

Sometimes you need to pass two functions where one doesn’t need all parameters. So to make certain parameters optional you can write a wrapper function that passes the default of the otherwise unnecessary parameters.