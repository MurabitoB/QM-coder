#include<iostream>
#include<fstream>
#include<stdlib.h>
#include<algorithm>
#include<string>
#include<vector>
using namespace std;

#define MSB(a) a < 0 ? 1:0
FILE *file_out;
vector<char>bit_plane(0);
char LPS = 1;
char MPS = 0;
unsigned char state = 0;
int A = 0x10000;
int C = 0x000;
char temp;
char output = 0;
char output_counter = 0;
long long filestream = 0;
long long totalsize =0;
unsigned short int qc_table[]
{
	0x59eb, 0x5522, 0x504f, 0x4b85, 0x4639, 0x415e, 0x3c3d, 0x375e, 0x32b4, 0x2e17, 0x299a, 0x2516, 0x1edf, 0x1aa9, 0x174e,
		0x1424, 0x119c, 0x0f6b, 0x0d51, 0x0bb6, 0x0a40, 0x0861, 0x0706, 0x05cd, 0x04de, 0x040f, 0x0363, 0x02d4, 0x025c, 0x01f8,
		0x01a4, 0x0160, 0x0125, 0x00f6, 0x00cb, 0x00ab, 0x008f, 0x0068, 0x004e, 0x003b, 0x002c, 0x001a, 0x000d, 0x0006, 0x0003,
		0x0001
};
int qc_state_inc[]
{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0};

int qc_state_dec[]
{
	-1, 1, 1, 1, 1, 1, 1, 1, 2, 1, 2, 1, 1, 2, 1, 2, 1, 2, 2, 1, 2, 2, 2, 2, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 2, 2, 2, 2, 2, 3, 2, 2, 2, 1
};
void output_C()
{
    output <<= 1;
	output += MSB(C);
	output_counter++;
	if (output_counter == 8)
	{
		fwrite(&output, sizeof(char), 1, file_out);
		output_counter = 0;
		output = 0;
		filestream++;
	}
}
void encoder(char symbol)
{
	if (symbol == MPS)
	{
		A -= qc_table[state];
		if (A < 0x8000)
		{
			if (A < qc_table[state]){ C += A; A = qc_table[state]; }
			A <<= 1;
			/*QC change its state according th table*/
			state += qc_state_inc[state];
			/*encoder output MSB of C*/
			output_C();
			/*shift left C*/
			C <<= 1;
		}
	}
	else
	{
		A -= qc_table[state];
		if (A >= qc_table[state]){ C += A; A = qc_table[state]; }
		A <<= 1;
		/*NOT first state*/
		if (state > 0){ state -= qc_state_dec[state]; }
		/*Decrease state by S*/
		else { swap(LPS, MPS); }
		/*encoder output MSB of C*/
		output_C();
		/*shift left C*/
		C <<= 1;
	}
}
int main()
{
	string filename;
	cout << "Please input your filename " << endl;
		cin >> filename;
		FILE *file = fopen(filename.c_str(), "rb");
		filename[filename.length() - 3] = 'o', filename[filename.length() - 2] = 'u', filename[filename.length() - 1] = 't';
		file_out = fopen(filename.c_str(), "wb");
		fseek(file, 0, SEEK_END);
		long filelen = ftell(file);
		fseek(file, 0, SEEK_SET);
		int counter = 0;
		while (!feof(file))
		{
			if (filelen--<=0){ break; }
			fread(&temp, sizeof(char), 1, file);
			bit_plane.push_back(temp);
		}
		/*encode by each plane*/
		for (int i = 0; i < 8; i++)
		{
			for (int j = 0; j < bit_plane.size(); j++)
			{
				encoder(MSB(bit_plane[j]));
				bit_plane[j] <<= 1;
			}
			totalsize += filestream;
			cout <<8 - i - 1 << "-plane bitstream-size = " << filestream<<endl;
			filestream = 0;
		}
		/*if there are some codes not been output to file*/
		if (output_counter > 0)
		{
			output <<= (8 - output_counter);
			fwrite(&output, sizeof(char), 1, file_out);
			totalsize++;
		}
		cout << "totalsize = " << totalsize << endl;
		fclose(file);
		fclose(file_out);
		filename.clear();
    system("PAUSE");
	return 0;
}

