#include <cstdlib>
#include <chrono> 
#include <string>
#include <iostream>
using namespace std;

unsigned char mask[] = { 0x80, 0x40, 0x20, 0x10, 0x08, 0x04, 0x02, 0x01 };
#define tget(i) ( (t[(i)/8]&mask[(i)%8]) ? 1 : 0 )
#define tset(i, b) t[(i)/8]=(b) ? (mask[(i)%8]|t[(i)/8]) : ((~mask[(i)%8])&t[(i)/8])
#define chr(i) (cs==sizeof(int)?((int*)s)[i]:((unsigned char *)s)[i])
#define isLMS(i) (i>0 && tget(i) && !tget(i-1))

void getBuckets(unsigned char* s, int* bkt, int n, int K, int cs, bool end) {
	int i, sum = 0;
	for (i = 0; i <= K; i++) bkt[i] = 0; 
	for (i = 0; i < n; i++) bkt[chr(i)]++; 
	for (i = 0; i <= K; i++) {
		sum += bkt[i];
		bkt[i] = end ? sum : sum - bkt[i];
	}
}

void induceSAl(unsigned char* t, int* SA, unsigned char* s, int* bkt, int n, int K, int cs, bool end) {
	int i, j;
	getBuckets(s, bkt, n, K, cs, end);
	for (i = 0; i < n; i++) {
		j = SA[i] - 1;
		if (j >= 0 && !tget(j)) SA[bkt[chr(j)]++] = j;
	}
}

void induceSAs(unsigned char* t, int* SA, unsigned char* s, int* bkt, int n, int K, int cs, bool end) {
	int i, j;
	getBuckets(s, bkt, n, K, cs, end); 
	for (i = n - 1; i >= 0; i--) {
		j = SA[i] - 1;
		if (j >= 0 && tget(j)) SA[--bkt[chr(j)]] = j;
	}
}

void SuffixArray(unsigned char* s, int* SA, int n, int K, int cs) {
	int i, j;
	unsigned char* t = (unsigned char*)malloc(n / 8 + 1); 
	tset(n - 2, 0);
	tset(n - 1, 1);
	for (i = n - 3; i >= 0; i--) tset(i, (chr(i) < chr(i + 1) || (chr(i) == chr(i + 1) && tget(i + 1) == 1)) ? 1 : 0);
	int* bkt = (int*)malloc(sizeof(int) * (K + 1)); 
	getBuckets(s, bkt, n, K, cs, true); 
	for (i = 0; i < n; i++) SA[i] = -1;
	for (i = 1; i < n; i++)
		if (isLMS(i)) SA[--bkt[chr(i)]] = i;
	induceSAl(t, SA, s, bkt, n, K, cs, false);
	induceSAs(t, SA, s, bkt, n, K, cs, true);
	free(bkt);
	int n1 = 0;
	for (i = 0; i < n; i++)
		if (isLMS(SA[i])) SA[n1++] = SA[i];
	for (i = n1; i < n; i++) SA[i] = -1; 
	int name = 0, prev = -1;
	for (i = 0; i < n1; i++) {
		int pos = SA[i];
		bool diff = false;
		for (int d = 0; d < n; d++)
			if (prev == -1 || chr(pos + d) != chr(prev + d) || tget(pos + d) != tget(prev + d)) {
				diff = true;
				break;
			}
			else if (d > 0 && (isLMS(pos + d) || isLMS(prev + d))) break;
		if (diff) {
			name++;
			prev = pos;
		}
		pos = (pos % 2 == 0) ? pos / 2 : (pos - 1) / 2;
		SA[n1 + pos] = name - 1;
	}
	for (i = n - 1, j = n - 1; i >= n1; i--)
		if (SA[i] >= 0) SA[j--] = SA[i];
	int* SA1 = SA, *s1 = SA + n - n1;
	if (name < n1) SuffixArray((unsigned char*)s1, SA1, n1, name - 1, sizeof(int));
	else
		for (i = 0; i < n1; i++) SA1[s1[i]] = i;
	bkt = (int*)malloc(sizeof(int) * (K + 1)); 
	getBuckets(s, bkt, n, K, cs, true); 
	for (i = 1, j = 0; i < n; i++)
		if (isLMS(i)) s1[j++] = i; 
	for (i = 0; i < n1; i++) SA1[i] = s1[SA1[i]]; 
	for (i = n1; i < n; i++) SA[i] = -1; 
	for (i = n1 - 1; i >= 0; i--) {
		j = SA[i];
		SA[i] = -1;
		SA[--bkt[chr(j)]] = j;
	}
	induceSAl(t, SA, s, bkt, n, K, cs, false);
	induceSAs(t, SA, s, bkt, n, K, cs, true);
	free(bkt);
	free(t);
}

void BruteForce(string const& chars, size_t max_len, string const& cur) {
	if (cur.length() == max_len) return;
	else {
		for (auto c : chars) {
			string next = cur + c;
			//cout << next << endl;
			BruteForce(chars, max_len, next);
		}
	}
}

class Timer
{
private:
	using clock_t = chrono::high_resolution_clock;
	using second_t = chrono::duration<double, std::ratio<1> >;
	chrono::time_point<clock_t> m_beg;

public:
	Timer() : m_beg(clock_t::now()){}

	void reset()
	{
		m_beg = clock_t::now();
	}

	double elapsed() const
	{
		return chrono::duration_cast<second_t>(clock_t::now() - m_beg).count();
	}
};

// Определяем диапазон генерируемых тестовых данных
// Здесь это от 'a' до 'z'
#define MAX 25

const int maxn = 200000;
int sa[maxn];
unsigned char* s;
int n;
string test[1500];
int NUM = 1;
int LEN;
string vr;
int MAXLEN = 20;

int main() {
	setlocale(LC_ALL, "Russian");
	srand(time(NULL));

	cout << "Введите кол-во строк: "; cin >> NUM; 
	cout << "Введите МАКС возможную длину строки: "; cin >> MAXLEN; cout << '\n';
	MAXLEN = 10;

	for (int i = 0; i < NUM; i++) {
		LEN = 1 + rand() % MAXLEN;

		for (int j = 0; j < LEN; j++) {
			vr = 'a' + rand() % MAX;
			test[i].insert(j, vr);
		}
	}

	Timer t;
	for (int z = 0; z < 20; z++) {
		n = test[z].size();
		s = (unsigned char*)test[z].c_str();
		SuffixArray(s, sa, n + 1, 256, 1);
	}
	cout << "Время суффиксного массива: " << t.elapsed() << '\n';

	Timer m;
	for (int p = 0; p < 20; p++) {

		BruteForce(test[p], 4, "");
	}
	cout << "Время наивного: " << m.elapsed() << '\n';
}