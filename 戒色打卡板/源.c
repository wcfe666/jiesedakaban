#pragma warning(disable : 4996)

#include<stdio.h>
#include<process.h>
#include<io.h>
#include <errno.h>
#include <sys\stat.h>
#include <fcntl.h>
#include<time.h >
#include<stdlib.h>
#include<string.h>

int w_get(int nem);   //获取函数
int n_get(char* name, int n_long);   //获取用户名
void welcome();   //根据现在时间打印欢迎语
//void text();    //测试
void chong(int yn, int last, int continuous,const char name[]);  //打印当前情况并记录
void infile(int yn, int file);  //将结果输出到指定文件
void last_h(int* last, int* continuous,const char name[]);  //获取上次冲没冲，连续几天了
int read_f(int* lyn, time_t *time, int file);  //读取文件接下来的时间冲没冲
int read_yn(const char name[]);    //读取今天签没签到

int main(void) {
	const char fail_name[] = "record.wcfe";   //存档名   额，起错名了，那就将错就错吧
	int pfile;   //打开文件的指针

	//text();     //测试位置

	/*文件为二进制，Unicode格式！！！*/
	while (1) {
		errno_t ret_ofile = _sopen_s(&pfile, fail_name, _O_RDONLY  | _O_BINARY, _SH_DENYRW, _S_IWRITE);   //试图读取文件，成功返回0
		//_close(pfile);

		//ret_ofile = fopen_s(&pfile, fail_name, "w");
		//printf("%d", ret_ofile);

		if (ret_ofile == ENOENT) {
			printf("哈喽哇，来戒色啦？\n");
			printf("1）是的\n");
			printf("0）不，我滚\n");

			int ret_get = w_get(1);   //0为否，1为是

			//printf("%d", ret_get);

			switch (ret_get)
			{
			case 0:
				exit(0);
			case 1:
				while (1) {
					char name[101];
					int ret_ng=n_get(name, 100 );

					//printf("%d", ret_ng);
					if (ret_ng == 0) {
						printf("滚！都说了最长100个字！\n");
						continue;
					}
					else {
						_sopen_s(&pfile, fail_name, _O_RDWR   | _O_CREAT| _O_BINARY, _SH_DENYRW, _S_IWRITE);
						_write(pfile, name, ret_ng);
						_write(pfile, ">", 1);
						_close(pfile);
						break;
					}
				}
				break;
			default:
				break;
			}
			continue;
		}
		else if (ret_ofile == 0) {
			_close(pfile);
			welcome();
			_sopen_s(&pfile, fail_name, _O_RDONLY  | _O_BINARY, _SH_DENYRW, _S_IWRITE);
			while (1) {   
				char name;
				int a=_read(pfile, &name, 1);
				if (name == '>') {
					break;
				}
				else {
					printf("%c", name);
					continue;
				}
			}
			_close(pfile);
			int ret_yn = read_yn(fail_name);
			if (ret_yn == 0) {
				printf("，你今天已经打过卡了哟！\n");
				printf("0）走了……\n");
				int ret_get = w_get(1);
				switch (ret_get) {
				case 0:
					exit(0);

				}

			}
			else if (ret_yn == 1) {
				printf("\n");
				printf("1）打卡捏~\n");
				printf("2）查看战绩！\n");
				printf("0）走了……\n");

				int ret_get = w_get(2);
				int last, continuous;
				last_h(&last, &continuous, fail_name);
				switch (ret_get)
				{
				case 0:
					exit(0);
				case 1:
					printf("客官，您今天冲了吗？\n");
					printf("1）嗯哼！（理直气壮）\n");
					printf("0）没有捏~\n");
					int ret_get1 = w_get(1);
					chong(ret_get1, last, continuous, fail_name);
					break;
				case 2:
					printf("正在开发……\n");
					break;
				default:
					break;
				}

			}
		}
		printf("还有什么需要服务的吗？\n");
		printf("0）走了……\n");
		int ret_get = w_get(1);
		switch (ret_get) {
		case 0:
			exit(0);

		}
	}
	return 0;

}

/*获取输入数字
输入：最大数字限制
输出：0~输入数字
注意：本函数从0开始！*/
int w_get(int nem) {

	while (1)
	{
		int in;
		printf(">");
		int ret_in = scanf("%d", &in);
		
		while (getchar() != '\n')
		{
			continue;
		}
		
		if (ret_in != 0&&in>=0&&in<=nem) {
			return in;
		}
		else {
			printf("别瞎寄吧输！\n");
			continue;
		}
	}
}

/*获取用户名  //bug:拒绝特殊符号
输入：获取名字的数组，最大长度
输出：成功为字符数，失败为0*/
int n_get(char name[], int n_long) {
	int ci = 0;
	printf("请问怎么称呼？  （最长100个字，禁用特殊符号！）\n");
	printf(">");

	while ( ci <n_long ) {

		char c_get=getchar();

		if (c_get == '\n') {
			//ci++;
			break;
		}
		else {
			name[ci] = c_get;
			ci++;
			continue;
		}
	}

	if (ci >= n_long ) {
		while (getchar() != '\n')
		{
			continue;
		}

		return 0;
	}
	//name[ci] = '\0';
	return ci;

}

/*欢迎标语
根据当先时间自动打印对应欢迎词*/
void welcome() {
	char time[10];
	_strtime(time);
	char hour_c[2];

	for (int ci = 0; ci < 2; ci++) {
		hour_c[ci] = time[ci];
	}
	int hour = atoi(hour_c);   //将字符串转化为int
	//printf("%d", hour);
	if (hour >= 6 && hour <= 10) {
		printf("上午好哇，");
	}
	else if (hour >= 11 && hour <= 13) {
		printf("中午好捏，");
	}
	else if (hour >= 14 && hour <= 17) {
		printf("下午好呀，");
	}
	else if (hour >= 18 && hour <= 23) {
		printf("晚上好哒，");
	}
	else if (hour >= 00 && hour <= 5) {
		printf("凌晨好哈，");
	}

}

/*打印情况并记录
输入：本次是否冲了，上次是什么情况（1冲，0没），连续天数，文件名*/
void chong(int yn, int last, int continuous, const char name[]) {
	if (yn == 0) {
		printf("帮帮哒！");
		if (last == 0) {
			printf("你已连续戒色%d天啦！你真棒！\n", (continuous+1));
		}
		else if(last==1) {
			printf("这是你戒色的第一天，坚持下去哦！\n");
		}
		else {
			printf("好的开始哦！坚持下去。\n");
		}
	}
	else if(yn == 1) {
		printf("哎~");
		if (last == 0) {
			printf("突然就破戒了，这可不行！\n");
		}
		else if (last == 1) {
			printf("你已连续冲了%d天了，再冲就死了!\n", (continuous+1));
		}
		else {
			printf("第一天就受不了了？怎么回事，小老弟？\n");
		}
	}
	int file;
	_sopen_s(&file, name, _O_RDWR  | _O_BINARY | _O_APPEND, _SH_DENYRW, _S_IWRITE);
	infile(yn, file);
	_close(file);
}

/*给文件输入本次情况
输入：本次是否冲了，文件句柄*/
void infile(int yn, int file){
	//char input[25]={'\0'};
	//char date[10];
	//char time[10];
	//const char fenge[] = "-";
	const char end = '|';
	time_t input_;
	time(&input_);
	_write(file, &end, 1);
	_write(file, &input_, sizeof(time_t));
	_write(file, &yn, sizeof(int));

	//printf("%d", a);
	/*_strtime(time);
	_strdate(date);

	strncat(input, date, 8);
	strncat(input, fenge, 1);
	strncat(input, time, 8);
	strncat(input, fenge, 1);
	if (yn == 0) {
		strncat(input, "0", 1);
	}
	else if (yn == 1) {
		strncat(input, "1", 1);
	}
	strncat(input, end, 2);

	_write(file, input, 20);

	//printf("%d", a);
	*/
}

/*上次的情况
输入：上次冲了没的指针，连续天数的指针
如果这是第一天签到，last=2*/
void last_h(int* last, int* continuous, const char name[]) {
	int file;
	_sopen_s(&file, name, _O_RDONLY | _O_WTEXT | _O_BINARY , _SH_DENYRW, _S_IWRITE);
	int  lyn, contin=1, nyn;
	time_t ltime, ntime;
	int ret_readf = read_f(&lyn, &ltime, file);
	if (ret_readf == 0) {
		(*last) = 2;
		(*continuous) = 0;
		_close(file);
		
	}
	else if(ret_readf==1) {
		while (1) {
			ret_readf = read_f(&nyn, &ntime, file);
			if (ret_readf == 0) {
				break;
			}
			time_t ge = ntime / 86400 - ltime / 86400;
			if (ge == 1) {
				if (lyn == nyn) {
					ltime = ntime;
					contin++;
					continue;
				}
				else if (lyn != nyn) {
					ltime = ntime;
					contin = 0;
					continue;
				}
			}
			else if (ge > 1) {
				lyn = nyn;
				contin = 0;
				continue;
			}

		}
		(*last) = lyn;
		(*continuous) = contin;

		_close(file);

	}
	/*while (1) {   //去掉名字
		char no_want;
		_read(file, &no_want, 1);
		if (no_want == '>') {
			break;
		}
		else {
			continue;
		}
	}
	*/
	//_read(file, &no_want, 1);

}

/*读取文件接下来的时间冲没冲
输入：接下来冲没冲的指针，接下来的时间，文件句柄
输出：读取到返回1，到达文件末尾返回0*/
int read_f(int* lyn, time_t* time, int file) {
	while (1) {
		char no_want;
		char a = '|';
		int ret_read=_read(file, &no_want, 1);
		if (no_want == a ) {
			break;
		}
		else if(ret_read==0) {
			return 0;
		}
		else {
			continue;
		}
	}
	
	_read(file, time, sizeof(time_t));
	_read(file, lyn, sizeof(int));
	return 1;
}

/*读取今天签没签到
输入：文件名
输出：签了（0），没签（1）*/
int read_yn(const char name[]) {
	int file;
	_sopen_s(&file, name, _O_RDONLY | _O_WTEXT | _O_BINARY, _SH_DENYRW, _S_IWRITE);
	int no_want1;
	time_t time1;
	while (1) {
		int ret_readf = read_f(&no_want1, &time1, file);
		if (ret_readf == 0) {
			time_t n_time;
			time(&n_time);
			if (n_time / 86400 == time1 / 86400) {
				_close(file);
				return 0;
			}
			else if (n_time / 86400 > time1 / 86400) {
				_close(file);
				return 1;
			}
			else {
				exit(1);
			}
		}
		else if (ret_readf == 1) {
			continue;
		}
	}
}

/*测试*/
/*void text() {
	//welcome();
	const char fail_name[] = "record.wcfe";   //存档名
	int pfile;   //打开文件的指针

	_sopen_s(&pfile, fail_name, _O_RDWR | _O_WTEXT | _O_BINARY| _O_APPEND, _SH_DENYRW, _S_IWRITE);

	int yn = 1, last = 1, continuous = 10;
	chong(yn, last, continuous, pfile);
}
*/
