# 数据库技术概述
## 数据库系统结构
### DBMS
数据字典: data dictionary 中存放关于数据库中各级模式的描述信息, 包括所有数据的结构名, 意义, 描述定义, 存储格式, 完整性约束, 使用权信息.
在结构上, 数据字典也是一个数据库, 为了区分物理数据库中的数据与数据字典中的数据, 通常将数据字典中的数据称为元数据, 组成数据字典文件的属性称为元属性.
数据字典是DBMS 存取和管理数据的基本依据, 主要有系统管理和使用.

用户访问数据库的过程:

用户对数据库进行操作, 是由DBMS 把操作从应用程序带到外模式,模式, 在导向内模式, 进而通过操作系统操纵存储器中的数据. 同时, DBMS 为应用程序在内存开辟了一个数据库的系统缓冲区, 用于数据的传输和格式转换. 而三级模式的定义存放在数据字典中

图中应用程序的子模式对应外模式

存储模式对应内模式

DBMS对数据库的保护通过4个方面来实现:数据库的恢复,数据库的并发控制,数据库的完整性控制,数据库安全性控制

DBMS的主要组成部分

- 查询处理器: 
	- 预编译器(完成嵌入在宿主语言中的查询语句), 
	- 编译器(对查询和更新语句进行优化并且转换成可执行的低层次命令), 
	- 解释器(负责编译或解释模式更新),模式更新是对元数据的更新
- 存储管理器: 缓冲区管理器, 文件管理器

### 数据库体系结构
三级模式结构, 外模式, 概念模式和内模式

- 外模式: external schema简称子模式, 用户模式. 一个数据库可以有多个外模式
- 概念模式: conceptual schema简称模式, 又称数据库模式, 逻辑模式, 他是数据库中全部数据的整体逻辑结构和特征的描述
- 内模式: internal schema也称存储模式

两级映像 mapping: 一种对应规则, 指出映像双方如何相互转换

- 外模式/ 概念模式 映像
- 概念模式/内模式映像

两级数据独立性

- 物理数据独立性: 物理独立性是指用户的应用程序与存储在磁盘上的数据库中数据是相互独立的.
	即,数据在磁盘上怎样存储由DBMS管理,用户程序不需要了解,应用程序要处理的只是数据的逻辑结构,这样当数据的物理存储改变了,应用程序不用改变.
- 逻辑数据独立性: 逻辑独立性是指用户的应用程序与数据库的逻辑结构是相互独立的,即,当数据的逻辑结构改变时,用户程序也可以不变

三个层次

- 数据存储层
- 业务处理层
- 界面表示层

数据库应用系统体系结构是指数据库系统中的数据存储层, 业务处理层, 界面表示层以及网络通信之间的布局与分布关系


结构类型

单用户结构: 三个层次都在单台PC上

集中式结构: 将操作系统, 应用程序, 数据库系统等数据和资源均放在作为核心的主机上, 而连接在主机上的许多终端, 只是作为主机的一个输入/输出设备

数据存储层和业务处理层都放在主机上, 而界面表示层放在与主机相连的各个终端上

客户机/服务器结构 client/server C/S

数据存储层处于服务器上, 业务处理层和界面表示层处于客户机上

客户机上需要安装要连接的数据库对应的客户端软件



浏览器/服务器结构browser/server B/S

三层架构, 客户机(浏览器) / 应用服务器(业务处理) / 数据库服务器(数据存储与控制)


## 数据模型
- 概念数据模型: 独立于计算机系统的数据模型
- 逻辑数据模型: 直接与DBMS 有关, 有严格的形式化定义

层次模型, 网状模型, 关系模型, 面向对象数据模型

**数据模型的组成: 数据结构, 数据操作, 数据的约束条件**

层次数据模型(hierarchical datamodel)

层次结构是树结构,树的结点是记录类型,非根结点有且只有一个父结点.上一层 记录类型和下一层记录类型是1:N联系.记录之间的联系通过指针来实现,查询效率较高.

网状模型(Network Model)

用网络结构表示实体类型及其实体之间联系的模型.顾名思义,一个事物和另外的几个都有联系^这样构成一张网状图.

网状模型的数据结构主要有以下两个特征:

(1)允许一个以上的节点无双亲.

(2)一个节点可以有多于一个的双亲.

### 关系模型
关系实际上就是关系模式在某一时刻的状态或内容.也就是说,关系模式是型,关系是它的值.关系模式是静态的,稳定的,而关系是动态的,随时间不断变化的,因为关系操作在不断地更新着数据库中的数据.但在实际当中,常常把关系模式和关系统称为关系,读者可以从上下文中加以区别.

数据操作: 

查询操作: 选择, 投影, 联接, 除, 并, 交, 差等

更新操作: 插入, 删除和修改

关系模型中数据操作的特点是集合操作方式, 操作的对象和结果都是集合, 这种操作方式也称为一次一集合的方式

非关系数据模型中的数据操作方式则为一次一记录的方式

关系模型中的数据联系是靠数据冗余实现的

保留适量的冗余(包括但不限于上面提到的冗余), 达到以空间换时间的目的

时间的两维性: 有效时间和事务时间

# 关系数据库
## 关系模型
关系: 二维表

元组: 一行tuple

属性: 一列

主键: primary key 某个属性组, 可以唯一确定一个元组

域: domain 属性的取值范围

分量: 元组中的一个属性值

关系模式: 对关系的描述, 一般表示为: 关系名(属性1, 属性2, …, 属性n)

关系的每一个分量必须是不可分的数据项

否则这个分量的维度就至少为2, 然后整个表的的维度就超过了2, 违背了二维表

关系操作

关系模型中的关系操纵能力早起通常是用代数方法或逻辑方法来表示

关系代数(relation algebra): 关系的原酸 和关系演算(relation calculus): 用谓词

谓词的基本对象是元组还是域变量分为元组关系演算和域关系演算

这三种语言在表达能力上是等价的, 均是抽象的查询语言, 这些抽象的语言与具体的DBMS 中实现的实际语言并不完全一样.

实际的查询语言还提供了附加功能: 关系赋值, 算术运算

SQL 是介于关系代数和关系演算之间的语言, 而且还具有关系定义和数据控制的功能

SQL 集DDL(Data definition language), DCl(Data control language) 和DML(data manipulation language)于一体

完整性约束

- 域完整性约束 domain integrity constraint
- 实体完整性 entity integrity constraint: 关系中的每一行都代表一个实体
- 参照完整性约束 referential integrity constraint
- 用户定义完整性约束

候选键:candidate key 在每一行数据里的值都不相同,就像自动增长的id一样,可以说成是候选的主键

主属性 primary attribute: 主属性 \in 候选键

主键: 若一个关系中有多个候选键, 则选定一个为主键

外键foreign key: 一个属性是所在关系之外的另外一个关系的主键, 则该属性就是他所在关系的外键

保持数据一致性,完整性,主要目的是控制存储在外键表中的数据. 使两张表形成关联,外键只能引用外表中的列的值或使用空值

## 关系代数
专门的关系运算符

选择: 从关系中找出满足给定条件的那些元组称为选择.其中的条件是以逻辑表达式给出的,值为真的元组将被选取.这种运算是从水平方向抽取元组.

\sigma_{<\text{select condition}>}(<\text{relation name}>)


查询学生表中年龄为19的男生

\sigma_{性别='男' AND 年龄=19}(学生)


投影: 从关系模式中挑选若干属性组成新的关系称为投影.这是从列的角度进行的运算,相当于对关系进行垂直分解

\prod_{(\text{list of properties})} (<\text{relation name}>)


要查询学生关系中在学号, 姓名, 年龄三个属性上的投影
\prod_{学号, 姓名, 年龄}(学生)

联接: 连接运算是从两个关系的笛卡尔积中选择属性间满足一定条件的元组
R  \Join_{(\text{join condition})} S = \sigma_{(\text{join condition})}(R \times S)

如果所有条件中都为"=", 则此种联接称为等联接

在等联接的结果中, 对应属性的值是相同的, 没有必要重复列出, 只要在两个对应属性中取一个即可. 这种消除冗余的等联接称为自然联接

除: 在关系代数中,除法运算可理解为笛卡尔积的逆运算.

设被除关系R为m元关系,除关系S为n元关系,那么它们的商为m-n元关系,记为R÷S.商的构成原则是:将被除关系R中的m-n列,按其值分成若干组,检查每一组的n列值的集合是否包含除关系S,若包含则取m-n列的值作为商的一个元组,否则不取.

T=R÷S

关系R中的属性真包含S 中的全部属性

P(T) = P(R) – P(S) //P(A) 为A的属性集合

T的元组 = {tuple | tuple \in S 且与R中对应值相等}

# SQL
SQL 集DDL(Data definition language), DCl(Data control language) 和DML(data manipulation language)于一体

DDL: 定义关系数据库的模式, 外模式和内模式, 以实现对基本表, 视图以及索引文件的定义, 修改和删除操作

DML: 查询和更新

DCL: 对基本表和视图的授权, 完整性规则的描述以及事物控制语句



嵌入式SQL: 规定了嵌入在宿主语言程序中使用的各种规则


SQL的命令动词

SQL 的功能	命令动词

数据定义	create, drop, alter

数据操作	select, insert, update, delete

数据控制	grant, revoke



高度非过程化

第四代语言, 用户只需要提出干什么, 无须具体指明怎么干, 像存取路径选择和具体处理操作等, 都是由系统自动完成



支持数据库三级模式结构

外模式对应于视图和部分基本表

概念模式对应于基本表

内模式对应于存储文件 和 索引文件


在SQL 中, 一个关系对应一个表; 一个或多个表对应一个存储文件; 一个表可以带若干个索引, 索引也存放在存储文件中. 存储文件的逻辑结构组成了关系数据库的内模式

## SQL基本知识
### 数据类型
数值型

int

smallint

real

double precision

float

numeric(p,q): 定点数, 小数点前有p 位数字, 之后有q 位数字



字符型

char(n)

varchar(n)



位串型

bit

bit varying(n)



日期型

date

time



### 表达式与运算符
比较运算符

算术运算符

逻辑运算符

not, and, or

谓词: 一般用在where 子句中

is null 和is not null: 判断是否是空值



between … and … 和 not between … and …

in



like 字符串的匹配

[not] like <匹配符> [escape <转义符>]

% 表示任意长度的字符串

_ 表示任意单个字符

如果like后面的匹配符不包含通配符, 则可以用"=" (<> 或!= )运算符取代谓词like(not like)



exists not exists

exits(子查询) 返回True 或False



unique



all 和any



函数

1.单行函数: 作用对象为tuple

2.统计函数: 作用对象为tuple的集合



语法规定与约定

1.<> 必选项, 不能为空

[] 任选项

{} 或| 必选其一

[,…n] 前面的项可以重复多次

2.语法规定

数据项(列项, 表和视图) 的分隔符为" , "

字符串常数的定界符用单引号" ' "表示

SQL的关键字一般用大写字母(此笔记为了方便书写我使用的是小写)

语句的结束符为;

## 数据定义语言
在SQL 中, 数据库被定义为对象( 即基本表, 视图和索引等) 的 集合, 并包含数据库中灭一个对象的定义

### 创建
create database <database name>;

alter database <database name>;

drop database <database name>;

create table <table name> (<列名> <数据类型> [<列级完整性约束条件>]

					[,<列名> <数据类型> [<列级完整性约束条件>]] … ]

					[<表级完整性约束条件>]);

### 约束的定义
完整性约束

primary key 约束: 可以在列级或表级上进行定义, 但不能同时在两个级别定义

1.primary key 直接写在列名及其类型之后

2.在之后单独列出, constraint <约束名> primary key

3.在create table 语句列出所有列之后, 在附加一个声明

primary key (<列名1> [,<列名2>, … n])

foreign key约束

references <表名> (<列名>)

foreign key (<列名>) references <表名> (<列名>)

constraint <约束名> foreign key (<列名>) references <表名> (<列名>)

unique 约束

constraint <约束名> unique

constraint <约束名> unique (<列名1> [,<列名2>, … n]);


check 约束

constraint <约束名> check (<约束表达式>);


### 修改基本表
alter table <表名>

		[add < 新列名> <数据类型> [<完整性约束> ] [, … n]]

		[drop [[[constraint ] <约束名>| [column <列名>]] [, … n]]

		[modify <列名> <数据类型> [, … n]];

例如

alter table 学生 drop column 周学时;

drop table <table name>;

### 索引
create [unique][clustered] index <索引名>

		on <表名> (<列名1> [<次序1>] [, <列名2> [<次序2>]] …);



drop index <索引名>[,…n];

### 视图
create view <view name> [(列名)[,<列名>]…]
		as <子查询>
		[with check option];

[with check option] 表示对视图进行update, insert和delete 操作时要保证修改的行满足视图定义的谓词条件, 即<子查询> 中where 子句的条件表达式.

默认情况下,由于行通过视图进行添加或更新,当其不再符合定义视图的查询的条件时,它们即从视图范围中消失.例如,可创建一个查询,从而定义一个视图以在表中检索所有员工薪水低于 $30,000 的行.如果该员工的薪水涨到了 $32,000,则查询视图时该特定员工将不再出现,因其薪水不符合视图所设的标准.但是,WITH CHECK OPTION 子句强制所有数据修改语句均根据视图执行,以符合定义视图的 SELECT 语句中所设的条件.如果使用该子句,修改行时需考虑到不让它在修改完后从视图中消失.任何可能导致行消失的修改都会被取消,并显示错误信息.



语句的执行结果, 仅仅是将视图的定义信息存入数据库的数据字典中, 而定义中的<子查询> 语句并不执行. 当系统运行到该视图定义语句的程序时, 根据字典中视图的定义信息临时生成该视图. 程序一旦执行结束, 该视图立即被撤销.

drop view <试图名>;

### 存储过程
存储过程(Stored Procedure)是在大型数据库系统中,一组为了完成特定功能的SQL 语句集,经编译后存储在数据库中,用户通过指定存储过程的名字并给出参数(如果该存储过程带有参数)来执行它.

再次调用的时候不需要重新编译

create proc[edure] <存储过程名>
		[recompile | encryption | recompile, encryption]
		as
			<SQL 语句序列>;

alter proc[edure] <存储过程名>
		[recompile | encryption | recompile, encryption]
		as
			<SQL 语句序列>;

drop procedure <存储过程名>[, … n];

## 数据更新语言
插入单个元组

INSERT INTO table_name (列1, 列2,...) VALUES (值1, 值2,....);

插入多个元组
insert into <表名> [<列名1> [, <列名2>, …] )]
	<子查询>;

UPDATE 表名称 SET 列名称 = 新值 WHERE 列名称 = 某值

DELETE FROM 表名称 WHERE 列名称 = 值

### 数据查询语言
select 语句

select [all | distinct ] [<目标列表达式>[,…n]]
	from <表名或试图名> [,<表名或试图名>,…]
	[where <条件表达式>]
	[group by <列名1> [having <条件表达式>]]
	[order by <列名2> [asc | desc],…];

group by子句: 按<列名1>的值进行分组

当select 子句后的<目标表达式> 中含有统计函数时, 则统计为分组统计, 否则为对整个结果集进行统计

having 子句必须跟随group by 字句使用

执行过程

根据where子句的条件表达式, 从from 子句指定的基本表或视图中找出满足条件的元组

再根据目标表达式选出元组中的列值形成结果集.

如果有group by 子句, 则将结果按照<列名1> 的值进行分组, 该列值相等的元组为一个组, 每个组产生结果集中的一个元组

如果group by后面带有having, 则只有满足条件的组才予以输出

如果后面有order by 子句, 则结果集还要按<列名2> 的值进行排序


联接查询

在where 子句中, 有时既有联接条件又有元组选择条件, 这是他们之间用AND 操作符连接, 

且一般将联接条件放在元组选择条件前面

(因为联接会导致结果集扩大, 且一般属性数目增加, 如果将元组选择条件放在前面, 且元组条件中含有扩大后的新元组的多个属性, 但是这些属性原来是属于不同的表的)



嵌套查询

使用比较词的嵌套

使用谓词的嵌套: in any, all, exists

	SELECT column_name(s)

FROM table_name

WHERE column_name IN (value1,value2,...)



	select 学号, 姓名, 性别, 班级号

		from 学生

		where exists (select * 

from 考试

where 学生.学号 = 学号 and 课程号 = 'A004');



集合查询union, intersection, minus

函数查询

## 数据控制语言
权限

grant <权限> [,<权限名>,…]
	[on <对象类型> <对象名>]
	to <用户> [,<用户>,…] | public
	[with grant option];

权限

属性列和视图: DELETE,INSERT,SELECT , UPDATE和all privileges(前面四种权限的总和)

表: DELETE,INSERT,SELECT 和 UPDATE, alter, index, all privileges(前面6种权限的总和)

数据库: creattab

public 指数据库的所有用户

授予关于属性列的权限时, 必须指明列名

若指定了with grant option, 则表示获得某种权限的用户还可以将这种权限再授予别的用户

将修改学生年龄和查询学生基本表的权限授予张三

grant update(年龄), select on 学生 to 张三

revoke <权限> [,<权限名>,…]
	[on <对象类型> <对象名>]
	from <用户> [,<用户>,…] | public

再不撤销权限的情况下, 拒绝用户访问

deny <权限> [,<权限名>,…]
	[on <对象类型> <对象名>]
	from <用户> [,<用户>,…] | public

## 事务控制
事物开始: begin transaction

事务提交: commit

事物撤销: rollback

## 嵌入式SQL Embedded SQL
EXEC SQL <SQL语句>;

嵌入式SQL的处理过程

对ESQL,RDBMS一般采用预编译方法处理

数据库工作单元与源程序工作单元之间的通信主要包括:

1．向主语言传递SQL语句的执行状态信息,使主语言能够根据此信息控制程序流程,主要用SQL通信区(SQL communication area)实现.
2．主语言向SQL语句提供参数,主要用宿主变量(Host variable 或称为共享变量)实现.
3．将SQL语句查询数据库的结果交主语言进一步处理,主要用主变量和游标实现.

### SQLCA: SQL Communication Area
SQLCA的用途

SQL语句执行后,DBMS反馈给应用程序信息

• 描述系统当前工作状态

• 描述运行环境的各种数据

这些信息将送到SQL通信区SQLCA中

应用程序从SQLCA中取出这些状态信息,据此决定接下来执行的语句

SQLCA的使用方法

定义SQLCA

用EXEC SQL INCLUDE SQLCA加以定义

使用SQLCA

SQLCA中有一个存放每次执行SQL语句后返回代码的变量SQLCODE,

如果SQLCODE等于预定义的常量SUCCESS(值为0),则表示SQL语句成功,否则表示出错, SQLCODE会存放着错误代码,

应用程序每执行完一条SQL 语句之后都应该测试一下SQLCODE的值,以了解该SQL语句执行情况并做相应处理.

### 变量
指示变量

一个主变量可以附带一个指示变量(Indicator Variable)

什么是指示变量

•整型变量

•用来"指示"所指主变量的值或条件

指示变量的用途

•输入主变量可以利用指示变量赋空值

•输出主变量可以利用指示变量检测出是否空值,值是否被截断



在SQL语句中使用主变量和指示变量的方法

1) 说明主变量和指示变量

BEGIN DECLARE SECTION

	......... 

	.........     (说明主变量和指示变量)

END DECLARE SECTION

例:EXEC SQL BEGIN DECLARE SECTION; 

        CHAR   stdno(6);

        CHAR   stdname(8);

        INT    stdage;

     CHAR   stdsex(2);

        CHAR   stddept(20);

   EXEC SQL END DECLARE SECTION;

为了与数据库对象名(表名,视图名,列名等)区别,SQL语句中的主变量名前要加冒号(:)作为标志

例:EXEC SQL INSERT

     INTO  S(sno,sname,sage,ssex,sdept) 	 VALUES(:stdno,:stdname,:stdage,:stdsex,:stddept);

### 游标(cursor)
SQL语言是面向集合的,一条SQL语句原则上可以产生或处理多条记录

主语言是面向变量(多个变量组成一个记录)的,一组主变量一次只能存放一条记录

嵌入式SQL引入了游标的概念,用来协调这两种不同的处理方式

游标是系统为用户开设的一个数据缓冲区,内存中一段连续的存储单元,存放SQL语句的执行结果

每个游标区都有一个名字

用户可以用SQL语句逐一从游标中获取记录,并赋给主变量,交由主语言进一步处理

使用游标的步骤

1. 说明游标
2. 打开游标: 这时游标处于活动状态,指针指向查询结果集中第一条记录之前, 还不能直接读数据
3. 推进游标指针并取当前记录
	EXEC SQL FETCH <游标名> INTO <宿主变量组> END_EXEC;
4. 关闭游标

### 使用
创建数据库的连接

   EXEC SQL CONNECT TO target [AS connection-name] [USER user-name]; 

    target是要连接的数据库服务器:

常见的服务器标识串,如<dbname>@<hostname>:<port> 

包含服务器标识的SQL串常量 

DEFAULT  

   connect-name是可选的连接名,连接必须是一个有效的标识符 

  在整个程序内只有一个连接时可以不指定连接名

关闭数据库连接 

    EXEC SQL DISCONNECT [connection];

程序运行过程中可以修改当前连接 :

	EXEC SQL SET CONNECTION connection-name | DEFAULT;

### 动态SQL
宿主变量和查询语句需要在程序运行的时候才能确定下来

占位符

# 关系数据库设计
人们把数据库设计分为需求分析,概念结构设计,逻辑结构设计,物理结构设计,数据库实施,数据库运行与维护6个阶段.

## 概念结构设计

概念结构设计的任务是在需求分析阶段产生的需求说明书的基础上,按照特定的方法把它们抽象为一个不依赖于任何具体机器的数据模型,即概念模型.

### 实体-联系方法
E-R 模型

E-R模型的构成成分是实体集,属性和联系集,其表示方法如下:

(1) 实体集用矩形框表示,矩形框内写上实体名.

(2) 实体的属性用椭圆框表示,框内写上属性名,并用无向边与其实体集相连.

(3) 实体间的联系用菱形框表示,联系以适当的含义命名,名字写在菱形框中,用无向连线将参加联系的实体矩形框分别与菱形框相连,并在连线上标明联系的类型,即1—1,1—N或M—N.

因此,E-R模型也称为E-R图.



例如系,学生和课程的联系的E-R模型

系,学生和课程作为实体集,一个系有多个学生,而一个学生仅属于一个系,所以系和学生之间是一对多的联系,一个学生可以选修多门课程,而一门课程有多个学生选修,所以学生和课程之间是多对多的联系.

实体

把具有属性,性质和特征相同的实体(即需要操作网站系统功能的人物等),用所有实体名和他的属性名称的来用抽象的形式描述同种类的实体;一般实体用矩形符号表示,矩形框内标注实体的名称,

属性

实体特有的一些特性,一个实体有包含有许多个属性来进行描述.用椭圆形的符号来表示,用无向直线将属性的椭圆和其相对应的实体图形串联起来,

联系

ER模型中的联系是用来反映实体内部和实体之间的属性关系.用菱形符号表示,在菱形框内标注联系的名称,然后使用无向直线将有关系的实体属性串联起来,还需要在无向直线上标出实体和联系的类型(1 : 1,1 : n或m : n)

### UML方法
Unified Modeling Language (UML)		统一建模语言或标准建模语言

UML规范用来描述建模的概念有,类(对象的),对象,关联,职责,行为,接口,用例,包,顺序,协作,以及状态

最常用的UML图包括:用例图,类图,序列图,状态图,活动图,组件图和部署图

E-R图中的术语	UML类图中术语

实体集 entity set	类 class

实体 entity	对象 object

联系 relationship	关联 association

联系元数	关联元数

实体的基数 cardinality	重复读 multiplicity


类图(Class Diagram): 类图是面向对象系统建模中最常用和最重要的图,是定义其它图的基础.类图主要是用来显示系统中的类,接口以及它们之间的静态结构和关系的一种静态模型.

http://www.uml.org.cn/oobject/201104212.asp

类图的3个基本组件:类名,属性,方法

泛化(generalization):表示is-a的关系,是对象之间耦合度最大的一种关系,子类继承父类的所有细节.直接使用语言中的继承表达.在类图中使用带三角箭头的实线表示,箭头从子类指向父类.

关联(Association) : 对象之间一种引用关系,比如客户类与订单类之间的关系.这种关系通常使用类的属性表达.关联又分为一般关联,聚合关联与组合关联.后两种在后面分析.在类图使用带箭头的实线表示,箭头从使用类指向被关联的类.可以是单向和双向

### 逻辑结构设计

### 物理结构设计
DBMS一般采用多种存取方法, 常用的存取方法有三类

	索引方法: 目前主要是B+树索引方法

	聚簇(cluster) 方法: 特定属性上具有相同值的元组集中存放在连续的物理块上

	HASH方法: 

# 网络数据库
## 网络数据库的发展
### CGI
公共网关接口CGI(Common GatewayInterface) 是WWW技术中最重要的技术之一,有着不可替代的重要地位.CGI是外部应用程序(CGI程序)与Web服务器之间的接口标准,是在CGI程序和Web服务器之间传递信息的规程.CGI规范允许Web服务器执行外部程序,并将它们的输出发送给Web浏览器,CGI将Web的一组简单的静态超媒体文档变成一个完整的新的交互式媒体.

那么我们看一个实际例子:现在的个人主页上大部分都有一个留言本.留言本的工作是这样的:先由用户在客户端输入一些信息,如名字之类的东西.接着用户按一下"留言"(到目前为止工作都在客户端),浏览器把这些信息传送到服务器的CGI目录下特定的cgi程序中,于是cgi程序在服务器上按照预定的方法进行处理.在本例中就是把用户提交的信息存入指定的文件中.然后cgi程序给客户端发送一个信息,表示请求的任务已经结束.此时用户在浏览器里将看到"留言结束"的字样.整个过程结束.

绝大多数的CGI程序被用来解释处理来自表单的输入信息,并在服务器产生相应的处理,或将相应的信息反馈给浏览器.CGI程序使网页具有交互功能.

### ASP
ASP是Active Server Page的缩写,意为"动态服务器页面".ASP是微软公司开发的代替CGI脚本程序的一种应用,它可以与数据库和其它程序进行交互,是一种简单,方便的编程工具.ASP的网页文件的格式是 .asp.现在常用于各种动态网站中.

asp的第一版是0.9测试版,自从1996年ASP1.0诞生,它给Web开发界带来了福音.早期的Web程序开发是十分繁琐的,以至于要制作一个简单的动态页面需要编写大量的C代码才能完成,这对于普通的程序员来说有点难了.而ASP却允许使用VBScript, JavaScript入在HTML网页中的代码.在进行程序设计的时候可以使用它的内部组件来实现一些高级功能(例如Cookie).它的最大的贡献在于它的ADO(ActiveX Data Object),这个组件使得程序对数据库的操作十分简单,所以进行动态网页设计也变成一件轻松的事情.因此一夜之间,Web程序设计不再是想象中的艰巨任务,仿佛很多人都可以一显身手.

四层的Web数据库体系结构

## 网络数据库的访问技术
### ODBC
开放数据库互连(Open Database Connectivity,ODBC)是微软公司开放服务结构(WOSA,Windows Open Services Architecture)中有关数据库的一个组成部分,它建立了一组规范,并提供了一组对数据库访问的标准API(应用程序编程接口).这些API利用SQL来完成其大部分任务.ODBC本身也提供了对SQL语言的支持,用户可以直接将SQL语句送给ODBC.

数据源是驱动程序与数据库系统连接的桥梁, 他不是数据库系统, 分为以下三类

	用户数据源, 	系统数据源, 	文件数据源

数据源包含了数据库位置和数据库类型等信息, 实际上是一种数据库连接的抽象

在连接中, 用数据源名来代表用户名, 服务器名, 所连接的数据库名, 可以将数据源名看成是与一个具体数据库建立的连接

### OLE DB
OLE 全称 Object Link and embed,即对象连接与嵌入.

作为微软的组件对象模型(COM)的一种设计,OLE DB是一组读写数据的方法(在过去可能被称为渠道)

以统一方式访问不同类型的数据存储设计的一种应用程序接口,是一组用组件对象模型(COM)实现的接口,而与对象连接与嵌入(OLE)无关.

例如RDB, FoxPro, 文本文件, Word文档, Excel, XML等

### JDBC
Java database connectivity

JDBC主要完成以下三方面的功能

第一, 和一个DMS建立连接

第二, 向DBMS发送SQL语句

第三, 处理DBMS的返回集

### 对象/ 关系映射
对象关系映射(Object Relational Mapping,简称ORM)是一种为了解决面向对象与关系数据库存在的互不匹配的现象的技术. 简单的说,ORM是通过使用描述对象和数据库之间映射的元数据,将java程序中的对象自动持久化到关系数据库中.本质上就是将数据从一种形式转换到另外一种形式.

ORM框架就是用于实现ORM技术的程序.

常见的ORM框架有:Hibernate,iBATIS,TopLink,Castor JDO,Apache OJB等.

Java中ORM的原理:  先说ORM的实现原理,其实,要实现JavaBean的属性到数据库表的字段的映射,任何ORM框架不外乎是读某个配置文件把JavaBean的属 性和数据库表的字段自动关联起来,当从数据库Query时,自动把字段的值塞进JavaBean的对应属性里,当做INSERT或UPDATE时,自动把 JavaBean的属性值绑定到SQL语句中.

EJB

Enterprise JavaBean

定义了一个用于开发基于组件的企业多重应用程序的标准.其特点包括网络服务支持和核心开发工具(SDK). 在J2EE里,Enterprise Java Beans(EJB)称为Java 企业Bean,是Java的核心代码,分别是会话Bean(Session Bean),实体Bean(Entity Bean)和消息驱动Bean(MessageDriven Bean).



1.SessionBean用于实现业务逻辑,它可以是有状态的,也可以是无状态的.每当客户端请求时,容器就会选择一个Session Bean来为客户端服务.Session Bean可以直接访问数据库,但更多时候,它会通过Entity Bean实现数据访问.

2.Entity Bean是域模[1]型对象,用于实现O/R映射,负责将数据库中的表记录映射为内存中的Entity对象,事实上,创建一个Entity Bean对象相当于新建一条记录,删除一个Entity Bean会同时从数据库中删除对应记录,修改一个Entity Bean时,容器会自动将Entity Bean的状态和数据库同步.

3.MessageDriven Bean是EJB2.0中引入的新的企业Bean,它基于JMS消息,只能接收客户端发送的JMS消息然后处理.MDB实际上是一个异步的无状态Session Bean,客户端调用MDB后无需等待,立刻返回,MDB将异步处理客户请求.这适合于需要异步处理请求的场合,比如订单处理,这样就能避免客户端长时间的等待一个方法调用直到返回结果.

EJB实际上是SUN的J2EE中的一套规范,并且规定了一系列的API用来实现把EJB概念转换成EJB产品.



在BMP中, 由实体Bean 的开发人员负责实现Entity Bean 的状态(对象)与数据存储中内容(一般为关系数据) 的相互映射

在CMP中, 由容器实现这个映射, 开发人员只需在实体Bean 描述文件中指定这个映射关系, 其他的工作由容器自动完成



#### hibernate

<hibernate-mapping>
　　<class name="sample.orm.hibernate.User" table="user" catalog="test">
　　<id name="userID" type="java.lang.Integer">
　　<column name="id" />
　　<generator class="assigned" />
　　</id>
　　<property name="userName" type="java.lang.String">
　　<column name="name" />
　　</property>
　　</class>
　　</hibernate-mapping>

## 网络数据库的Web应用
### Web应用体系结构
一个基于HTTP协议的Web服务器主要是有Web Server, Application Server和数据库三层结构组成

Web server

Web server是整个Web 服务器的入口, 主要作用并不是进行请求的处理, 而是监听从Internet上传来的HTTP请求, 然后和用户建立连接. 与用户建立连接后, Web server根据请求所访问的资源不同, 对请求进行不同的处理

如果请求的对象是HTML脚本, 静态对象, Applet 的字节码等静态资源, 则Web Server将这些资源从本体的文件系统中读出, 发送回客户端

如果请求的是CGI程序, ASP或PHP,脚本, 则Web Server会将请求发送给相应的服务对象, 例如CGI程序或脚本解释器

如果请求的是分布式对象, 则Web Server需要将请求发送给Application Server, 由Application Server 做进一步处理

根据我们的定义,作为应用程序服务器,它通过各种协议,可以包括HTTP,把商业逻辑暴露给(expose)客户端应用程序.Web服务器主要是处理向浏览器发送HTML以供浏览,而应用程序服务器提供访问商业逻辑的途径以供客户端应用程序使用.应用程序使用此商业逻辑就象你调用对象的一个方法(或过程语言中的一个函数)一样.

Application Server

一个HTTP请求服务可能是Java Servlet, PL/SQL插件等独立的应用对象, 也可能是分布式的应用对象, Application Server 所要做的就是要能够识别出请求所要调用的应用对象, 并调用这些应用对象为请求服务, 同时还要负责客户端和应用对象之间的通信.

数据库

与数据库进行连接的实际上并不是Application Server, 而是由Application Server所调用的那些应用对象. 应用对象和数据库的连接可以有很多方式, 既可以通过标准的数据库接口ODBC, 也可以通过专门面向某个数据库的数据驱动



具体实现

Web Server: 可以选择Apache, Microsoft IIS,  Netscape Enterprise Server etc

Application Server: Oracle 9i Application Server(iAS), IBM Web Spher, Tomcat

数据库: Oracle, SQL Server, MySQL.

### 常用的Web应用开发方式
静态HTML, CGI, ASP, PHP, PL/SQL, Java(Java Applet, JSP)

CGI 是编译执行的, 所以效率高, 但是系统是开辟进程的方式运行CGI程序, 所以开销大

ASP 解释脚本, VBScript, JavaScript, 开辟线程进行服务, 可以调用ActiveX, 分布式计算

PHP: 脚本由类C元语言编写, 不支持分布式, 跨平台

PL/SQL: 数据库内执行PL/SQL, 因此效率高

Java Applet: 依赖于浏览器, 在客户端运行的程序(需要Java虚拟机), 

因此当Applet 开始运行时, 就不用在通过无连接和无状态的HTTP进行通信, 而是可以通	过自己调用Socket直接和服务器进行端到端的通信

JSP: java server page, JSP 是基于Java虚拟机运行的

和ASP的单纯解释执行不同, 第一次访问JSP的脚本时, 需要通过Java编译器将JSP的脚本程序编译成Servlet, 然后由Java虚拟器解释之星这个Servlet, 以后在访问这个脚本时则不需要重新编译, 而是直接用Java虚拟机运行编译好的Servlet

集合了PHP跨平台和ASP的分布式对象支持的优点

