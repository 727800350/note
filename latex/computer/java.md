# Spring


# Maven
Maven是一个优秀的项目构建工具.

Maven帮助我们自动化项目构建过程,具体来说包括项目的清理,编译,测试,生成测试报告,打包和部署.
只需要输入很简洁的命令,就能实现这些构建过程.比如输入`mvn clean compile` ,就能完成项目的清理和编译.
同时,Maven提供jar包的依赖管理

## [命令行创建Maven项目](http://my.oschina.net/u/1162561/blog/145869)
1. 打开命令行, 输入 `mvn archetype:generate`
1. 界面上会输出很多信息,并提示输入要选择的archetype编号, 这里我们直接按回车键,即选择默认编号.
1. 然后提示选择其version, 按回车键,选择默认编号对应的version.
1. 之后会提示输入groupId, artifactId, groupId 输入 com.eric.test, artifactId 输入test.(groupId指 创建项目的小组的唯一标示. ArtifactId是 该产品id)
1. 后面的version和package,系统都提供了默认值.如果要采纳默认值,直接按回车键.
1. 最后Y:处,表示是否确认以上信息,填Y按回车则开始创建.
1. 至此,项目创建成功.
1. `mvn compile` 进行编译
1. `mvn test` 进行测试
1. `mvn package` 打包, 会在target 的文件夹生成一个`test-1.0-SNAPSHOT.jar` 的文件, 也就是artifactId-version.jar 的命名方式
1. `java -cp target/test-1.0-SNAPSHOT.jar com.eric.test.App` 就会运行jar 包中的 com.eric.test package下的App(App 是默认创建的含有Main函数的类)

![生成的目录结构](http://i.imgbox.com/OkX8oksm.png)

注意:

1. 如果是第一次构建项目,所有依赖的jar包都要从maven的中央仓库下载.
2. SNAPSHOT表示该版本处于开发阶段,是不稳定的版本.

生成的pom.xml的内容如下:
```
<project xmlns="http://maven.apache.org/POM/4.0.0" xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
  xsi:schemaLocation="http://maven.apache.org/POM/4.0.0 http://maven.apache.org/xsd/maven-4.0.0.xsd">
  <modelVersion>4.0.0</modelVersion>

  <groupId>com.eric.test</groupId>
  <artifactId>test</artifactId>
  <version>1.0-SNAPSHOT</version>
  <packaging>jar</packaging>

  <name>test</name>
  <url>http://maven.apache.org</url>

  <properties>
    <project.build.sourceEncoding>UTF-8</project.build.sourceEncoding>
  </properties>

  <dependencies>
    <dependency>
      <groupId>junit</groupId>
      <artifactId>junit</artifactId>
      <version>3.8.1</version>
      <scope>test</scope>
    </dependency>
  </dependencies>
</project>
```

pom.xml 文件中个条目的解释

- groupId :  创建项目的小组的唯一标示.
- artifactId:  该产品id.会作为产生的jar/war包名的一部分.它和version一起构成输出包的包名.
- Version: 产品版本
- Packaging: 打包的类型
- Name: 项目名称,用于maven产生的文档中.
- url:指定项目站点,通常用于maven产生的文档中. 
- description:描述此项目,通常用于maven产生的文档中.
- Properties: 属性.这里设置编码方式为UTF-8
- Dependencies: 依赖的jar包 

我们注意到,在dependency中,也有<groupId>,<artifactId>,<version>这样的节点.这三个元素是jar包坐标的重要组成部分.它指定了我们要从中央仓库获取的jar.

那么如何知道所要jar的坐标呢?
可以在[Maven搜索站](http://search.maven.org/)中搜索.
在搜索结果中找到需要的jar包,把相应的坐标填写在pom.xml文件中.

此外,我们还注意到,<dependency>下有一个<scope>test</scope>.它设定了jar包的作用域,这里指junit包仅在测试时使用.在正式打包的时候,并不会引入到工程中.
如果去掉这行代码,那么Maven会采用默认的compile范围,那么无论在测试或者打包时,该jar包都会被工程引用,生成的包中会含有junit. 

## Maven Phases
Although hardly a comprehensive list, these are the most common default lifecycle phases executed.

- validate: validate the project is correct and all necessary information is available
- compile: compile the source code of the project
- test: test the compiled source code using a suitable unit testing framework. These tests should not require the code be packaged or deployed
- package: take the compiled code and package it in its distributable format, such as a JAR.
- integration-test: process and deploy the package if necessary into an environment where integration tests can be run
- verify: run any checks to verify the package is valid and meets quality criteria
- install: install the package into the local repository, for use as a dependency in other projects locally
- deploy: done in an integration or release environment, copies the final package to the remote repository for sharing with other developers and projects.
- clean: cleans up artifacts created by prior builds
- site: generates site documentation for this project
mvn eclipse:eclipse 生成 Eclipse 项目文件及包引用定义,注意,需确保定义Classpath Variables: M2_REPO,指向本地maven类库目录.

Phases are actually mapped to underlying goals. The specific goals executed per phase is dependant upon the packaging type of the project. 
For example, package executes jar:jar if the project type is a JAR, and war:war if the project type is - you guessed it - a WAR.

An interesting thing to note is that phases and goals may be executed in sequence.
```
mvn clean dependency:copy-dependencies package
```
This command will clean the project, copy dependencies, and package the project (executing all phases up to package, of course).
