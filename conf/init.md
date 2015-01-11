# YUM Downgrade Basic Usage
how to downgrade packages using YUM (example) on Fedora, CentOS, Red Hat (RHEL). 

Downgrade is very straightforward when package have not any dependencies, which affect the downgrade.
YUM Downgrade Single Package
```
yum downgrade package
```

YUM Downgrade Package Which Have Dependencies

YUM downgrade is more difficult with packages that have dependencies. YUM downgrade does not resolve dependencies automatically, so it must be done manually.
YUM Downgrade Package and Dependencies
```
yum downgrade lib1 lib2 package
```

例如
```
yum downgrade sqlite
```
但是报错了, 提示如下 
```
Setting up Downgrade Process
Resolving Dependencies
--> Running transaction check
---> Package sqlite.i686 0:3.6.22-1.fc13 set to be downgraded
---> Package sqlite.i686 0:3.6.23-1.fc14 set to be erased
--> Finished Dependency Resolution
Error: Package: sqlite-devel-3.6.23-1.fc14.i686 (@rawhide/12)
           Requires: sqlite = 3.6.23-1.fc14
           Removing: sqlite-3.6.23-1.fc14.i686 (@rawhide/12)
               sqlite = 3.6.23-1.fc14
           Downgraded By: sqlite-3.6.22-1.fc13.i686 (fedora)
               sqlite = 3.6.22-1.fc13
You could try using --skip-broken to work around the problem
```
看到有依赖关系, 所以需要将sqlite-devel 也一起降级
SQLite package depends on sqlite-devel package, so both (sqlite-devel and sqlite) have to be downgraded:
```
yum downgrade sqlite-devel sqlite
```
