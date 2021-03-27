- 一个文件夹下一般只能有一个包名
- 但以`_test.go` 作为结尾的源码文件，可以用 `<被测试的包名>_test` 作为包名，放置在相同源码目录下而不产生编译错误
- 同一个package中, 只能有一个main 方法
- 同一个package 的test 中, 只能有一个 `func TestMain(m *testing.M)`
- If the package has multiple .go files, they are initialized in the order in which the files are given to the compiler; the go tool sorts .go files by name before invoking the compiler.
- The main package is the last to be initialized. In this manner, all packages are fully initialized before the application's main function begins.

