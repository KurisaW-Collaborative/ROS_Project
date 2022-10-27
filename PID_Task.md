**目录**

[前言](#%E5%89%8D%E8%A8%80)

[仿真调参环境](#%E6%A8%A1%E6%8B%9F%E8%B0%83%E5%8F%82%E7%8E%AF%E5%A2%83)

[案例引入——小球位置控制](#%E6%A1%88%E4%BE%8B%E5%BC%95%E5%85%A5%E2%80%94%E2%80%94%E5%B0%8F%E7%90%83%E4%BD%8D%E7%BD%AE%E6%8E%A7%E5%88%B6)

[抛开案例——更专业地理解PID](#%E6%8A%9B%E5%BC%80%E6%A1%88%E4%BE%8B%E2%80%94%E2%80%94%E6%9B%B4%E4%B8%93%E4%B8%9A%E5%9C%B0%E7%90%86%E8%A7%A3PID)

[由虚到实——代码编写](#%E7%94%B1%E8%99%9A%E5%88%B0%E5%AE%9E%E2%80%94%E2%80%94%E4%BB%A3%E7%A0%81%E7%BC%96%E5%86%99)

[最后一步——PID参数调整](#PID%E5%8F%82%E6%95%B0%E8%B0%83%E8%AF%95)

[总结——使用PID的步骤](#%E6%80%BB%E7%BB%93%EF%BC%9A%E4%BD%BF%E7%94%A8PID%E7%9A%84%E6%AD%A5%E9%AA%A4)

[更进一步——串级PID](#%E4%B8%B2%E7%BA%A7PID)

---

# 前言

很多人应该都听说过PID，它的运算过程简单，并能在大多情况下实现较好的控制效果，因此它是工程实践中使用最广泛的控制方法之一。

抛开公式，我将带你从案例出发，详细了解PID的工作原理和使用方法。

_注：阅读本文不需要有过多的基础知识，只需中学物理和数学知识就能看懂（当然如果有高等数学知识和单片机知识的话理解起来会更容易）_

---

# 仿真调参环境

我专门为本文搭了一个在线仿真环境，下面使用的案例都来自这个环境，读者可以搭配使用[https://skythinker.gitee.io/pid-simulator-web/![](https://csdnimg.cn/release/blog_editor_html/release2.0.6/ckeditor/plugins/CsdnLink/icons/icon-default.png?t=M1H3)https://skythinker.gitee.io/pid-simulator-web/](http://skythinker.gitee.io/pid-simulator-web/ "https://skythinker.gitee.io/pid-simulator-web/")

---

# 案例引入——小球位置控制

## 任务介绍

我们假设有一个一维的坐标轴（向右为正方向），在上面上有一个小球（可以看作质点），小球不受任何阻力，可以自由左右滑动；另外，我们还为小球规定了一个目标位置（图中的绿色标线）：

![](https://img-blog.csdnimg.cn/1e5ca62efeae43b8a8f123470b411748.png?x-oss-process=image/watermark,type_d3F5LXplbmhlaQ,shadow_50,text_Q1NETiBAc2t5dGhpbmtlcjYxNg==,size_20,color_FFFFFF,t_70,g_se,x_16)

小球控制虚拟环境

现在我们有下述任务：

- 目标：在小球上施加一个水平方向的力（称为控制力），使小球在偏离目标位置时回到目标位置
- 已知条件：小球的实时坐标、目标位置坐标

看到这里的你可以停下来想一想，应该用什么样的策略来计算这个力呢？

到这里大多数人应该都能想出来这样的方法：

_“当小球在目标左边的时候向右施力，当小球在目标右边的时候向左施力，就可以保证小球一直在目标位置上了”_

思路是非常正确的，但这个策略仍不够完善，因为小球存在惯性，我们施加的力将小球拉回目标位置后小球还会具有一定的速度继续运动，并不会直接停在目标位置。

## 用PID完成任务

接下来我们来看看如果使用PID，我们应该如何计算出这个控制力呢？

### 误差计算

计算PID的第一步就是计算误差（Error）：**误差=目标值-反馈值**，在这个例子中，误差就是小球当前位置与目标值间的距离。

接下来的运算我们都会围绕误差进行，分为三个步骤使用误差分别算出一个分力，并将三个分力一起施加在小球上。

### 比例环节

第一个环节是比例环节**P\(Proportion\)**，这个环节产生的分力是：

![F_p=k_p*Error](https://latex.codecogs.com/gif.latex?F_p%3Dk_p*Error)

也就是说分力大小与误差成正比，当小球在目标左边的时候分力向右，当小球在目标右边的时候分力向左，其中![k_p](https://latex.codecogs.com/gif.latex?k_p)是比例系数。

比例环节的计算方法其实就与上面大家通过直觉得出的方法差不多，如果只有这个分力作用的话，会产生什么效果呢？

大家可能会发现这不就跟中学物理里的弹簧滑块模型是一样的嘛，力与距离成正比，很明显小球会以目标位置为中心进行左右摆动（简谐振动）（注：图中蓝色短线表示控制力）：

![](https://img-blog.csdnimg.cn/6a8ecaabfe624cd4a49499467d65b58e.gif)

只有比例环节时的小球运动

### 微分环节

那么如何让小球能够静止在目标点呢？这就要请出PID的另一个环节：微分环节**D\(Differential\)**。

微分环节也会计算出一个分力，计算方法是：

![F_d=k_d*\frac{\mathrm{d} Error}{\mathrm{d} t}](https://latex.codecogs.com/gif.latex?F_d%3Dk_d*%5Cfrac%7B%5Cmathrm%7Bd%7D%20Error%7D%7B%5Cmathrm%7Bd%7D%20t%7D)

也就是说，这个分力与误差的变化速度有关，在目标位置不变的情况下，小球向右运动时误差变化速度为负，分力向左；反之当小球向左运动时分力向右；综合看来，微分环节产生的分力始终阻碍小球的运动。

因此如果在刚刚的基础上加入微分产生的分力，就会产生一个阻尼效果，小球会仿佛始终受到一个阻力，因此左右摆动的幅度会逐渐减小，最终收敛到目标位置上：

![](https://img-blog.csdnimg.cn/09701099ed11424c94961ecdf6e3058b.gif)

有比例和微分环节时的小球运动

由公式还可以看出，微分系数![k_d](https://latex.codecogs.com/gif.latex?k_d)可以影响这个“阻力”的大小，因此如果我们把系数调大一些，就可以让小球的运动收敛得更快一些：

![](https://img-blog.csdnimg.cn/7e5531c7dcf2409c96074dc39298b536.gif)

调大kd后的小球运动

到这里，其实我们已经完成我们的目标任务了，小球可以在驱动力的作用下运动到目标位置。

### 积分环节

但现在，我们更希望在小球有一些外部干扰时也能实现上面的效果，比如我们在小球上加上一个水平向右的恒力，此时会发生什么呢？

![](https://img-blog.csdnimg.cn/2f191fff0ae34ee59ed77aa72a926196.png?x-oss-process=image/watermark,type_d3F5LXplbmhlaQ,shadow_50,text_Q1NETiBAc2t5dGhpbmtlcjYxNg==,size_11,color_FFFFFF,t_70,g_se,x_16)

恒力干扰下小球静止状态

小球在运动过程中仍然会像之前一样接近目标点，但在最终停下来时我们会发现，小球无法精确停在目标点上，而是像上图一样停在离目标点有一定距离的地方，此时控制力与干扰恒力平衡，小球静止。

稍加分析我们就能发现，此时小球静止，微分环节产生的分力为零，控制力完全由比例环节产生，且若距离更小则比例环节的输出更小，更无法平衡干扰力，因此小球无法继续向目标点接近。

此时就需要我们的第三个环节出场了：积分环节**I\(Integral\)**，它的计算方法是:

![F_i=k_i*\int Error\mathrm{d}t](https://latex.codecogs.com/gif.latex?F_i%3Dk_i*%5Cint%20Error%5Cmathrm%7Bd%7Dt)

也就是说积分环节产生的分力输出正比于误差的积分，当误差持续存在时，这个分力会逐渐变大，试图消除误差。

加入积分作用，我们的PID就能完美实现在有恒力干扰的情况下对小球的控制了：

![](https://img-blog.csdnimg.cn/b67c4509897745ad97da69e3130dde76.gif)

PID作用下小球控制效果

---

# 抛开案例——更专业地理解PID

## 常用术语

- 被控对象：需要控制的对象，案例中指小球
- 目标值：期望被控对象达到的状态量，案例中指目标位置的坐标
- 反馈值：被控对象当前时刻的状态量，案例中指小球的实时位置坐标
- 输出量：PID的计算结果，案例中指控制力
- 误差：目标值-反馈值
- 稳态误差：系统稳定状态下仍存在的误差，如案例中加入干扰恒力后小球静止时仍存在的误差

![](https://img-blog.csdnimg.cn/img_convert/9737301a6ab3f28b1205c15b0a148318.png)

不同参数下系统的阶跃响应（源：百度百科）

- 阶跃输入：在稳定状态下目标值发生突然变化（上图目标值在0时刻由0跃升到虚线位置）
- 阶跃响应：阶跃输入后被控对象的跟随状态，能够代表系统的控制性能（上图彩色线条）
- 响应速度：阶跃输入后被控对象再次到达目标值的速度
- 超调量：阶跃输入后，被控对象到达目标值后超出目标值的距离

## PID计算过程

![](https://img-blog.csdnimg.cn/5d7f75bf0d8f4656bcfb30f7a683b4ce.png?x-oss-process=image/watermark,type_d3F5LXplbmhlaQ,shadow_50,text_Q1NETiBAc2t5dGhpbmtlcjYxNg==,size_20,color_FFFFFF,t_70,g_se,x_16)

PID信号框图

上图就是PID的信号框图，表示了PID的运行过程：

1.  为系统指定一个目标值
2.  PID将目标值与被控对象当前的反馈量作差得到误差
3.  PID将误差值分别经过三个环节计算得到输出分量，三个分量加起来得到PID的输出
4.  将PID的输出施加到被控对象上，使反馈量向目标值靠拢

## PID三个环节的作用

由控制小球案例我们可以总结出PID三个环节各自的主要作用和效应：

- 比例环节：起主要控制作用，使反馈量向目标值靠拢，但可能导致振荡
- 积分环节：消除稳态误差，但会增加超调量
- 微分环节：产生阻尼效果，抑制振荡和超调，但会降低响应速度

## PID中物理量的设计

我们在设计PID时主要关注三个量：**目标值**、**反馈值**、**输出值**，PID会根据目标值和反馈值计算输出值。

需要强调的是，PID的整个计算过程其实与被控对象是什么完全没有关系，它只是负责进行数值计算，而我们——作为控制系统的设计者，就需要为PID指定这三个量所对应的实际物理量，这在不同的控制系统中是不一样的。

那么如何确定实际物理量呢，我为大家总结了一个常用准则：

- 目标值和反馈值通常为同种物理量，就是你需要控制的物理量
- 输出值通常是直接驱动被控对象的控制量
- 输出量作用在被控对象上需要经过时间积累才会产生反馈量的变化，换言之输出值通常为反馈值对于时间的低阶物理量。_比如：目标/反馈值为位置，则输出值可以为速度或加速度_
- 对于线性关系的两个物理量（只差一个系数），可以直接替换。_比如：目标/反馈值为位置，输出值可以为加速度，但我们无法直接控制加速度而是控制驱动力大小，由于驱动力与加速度只差一个系数（F=ma），因此可以将输出值直接定为驱动力_

接下来给出几个例子：

> 任务一：对小球进行速度控制
>
> 可用条件：已知小球的实时速度，并且可施加一个力来改变小球的速度
>
> PID目标值：需要小球达到的速度
>
> PID反馈值：小球的实时速度
>
> PID输出值：施加在小球上的力
>
> 分析：小球加速度是小球速度的低阶物理量，而施加的力正比于小球加速度

> 任务二：对电机转速进行控制
>
> 可用条件：已知电机的实时转速，并且可控制电机中流过的电流大小
>
> PID目标值：需要电机达到的转速
>
> PID反馈值：电机的实时转速
>
> PID输出值：电机中流过的电流大小
>
> 分析：电机中流过的电流大小近似正比于电机的扭矩，也就近似正比于电机角加速度的大小，是转速的低阶物理量，因此可以用电流大小作为输出值

> 任务三：液位高度控制
>
> 描述：容器有进水口和出水口，需要通过进水口的阀门控制容器内液位的高度
>
> 可用条件：容器内液位实时高度，可控制进水口阀门液体流速
>
> PID目标值：需要达到的液位高度
>
> PID反馈值：液位实时高度
>
> PID输出值：阀门液体流速
>
> 分析：阀门液体流速正比于液位高度的变化速度，是液位高度的低阶物理量

---

# 由虚到实——代码编写

## 程序流程

根据上面的步骤，我们其实很容易就能得出程序的执行流程了，就是在计算误差后逐个进行PID各环节的计算就行了，要注意的是整个**采样-计算-输出**的流程需要定时执行，可以在每次流程运行完后延时一段固定的时间（一般而言计算频率不高于传感器反馈频率，而若频率过低可能使控制精度下降）。

![](https://img-blog.csdnimg.cn/62bed4b0b410425ea09c9836a3605a78.png?x-oss-process=image/watermark,type_d3F5LXplbmhlaQ,shadow_50,text_Q1NETiBAc2t5dGhpbmtlcjYxNg==,size_20,color_FFFFFF,t_70,g_se,x_16)

代码流程图

## 实现细节

有一个问题没有解决，积分和微分应该怎么算呢？毕竟微积分都是连续的，而我们采样得到的是离散的数据点。其实也很简单，离散状态下的积分计算其实就是把过去采样得到的所有误差加在一起，而微分计算其实就是把这一轮计算得到的误差与上一轮的误差相减。

最后，我们一般还会对PID的积分和输出进行限幅（规定上下限），积分限幅可以减小积分引起的超调，输出限幅可以保护执行机构或被控对象。

## C语言代码

```cpp
//首先定义PID结构体用于存放一个PID的数据
typedef struct
{
   	float kp,ki,kd;//三个系数
    float error,lastError;//误差、上次误差
    float integral,maxIntegral;//积分、积分限幅
    float output,maxOutput;//输出、输出限幅
}PID;

//用于初始化pid参数的函数
void PID_Init(PID *pid,float p,float i,float d,float maxI,float maxOut)
{
    pid->kp=p;
    pid->ki=i;
    pid->kd=d;
    pid->maxIntegral=maxI;
    pid->maxOutput=maxOut;
}

//进行一次pid计算
//参数为(pid结构体,目标值,反馈值)，计算结果放在pid结构体的output成员中
void PID_Calc(PID *pid,float reference,float feedback)
{
 	//更新数据
    pid->lastError=pid->error;//将旧error存起来
    pid->error=reference-feedback;//计算新error
    //计算微分
    float dout=(pid->error-pid->lastError)*pid->kd;
    //计算比例
    float pout=pid->error*pid->kp;
    //计算积分
    pid->integral+=pid->error*pid->ki;
    //积分限幅
    if(pid->integral > pid->maxIntegral) pid->integral=pid->maxIntegral;
    else if(pid->integral < -pid->maxIntegral) pid->integral=-pid->maxIntegral;
    //计算输出
    pid->output=pout+dout+pid->integral;
    //输出限幅
    if(pid->output > pid->maxOutput) pid->output=pid->maxOutput;
    else if(pid->output < -pid->maxOutput) pid->output=-pid->maxOutput;
}

PID mypid;//创建一个PID结构体变量

int main()
{
    //...这里有些其他初始化代码
    PID_Init(&mypid,10,1,5,800,1000);//初始化PID参数
    while(1)//进入循环运行
    {
        float feedbackValue=...;//这里获取到被控对象的反馈值
        float targetValue=...;//这里获取到目标值
        PID_Calc(&mypid,targetValue,feedbackValue);//进行PID计算，结果在output成员变量中
        设定执行器输出大小(mypid.output);
        delay(10);//等待一定时间再开始下一次循环
    }
}
```

---

# 最后一步——PID参数调整

在完成控制器代码编写后，就要连接好系统进行调参了，我们需要确定最合适的![k_pk_ik_d](https://latex.codecogs.com/gif.latex?k_pk_ik_d)使控制效果最优。

通常还是使用经验法调参，通俗而言就是“试参数”，测试多个参数选取最好的控制效果，一般的步骤如下：

1.  先将所有参数置零
2.  将输出限幅设为执行机构能接受的最大值
3.  增大p参数，使响应速度达到比较好的水平
4.  若存在稳态误差，逐渐增加i参数和积分限幅，使稳态误差消失
5.  若希望减少超调或振荡，逐渐增加d参数，在保证响应速度的前提下尽可能降低超调

此时大家可以使用上述的小球仿真环境体验一下各参数对系统的影响。到这里，我们就已经能够使用PID来控制各种对象了。

---

# 总结——使用PID的步骤

1.  确定需要控制的对象，确定需要控制的物理量，确定反馈量的获取方式，确定被控对象的控制方式
2.  检查目标值、反馈值、输出值对应物理量的关系是否符合上面说的准则
3.  编写代码，将上述三个值的数值变量传入PID进行运算，并将PID运算结果输出到执行机构
4.  进行参数调整

---

# 更进一步——串级PID

## 从单级到串级

当我们在进行小球的位置控制时，我们可能经常会发现一个问题，就是如果小球与目标之间的距离较远的话，小球在运动过程中的速度会很快，同时也总是会导致较大的超调，而且不论怎么修改参数都很难让系统的表现更好一些。

![](https://img-blog.csdnimg.cn/d704fbfaf36549388a6bee98a1661d5d.gif)

单级PID控制小球效果

这时你可能会想，如果运动过程中的速度没这么快就好了，这样就不会冲过头了。没错，这就要用到串级PID了。

我们上面所说的算法其实就是单级PID，目标值和反馈值经过一次PID计算就得到输出值并直接作为控制量，如果目标物理量和输出物理量直接不止差了一阶的话，中间阶次的物理量我们是无法控制的。_比如：目标物理量是位置，输出物理量是加速度，则小球的速度是无法控制的。_

而串级PID就可以改善这一点。串级PID其实就是两个单级PID“串”在一起组成的，它的信号框图如下：

![](https://img-blog.csdnimg.cn/b59feebe4e6e4de6a32fcee88a35b525.png?x-oss-process=image/watermark,type_d3F5LXplbmhlaQ,shadow_50,text_Q1NETiBAc2t5dGhpbmtlcjYxNg==,size_20,color_FFFFFF,t_70,g_se,x_16)

串级PID信号框图

图中的外环和内环就分别是一个单级PID，每个单级PID就如我们之前所说，需要获取一个目标值和一个反馈值，然后可以产生一个输出值。串级PID中两个环相“串”的方式就是将外环的输出作为内环的目标值。

## 串级PID的物理量

如果将串级PID看作一个整体，可以看到他有三个输入和一个输出，而此时被控对象也需要提供两个反馈量，那么它们都应该对应些什么物理量呢？

首先我们回到最开始的小球案例中，如果用串级PID完成同样的任务，应该这样设计：

> 可用条件：小球实时位置、小球实时速度、施加在小球上的控制力
>
> 目标值：小球目标位置
>
> 外环反馈：小球实时位置
>
> 内环反馈：小球实时速度
>
> 输出值：施加在小球上的控制力

此时的信号框图会变成这样：

![](https://img-blog.csdnimg.cn/3577d61ff13140d28ee23cbaa9982aa4.png?x-oss-process=image/watermark,type_d3F5LXplbmhlaQ,shadow_50,text_Q1NETiBAc2t5dGhpbmtlcjYxNg==,size_20,color_FFFFFF,t_70,g_se,x_16)

串级PID控制小球信号框图

可以发现，内环与小球构成了一个恒速系统，PID内环负责小球的速度控制；而如果把内环和小球看作一个整体被控对象，外环又与这个对象一起构成了一个位置控制系统，外环负责位置控制；总体来说，外环负责根据小球位置误差计算出小球需要达到的速度，而内环负责计算出控制力使小球达到这个目标速度，两个环协同工作，就可以完成任务了。

如果不局限于这个案例来说，串级PID的内环一般负责低阶物理量的调节，而外环负责高阶物理量的调节并计算出低阶物理量的目标值，比如下面这个例子:

> 任务：对电机进行串级角度控制
>
> 可用条件：电机实时角度、电机实时转速、可以控制电机电流大小
>
> 外环目标值：需要电机达到的角度
>
> 外环反馈值：电机的实时角度
>
> 内环反馈值：电机的实时速度
>
> 输出值：电机电流大小
>
> 分析：外环负责电机角度控制，根据电机目标角度和反馈角度计算出目标转速；内环负责转速控制，根据速度反馈和目标转速计算出电流

## 串级PID的效果

回到我们的小球控制案例，之前说使用串级之后我们就可以对速度进行控制了，如何进行控制呢？其实就是对外环PID的输出进行限幅，因为外环PID输出的是目标速度，限制外环输出就相当于限制了小球目标速度的最大值，内环也就会维持小球的速度不超过这个最大值了。

![](https://img-blog.csdnimg.cn/1a5ae8e11e7241f5ac878b51bcf7c787.gif)

串级PID控制小球效果

可以看到，使用串级PID后小球不再像之前那样“着急”地奔着目标而去，由于位置误差很大，外环输出在大部分时间内都处于给定的最大值，因此小球在运动中接近匀速，这个速度就是所设定的外环输出最大值。而且由于运动速度变慢了，超调也几乎消失了。这就是我们想要的“控制位置的同时还能控制速度”的效果。

## 串级PID的C语言代码

```cpp
//此处需要插入上面的单级PID相关代码

//串级PID的结构体，包含两个单级PID
typedef struct
{
    PID inner;//内环
    PID outer;//外环
    float output;//串级输出，等于inner.output
}CascadePID;

//串级PID的计算函数
//参数(PID结构体,外环目标值,外环反馈值,内环反馈值)
void PID_CascadeCalc(CascadePID *pid,float outerRef,float outerFdb,float innerFdb)
{
    PID_Calc(&pid->outer,outerRef,outerFdb);//计算外环
    PID_Calc(&pid->inner,pid->outer.output,innerFdb);//计算内环
    pid->output=pid->inner.output;//内环输出就是串级PID的输出
}

CascadePID mypid;//创建串级PID结构体变量

int main()
{
    //...其他初始化代码
    PID_Init(&mypid.inner,10,0,0,0,1000);//初始化内环参数
    PID_Init(&mypid.outer,5,0,5,0,100);//初始化外环参数
    while(1)//进入循环运行
    {
        float outerTarget=...;//获取外环目标值
        float outerFeedback=...;//获取外环反馈值
        float innerFeedback=...;//获取内环反馈值
        PID_CascadeCalc(&mypid,outerTarget,outerFeedback,innerFeedback);//进行PID计算
        设定执行机构输出大小(mypid.output);
        delay(10);//延时一段时间
    }
}
```

## 串级PID的调参

一般而言，需要先断开两环的连接，手动指定内环目标值，进行内环调参，当内环控制效果较好后再接上外环进行外环调参，具体的调参方法与单级PID相同。

此时大家也可以使用小球仿真环境体验一下串级控制的效果。

---

原创文章，未经授权请勿转载

by skythinker

2022.2.21