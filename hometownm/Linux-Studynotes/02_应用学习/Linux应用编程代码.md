# 1 头文件汇总



## 1.1 open

```c
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int open(const char *pathname, int flags);
```



## 1.2 ioctl

```c
#include <sys/ioctl.h>

int ioctl(int fd, unsigned long request, ...);
```



## 1.3 close

```c
#include <unistd.h>

int close(int fd);
```



## 1.4 framebuffer

```c
#include <linux/fb.h>

struct fb_var_screeninfo
```



## 1.5 printf

```c
#include <stdio.h>
```



## 1.6 mmap

```c
#include <sys/mman.h>

/* 作用：将内存空间映射到用户空间，得到虚拟地址基地址
 * addr：新映射的起始地址在addr中指定，一般设置为 NULL
 * length：映射多少字节
 * prot：可读可写属性 PROT_EXEC  PROT_READ  PROT_WRITE  PROT_NONE
 * flags：MAP_SHARED写共享模式，两边同步   MAP_PRIVATE写复制模式
 * fd：打开lcd的设备文件，里面有对应的 mmap函数
 * offset：偏移多少
 * 返回虚拟基地址：fb_base
 */
void *mmap(void *addr, size_t length, int prot, int flags,
           int fd, off_t offset);

int munmap(void *addr, size_t length);
```



## 1.7 memset

```c
#include <string.h>

/* 作用：给一块连续的内存空间填充某个值
 * s：起始地址
 * c：填充什么数值
 * n：填充多少字节
 */
void *memset(void *s, int c, size_t n);
```





## 1.8 nanosleep



```c
#include <time.h>

/* 作用：应用程序中的延时函数
 * req：延时时间结构体
 * rem：一般为空
 */
int nanosleep(const struct timespec *req, struct timespec *rem);
```

例子

```c
/*
    struct timespec {
        time_t tv_sec;        // seconds
        long   tv_nsec;       // nanoseconds
    };
*/

struct timespec time;           // 定义要延时的时间
time.tv_sec = 0;                /* seconds：秒 */
// 10^-1s = 100ms
time.tv_nsec = 100000000;       /* nanoseconds：纳秒 */

nanosleep(&time, NULL);  // 纳秒延时 10ms
```



## 1.9  fstat

```c
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

int stat(const char *pathname, struct stat *statbuf);
int fstat(int fd, struct stat *statbuf);
int lstat(const char *pathname, struct stat *statbuf);

// 返回的 statbuf 可以获取其中的这些参数
struct stat {
               dev_t     st_dev;         /* ID of device containing file */
               ino_t     st_ino;         /* Inode number */
               mode_t    st_mode;        /* File type and mode */
               nlink_t   st_nlink;       /* Number of hard links */
               uid_t     st_uid;         /* User ID of owner */
               gid_t     st_gid;         /* Group ID of owner */
               dev_t     st_rdev;        /* Device ID (if special file) */
               off_t     st_size;        /* Total size, in bytes */
               blksize_t st_blksize;     /* Block size for filesystem I/O */
               blkcnt_t  st_blocks;      /* Number of 512B blocks allocated */

               /* Since Linux 2.6, the kernel supports nanosecond
                  precision for the following timestamp fields.
                  For the details before Linux 2.6, see NOTES. */

               struct timespec st_atim;  /* Time of last access */
               struct timespec st_mtim;  /* Time of last modification */
               struct timespec st_ctim;  /* Time of last status change */

           #define st_atime st_atim.tv_sec      /* Backward compatibility */
           #define st_mtime st_mtim.tv_sec
           #define st_ctime st_ctim.tv_sec
};
```











# 2 Framebuffer画线



## 2.1 ioctl获取可变参数



```c
struct fb_var_screeninfo {
	__u32 xres;             // x方向多少个像素点
	__u32 yres;             // y方向多少个像素点
	__u32 xres_virtual;     // x方向虚拟分辨率 = x方向真实分辨率
	__u32 yres_virtual;     // y方向虚拟分辨率 = y方向真实分辨率 * 多buffer方式
	__u32 xoffset;			// 左上角显示位置和LCD真实左上角位置的偏移：x方向偏移
	__u32 yoffset;			// 多buffer时，靠这个 yoffset设定下一个buffer基地址

	__u32 bits_per_pixel;	// bpp，一个像素使用多少位来表示
    
	__u32 grayscale;		/* 0 = color, 1 = grayscale,	*/
							/* >1 = FOURCC			*/
	struct fb_bitfield red;		/* bitfield in fb mem if true color, */
	struct fb_bitfield green;	/* else only length is significant */
	struct fb_bitfield blue;
	struct fb_bitfield transp;	/* transparency			*/	

	__u32 nonstd;			/* != 0 Non standard pixel format */

	__u32 activate;			/* see FB_ACTIVATE_*		*/

	__u32 height;			/* height of picture in mm    */
	__u32 width;			/* width of picture in mm     */

	__u32 accel_flags;		/* (OBSOLETE) see fb_info.flags */

	/* Timing: All values in pixclocks, except pixclock (of course) */
	__u32 pixclock;			/* pixel clock in ps (pico seconds) */
	__u32 left_margin;		/* time from sync to picture	*/
	__u32 right_margin;		/* time from picture to sync	*/
	__u32 upper_margin;		/* time from sync to picture	*/
	__u32 lower_margin;
	__u32 hsync_len;		/* length of horizontal sync	*/
	__u32 vsync_len;		/* length of vertical sync	*/
	__u32 sync;				/* see FB_SYNC_*		*/
	__u32 vmode;			/* see FB_VMODE_*		*/
	__u32 rotate;			/* angle we rotate counter clockwise */
	__u32 colorspace;		/* colorspace for FOURCC-based modes */
	__u32 reserved[4];		/* Reserved for future compatibility */
}
```



![image-20220120105532714](Linux应用编程代码快速目录.assets/image-20220120105532714.png)



## 2.2 LCD屏幕像素计算方法

![image-20220120115230217](Linux应用编程代码快速目录.assets/image-20220120115230217.png)



## 2.3 应用程序：画线



```c
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <stdio.h>
#include <linux/fb.h>
#include <sys/mman.h>
#include <string.h>


static int fd_fb;  // lcd节点句柄
static struct fb_var_screeninfo var;  // 可变参数
static unsigned int line_width;       // 显示一行需要多少字节
static unsigned int pixel_width;      // 显示一个像素需要多少字节
static int screen_size;
static unsigned char *fb_base;


// 在LCD指定位置上输出指定颜色（描点）
// 输入参数：framebuffer基地址，x坐标，y坐标，颜色
void lcd_put_pixel(void *fb_base, int x, int y, unsigned int color)
{
	// 需要根据可变信息，根据 x y 的值，找到相对于基地址的偏移地址，然后写入颜色值
	unsigned char *pen_8 = fb_base + y*line_width + x*pixel_width;
	unsigned short *pen_16;
	unsigned int *pen_32;

	unsigned int red,green,blue;

	pen_16 = (unsigned short *)pen_8;
	pen_32 = (unsigned int *)pen_8;

	switch(var.bits_per_pixel)  // bpp是 framebuffer中的数据格式
	{
		case 8:
		{
			*pen_8 = color;
			break;
		}
		case 16:
		{
			// RGB565格式
			red = (color >> 16) & 0xff;   // 提取出 red信息
			green = (color >> 8) & 0xff;
			blue = (color >> 0) & 0xff;
			color = ((red>>3)<<11)|((green>>2)<<5)|(blue>>3);
			*pen_16 = color;
			break;
		}
		case 32:
		{
			*pen_32 = color;
			break;
		}
		default:
		{
			printf("can't suport %d bpp\n", var.bits_per_pixel);
			break;
		}
		
	}

}



int main(int argc, char **argv)
{
	int i;

	// 1.打开framebuffer设备
	fd_fb = open("/dev/fb0", O_RDWR);
	if(fd_fb<0)
	{
		printf("can't open /dev/fb0 \n");
		return -1;
	}
	// 2.测试ioctl能否获取可变信息，信息存储在 var变量中
	if(ioctl(fd_fb, FBIOGET_VSCREENINFO, &var))
	{
		printf("can't get var \n");
		return -1;
	}

	// 3.从可变参数总提取重要信息
	// 3.1 x方向1行需要多少个字节
	line_width = var.xres * var.bits_per_pixel / 8;
	// 3.2 1个像素需要多少个字节
	pixel_width = var.bits_per_pixel / 8;
	// 3.3 1幅屏幕需要多少个字节
	screen_size = var.xres * var.yres * var.bits_per_pixel / 8;
	
	// 4.framebuffer基地址
	/* 作用：将内存空间映射到用户空间，用户空间直接访问映射的空间就是访问实际的内核态的内存空间
	 * addr：都写为NULL
	 * length：映射多少字节
	 * prot：可读可写属性 PROT_EXEC  PROT_READ  PROT_WRITE  PROT_NONE
	 * flags：MAP_SHARED写共享模式，两边同步   MAP_PRIVATE写复制模式
	 * fd：打开lcd的设备文件，里面有对应的 mmap函数
	 * offset：偏移多少
	 * 返回虚拟基地址：fb_base
	 */
	fb_base = (unsigned char *)mmap(NULL, screen_size, PROT_READ | PROT_WRITE,
									MAP_SHARED, fd_fb, 0);
	if(fb_base == (unsigned char *)-1)
	{
		printf("can't mmap \n");
		return -1;
	}

	// 5.虚拟地址映射之后，直接往该framebuffer中填充像素数据就可以显示出来了
	// 5.1 清屏：0xff是黑色，0x00是白色
	memset(fb_base, 0xff, screen_size);

	// 5.2 随便画出一条直线
	for(i=0;i<100;i++)
	{
		// RGB565格式      FF0000  是红线
		lcd_put_pixel(fb_base, var.xres/2 + i, var.yres/2, 0xFF0000);
	}

	// 6.取消mmap地址映射
	munmap(fb_base, screen_size);
	
	close(fd_fb);
	
	return 0;
}
```

**注意**：应用程序绘制完图案之后，马上就退出应用程序了，但是屏幕还保留上一次的画面。

**实验结果**

![image-20220120120203071](Linux应用编程代码快速目录.assets/image-20220120120203071.png)



## 2.4 应用程序：双buffer

```c
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <linux/fb.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <sys/ioctl.h>
#include <time.h>

// 全局变量
static int fd_fb;                     // 文件句柄
static struct fb_fix_screeninfo fix;  // 固定信息
static struct fb_var_screeninfo var;  // 可变信息
static unsigned int line_width;       // 一行有多宽(占用多少个字节)
static unsigned int pixel_width;      // 一个像素有多宽(占用多少个字节)
static int screen_size;               // 一屏幕有多宽(占用多少个字节)
static unsigned char *fb_base;        // framebuffer的基地址

// 在LCD指定位置上输出指定颜色（描点）
// 输入参数：framebuffer基地址，x坐标，y坐标，颜色
void lcd_put_pixel(void *fb_base, int x, int y, unsigned int color)
{
	// 需要根据可变信息，根据 x y 的值，找到相对于基地址的偏移地址，然后写入颜色值
	unsigned char *pen_8 = fb_base + y*line_width + x*pixel_width;
	unsigned short *pen_16;
	unsigned int *pen_32;

	unsigned int red,green,blue;

	pen_16 = (unsigned short *)pen_8;
	pen_32 = (unsigned int *)pen_8;

	switch(var.bits_per_pixel)  // bpp是 framebuffer中的数据格式
	{
		case 8:
		{
			*pen_8 = color;
			break;
		}
		case 16:
		{
			// RGB565格式
			red = (color >> 16) & 0xff;   // 提取出 red信息
			green = (color >> 8) & 0xff;
			blue = (color >> 0) & 0xff;
			color = ((red>>3)<<11)|((green>>2)<<5)|(blue>>3);
			*pen_16 = color;
			break;
		}
		case 32:
		{
			*pen_32 = color;
			break;
		}
		default:
		{
			printf("can't suport %d bpp\n", var.bits_per_pixel);
			break;
		}
		
	}

}


// 写一屏幕同样颜色的绘图函数
void lcd_draw_screen(void *fb_base, unsigned int color)
{
	// 应用程序中，往mmap之后的framebuffer地址中直接写入0x00RRGGBB的值就行了
	int x,y;
	for(x=0;x<var.xres;x++)
	{
		for(y=0;y<var.yres;y++)
		{
			// 往mmap后的地址中写入0x00RRGGBB数据值的函数
			lcd_put_pixel(fb_base, x, y, color);
		}
	}
}



/* ./multi_framebuffer_test single
 * ./multi_framebuffer_test double
 */
int main(int argc, char **argv)
{
	int i;
	int ret;
	int nBuffers;  // buffer数量
	unsigned int colors[] = {0x00FF0000, 0x0000FF00, 0x000000FF,
	0x00000000, 0x00FFFFFF};  // 颜色：红绿蓝白黑  RGB00RRGGBB

	struct timespec time;
	char *pNextBuffer;
	int nNextBuffer = 1;
	
	time.tv_sec = 0;                /* seconds：秒 */
	// 10^-1s = 100ms
    time.tv_nsec = 100000000;       /* nanoseconds：纳秒 */
	

	// 1.判断参数
	if(argc != 2)
	{
		printf("Usage : %s <single|double>\n", argv[0]);
		return -1;
	}
	
	// 2.打开文件
	//   设备节点是固定的"/dev/fb0"，可读可写
	fd_fb = open("/dev/fb0", O_RDWR);
	if(fd_fb<0)
	{
		printf("can't open /dev/fb0 \n");
		return -1;
	}

	// 3.获取驱动中的LCD固定信息
	if(ioctl(fd_fb, FBIOGET_FSCREENINFO, &fix))
	{
		printf("can't get fix \n");
		return -1;	
	}
	
	// 4.获取驱动中的LCD可变信息	
	if(ioctl(fd_fb, FBIOGET_VSCREENINFO, &var))
	{
		printf("can't get var \n");
		return -1;	
	}

	// 5.根据可变信息可以算出下面的参数
	line_width = var.xres * var.bits_per_pixel / 8;  // 一行有多宽
	pixel_width = var.bits_per_pixel / 8;  // 一个像素有多宽
	screen_size = var.xres * var.yres * var.bits_per_pixel / 8;  // 一屏幕有多宽

	// 6.计算出驱动的分配的显存可以支持多少个buffer
	nBuffers = fix.smem_len / screen_size;
	printf("nBuffers = %d\n", nBuffers);
	printf("fix.smem_len = %d\n", fix.smem_len);

	// 7.映射，是映射所有的buffer
	fb_base = (unsigned char*)mmap(NULL, fix.smem_len, PROT_READ |
	          PROT_WRITE, MAP_SHARED, fd_fb, 0);
	if(fb_base == (unsigned char *)-1)
	{
		printf("can't mmap \n");
		return -1;
	}

	// 8.判断是写单buffer还是双buffer
	//   直接判断第一个字母就行，也是字节，8bit的ASCII
	if((argv[1][0] == 's') || (nBuffers == 1))
	{
		// 单 buffer，死循环一直刷新 lcd 
		while(1)
		{
			for(i=0;i<sizeof(colors)/sizeof(colors[0]);i++)
			{
				lcd_draw_screen(fb_base, colors[i]);
				nanosleep(&time, NULL);  // 纳秒延时 10ms
			}
		}
	}
	else
	{
		// 使用多 buffer，需要应用程序先使能驱动的多 buffer功能
		// 1.先设置y的虚拟分辨率
		var.yres_virtual = nBuffers * var.yres;
		// 2.通知驱动程序开启多Buffer服务
		ioctl(fd_fb, FBIOPUT_VSCREENINFO, &var);
	
		while(1)
		{
			for(i=0;i<sizeof(colors)/sizeof(colors[0]);i++)
			{
				// 3.计算下一个buffer的地址
				pNextBuffer = fb_base + nNextBuffer * screen_size;

				// 4.往这个buffer中填充数据
				lcd_draw_screen(pNextBuffer, colors[i]);

				// 5.通知驱动程序切换buffer，写入LCD寄存器中，并且等待切换完成
				var.yoffset = nNextBuffer * var.yres;
				ioctl(fd_fb, FBIOPAN_DISPLAY, &var);

				// 老版本的驱动中才需要发送这个等待 ioctl
				ret = 0;
				ioctl(fd_fb, FBIO_WAITFORVSYNC, &ret);

				nNextBuffer = ! nNextBuffer;  // 回到0

				nanosleep(&time, NULL);  // 纳秒延时 10ms
			}
		}
	}
	
	munmap(fb_base, fix.smem_len);
	close(fd_fb);

	return 0;
	
}

```



## 2.5 LCD自动黑屏处理

为了省电，LCD在10分钟左右会自动黑屏。
如果你正在运行multi_framebuffer_test程序，可能会有如下提示(以IMX6ULL为例)：

```shell
[  961.147548] mxsfb 21c8000.lcdif: can't wait for VSYNC when fb is blank
```

这表示：当屏幕为blank(黑屏)时，无法等待VSYNC。

我们可以禁止LCD自动黑屏，执行以下命令即可：

```shell
#close lcd sleep
echo -e "\033[9;0]" > /dev/tty1
echo -e "\033[?25l"  > /dev/tty1
```



## 2.4 问题



hdmi如何使用







# 3 文字显示及图片显示



## 3.1 显示ASCII码



```c
// 在LCD指定位置显示一个 8*16的字符
// 参数：x坐标，y坐标，ascii码  （以字符的左上角为原点）
// static const unsigned char fontdata_8x16[FONTDATAMAX]
void lcd_put_ascii(void *fb_base, int x, int y, unsigned char c,
						unsigned int backgroundColor, unsigned int color)
{
	// 显示一个字符，实际上也是把这个字符的8x16个像素全部显示
	// 8x16，8列，16行，所以一行行显示，显示16行
	unsigned char *dots = (unsigned char *)&fontdata_8x16[c*16];
	int i,b;
	unsigned char byte;

	for(i=0;i<16;i++)        // 显示16行
	{
		byte = dots[i];      // 提取出第i行
		for(b=7; b>=0; b--)  // 一列列显示
		{
			if(byte & (1<<b))
			{
				// 显示
				lcd_put_pixel(fb_base, x+7-b, y+i, color);  // 字符色
			}
			else
			{
				// 隐藏
				lcd_put_pixel(fb_base, x+7-b, y+i, backgroundColor);  // 背景色
			}
		}
	}
}
```



## 3.2 显示中文



### 3.2.1 使用 mmap的好处

​		对于存储类型的文件，比如说字库或者framebuffer，使用mmap之后，就可以像访问内存一样来访问这些文件了，可以定义一个指针指向某个地址，然后通过指针的移动以及指针取值，来获取或者修改里面的值。

​		如果不采用mmap映射，则只能采用 read 标准接口来读取文件中的字节内容，对于这种数据比较大的数据读取，效率会比较低。



### 3.2.2 HZK16字库说明

​		HZK16中是以GB2312编码值来查找点阵的，以“中”字为例，它的编码值是“0xd6 0xd0”，其中的0xd6表示“区码”，表示在哪一个区：第“0xd6 - 0xa1”区；其中的0xd0表示“位码”，表示它是这个区里的哪一个字符：第“0xd0 - 0xa1”个。每一个区有94个汉字。区位码从0xa1而不是从0开始，是为了兼容ASCII码。

​		所以，我们要显示的“中”字，它的GB2312编码是d6d0，它是HZK16里第“(0xd6-0xa1)*94+(0xd0-0xa1)”个字符。

![image-20220120162629841](Linux应用编程代码快速目录.assets/image-20220120162629841.png)





### 3.3.3 应用程序：显示中文

```c
// 在LCD指定位置上显示一个16*16的汉字
// 参数：hzk_base字库mmap映射基地址
// 参数：fb_base是framebuffer映射基地址
// 参数：x,y坐标，str两字节的编码值
// 参数：backgroundColor背景颜色，color字体颜色
void lcd_put_chinese(void *hzk_base, void *fb_base,int x, int y, unsigned char *str,
						  unsigned int backgroundColor, unsigned int color)
{
	// 中文使用2个字节来表示
	// 减掉 0xA1 是为了兼容 ASCII，避免前面和 ASCII 字符重复
	unsigned int area = str[0] - 0xA1;  // 第一个字节确定域
	unsigned int where = str[1] - 0xA1;  // 第二个字节确定位于域中的第几个
	
	// 每一个区有94个汉字
	// 一个中文使用32个字节
	// 找到中文字库的那个字符上，使用mmap映射，可以像访问内存一样访问这个文件，不用使用 read了
	unsigned char *dots = hzk_base + (area * 94 + where)*32;  
	unsigned char byte;

	int i,j,b;
	for(i=0;i<16;i++)  // 16行
	{
		for(j=0;j<2;j++)  // 1行2个字节
		{
			byte = dots[i*2 + j];
			for(b=7;b>=0;b--)  // 1个字节8位
			{
				if(byte & (1<<b))
				{
					// 显示
					lcd_put_pixel(fb_base, x+j*8+7-b, y+i, color);	// 字符色
				}
				else
				{
					// 隐藏
					lcd_put_pixel(fb_base, x+j*8+7-b, y+i, backgroundColor);  // 背景色
				}
			}
		}
	}
}
```

```c
	// 5.打开字库文件（普通文件）
	fd_hzk16 = open("HZK16", O_RDONLY);  // 只读模式打开
	if(fd_hzk16<0)
	{
		printf("can't open HZK16 \n");
		return -1;
	}
	
	// 5.1 获取文件的相关信息：eg：占用多少个字节
	if(fstat(fd_hzk16, &hzk_stat))
	{
		printf("can't get fstat \n");
		return -1;		
	}

	// 5.2 对于普通文件，其驱动的mmap应该是把整个文件的内容都映射到用户空间
	hzk_base = (unsigned char *)mmap(NULL, hzk_stat.st_size, PROT_READ, MAP_SHARED, fd_hzk16, 0);
	
	// 6.虚拟地址映射之后，直接往该framebuffer中填充像素数据就可以显示出来了
	// 6.1 清屏：0xff是白色，0x00是黑色
	memset(fb_base, 0x00, screen_size);

	// 6.2 显示ASCII字库
	//     在屏幕中间显示8*16的字母A
	lcd_put_ascii(fb_base, var.xres/2, var.yres/2, 'A', 0x0, 0xffffff);

	// 6.3 显示中文
	//     在屏幕中间ASCII "A"的后面显示16*16的汉字"中"
	lcd_put_chinese(hzk_base, fb_base, var.xres/2 + 8, var.yres/2, 
					str, 0x0, 0xffffff);
	printf("chinese code = %02x %02x \n", str[0], str[1]);
	
	// 7.取消mmap地址映射
	munmap(hzk_base, hzk_stat.st_size);	

	close(fd_hzk16);
```



### 3.3.4 编译需要注意文件格式

**注意**：一般不会使用UTF-16的编码方式，在这种方式下ASCII字符也是用2字节来表示，而其中一个字节是0，但是在C语言中0表示字符串的结束符，会引起误会。

我们编写C程序时，可以使用ANSI编码，或是UTF-8编码；在编译程序时，可以使用以下的选项告诉编译器：

```c
-finput-charset=GB2312

-finput-charset=UTF-8
```

​		如果不指定“-finput-charset”，GCC就会默认C程序的编码方式为UTF-8，即使你是以ANSI格式保存，也会被当作UTF-8来对待。

​		对于编译出来的可执行程序，可以指定它里面的字符是以什么方式编码，可以使用以下的选项编译器：

```c
-fexec-charset=GB2312

-fexec-charset=UTF-8
```

​		如果不指定“-fexec-charset”，GCC就会默认编译出的可执行程序中字符的编码方式为UTF-8。

​		如果“-finput-charset”与“-fexec-charset”不一样，编译器会进行格式转换。



实例：

![image-20220120163104243](Linux应用编程代码快速目录.assets/image-20220120163104243.png)



![image-20220120163347424](Linux应用编程代码快速目录.assets/image-20220120163347424.png)

​		传到 Ubuntu 编译的时候，如果不在 Makefile 的GCC指令中特别指定，最后编译成的代码，也是按照这个之后.c文件按照 utf-8编码之后乱码的格式打印出来的。

![image-20220120163529562](Linux应用编程代码快速目录.assets/image-20220120163529562.png)

解决方法：

（1）用 notepad++打开将.c文件转换回 GB2312格式

（2）在makefile中添加一些参数指定编译类型

```makefile
KERN_DIR = /home/book/myDoc/100ask_imx6ull-sdk/Linux-4.9.88

all:
	$(CROSS_COMPILE)gcc -o show_chinese -finput-charset=UTF-8 -fexec-charset=GB2312 show_chinese.c 	
	
clean:
	rm -f show_chinese
```

可以了，makefile中指定了输入.c文件的编码格式，指定编译之后得到的可执行文件的编码格式。

![image-20220120164045747](Linux应用编程代码快速目录.assets/image-20220120164045747.png)





# 4 freestyle

## 4.1 万能的交叉编译库的指令

```c
./configure  --host=arm-buildroot-linux-gnueabihf   --prefix=$PWD/tmp
make
make install
```



```c
// Ubuntu交叉编译工具链存放头文件的路径：

/home/book/myDoc/100ask_imx6ull-sdk/ToolChain/arm-buildroot-linux-gnueabihf_sdk-buildroot/arm-buildroot-linux-gnueabihf/sysroot/usr/include


// ubuntu交叉编译工具链存放库文件的路径：
    
/home/book/myDoc/100ask_imx6ull-sdk/ToolChain/arm-buildroot-linux-gnueabihf_sdk-buildroot/arm-buildroot-linux-gnueabihf/sysroot/usr/lib/

```





## 4.2 查找 ubuntu中交叉编译工具链的位置

​		如果你编译的是一个库，请把得到的头文件、库文件放入工具链的include、lib目录里。别的程序要使用这些头文件、库时，会很方便。

​		工具链里可能有多个include、lib目录，放到哪里去？

​		执行下面命令来确定目录：

`echo 'main(){}'| arm-buildroot-linux-gnueabihf-gcc -E -v -`

​		它会列出头文件目录、库目录(LIBRARY_PATH)。

![image-20220121160305875](Linux应用编程代码快速目录.assets/image-20220121160305875.png)

![image-20220121160311125](Linux应用编程代码快速目录.assets/image-20220121160311125.png)

![image-20220121160318460](Linux应用编程代码快速目录.assets/image-20220121160318460.png)



## 4.3 交叉编译、安装 zlib

```c
tar xzf zlib-1.2.11.tar.gz
cd zlib-1.2.11
// 不添加下面这句，可能就给x86机器编译 zlib了，添加这句话编译出来的才是 arm使用的
export CC=arm-buildroot-linux-gnueabihf-gcc
// 指定之后编译生成的可执行文件、库、头文件存放在当前目录的 tmp 目录下
./configure --prefix=$PWD/tmp  
make
make install   
cd  tmp
```

拷贝头文件、库文件到交叉编译工具链的头文件、库文件目录下：

```c
cp include/* -rf /home/book/myDoc/100ask_imx6ull-sdk/ToolChain/arm-buildroot-linux-gnueabihf_sdk-buildroot/arm-buildroot-linux-gnueabihf/sysroot/usr/include
```

```c
cp lib/* -rfd /home/book/myDoc/100ask_imx6ull-sdk/ToolChain/arm-buildroot-linux-gnueabihf_sdk-buildroot/arm-buildroot-linux-gnueabihf/sysroot/usr/lib/
```



## 4.4 交叉编译、安装 libpng

```c
tar xJf libpng-1.6.37.tar.xz
cd libpng-1.6.37
// 不添加下面这句，可能就给x86机器编译 zlib了，添加这句话编译出来的才是 arm使用的
// 指定之后编译生成的可执行文件、库、头文件存放在当前目录的 tmp 目录下
./configure  --host=arm-buildroot-linux-gnueabihf   --prefix=$PWD/tmp
make
make install   
cd  tmp
```

拷贝头文件、库文件到交叉编译工具链的头文件、库文件目录下：

```c
cp include/* -rf /home/book/myDoc/100ask_imx6ull-sdk/ToolChain/arm-buildroot-linux-gnueabihf_sdk-buildroot/arm-buildroot-linux-gnueabihf/sysroot/usr/include
```

```c
cp lib/* -rfd /home/book/myDoc/100ask_imx6ull-sdk/ToolChain/arm-buildroot-linux-gnueabihf_sdk-buildroot/arm-buildroot-linux-gnueabihf/sysroot/usr/lib/
```



## 4.5 交叉编译、安装 freetype

```c
book@PC$ tar xJf freetype-2.10.2.tar.xz
book@PC$ cd  freetype-2.10.2
book@PC$ ./configure  --host=arm-buildroot-linux-gnueabihf   --prefix=$PWD/tmp
book@PC$ make
book@PC$ make install
book@PC$ cd  tmp
```

![image-20220121155553028](Linux应用编程代码快速目录.assets/image-20220121155553028.png)

```c
cp include/* -rf /home/book/myDoc/100ask_imx6ull-sdk/ToolChain/arm-buildroot-linux-gnueabihf_sdk-buildroot/arm-buildroot-linux-gnueabihf/sysroot/usr/include
```

```c
cp lib/* -rfd /home/book/myDoc/100ask_imx6ull-sdk/ToolChain/arm-buildroot-linux-gnueabihf_sdk-buildroot/arm-buildroot-linux-gnueabihf/sysroot/usr/lib/
```



严格按照上面的步骤进行就不会出错，之前有出错就是 zlib 那里步骤好像有点问题。



## 4.6 注意编译问题



编译命令(如果你使用的交叉编译链前缀不是arm-buildroot-linux-gnueabihf，请自行修改命令)：

$ arm-buildroot-linux-gnueabihf-gcc -o freetype_show_font freetype_show_font.c -lfreetype 

 

它会提示如下错误：

freetype_show_font.c:12:10: fatal error: ft2build.h: No such file or directory

 \#include <ft2build.h>

​     ^~~~~~~~~~~~

compilation terminated.





要么把工具链里incldue/freetype2/*.h 复制到上一级目录，我们使用这种方法：跟freetype文档保持一致。执行以下命令：
book@PC$ cd   /home/book/100ask_stm32mp157_pro-sdk/ToolChain/arm-buildroot-linux-gnueabihf_sdk-buildroot/arm-buildroot-linux-gnueabihf/sysroot/usr/include
book@PC$ mv  freetype2/*   ./

然后再次执行以下命令：
$ arm-buildroot-linux-gnueabihf-gcc -o freetype_show_font freetype_show_font.c  -lfreetype 













