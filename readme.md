# VOFA-Lib

## 1. 项目介绍
对 vofa+ 上位机通信进行模块化，实现采样数据传输、图片传输、文本传输、控件命令解析，内置firewater、justfloat协议 和 字符串命令解析，开箱即用。

代码分为 vofa主体、interface模块、protocol模块、decode模块，对基础功能进行了默认配置，同时保留自定义接口，保证灵活性。

## 2. 文件结构

```
└── Vofa-Lib
    ├── readme.md
    ├── doc (文档)
    ├── src (vofa主体,内置协议)
        ├── vofa (vofa主体)
        ├── vofa_decode_text (默认文本解码)
        ├── vofa_protocol_firewater
        └── vofa_protocol_justfloat
    ├── interface (接口实现)
        └── vofa_interface
    ├── example (使用示例)
        └── vofa_basic
    ├── app (应用实现)
        ├── Virtual_OLED (虚拟OLED)
        ├── Virtual_LCD (虚拟LCD)
        ├── cnadle (蜡烛火焰模拟)
        ├── snake (贪吃蛇)
        └── QR_code (二维码)
    └── project (项目示例)
        ├── STC8
        └── STM32F103
```

## 3. 使用方法

1. 声明 vofa 对象
```c
vofa_t vofa;
```
2. 挂载 数据、图像、控件
```c
int a = 0;
float c = 0;
double d = 0; 
// 添加数据
VOFA_ADD_DATA(square, VOFA_SAMPLE_INT, &a);
VOFA_ADD_DATA(triang, VOFA_SAMPLE_FLOAT, &b);
VOFA_ADD_DATA(sin, VOFA_SAMPLE_FLOAT, &c);
VOFA_ADD_DATA(cos, VOFA_SAMPLE_DOUBLE, &d);

// 添加图片
vofa_add_image(&vofa_handle, 1, 4608, 48, 48, Format_RGB16, (uint8_t *)gImage_img); 

// 添加控件
vofa_widget_t my_widget;
vofa_widget_init(&my_widget, "T", &T);
vofa_add_widget(&vofa_handle, &my_widget);

```
3. 挂载 vofa 接口，并初始化
```c
memset(&vofa_handle, 0, sizeof(vofa_t)); // 初始化句柄
vofa_handle.init = vofa_interface_init;  // 挂载接口函数
vofa_handle.send = vofa_interface_send;
vofa_handle.recv = vofa_interface_recv;

vofa_handle.protocol = VOFA_PROTOCOL_JUSTFLOAT;

vofa_init(&vofa_handle); // 初始化
```
4. 调用 接收处理函数，数据图片发送函数
```c
vofa_handle.recv_handle(&vofa_handle);
vofa_send_samples(&vofa_handle);
vofa_send_image(&vofa_handle);
```


## 4. 版本特性


## 5. vofa image 插件修改
image 插件默认会对图片进行平滑处理，但是对于低像素图片，这会使图片模糊，需要修改插件代码
1. 找到 vofa 安装路径，进入 vofa1.3.10\x64\plugins\widgets\image 文件夹
2. 找到 image.qml 文件并打开
3. 找到 第66行，在 image 对象结尾插入两个属性
4. 重启 vofa
```qml
Image {
    id: image
    anchors {
        top: parent.top
        left: parent.left
        right: parent.right
        bottom: title_input.bottom
        bottomMargin: is_hide_name?0:title_input.height
    }
    cache: false
    source: "image://data/" + img_index
    fillMode: Image.PreserveAspectFit
    // 插入属性 start
    mipmap: true  // 改善缩小时的显示质量
    smooth: false  // 禁用平滑处理，保持锐利边缘
    // 插入属性 end
}
```

## 待优化列表

- [ ] 处理函数无延时时，无法运行。输入数据过快时，环形缓冲区指针溢出，触发hardfault。优化方向：增强环形缓冲区的数据同步性
