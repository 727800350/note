gdb program

**Shortcuts**  
break, delete, run, continue, step, next and print  
**you need only use the first letter**

# Running the program

- run: Reset the program, run (or rerun) from the beginning. You can supply command-line arguments to 'run' the same way you can supply command-line arguments to your executable from the shell.  
**程序的参数是通过run给的**, eg: `(gdb) run -i wlan0 -m 10`

- step: Run next line of source and return to debugger. If a subroutine call is encountered, **follow into that subroutine**

- step count: Run count lines of source

- next: Similar to step, but **doesn't step into subroutines**

- finish: Run until the current function/method returns

- return:	Make selected stack frame return to its caller.

- jump address:	Continue program at specified **line or address**

- quit:	退出调试

# Breakpoints

- info break: Prints a list of all breakpoints with numbers and status

- break function: Place a breakpoint at start of the specified function 

- break linenumber: Place a breakpoint at line, relative to current source file

- break filename:linenumber: Place a breakpoint at the specified line within the specified source file

- break fn if expression: Stop at the breakpoint, only if expression  evaluates to true.  Expression is any valid C expression, evaluated within current stack frame when hitting the breakpoint. 

- disable breaknum: Disable/enable breakpoint identified by breaknum.. 

- enable breaknum: Disable/enable breakpoint identified by breaknum.. 

- delete breaknum: Delete the breakpoint identified by breaknum

- commands breaknum: Specify commands to be executed when breaknum is reached.  The commands can be any list of C statements or gdb commands.  This can be useful to fix code on-the-fly in the debugger without re-compiling

- cont: Continue a program that has been stopped

# Examining Stack

The runtime stack is like a "trail of breadcrumbs" in a program; each time a function call is made, a "crumb is dropped" (an RT stack frame is pushed).  
When a return from a function occurs, the corresponding RT stack frame is popped and discarded.

**gdb assigns numbers to stack frames counting from zero** for the innermost (currently executing) frame
 
- backtrace: Show stack frames, useful to find the calling sequence that produced a crash.

- frame framenumber: Start examining the frame with framenumber.  
This does not change the execution context, but allows to examine variables for a different frame.  每个函数调用在堆栈中称为一帧

- down: Select and print stack frame called by this one. 

- up: Select and print stack frame that called this one

- info args: Show the argument variables of current stack frame. 

- info locals: Show the local variables of current stack frame.

- where: 输出堆栈中函数的调用记录，最近调用的在最前面输出

# Examining source files
- list linenum: Print ten lines centered around linenum in current source file. 

- list function: Print ten lines centered around beginning of function (or method). 

- list: Print ten more lines.

# Examining data
- print expression: Print value of expression. Expression is any valid C expression, can include function calls and arithmetic expressions, all evaluated within current stack frame

- set variable = expression: Assign value of variable to expression.  You can set any variable in the current scope.  Variables which begin with $ can be used as convenience variables in gdb.

- display expression: Print value of expression each time the program stops.  This can be useful to watch the change in a variable as you step through code. 

- undisplay: Cancels previous display requests. 

- what i: 观察变量i 的变化，一旦指定变量发生变化就停止程序

you can specify **how many elements you want to print from a the n-th element** in the array by using this extension to the print command:  
`print array[starting_point]@num_elements_to_print`  
This works on the display command also.  

For example, if you want to print ten elements of the array x starting from the 0th element, you would issue this:  
`print x[0]@10`

