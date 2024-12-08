# LISP Interpreter

This project implements a simple LISP interpreter in C++. The interpreter supports a variety of mathematical, logical, list, and conditional operations. Users can define functions, assign variables, and interact with the interpreter via a REPL (Read-Eval-Print Loop).

---

## 1️⃣ **Commands and Syntax**

This section provides the syntax of each LISP command supported by the interpreter.

### 📘 **1.1 Mathematical Operations**
| **Command**       | **Description**                  |
|-------------------|-----------------------------------|
| `(+ a b)`         | Returns the sum of `a` and `b`.   |
| `(- a b)`         | Subtracts `b` from `a`.           |
| `(* a b)`         | Multiplies `a` and `b`.           |
| `(/ a b)`         | Divides `a` by `b`.               |

---

### 📘 **1.2 Logical Operations**
| **Command**          | **Description**                        |
|----------------------|-----------------------------------------|
| `(and? a b)`         | Returns `T` if both `a` and `b` are truthy. |
| `(or? a b)`          | Returns `T` if either `a` or `b` is truthy. |

---

### 📘 **1.3 List Operations**
| **Command**           | **Description**                        |
|----------------------|-----------------------------------------|
| `(cons a b)`          | Creates a pair with `a` as the head and `b` as the tail. |
| `(car a)`             | Returns the first element of the list `a`. |
| `(cdr a)`             | Returns the tail of the list `a`.       |

---

### 📘 **1.4 Conditionals**
| **Command**                          | **Description**                                              |
|--------------------------------------|-------------------------------------------------------------|
| `(if condition true-branch false-branch)` | If `condition` is truthy, returns `true-branch`, otherwise returns `false-branch`. |
| `(cond cond1 expr1 cond2 expr2 ...)` | Returns the first `expr` where `cond` is truthy. |

---

### 📘 **1.5 Definition and Assignment**
| **Command**                             | **Description**                            |
|-----------------------------------------|------------------------------------------|
| `(set var value)`                       | Assigns `value` to `var`.                 |
| `(define function-name (params) body)` | Defines a new function.                   |
| `(function-name param1 param2 ...)`          | Calls a user defined function             | 

---

### 📘 **1.6 Type Checking**
| **Command**           | **Description**                          |
|----------------------|-------------------------------------------|
| `(number? a)`         | Returns `T` if `a` is a number.            |
| `(symbol? a)`         | Returns `T` if `a` is a symbol.            |
| `(list? a)`           | Returns `T` if `a` is a list.              |
| `(nil? a)`            | Returns `T` if `a` is `nil`.               |

---

## 2️⃣ **Testing Instructions**

Test cases are in `test_cases.txt` and Test results are in `test_results.txt`. 

To test the LISP interpreter, follow these steps:

1. **Build the project**:  
   ```bash
   make
   ```

2. **Run the test suite**:
    ```bash
    python3 tester.py
    ```

## 3️⃣ Example Usage

Here is an example session with the LISP interpreter:
```bash
user (CTRL + C to exit) > (+ 1 2)
3

user (CTRL + C to exit) > (define square (x) (* x x))
<fn>

user (CTRL + C to exit) > (square 4)
16
```

## 🔧 Build Instructions

Ensure you have g++, make, and python3 installed on your system.

Use the make command to compile the program.
Run ./main to launch the LISP interpreter.