# Amplificateur Opérational
一种直流耦合,差模(差动模式)输入,通常为单端输出(Differential-in,
single-ended output)的高增益(gain)电压放大器.
因为刚开始主要用于加法,减法等模拟运算电路中,因而得名

通常使用运算放大器时,会将其输出端与其反相输入端(inverting input
node)连接,形成一负反馈组态.
原因是运算放大器的电压增益非常大,范围从数百至数万倍不等,使用负反馈方可保证电路的稳定运作.  
但是这并不代表运算放大器不能连接成正反馈组态,相反地,在很多需要产生震荡信号的系统中,正反馈组态的运算放大器是很常见的组成元件.

**理想运算放大器的操作原理**  
一个理想的运算放大器(ideal OPAMP)必须具备下列特性:

1. 无限大的输入阻抗($Z_{in}=\infty$):理想的运算放大器输入端不容许任何电流流入,即上图中的$V_{+}$与$V_{-}$两端点的电流信号恒为零,亦即输入阻抗无限大.
2. 趋近于零的输出阻抗($Z_{out}=0$):理想运算放大器的输出端是一个完美的电压源,无论流至放大器负载的电流如何变化,放大器的输出电压恒为一定值,亦即输出阻抗为零.
3. 无限大的开回路增益($Ad=\infty$):理想运算放大器的一个重要性质就是开回路的状态下,输入端的差动信号有无限大的电压增益,这个特性使得运算放大器在实际应用时十分适合加上负反馈组态.
4. 无限大的共模排斥比($CMRR=\infty$):理想运算放大器只能对$V_{+}$与$V_{-}$两端点电压的差值有反应,亦即只放大$V_{+}-V_{-}$的部份.对于两输入信号的相同的部分(即共模信号)将完全忽略不计.
5. 无限大的带宽:理想的运算放大器对于任何频率的输入信号都将以一样的差动增益放大之,不因为信号频率的改变而改变.
6. 趋于零的失调和漂移

**黄金规则**  
在负反馈的情况下,以上理想放大器之特性可总结为以下二点

1. 输出会使得输入电压间的差异成为零,$V_{+}=V_{-}$
2. 因输入阻抗无限大,故输入电流$I+=0,I-=0$

**组态**

- 开回路组态
- 负反馈组态
   - 反相闭回路放大器
   - 非反相闭回路放大器
- 正反馈组态, 会使用正反馈的情况有:
   - 作为有迟滞的比较器,形成施密特电路
   - 产生振荡

**实际运算放大器的局限**  
理想的运算放大器并不存在于这个世界上,所有的运算放大器电路都会遇到下列的问题,影响了它们的应用,也让设计者在使用运算放大器时必须考量到更多可能会发生的问题.

- 直流的非理想问题:
    实际的运算放大器开回路增益为有限的而不是无限的.根据电子电路相关书籍资料,以OP
    Amp 741元件而言,其开回路电压增益大约为200000.
- 有限的输入阻抗
- 大于零的输出阻抗
- 大于零的输入偏置电流
- 大于零的共模增益
- 交流的非理想问题:
    有限的带宽(訊號頻率高到一定程度時,也不能忽略頻率愈高,增益愈低的情形);
    输入电容
- 非线性的问题: 信号饱和, 延迟率, 非线性转换函数.
- 功率损耗的考量: 输出功率的限制, 输出电流的限制

"反馈放大电路的反馈极性在线路接成后就固定不变",这种说法是否正确?  
不正确.反馈放大电路的接线是在假定信号处于中频段时接成的负反馈.在信号处于低频段或高频段时,各级放大电路中由于电路存在电抗器件,出现附加相移,若级数较多时,附加相移大于或等于180度,就会使原来的负反馈转化为正反馈.


**fréquence de coupure**  
截止频率(英语:Cutoff frequency):指一个系统的输出信号能量开始大幅下降(在带阻滤波器中为大幅上升)的边界频率

La fréquence de coupure d'un circuit électronique est la fréquence
limite de fonctionnement utile d'un circuit électronique. La pulsation
de coupure est la pulsation correspondante.

Les fréquences de coupure basse et haute définissent la bande passante.

Conventionellement, cette limite est souvent fixée à la fréquence pour
laquelle la puissance de sortie est réduite de moité, pour un signal
d'entrée de même amplitude, par rapport à la puissance de sortie à la
fréquence de référence.

# Comparateur
[COMPARATEUR à HYSTéRéSIS](http://jacques.boudier.pagesperso-orange.fr/c_online/Electronique/Electronique%20analogique/cours%20009.htm)

# Amplification de Puissance
