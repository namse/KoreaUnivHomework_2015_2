// ConsoleApplication3.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include <fstream>
#include <vector>
#include <stdlib.h>
#include <windows.h>

int _tmain(int argc, _TCHAR* argv[])
{
	std::ifstream ifs("9001.txt");
	std::vector<long long>vector[3];
	for (int i = 0; i < 3; i++)
	{
		char buf[] = "9001.txt";
		buf[3] += i;
		std::ifstream ifs(buf);
		while (ifs.good()){
			long long value;
			ifs >> value;
			vector[i].push_back(value);
		}
	}

	printf("%d %d %d", vector[0].size(), vector[1].size(), vector[2].size());

	long long smallestTimestamp = min(min(vector[0][0], vector[1][0]), vector[2][0]);
	long long currentTimestamp = smallestTimestamp;
	long long timeRange = 1000;

	std::ofstream ofs("result.txt");
	int index[3] = { 0, 0, 0 };
	while (true)
	{
		for (int i = 0; i < 3; i++)
		{
			if (vector[i].size() > index[i])
				goto notEnd;
		}
		goto end;
	notEnd:
		
		int count[3] = { 0, 0, 0 };
		for (int i = 0; i < 3; i++)
		{
			while (vector[i].size() > index[i]
				&& vector[i][index[i]] <= currentTimestamp + timeRange)
			{
				++count[i];
				++index[i];
			}
		}

		ofs << count[0] << "\t"
			<< count[1] << "\t"
			<< count[2] << "\n";
		currentTimestamp += timeRange;
	}
end:

	getchar();
	return 0;
}

