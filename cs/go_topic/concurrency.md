When we cannot confidently say that one event happends before the other, then the events x and y are concurrent.

A data race occurs whenever two goroutines access the same variable concurrently and at least one of the accesses is a write.

Mutex locks are not re-entrant - it's not possible to lock a mutex that's already locked.

The Race Detector

Just add the -race flag to your go build, go run or go test command.
This causes the compiler to build a modified version of your application or test with additional instrumentation that effectively records all accesses to shared variables that occurred during execution,
along with the identity of the goroutine that read or write the variable.
In addition, the modified program records all synchronization events, such as go statements, channel operations and calls to `(*sync.Mutex).Lock, (*sync.WaitGroup).Wait` and so on.

The race detector studies this stream of events, looking for cases in which one goroutine reads or writes a shared variable that was most recently written by a different goroutine without an intervening synchronization operation.
This indicates a concurrent access to the shared variable, and thus a data race.

# Concurrent Non-Blocking Cache
```go
```
