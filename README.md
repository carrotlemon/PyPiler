# PyPiler - a Python to C++ transpiler
written in c++20

to run
```bash
make
./pypiler input_file.py
```
Example:
```python
def fizzBuzz(n: int) -> list[str]:
    res = []
    for i in range(1, n+1):
        if i % 3 == 0 and i % 5 == 0:
            res.append("FizzBuzz")
        elif i % 3 == 0:
            res.append("Fizz")
        elif i % 5 == 0:
            res.append("Buzz")
        else:
            res.append(str(i))
    return res
```
Transpiles to
```c++
std::vector<string>fizzBuzz(int n) {
    auto res = std::vector<string>{};
    for(int i = 1; i < n + 1; ++i) {
        if(i % 3 == 0 && i % 5 == 0) {
            res.push_back("FizzBuzz");
        }
        else if(i % 3 == 0) {
            res.push_back("Fizz");
        }
        else if(i % 5 == 0) {
            res.push_back("Buzz");
        }
        else {
            res.push_back(to_string(i));
        }
    }
    return res;
}
```
![alt text](image.png)