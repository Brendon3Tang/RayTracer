# RayTracer

## Based on xxxx

# 1. Ray类
1. 射线原点$m_{origin}$
2. 射线方向$m_{direction}$
3. 射线上的每个点表示为$p(t) = m_{origin} + t * m_{direction}$，射线上每一个独立的点都有一个自己唯一的$t$值，表示在某个时间点时射线上的点$p$。

# 2. Camera类
1. Camera的position（Vector3D）
2. Camera看的方向Target（Vector3D）
3. 表示视锥近平面的左下角位置的$m_{lowerLeftCorner}$（Vector3D）
4. 表示近平面在摄像机坐标系下水平方向的跨度$m_{horizontal}$（Vector3D）
5. 表示近平面在摄像机坐标系下垂直方向的跨度$m_{vertical}$（Vector3D）
6. 视锥的垂直视域$m_{fovy}$和屏幕的宽高比$m_{aspect}$ （float）

Ray和Camera类定义好了之后要创建ray，为此需要光线的起点和方向：
   - 我们一般以camera的位置为起点。
   - Ray的方向比较复杂：Ray的方向是指向当前视锥体的某个点
   - 如何取得视锥体某个点的坐标呢？
     - 首先我们把值域为$[0, m_{width}]$和$[0, m_{height}]$的像素坐标映射到$[0,1]$上，得到$(u, v)$
     - 接下来我们根据$u$和$v$获取射线方向向量，这涉及到两个方面，一个摄像机的坐标系统，另一个是关于视锥的大小设置。摄像机的坐标轴决定了当前的朝向，视锥的大小设定决定了当前视域的大小。为此，我把摄像机与视锥合并一起，坐标系类型依然是右手坐标系。
     - 然后从视锥体的左下角$m_{lowerLeftCorner}$开始，把它加上 $u * m_{horizontal}$和 $v * m_{vertical}$ 


# 3. 物体求交
Base函数：Hitable
1. HitRecord有参数$t$，并且记录了hit点的位置、法线、材质信息
2. 创建$Hitable$。射线发射出去之后要与物体进行求交运行，对于这类能够被射线碰撞到的物体我们把它抽象为$Hitable$。函数返回值为bool，表示是否有相交。并用一个虚函数$hit$作为所有的碰撞求交的接口，他以ray、t_min、t_max为参数。
派生函数：Sphere
3. 然后需要向场景中添加物体。首先添加球体：
   1. 球体公式：$(x - cx)^2 + (y - cy)^2 + (z - cz)^2 = R^2$，其中$(cx, cy, cz)$为球的球心 $c$。，$R$为半径。
   2. 我们要求对于射线上的点$P(t) = Ori + Dir * t = (x, y, z)$与球的交点。
   3. 带入得到$(P - c)*(P - c) = R^2$。带入$P = P(t) = Ori + Dir * t$得到
      - $(Dir * Dir)*t^2 + 2(Dir * (Ori - c))*t + (Ori - c)*(Ori - c) - R^2 = 0$ 
   4. 用公式法解方程$t = \frac{-b +/- \sqrt{b^2 - 4 ac}}{2a}$，其中$a = (Dir * Dir), b = 2(Dir * (Ori - c)), c = (Ori - c)*(Ori - c) - R^2$。
   5. 在求解之前，先计算$b^2 - 4ac$ 是否大于0。如果是则有交点，注意，在有两个交点的情况下，我们首先取较近的点（用-号的根），不符合再取较远的那个点（用+号的根）；如果小于等于0则没交点（=0时有一个交点，但我们决定忽略它）。

派生函数：HitableList（🌟🌟🌟）
4. 注意，由于一般场景中不会只有一个物体，因此我们需要用一个vector保存所有的物体。
- 我们用Hitable作为基类，时HitableList继承Hitable，这样Hitable就好像是一个很大的物体一样。
- HitableList中有一个新的成员变量$m_{list}$，用来保存所有的物体
- HitableList有函数addHitable(Hitable *target) 可以保存各种不同的派生物体。
- HitableList有函数void clearHitable()可以清空列表。
- HitableList中也有一个虚函数hit，这个hit会做一个类似深度测试的检测。
  - HitableList会初始化一个closestSoFar保存当前这个光线和List里所有物体中最近的物体的最近的那个交点时的t。
  - HitableList的作用：对于当前光线ray，遍历List中的物体，调用这些具体物体的hit()函数，如果相交的话，hitAny = true，使ret（类型为HitRecord） = tempRec（当前这个ray的情况下，且t_max=当前的closetSoFar时的HitRecord），并不断更新closetSoFar，把closetSoFar再放入下一个具体物体的hit()函数作为t_max的拷贝。在不断的遍历下，最终ret的值会是最近一个与物体的交点(closetSoFar)时的HitRecord。然后返回true，使tracer知道找到了最近的交点，并且得到交点的HitRecord ret。
- 总的来看：继承之后我们将所有的物体存入HitableList中，然后HitableList->hit()会被tracer调用，然后HitableList里在调用他保存的每个具体物体的hit()函数判断是否有交点，且找到最近的交点的HitRecord，把它返回给tracer，且让tracer知道是有交点的。

# 4. 物体材质
- 建立一个基类Material，包含默认的构造函数与析构函数以及一个虚函数scatter。scatter会根据入射光线Ray与交点信息HitRecord，还有反射颜色attenuation，计算出散射后的光线Ray scatterRay。
  - 其中的attenuation本质上是物体自身的反射颜色，之所以叫attenuation是因为光线照射到物体上，物体一般会吸收光线中的大部分颜色，然后仅反射自身颜色的部分，这个过程使得光线在反射过程中不断衰减。

## 1. Lambertian反射
- 理想漫反射，一束光打在漫反射材质的平面上会均匀地反射到各个方向。
- 实现方式：使射线与平面相交的点P后，在P点的半球方向随机反射，如果随机性够均匀，就可以模拟理想散射的情况。
- 具体实现：
  1. 写一个drand48，该函数是个生成[0, 1)之间的均匀随机数函数。
  2. 我们取一个正切于交点P表面的单位球体$S_1$
  3. 在一个general的单位球体中随机生成一个点$E_{unit}$（该球体的球心是(1,1,1)），求出单位球体球心到点$E$的向量$\overrightarrow{C_{unit}E_{unit}}$，那么$E_{unit}$在以$P$点为切点的这个球体中的映射$E'$就是$\overrightarrow{C_{unit}E_{unit}} + C_{S1}$。
     - $C_{S1}$也很好求：就是切点$P$加上法线向量$\overrightarrow{n}$
     - 如何随机生成一点$E_{unit}$？用do while循环，只要$(x-1)^2+(y-1)^2+(z-1)^2 >= 1(单位球R = 1， 球心(1,1,1))$，就说明这个随机点$(x,y,z)$不在球内部，如果小于1，则说明这个点在球内部，返回这个点。
  4. 得到$E'$之后就可以求反射光线$\overrightarrow{P
     E'}$。即创建Ray对象，以$P$为起点，$E'-P$为方向。
  5. 设置attenuation = m_albedo，m_albedo为物体自身的反射颜色。


## 2. 金属镜面反射
- 对于入射方向向量$L$， 反射向量$R$，以及平面法线$N$，$R = L - 2(N*L)N$
- 注意：**上述公式只适用于完美镜面反射。**有些金属并不是那么光滑，因此我们需要对反射向量做一个扰动，使他看起来更粗糙一点，没有那么锐利。我们在反射向量的终点去一个半径一定的球体，时反射向量的终点最终指向这个球体中的随机一点。我们用一个变量m_fuzz来进行保存这个半径，m_fuzz越大则金属越粗糙。
- 具体实现就是先根据$L$、$N$得到反射方向$refDir$，然后在创建反射光线时，方向向量从$refDir$变成$refDir+ RadomInUnitSquare * m_fuzz$
- **返回时不能直接返回true，因为当观察方向于平面夹角大于90度时，即反射方向于法线的dotproduct小于等于0时，我们看不到反射。所以只有大于0时我们返回true。**

# 5.Tracer
## 接口骨架：
1. 成员类:
Setting类：用于记录Tracer的配置，如maxDepth，height，width，channel
2. 成员变量：
   1. unsiged char* m_image：指向图像buffer的指针
   2. Setting m_config：Setting变量，记录了Tracer的信息
3. 成员函数：
  1. initialize
  2. get函数：
      1. getWidth
      2. getHeight
      3. getChannel
      4. getMaxDepth
      5. getImage
  3. set函数
      1. setMaxDepth
      2. setCamera：设置虚拟相机，参数为：
         1. Vec3 camPos, 
         2. Vec3 target, 
         3. Vec3 upVector, 
         4. float fovy（field of view of y，垂直的视距范围，一般是45度）,
         5. float aspect（宽高比），
         6. float aperture（景深相关）
         7. float focus_dis（景深相关）
  4. **Hitable *randomScene()**：randomScene()会在一定范围内构造不同的球，这些球具有不同的材质、大小、颜色。最终被保存在HitableList中
  5. **Vector4D tracing(const Ray &r, Hitable *world, int depth)**：tracing会把与当前ray相交的最近的物体的相交点着色。首先判断ray与物体是否相交：
     1. 如果相交，那么判断物体是否会有scatter（根据材质判断是否存在反射（metal）、折射（Dielectric透明玻璃材质）、漫反射（lambertian材质））
        1. 如果有，且未到达maxDepth，则返回attenuation*tracing它下一层（用他的scatterRay作为ray参数），注意，这里attenuation也同时包含了衰减度和当前与ray相交的最近物体的颜色信息
        2. 如果没有，或者到达maxDepth了，则返回(0,0,0,1)的color
     2. 如果不相交，则返回背景色：
        1. 背景色需要用lerp来求。根据t的值来lerp白色和蓝色之间的颜色。t根据当前光线的单位方向向量的y值来得到。
  6. **unsigned char *render()**：render会调用randomScene函数来创造一个Scene，然后创建一个虚拟camera，然后遍历像素，对于每个像素，从camera发出一条ray开始tracing，得到color，最后用drawPixel画出
  7. **void drawPixel(unsigned int x, unsigned int y, const Vector4D &color)**：画出pixel