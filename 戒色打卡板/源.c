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

int w_get(int nem);   //��ȡ����
int n_get(char* name, int n_long);   //��ȡ�û���
void welcome();   //��������ʱ���ӡ��ӭ��
//void text();    //����
void chong(int yn, int last, int continuous,const char name[]);  //��ӡ��ǰ�������¼
void infile(int yn, int file);  //����������ָ���ļ�
void last_h(int* last, int* continuous,const char name[]);  //��ȡ�ϴγ�û�壬����������
int read_f(int* lyn, time_t *time, int file);  //��ȡ�ļ���������ʱ���û��
int read_yn(const char name[]);    //��ȡ����ǩûǩ��

int main(void) {
	const char fail_name[] = "record.wcfe";   //�浵��   �������ˣ��Ǿͽ���ʹ��
	int pfile;   //���ļ���ָ��

	//text();     //����λ��

	/*�ļ�Ϊ�����ƣ�Unicode��ʽ������*/
	while (1) {
		errno_t ret_ofile = _sopen_s(&pfile, fail_name, _O_RDONLY  | _O_BINARY, _SH_DENYRW, _S_IWRITE);   //��ͼ��ȡ�ļ����ɹ�����0
		//_close(pfile);

		//ret_ofile = fopen_s(&pfile, fail_name, "w");
		//printf("%d", ret_ofile);

		if (ret_ofile == ENOENT) {
			printf("����ۣ�����ɫ����\n");
			printf("1���ǵ�\n");
			printf("0�������ҹ�\n");

			int ret_get = w_get(1);   //0Ϊ��1Ϊ��

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
						printf("������˵���100���֣�\n");
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
				printf("��������Ѿ��������Ӵ��\n");
				printf("0�����ˡ���\n");
				int ret_get = w_get(1);
				switch (ret_get) {
				case 0:
					exit(0);

				}

			}
			else if (ret_yn == 1) {
				printf("\n");
				printf("1������~\n");
				printf("2���鿴ս����\n");
				printf("0�����ˡ���\n");

				int ret_get = w_get(2);
				int last, continuous;
				last_h(&last, &continuous, fail_name);
				switch (ret_get)
				{
				case 0:
					exit(0);
				case 1:
					printf("�͹٣������������\n");
					printf("1���źߣ�����ֱ��׳��\n");
					printf("0��û����~\n");
					int ret_get1 = w_get(1);
					chong(ret_get1, last, continuous, fail_name);
					break;
				case 2:
					printf("���ڿ�������\n");
					break;
				default:
					break;
				}

			}
		}
		printf("����ʲô��Ҫ�������\n");
		printf("0�����ˡ���\n");
		int ret_get = w_get(1);
		switch (ret_get) {
		case 0:
			exit(0);

		}
	}
	return 0;

}

/*��ȡ��������
���룺�����������
�����0~��������
ע�⣺��������0��ʼ��*/
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
			printf("��Ϲ�İ��䣡\n");
			continue;
		}
	}
}

/*��ȡ�û���  //bug:�ܾ��������
���룺��ȡ���ֵ����飬��󳤶�
������ɹ�Ϊ�ַ�����ʧ��Ϊ0*/
int n_get(char name[], int n_long) {
	int ci = 0;
	printf("������ô�ƺ���  ���100���֣�����������ţ���\n");
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

/*��ӭ����
���ݵ���ʱ���Զ���ӡ��Ӧ��ӭ��*/
void welcome() {
	char time[10];
	_strtime(time);
	char hour_c[2];

	for (int ci = 0; ci < 2; ci++) {
		hour_c[ci] = time[ci];
	}
	int hour = atoi(hour_c);   //���ַ���ת��Ϊint
	//printf("%d", hour);
	if (hour >= 6 && hour <= 10) {
		printf("������ۣ�");
	}
	else if (hour >= 11 && hour <= 13) {
		printf("�������");
	}
	else if (hour >= 14 && hour <= 17) {
		printf("�����ѽ��");
	}
	else if (hour >= 18 && hour <= 23) {
		printf("���Ϻ��գ�");
	}
	else if (hour >= 00 && hour <= 5) {
		printf("�賿�ù���");
	}

}

/*��ӡ�������¼
���룺�����Ƿ���ˣ��ϴ���ʲô�����1�壬0û���������������ļ���*/
void chong(int yn, int last, int continuous, const char name[]) {
	if (yn == 0) {
		printf("����գ�");
		if (last == 0) {
			printf("����������ɫ%d�������������\n", (continuous+1));
		}
		else if(last==1) {
			printf("�������ɫ�ĵ�һ�죬�����ȥŶ��\n");
		}
		else {
			printf("�õĿ�ʼŶ�������ȥ��\n");
		}
	}
	else if(yn == 1) {
		printf("��~");
		if (last == 0) {
			printf("ͻȻ���ƽ��ˣ���ɲ��У�\n");
		}
		else if (last == 1) {
			printf("������������%d���ˣ��ٳ������!\n", (continuous+1));
		}
		else {
			printf("��һ����ܲ����ˣ���ô���£�С�ϵܣ�\n");
		}
	}
	int file;
	_sopen_s(&file, name, _O_RDWR  | _O_BINARY | _O_APPEND, _SH_DENYRW, _S_IWRITE);
	infile(yn, file);
	_close(file);
}

/*���ļ����뱾�����
���룺�����Ƿ���ˣ��ļ����*/
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

/*�ϴε����
���룺�ϴγ���û��ָ�룬����������ָ��
������ǵ�һ��ǩ����last=2*/
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
	/*while (1) {   //ȥ������
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

/*��ȡ�ļ���������ʱ���û��
���룺��������û���ָ�룬��������ʱ�䣬�ļ����
�������ȡ������1�������ļ�ĩβ����0*/
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

/*��ȡ����ǩûǩ��
���룺�ļ���
�����ǩ�ˣ�0����ûǩ��1��*/
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

/*����*/
/*void text() {
	//welcome();
	const char fail_name[] = "record.wcfe";   //�浵��
	int pfile;   //���ļ���ָ��

	_sopen_s(&pfile, fail_name, _O_RDWR | _O_WTEXT | _O_BINARY| _O_APPEND, _SH_DENYRW, _S_IWRITE);

	int yn = 1, last = 1, continuous = 10;
	chong(yn, last, continuous, pfile);
}
*/
