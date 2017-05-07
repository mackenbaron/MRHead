# 跨平台自动链接头文件

本工程提供跨平台头文件链接功能，其可以自动加入头文件并链接所需的库，只需在使用时加入对应的都文件即可，比如说说你想引用opencv的头文件和链接库，只需包含mropencv.h即可。

此外，本项目还提供了使用CMake制作的一个opencv的工程，其可以跨平台编译和运行。

### Windows

视图->其他窗口里面的属性管理器

![](http://i.imgur.com/a6oRBWB.png)

选择所需的平台，例如Debug|x64，右键，属性

在VC++目录下的包含目录里加入这些文件所在文件夹即可，如下图所示：
![](http://i.imgur.com/wyOVY6A.png)

### Linux

sudo sh setup.sh即可

## 跨平台编译
OpenCV示例程序MRCrossPlatform编译方法：
### Windows

1.[下载](https://cmake.org/download/)并安装CMake最新版本，比如[3.8版](https://cmake.org/files/v3.8/cmake-3.8.0-win64-x64.msi)，参考[已有教程](http://jingyan.baidu.com/article/acf728fd50de96f8e510a3f2.html)即可
2.打开CMake，源代码路径选择MRCrossPlatform所在文件件，生成路径在后面加上/build，然后现在下面的Configure按钮，选择是
![](http://i.imgur.com/cjIVU7w.png)

3.依自己本地安装的VS选择合适的编译器，比如Visual stdio 12 2013 Win64,点击Finish，CMake会进行一系列的检测，如果没有错误，会提示Configuring done，然后再点击COnfigure右侧的Generate按钮，生成对应的VS工程文件
4.点击Open Project按钮，打开对应的VS工程，编译即可
5.把启动项选为MRCrossPlatform运行即可
程序会从当前文件夹下读取并显示opencv.png图片


### Linux
1.下载并安装[CMake在linux的发行版](https://cmake.org/files/v3.8/cmake-3.8.0.tar.gz)
2.打开终端，并且切换至MRCrossPlatform所在目录，
```
mkdir build
cd build
cmake ..
make -j 10
./MRCrossPlatform
```

如果想要指定Release和安装路径/usr/local，可以把
```
cmake ..
```
替换为
```
cmake -D CMAKE_BUILD_TYPE=RELEASE -D CMAKE_INSTALL_PREFIX=/usr/local ..
```