# 半导体
能带  
![能带](http://i.imgbox.com/ImFYUiOp.jpg)

费米能级  
![费米能级](http://i.imgbox.com/KFmNlmSY.jpg)

半导体的能带  
![半导体的能带](http://i.imgbox.com/n8XD28Mc.jpg)

dopage: 使半导体导电  
![dopage](http://i.imgbox.com/fqVVgWFf.jpg)

N型和P型半导体  
![N型和P型半导体](http://i.imgbox.com/MxwQQfLO.jpg)

# 电路元器件
参考自[半导体基础](http://book.douban.com/subject/2108871/)

在电路元器件中, 包括**有源元件和无源元件**

## 无源元件
阻碍电流的流动, 包括阻止电流流动的电阻器, 阻止电流(磁通)发生变化的电感器和阻止电压(电荷量)发生变化的电容器.

阻抗L'impédance électrique: Z表示, 单位为$\Omega$.

电容具有储存电能的作用, 电感器具有储存磁能的作用.
电压相量和电流相量的比值称为阻抗,为一复数.电容器的阻抗只有电抗成份(即复数只有虚部,实部为0),数值如下  
![电容阻抗](http://upload.wikimedia.org/math/e/2/4/e24999de39cef7acecb252ed3a07bdb3.png)

Z_C = \frac{-j}{2 \pi f C} = j X_C 

其中:

X_C = -\frac{1}{\omega C}是电容器的电抗

\omega = 2 \pi f,是角频率

f = 输入频率

C = 电容,单位是法拉

j=\sqrt{-1}

电容的阻抗和频率成反比,若有非常高频的电流流过电容,阻抗值几乎为0,此时可将电容视为短路.相反地,若有非常低频的电流流过电容,阻抗值相当大,此时可将电容视为断路.

电容的阻抗的实部为0,虚部为负值.虚部的负数表示电流领先电压90度的相角,这和电感恰好相反,电感的电流落后电压90度的相角.

**电感**

Z_L = j L \omega = j X et X = L \omega > 0.

**电导率**: conductance, 物质传输电流能力强弱的一种測量值

电导率 \sigma\,\! 是以欧姆定律定义为电流密度 \mathbf{J}\,\! 和电场强度 \mathbf{E}\,\! 的比率:

\mathbf{J} = \sigma \mathbf{E}\,\! 

电导率是电阻率 \rho\,\! 的倒数.在国际单位制中的单位是西门子/米 (S \cdot m^{-1}):

\sigma = {1\over\rho}\,\!

## 有源元件
能够产生电流并放大电流的器件. 主角之一是晶体管.

按照晶体管不同的使用方法, 可以构成模拟电路(连续变化)和数字电路(离散取值)两类不同的电路.  
在模拟电路中, 利用晶体管的放大作用; 而在数字电路中, 利用晶体管的开关作用.

用水闸形象描绘晶体管的放大功能. 晶体管的输入端相当于水闸, 公共端相当于水的入口, 输出端相当于水的出口. 只要水闸稍微打开一点, 就会使水流发生很大的变化; 
相对应地, 所谓放大, 就是当输入波形有微小的振幅变化时, 输出地波形就会发生很大的振幅变化.

在数字电路中, 水闸的开和闭

单晶生长的方法:

1. 像雪花那样由气体结晶
1. 像冰那样从液体结晶
1. 像陶瓷那样由固体粒子结晶

对于硅单晶来说, 主流的方法是液体结晶.

# Diode
polarisation directe: 正向导通

polarisation inverse: 断路

# PN结
## 杂质半导体 dopage
不含杂质的纯粹的半导体物质叫做本征半导体(I 型半导体 semi-conducteur intrinseque)

### N型半导体
(N为Negative的字头,由于电子带负电而得此名):
掺入少量杂质磷元素(或锑元素)的硅晶体(或锗晶体)中,由于半导体原子(如硅原子)被杂质原子取代,磷原子外层的五个外层电子的其中四个与周围的半导体原子形成共价键,**多出的一个电子几乎不受束缚,较为容易地成为自由电子**.
于是,N型半导体就成为了含电子浓度较高的半导体,其导电性主要是因为自由电子导电.  

### P型半导体
(P为Positive的字头,由于空穴带正电而得此名):
掺入少量杂质硼元素(或铟元素)的硅晶体(或锗晶体)中,由于半导体原子(如硅原子)被杂质原子取代,硼原子外层的三个外层电子与周围的半导体原子形成共价键的时候,会**产生一个"空穴",这个空穴可能吸引束缚电子来"填充"**,使得硼原子成为带负电的离子.
这样,这类半导体由于含有较高浓度的"空穴"("相当于"正电荷),成为能够导电的物质.

**空穴导电的实质**: 刚开始, 我们考虑空穴右边的电子, 电子向左移动填充空穴, 自然电子原来的位置空出来, 形成一个空穴. 
其结果相当于电子与空穴交换位置, 电子左移, 空穴右移.
然后右边的电子继续与空穴交换位置,..., 最终的结果就是空穴沿着电场的方向移动. 看起来宛如正的电荷在移动.

## PN结的形成
在一块完整的硅片上,用不同的掺杂工艺使其一边形成N型半导体,另一边形成P型半导体,那么在两种半导体的交界面附近就形成了PN结.  
![PN结](https://encrypted-tbn1.gstatic.com/images?q=tbn:ANd9GcTPAEhtIiAwPxSFR81daEhDstJgy1C19Z-7DJCnwGRJ3HPGzaZd)

![Zone_de_charge_espace](http://upload.wikimedia.org/wikipedia/commons/1/1a/Zone_de_charge_espace.png)

载流子: 电子与空穴载流子

在P型半导体和N型半导体结合后,由于N型区内电子很多而空穴很少,而P型区内空穴很多电子很少,在它们的交界处就出现了电子和空穴的**浓度差别**.
这样,电子和空穴都要**从浓度高的地方向浓度低的地方扩散**.
于是,有一些电子要从N型区向P型区扩散,也有一些空穴要从P型区向N型区扩散.
它们扩散的结果就使P区一边失去空穴,留下了带负电的杂质离子,N区一边失去电子,留下了带正电的杂质离子.半导体中的离子不能任意移动,因此不参与导电.
这些不能移动的带电粒子在P和N区交界面附近,形成了一个很薄的空间电荷区.
  
在出现了空间电荷区以后,由于正负电荷之间的相互作用,在空间电荷区就形成了一个**内电场**,其方向是从**带正电的N区指向带负电的P区**.
显然,这个电场的方向与载流子扩散运动的方向相反,阻止扩散.  
另一方面,这个电场将使N区的少数载流子空穴向P区**漂移**,使P区的少数载流子电子向N区漂移,漂移运动的方向正好与扩散运动的方向相反.
从N区漂移到P区的空穴补充了原来交界面上P区所失去的空穴,从P区漂移到N区的电子补充了原来交界面上N区所失去的电子,这就使空间电荷减少,内电场减弱.
因此,漂移运动的结果是使空间电荷区变窄,扩散运动加强.  
最后,**多子的扩散和少子的漂移达到动态平衡**.在P型半导体和N型半导体的结合面两侧,留下离子薄层,这个离子薄层形成的空间电荷区称为PN结.PN结的内电场方向由N区指向P区.
在空间电荷区,由于缺少多子,所以也称耗尽层zone de déplétion.

耗尽层(空间电荷层, 空间电荷区): 所谓耗尽, 是指几乎没有载流子(能够流动的荷电粒子)

在热平衡状态下, P 型区和N 型区的费米能级是相等的, 如下图所示  
![热平衡下的费米能级](http://www.cali-light.com/b2b/file/upload/201108/25/1535324024.jpg)

## 击穿性
PN结加反向电压时,空间电荷区变宽,区中电场增强.反向电压增大到一定程度时,反向电流将突然增大.如果外电路不能限制电流,则电流会大到将PN结烧毁.反向电流突然增大时的电压称击穿电压.基本的击穿机构有两种,即隧道击穿(也叫齐纳击穿)和雪崩击穿,前者击穿电压小于6V,有负的温度系数,后者击穿电压大于6V,有正的温度系数.

### 雪崩击穿
耗尽区内强电场作用下反向电流剧增  
阻挡层中的载流子漂移速度随内部电场的增强而相应加快到一定程度时,其动能足以把束缚在共价键中的价电子碰撞出来,产生自由电子—空穴对,新产生的载流子在强电场作用下,再去碰撞其它中性原子,又产生新的自由电子—空穴对,如此连锁反应,使阻挡层中的载流子数量急剧增加,象雪崩一样.雪崩击穿发生在掺杂浓度较低的PN结中,阻挡层宽,碰撞电离的机会较多,雪崩击穿的击穿电压高.

### 齐纳击穿
隧道效应  
齐纳击穿通常发生在掺杂浓度很高的PN结内.**由于掺杂浓度很高,PN结很窄**,这样即使施加较小的反向电压(5V以下),结层中的电场却很强(可达2.5×105V/m左右).在强电场作用下,会强行促使PN结内原子的**价电子从共价键中拉出来**,形成"电子一空穴对",从而产生大量的载流子.它们在反向电压的作用下,形成很大的反向电流,出现了击穿.显然,**齐纳击穿的物理本质是场致电离**.

采取适当的掺杂工艺,将硅PN结的雪崩击穿电压可控制在8～1000V.而齐纳击穿电压低于5V.在5～8V之间两种击穿可能同时发生.

击穿电压的温度特性:温度升高后,晶格振动加剧,致使载流子运动的平 均自由路程缩短,碰撞前动能减小,必须加大反向电压才能发生雪崩击穿具有正的温度系数,但温度升高,共价键中的价电子能量状态高,从而齐纳击穿电压随温度升高而降低,具有负的温度系数.

## 单向导电性
### PN结加正向电压时导通
![加正向电压](http://ts3.mm.bing.net/th?id=HN.608023771291321554&pid=1.7)

如果电源的正极接P区,负极接N区,外加的正向电压有一部分降落在PN结区,PN结处于正向偏置.
电流便从P型一边流向N型一边,空穴和电子都向界面运动,使空间电荷区变窄,电流可以顺利通过,方向与PN结内电场方向相反,**削弱了内电场**.
于是,内电场对多子扩散运动的阻碍减弱,扩散电流加大.扩散电流远大于漂移电流,可忽略漂移电流的影响,PN结呈现低阻性.

当PN结加正向电压时, Z.C.E的宽度将减小, 当正向电压适当时, 宽度将减少为0, 这个电压值就是PN结的tension de seuil. 

### PN结加反向电压时截止
![加反向电压](http://www.icsmar.com/upload/images/editorUp/2010/01/6bf156d073aa4a0b9b45ef5410339c82.jpg)
如果电源的正极接N区,负极接P区,外加的反向电压有一部分降落在PN结区,PN结处于反向偏置.
则空穴和电子都向远离界面的方向运动,使空间电荷区变宽,电流不能流过,方向与PN结内电场方向相同,**加强了内电场**.
内电场对多子扩散运动的阻碍增强,扩散电流大大减小.此时PN结区的少子在内电场作用下形成的漂移电流大于扩散电流,可忽略扩散电流,PN结呈现高阻性.

加反向电压时, Z.C.E的宽度将增加.

在一定的温度条件下,由本征激发决定的少子浓度是一定的,故少子形成的漂移电流是恒定的,基本上与所加反向电压的大小无关,这个电流也称为反向饱和电流.  
PN结加正向电压时,呈现低电阻,具有较大的正向扩散电流,PN结加反向电压时,呈现高电阻,具有很小的反向漂移电流.由此可以得出结论:PN结具有单向导电性.

## Application
PN 结具有整流, 开关及电容器的作用

整流: 电流只能沿一个方向流动. 交流变直流  
整流二极管工作在正向偏置特性范围.  
整流电路把交流电变为脉动的直流电, 这种直流电波动很大, 主要是含有不同幅值和频率的交流成分. 为了获得平稳的直流电, 必须利用滤波器将交流成分滤掉.  
常用的滤波器有电容滤波, 电感滤波和复合式滤波.

耗尽层还具有电容器效应, 这种电容器叫做结电容器, 其特点是耗尽层宽度可以随反向偏转电压的大小而改变, 可以作为可变电容器工作.  
变容二极管工作在反向偏置特性范围

利用反向偏压时的齐纳击穿和雪崩击穿现象制成的稳压二极管(齐纳二极管)  
![稳压管](http://www.shenmeshi.com/Science/UploadFiles_8975/200805/20080522131839684.jpg)  
反向电流在I_{Zmin} 和 I_{Zmax} 之间时, 电压变化非常小.

利用隧道效应的负阻效应制成的隧道二极管(江崎二极管)

LED(发光二极管light-emitting diode) 是自发辐射émission spontanée  
当PN结加正向电压时, 多数载流子在进行扩散运动的过程中相遇而复合, 其过剩的能量以光子的形式释放出来.

LD(激光二极管Laser diode) 是受激辐射émission stimulée

# Transistor diplolaire
Son principe de fonctionnement est basé sur deux jonctions PN, l'une en direct et l'autre en inverse.

双极性晶体管的三个极:
B : Base基极 - C : Collecteur集电极 - E : émetteur发射极

器件结构的要求

1. 发射区的自由电子密度要比基区的空穴密度高的多
1. 基区的宽度非常薄, 越薄, 电流放大倍数越大
1. 发射结要正向偏置(加正向电压),集电结要反偏置(放大作用)

集电极电流的变化部分与基极电流的变化部分之比叫做电流放大倍数, 一般用$\beta$ 表示.

B-E 间正偏压, C-B 间正偏压: 开关作用, 数字电路

B-E 间正偏压, C-B 间反偏压: 放大电路, 模拟电路

## Types et symboles
晶体管主要分为两大类:双极性晶体管(BJT,bipolar junction transistor,BJT)和场效应晶体管(FET, field effect transistor).

le type : NPN ou PNP. Ces deux types sont complémentaires, c'est-à-dire que le sens des courants et tensions pour le PNP est le complément de ceux du NPN. **NPN ayant en général des caractéristiques meilleures** que les PNP, ils sont les plus utilisés. La suite de l'article discutera donc uniquement les circuits utilisant des transistors NPN ;

![Symboles de transistors bipolaires](http://upload.wikimedia.org/wikipedia/commons/thumb/c/cb/BJT_NPN_symbol_%28case%29.svg/70px-BJT_NPN_symbol_%28case%29.svg.png)  
La figure ci-contre montre le symbole et indique le nom des 3 électrodes des transistors. On peut donc distinguer 3 différences de potentiel intéressantes : VBE, VCE et Vcb ; et 3 courants : courant de base IB, d'émetteur IE et de collecteur IC. Cependant, ces 6 variables ne sont pas indépendantes. En effet, on peut écrire :  
V_{CE} = V_{CB} + V_{BE} et I_E = I_C + I_B

模型简化  
![Modèle simple d'un transistor en fonctionnement linéaire](http://upload.wikimedia.org/wikipedia/commons/thumb/f/fe/NPN_BJT_LSEM_CE_Simple.svg/500px-NPN_BJT_LSEM_CE_Simple.svg.png)
