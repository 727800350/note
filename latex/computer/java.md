# Java
[环境变量配置](http://jingyan.baidu.com/article/f96699bb8b38e0894e3c1bef.html)

## Cast
First, you must understand, that by casting you are not actually changing the object itself, you are just labeling it differently.

[Cast ex](../../demo/java/Cast.java)

![Cast example](http://uploads.codecall.net/monthly_05_2014/post-27003-0-90059300-1399049433.jpg)

For example, if you create a Cat and upcast it to Animal, then the object doesn't stop from being a Cat. 
It's still a Cat, but it's just treated as any other Animal and it's Cat properties are **hidden**(数据还是在实实在在的在那里, 只是无法访问) until it's downcasted to a Cat again.
```
Mammal m = (Mammal)new Cat();
```
is equal to
```
Mammal m = new Cat();
```
Upcasting 是自动进行的.

m 具有cat类型的数据, 但是m 是一个Mammal 类的实例.
也就是说 `m.f_cat();` 不能编译通过, `((Cat) m).f_cat()` 编译通过且运行正常.

But downcasting must always be done manually.
```
Mammal m = new Mammal();
Cat c = (Cat)m;
```
Such code passes compiling, but throws "java.lang.ClassCastException: Mammal cannot be cast to Cat" exception during running, 
because it is trying to cast a Mammal, which is not a Cat, to a Cat.

Variables can hold instance of objects that are equal or are hierarchically below them. 
For example Cat c; can hold instances of Cat and anything that is extended from a Cat. Animal can hold Animal, Mammal, etc..  
Remember, that instances will always be upcasted to the variable level.

## Data Type
### Array与ArrayList的主要区别
1. 精辟阐述: 可以将 ArrayList想象成一种"会自动扩增容量的Array".

1. Array():最高效,但是其容量固定且无法动态改变, ArrayList: 容量可动态增长,但牺牲效率,

1. Java中一切皆对象,Array也是对象.不论你所使用得Array型别为何, Array名称本身实际上是个reference,指向heap之内得某个实际对象.这个对象可经由"Array初始化语法"被自动产生,也可以以new表达式手动产生.

1. Array可做为函数返回值,因为它本身是对象的reference

1. 对象数组与基本类型数组在运用上几乎一模一样,唯一差别在于,前者持有得是reference,后者直接持有基本型别之值,

1. 容器所持有的其实是一个个reference指向Object,进而才能存储任意型别.当然这不包括基本型别,因为基本型别并不继承自任何classes.

1. 面对Array,我们可以直接持有基本型别数值的Array(例如:`int [] num;`),也可以持有reference(指向对象)的Array, 但是容器类仅能持有reference(指向对象),若要将基本型别置于容器内,需要使用wrapper类.
但是wrapper类使用起来可能不很容易上手,此外,primitives Array的效率比起"容纳基本型别之外覆类(的reference)"的容器好太多了. 
当然,如果你的操作对象是基本型别,而且需要在空间不足时自动扩增容量,Array便不适合,此时就得使用外覆类的容器了.

1. 某些情况下,容器类即使没有转型至原来的型别,仍然可以运作无误.有一种情况尤其特别:编译器对String class提供了一些额外的支持,使它可以平滑运作.

1. 对数组的一些基本操作,像排序,搜索与比较等是很常见的.因此在Java中提供了Arrays类协助这几个操作:sort(),binarySearch(),equals(),fill(),asList().
不过Arrays类没有提供删除方法,而ArrayList中有remove()方法,不知道是否是不需要在Array中做删除等操作的原因(因为此时应该使用链表).

1. ArrayList的使用也很简单:产生ArrayList,利用add()将对象置入,利用get(i)配合索引值将它们取出.这一切就和Array的使用方式完全相同,只不过少了`[]`而已.

1. 类型识别: ArrayList存入对象时,抛弃类型信息,所有对象屏蔽为Object,编译时不检查类型,但是运行时会报错.

1. ArrayList可以存任何Object,如String等.

## Junit Test
对于Web 项目, 通过Junit Test, 可以不必把项目部署到tomcat 上就可以对业务逻辑进行测试.
而且, 实际的项目开发, 业务逻辑和前段展现, 很可能不是同一个人进行, 所以进行业务逻辑的测试时不应该依赖于前端.

测试完成后, 要恢复现场. 例如测试数据库的插入方法, 测试之后, 需要把插入的测试数据删除掉.

# Spring
entity(model) --- service --- DAO -- DAOImplementation
解析xml 文件: jdom, dom4j

Spring在读取xml配置文件中的bean的时候采用的是反射机制, 读取的jvm的.class文件(采用classLoader), 然后再用调用类的方法.

动态装配

AOP: Aspect Oriented Programming

The Spring Framework

- **Core Container** consists of the Core, Beans, Context, and Expression Language modules
	- Core module provides the fundamental parts of the framework, including the IoC and Dependency Injection features
	- The Context module builds on the solid base provided by the Core and Beans modules
- **Data Access/Integration** layer consists of the JDBC, ORM, OXM, JMS and Transaction modules
	- The ORM module provides integration layers for popular object-relational mapping APIs, including JPA, JDO, Hibernate, and iBatis
- **Web layer** consists of the Web, Web-Servlet, Web-Struts, and Web-Portlet modules
	- The Web-Servlet module  contains  Spring's  model-view-controller  (MVC) implementation for web applications
- Few other important modules like AOP, Aspects, Instrumentation, Web and Test modules

Bean Configuration file(an XML file) acts as cement that glues the beans ie. classes together. 
This file needs to be created **under the src directory**.

The Spring container is at the core of the Spring Framework. 
The container will create the objects, wire them together, configure them, and manage their complete lifecycle from creation till destruction. 
The Spring container uses dependency injection (DI) to manage the components that make up an application.

The container 通过读取配置文件中的metadata来进行上面提到的各种操作.
The configuration metadata can be represented either by XML, Java annotations, or Java code.

The Spring IoC container makes use of Java POJO(Plain Old Java Object) classes and configuration metadata to produce a fully configured and executable system or application.

两种container:

1. Spring BeanFactory Container  
	- This is the simplest container providing basic support for DI and defined by the `org.springframework.beans.factory.BeanFactory` interface.
	- The most commonly used BeanFactory implementation is the XmlBeanFactoryclass.
1. Spring ApplicationContext Container  
	- This container adds more enterprise-specific functionality such as the ability to resolve textual messages from a properties file and 
		the ability to publish application events to interested event listeners. This container is defined by the `org.springframework.context.ApplicationContext` interface
	- The most commonly used ApplicationContext implementations are
		1. FileSystemXmlApplicationContext: 使用xml 文件的绝对路径
		1. ClassPathXmlApplicationContext: xml 文件在classpath的路径里面
		1. WebXmlApplicationContext: Web Application

由于ApplicationContext container 包括 BeanFactory container 的所有功能, 因此一般建议使用ApplicationContext container, 除非是在内存等资源有限的情况或者对性能要求比较高的情况下(例如Mobile).

## Spring Bean
Spring Bean Properties

- class This attribute is mandatory and specify the bean class to be used to create the bean.
- name This attribute specifies the bean identifier uniquely. In XML-based configuration metadata, you use the id and/or name attributes to specify the bean identifier(s).
- scope(`scope = ""`) This attribute specifies the scope of the objects created from a particular bean definition
- constructor-arg This is used to inject the dependencies and will be discussed in next chapters.
- properties This is used to inject the dependencies
- autowiring mode This is used to inject the dependencies
- lazy-initialization mode(`lazy-init = "true"` A lazy-initialized bean tells the IoC container to create a bean instance when it is first requested, rather than at startup.
- initialization method(`init-method = ""`) A callback to be called just after all necessary properties on the bean have been set by the container.
- destruction method(`destroy-method = ""`) A callback to be used when the container containing the bean is destroyed.

Scope 的可选项:

- singleton This scopes the bean definition to a single instance per Spring IoC container (default) 只存在一个
- prototype This scopes a single bean definition to have any number of object instances, 每次需要的时候都会创建一个新的
- request This scopes a bean definition to an HTTP request. Only valid in the context of a web-aware Spring ApplicationContext.
- session This scopes a bean definition to an HTTP session. Only valid in the context of a web-aware Spring ApplicationContext.
- global-session This scopes a bean definition to a global HTTP session. Only valid in the context of a web-aware Spring ApplicationContext.

prototype 不要和init-method, destroymethod 一起使用, applicationcontext 不会监控prototype的lifecycle.

当beans 的init 和destroy 方法都一样时, 可以在beans的层级统一设置
```
<beans default-init-method ="init" default-destroy-method ="destroy" >
	<bean id ="..."  class ="..." ></bean>
</beans>
```

Post processor
postProcessBeforeInitialization: 在初始化一个bean 之前调用的方法
postProcessAfterInitialization: 在初始化一个bean 之后调用的方法.
和之前的init-method, destroy-method 的顺序为: postProcessorBeforeInitialization -> Bean 的创建 -> init-method -> postProcessorAfterInitialization -> destroy-method -> Bean的销毁

Bean 的定义也可以有继承, 只需要在`<bean parent = "pid"></bean>` 中指定parent的id 就可以了.
还可以定义bean template

## Dependency Injection
IoC: Inversion of Control 控制反转(交给容器控制)  
DI: Dependency Injection 依赖注入  
Dependency Injection is merely one concrete example of Inversion of Control.

Dependency Injection (or sometime called wiring) helps in gluing these classes together and same time keeping them independent.

Consider you have an application which has a text editor component and you want to provide spell checking. Your standard code would look something like this:
```
public class TextEditor {
	private SpellChecker spellChecker;
	public TextEditor() {
		spellChecker = new SpellChecker();
	}
}
```
What we've done here is create a dependency between the TextEditor and the SpellChecker. 
In an inversion of control scenario we would instead do something like this:
```
public class TextEditor {
	private SpellChecker spellChecker;
	public TextEditor(SpellChecker spellChecker) {
		this.spellChecker = spellChecker;
	}
}
```
这里, 我们在创建一个texteditor 的时候将spellchecker 作为一个参数传进去, 就可以解决上面实现中依赖问题, 从而得到了很大的灵活性.
在Spring Framework 中, 这种工作时通过一个xml 文件来进行配置的.
Here, we have removed the total control from TextEditor and kept it somewhere else (ie. XML
configuration file) and the dependency ( ie. class SpellChecker) is being injected into the class TextEditor through a Class Constructor.

DI exists in two major variants 

1. Constructor-based dependency injection  
Constructor-based DI is accomplished when the container invokes a class constructor with a number of arguments, each representing a dependency on other class.
2. Setter-based dependency injection  
Setter-based DI is accomplished by the container calling setter methods on your beans after invoking a no-argument constructor or no-argument static factory method to instantiate your bean.

You can mix both, Constructor-based and Setter-based DI but it is a good rule of thumb to use
constructor arguments for mandatory dependencies and setters for optional dependencies.

### Constructor-based dependency injection  
TextEditor 的构造函数
```
public TextEditor(SpellChecker spellChecker) {
		System.out.println("Inside TextEditor constructor.");
		this.spellChecker = spellChecker;
}
```
Beans.xml 中将两者关联起来, spellChecker 作为textEditor的构造函数的参数.
```
<bean id ="textEditor"  class ="com.tutorialspoint.TextEditor" > <constructor-arg ref ="spellChecker" /> </bean>
<bean id ="spellChecker"  class ="com.tutorialspoint.SpellChecker" > </bean>
```
当构造函数需要多个参数时, constructor-arg 的顺序要与构造函数中参数的顺序一致.

### Setter-based Dependency Injection
TextEditor 中设置spellChecker 的方法
```
public void setSpellChecker(SpellChecker spellChecker) {
	System.out.println("Inside SetTextEditor");
	this.spellChecker = spellChecker;
}
```
Beans.xml 将两者关联起来, 通过setter 来设置参数, 而不是构造函数.
```
<bean id ="textEditor"  class ="com.tutorialspoint.TextEditor" >
<property name="spellChecker" ref="spellChecker" />
</bean>

<bean id ="spellChecker"  class ="com.tutorialspoint.SpellChecker" >
</bean>
```
in case you are passing a reference to an object, you need to use ref attribute of <property> tag and 
if you are passing a value directly then you should use value attribute

除了可以按照上面的方法进行设置外, 还可以通过inner bean的方式进行设置(直接将ref 的 bean 写到property 里面).
```
<bean id ="textEditor"  class ="com.tutorialspoint.TextEditor" >
	<property name="spellChecker">
		<bean id ="spellChecker"  class ="com.tutorialspoint.SpellChecker" />
	</property>
</bean>
```

### Spring Injecting Collection

- <list>  This helps in wiring ie injecting a list of values, allowing duplicates.
- <set>  This helps in wiring a set of values but without any duplicates.
- <map> This can be used to inject a collection of name-value pairs where name and value can be of any type.
- <props> This can be used to inject a collection of name-value pairs where the name and value are both Strings.

```
<property name="addressList" >
	<list>
		<value> INDIA </value>
		<value> Pakistan </value>
		<value> USA </value>
		<value> USA </value>
	</list>
</property>

<property name="addressList" >
	<list>
		<ref bean="address1" />
		<ref bean="address2" />
		<value> Pakistan </value>
	</list>
</property>
```

empty string: `<property name="email" value="" />`  
null value: `<property name="email" ><null/></property>`

autowire 自动装配

Spring Annotation Based Configuration  
Annotation injection is performed before XML injection, 
thus the latter configuration will override the former for properties wired through both approaches

annotation 默认是关闭的, 需要在Beans.xml 加入下面的内容来开启
```
<context: annotation-config>
```

Spring Java Based Configuration

## Event Handling in Spring
1. ContextRefreshedEvent  
This event is published when the ApplicationContext is either initialized or refreshed.
This can also be raised using the refresh() method on the ConfigurableApplicationContext interface.
2. ContextStartedEvent  
This event is published when the ApplicationContext is started using the start() method on theConfigurableApplicationContext interface. 
You can poll your database or you can re/start any stopped application after receiving this event.
3. ContextStoppedEvent  
This event is published when the ApplicationContext is stopped using the stop() method on theConfigurableApplicationContext interface. 
You can do required housekeep work after receiving this event.
4. ContextClosedEvent  
This event is published when the ApplicationContext is closed using the close() method on theConfigurableApplicationContext interface. 
A closed context reaches its end of life; it cannot be refreshed or restarted.
5. RequestHandledEvent  
This is a web-specific event telling all beans that an HTTP request has been serviced.

Spring's event handling is single-threaded so if an event is published, until and unless all the receivers get the message, the processes are blocked and the flow will not continue.

To listen a context event, a bean should implement the ApplicationListener interface which has just one method `onApplicationEvent()`.

```
public class CStartEventHandler implements ApplicationListener<ContextStartedEvent>{
	public void onApplicationEvent(ContextStartedEvent event) {
		System.out.println("ContextStartedEvent Received");
	}
}

public class CStopEventHandler implements ApplicationListener<ContextStopedEvent>{
	public void onApplicationEvent(ContextStartedEvent event) {
		System.out.println("ContextStopedEvent Received");
	}
}
```

## AOP 面向切面编程
Aspect Oriented Programming entails breaking down program logic into distinct parts called so-called concerns. 
The functions that span multiple points of an application are called cross-cutting concerns and these cross-cutting concerns are conceptually separate from the application's business logic. 
There are various common good examples of aspects like logging, auditing, declarative transactions, security, and caching etc.

Spring AOP module provides interceptors to intercept an application, for example, when a method is executed, you can add extra functionality before or after the method execution.

通过AOP, 我们可以在不改变原有的代码情况下, 而增加业务逻辑.

动态代理

AOP 里面的概念

- JointPoint: 连接点
- PointCut: JointPoint集合
- Aspect: 切面
- Advice: 可以简单的理解成 @Before, @AfterReturning @AfterThrowing @After @Around (before and after)
- Introduction An introduction allows you to add new methods or attributes to existing classes.
- Target: 被代理对象, 把我们的逻辑织入到哪个对象上面去
- Weaving: 织布 Weaving is the process of linking aspects with other application types or objects to create an advised object. This can be done at compile time, load time, or at runtime

AOP 的两种实现方式

1. XML Schema based   
Aspects are implemented using regular classes along with XML based configuration.
1. @AspectJ based  
@AspectJ refers to a style of declaring aspects as regular Java classes annotated with Java 5 annotations

## Spring JDBC Framework
Spring JDBC provides several approaches and correspondingly different classes to interface with the database.
the most popular approach which makes use of JdbcTemplateclass of the framework

Instances of the JdbcTemplate class are threadsafe once configured. So you can configure a single instance of a JdbcTemplate and then safely inject this shared reference into multiple DAOs.

DAO: Data Access Object

DataSource 一个抽象接口
因为拿到connection 的方法很多, 所以需要一个抽象接口
dbcp: database connection pool
```
<bean id="dataSource" destroy-method="close" class="org.apache.commons.dhcp.BasicDataSource">
	<property name="driverClassName" value="com.mysql.jdbc.Driver"/>
	<property name="url" value="jdbc:mysql://localhost:3306/db"/>
	<property name="username" value="root"/>
	<property name="password" value="password"/>
</bean>
```
在上面是把数据库的参数直接写到了Beans.xml 的配置文件中, 我们还可以通过下面占位符的方式, 把数据库的连接参数单独放在一个文件中.
```
<bean class="org.springframework.beans.factory.config.PropertyPlaceholderConfigurer">
	<property name="locations">
		<value>classpath:jdbc.properties</value>
	</property>
</bean>
在classpath 的目录下有一个jdbc.properties的文件
<bean id="dataSource" destroy-method="close" class="org.apache.commons.dhcp.BasicDataSource">
	<property name="driverClassName" value="${jdbc.driverClassName}$"/>
	<property name="url" value="${jdbc.driverClassName}$"/>
	<property name="username" value="${jdbc.username}$"/>
	<property name="password" value="${jdbc.password}$"/>
</bean>
```

# 架构

用户注册的例子
```
client --- jsp --- UserManager --- UserManagerImpl --- User --- UserDAO --- UserDAOImpl --- DB
```
在这里, jsp 里包含了view, 和 control的功能.  
我们可以将strut 2 加进来, 实现control的功能;  
把spring 加进来实现model 的功能, 把UserManager 交给Spring 管理;  
把hibernate加到UserDAOImpl 和 DB 之间, 用来隔绝具体的数据库.
```
client --- jsp --- struts 2 filter --- Action --- view:jsp --- UserManager --- UserManagerImpl --- User --- UserDAO --- UserDAOImpl --- DB
```

# Maven
Maven是一个优秀的项目构建工具.

Maven帮助我们自动化项目构建过程,具体来说包括项目的清理,编译,测试,生成测试报告,打包和部署.
只需要输入很简洁的命令,就能实现这些构建过程.比如输入`mvn clean compile` ,就能完成项目的清理和编译.
同时,Maven提供jar包的依赖管理

[Maven的安装](http://blog.csdn.net/zhangbaidu555/article/details/12955639)

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
1. `java -cp target/test-1.0-SNAPSHOT.jar com.eric.test.App` 就会运行jar 包中的 com.eric.test package下的App(App 是默认创建的含有Main函数的类), 输出为 `Hello World!`

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

- modelVersion: POM model version (always 4.0.0).
- groupId:  创建项目的小组的唯一标示. Group or organization that the project belongs to. Often expressed as an inverted domain name
- artifactId:  该产品id.会作为产生的jar/war包名的一部分.它和version一起构成输出包的包名.
- Version: 产品版本
- Packaging: 打包的类型, 默认是jar, 可以设置为war
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

如果在运行jar 包时, 出现类似于下面的错误, 可能是由于jar 不可执行造成的
```
[eric@alien spring-demo]$ java -cp target/gs-maven-0.1.0.jar hello.Application
Exception in thread "main" java.lang.NoClassDefFoundError: org/springframework/context/ApplicationContext
        at java.lang.Class.getDeclaredMethods0(Native Method)
        at java.lang.Class.privateGetDeclaredMethods(Class.java:2570)
        at java.lang.Class.getMethod0(Class.java:2813)
        at java.lang.Class.getMethod(Class.java:1663)
        at sun.launcher.LauncherHelper.getMainMethod(LauncherHelper.java:494)
        at sun.launcher.LauncherHelper.checkAndLoadMain(LauncherHelper.java:486)
Caused by: java.lang.ClassNotFoundException: org.springframework.context.ApplicationContext
        at java.net.URLClassLoader$1.run(URLClassLoader.java:366)
        at java.net.URLClassLoader$1.run(URLClassLoader.java:355)
        at java.security.AccessController.doPrivileged(Native Method)
        at java.net.URLClassLoader.findClass(URLClassLoader.java:354)
        at java.lang.ClassLoader.loadClass(ClassLoader.java:425)
        at sun.misc.Launcher$AppClassLoader.loadClass(Launcher.java:308)
        at java.lang.ClassLoader.loadClass(ClassLoader.java:358)
        ... 6 more
```
需要借助maven-shade-plugin 来生成可执行的jar 包.
```
<plugin>
  <groupId>org.apache.maven.plugins</groupId>
  <artifactId>maven-shade-plugin</artifactId>
  <version>1.2.1</version>
  <executions>
    <execution>
      <phase>package</phase>
      <goals>
        <goal>shade</goal>
      </goals>
      <configuration>
      <transformers>
        <transformer implementation="org.apache.maven.plugins.shade.resource.ManifestResourceTransformer">
          <mainClass>package_path_to_mainclass.mainclass</mainClass>
        </transformer>
      </transformers>
    </configuration>
  </executions>
</plugin>
```

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
- clean: cleans up artifacts created by prior builds, 会删除target 目录
- site: generates site documentation for this project
mvn eclipse:eclipse 生成 Eclipse 项目文件及包引用定义,注意,需确保定义Classpath Variables: M2_REPO,指向本地maven类库目录.

Phases are actually mapped to underlying goals. The specific goals executed per phase is dependant upon the packaging type of the project. 
For example, package executes jar:jar if the project type is a JAR, and war:war if the project type is - you guessed it - a WAR.

An interesting thing to note is that phases and goals may be executed in sequence.
```
mvn clean dependency:copy-dependencies package
```
This command will clean the project, copy dependencies, and package the project (executing all phases up to package, of course).

# MyEclipse
[MyEclipse10.0中的Maven配置](http://blog.csdn.net/zhangbaidu555/article/details/12955639)
