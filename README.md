demo结构：
OpenGLDemo\
|--assets\			//资源文件
|	|--images\		//贴图文件
|	|--models\		//模型文件
|--shaders\			//glsl文件
|
|--glad.c			//opengl版本API管理库
|--camera.h			//相机类
|--camera.cpp
|--light.h			//灯光类
|--light.cpp
|--shader.h			//着色器类（加载glsl文件）
|--stb_image.h
|--mesh.h			//网格类（绘制的基本单元），每个网格对应一个VAO
|--mesh.cpp	
|--model.h			//模型类，继承于gameobject，从文件中加载模型数据信息，包含多个mesh，材质，贴图，骨骼，动画等，是控制的基本单元
|--model.cpp
|--gameobject.h		//场景中物体的基类，暂时只被模型继承，后面考虑把灯光，相机等都继承此类
|--gameobject.cpp
|--scene.h			//场景，包含了一些物体，shader，灯光的数组，封装了自己的渲染流程
|--scene.cpp
|--game.h			//游戏类，单例，包含窗口和一个场景，还有事件处理和模型控制
|--game.cpp
|--game_user.cpp	//游戏类中可以自己扩展的一部分，稍微模仿了unity的start和update
|--mylib.h			//练习用的矩阵计算还有四元素定义库
|--mylib.cpp
|--main.cpp			//主函数
|--*.dll			//程序运行必须的动态链接库