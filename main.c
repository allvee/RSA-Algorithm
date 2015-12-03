#include <iostream>
#include <cstdlib>
#include <cmath>
#include <cstdio>
#include <cstring>
#include <cassert>
using namespace std;
/* Base64 algo Start from here */

/* ---- Base64 Encoding/Decoding Table --- */
char b64[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

/* decodeblock - decode 4 '6-bit' characters into 3 8-bit binary bytes */
void decodeblock(unsigned char in[], char *clrstr) {
	char out[4];
	out[0] = in[0] << 2 | in[1] >> 4;
	out[1] = in[1] << 4 | in[2] >> 2;
	out[2] = in[2] << 6 | in[3] >> 0;
	out[3] = '\0';
	strncat(clrstr, out, sizeof(out));
}

void b64_decode(char *b64src, char *clrdst) {
	int c, phase, i;
	unsigned char in[4];
	char *p;

	clrdst[0] = '\0';
	phase = 0;
	i = 0;
	while (b64src[i]) {
		c = (int)b64src[i];
		if (c == '=') {
			decodeblock(in, clrdst);
			break;
		}
		p = strchr(b64, c);
		if (p) {
			in[phase] = p - b64;
			phase = (phase + 1) % 4;
			if (phase == 0) {
				decodeblock(in, clrdst);
				in[0] = in[1] = in[2] = in[3] = 0;
			}
		}
		i++;
	}
}

/* encodeblock - encode 3 8-bit binary bytes as 4 '6-bit' characters */
void encodeblock(unsigned char in[], char b64str[], int len) {
	char out[5];
	out[0] = b64[in[0] >> 2];
	out[1] = b64[((in[0] & 0x03) << 4) | ((in[1] & 0xf0) >> 4)];
	out[2] = (unsigned char)(len > 1 ? b64[((in[1] & 0x0f) << 2) |
		((in[2] & 0xc0) >> 6)] : '=');
	out[3] = (unsigned char)(len > 2 ? b64[in[2] & 0x3f] : '=');
	out[4] = '\0';
	strncat(b64str, out, sizeof(out));
}

/* encode - base64 encode a stream, adding padding if needed */
void b64_encode(char *clrstr, char *b64dst) {
	unsigned char in[3];
	int i, len = 0;
	int j = 0;

	b64dst[0] = '\0';
	while (clrstr[j]) {
		len = 0;
		for (i = 0; i<3; i++) {
			in[i] = (unsigned char)clrstr[j];
			if (clrstr[j]) {
				len++;
				j++;
			}
			else in[i] = 0;
		}
		if (len) {
			encodeblock(in, b64dst, len);
		}
	}
}

/* Base64 algo end */
typedef union {
	int a[256];
	char ch[1024];
} converter;
int p, q, n, t, flag, e[100], d[100], temp[100], j, m[100], en[100], i, msglen;
char msg[1024], ch, cipher[1024];

int prime(int pr) {
	int i;
	j = (int)sqrt(pr);
	for (i = 2; i <= j; i++) {
		if (pr%i == 0)
			return 0;
	}
	return 1;
}

int calculat_d(long int x) {
	long int k = 1;
	while (1) {
		k = k + t;
		if (k%x == 0)
			return(k / x);
	}
}

void calculate_e() {
	int k;
	k = 0;
	for (i = 2; i<t; i++) {
		if (t%i == 0)
			continue;
		flag = prime(i);
		if (flag == 1 && i != p&&i != q) {
			e[k] = i;
			flag = calculat_d(e[k]);
			if (flag>0) {
				d[k] = flag;
				k++;
			}
			if (k == 99)
				break;
		}
	}
}
int generalmod(int M, int e, int N) { // finds M ^ e MOD N
	int res = 1, x = M;
	while (e) {
		if (e & 1)
			res = (res * x) % N;
		x = (x * x) % N;
		e >>= 1;
	}
	return res;
}
void encrypt(char *plaintext, char *ciphertext, int n, int e) {
	converter message;
	FILE *encryptfp;
	int len = 0;
	char arrayelement[100];
	msglen = (int)strlen(plaintext);
	printf("Message Length:%d\n", msglen);

	for (int i = 0; i<msglen; i++) {
		message.a[i] = plaintext[i];
		printf("%d ", message.a[i]);
	}
	printf("\n");
	for (int i = 0; i<msglen; i++) {
		message.a[i] = generalmod(message.a[i], e, n); // to calculate M ^ e mod n
		printf("%d : %d\n", (int)plaintext[i], message.a[i]);
	}

	encryptfp = fopen("encryptmassage.txt", "w");
	if (encryptfp == NULL) {
		printf("Error!");
		exit(1);
	}
	printf("Length : %d\n", msglen);
	printf("\nSend to Encryted file is :");
	for (int i = 0; i < msglen; i++) {
		fprintf_s(encryptfp, "%d ", message.a[i]);
		printf("%d ", message.a[i]);
	}
	printf("\n");
	fclose(encryptfp); // Write on encryptmassage.txt and closed

	encryptfp = fopen("encryptmassage.txt", "r");

	while (1) {
		fscanf(encryptfp, "%s", arrayelement);
		if (feof(encryptfp)) {
			break;
		}
		printf("%s\n", arrayelement);
		strcat(arrayelement, " ");
		strcat(ciphertext, arrayelement);
	}

	fclose(encryptfp);
	printf("\nWe get from Encrypted file:%s\n", ciphertext);

	char myb64[2048] = "";
	char mydst[2048] = "";

	b64_encode(ciphertext, myb64);
	printf("\n%s\nencodes into base64 as:\n%s\n", ciphertext, myb64);

	encryptfp = fopen("Base64Encoded.txt", "w"); // open Base64 Encoded file to write
	if (encryptfp == NULL) {
		printf("Error!");
		exit(1);
	}
	printf("\nSend to Base64 Encoded file is :");
	fprintf_s(encryptfp, "%s", myb64);
	fclose(encryptfp);  // write on Base64 Encoded file and closed
	printf("\n");

}
void decrypt(char *ciphertext, char* plain, int n, int d) {
	converter message;
	const char s[2] = " ";
	char *token;
	int cipher_index = 0, outputmsglen;
	FILE *outfp;
	puts("\nNow In Decryption:");

	token = strtok(ciphertext, s);

	while (token != NULL) {
		message.a[cipher_index++] = atoi(token);
		token = strtok(NULL, s);
	}

	for (i = 0; i < cipher_index; i++) {

		message.a[i] = generalmod(message.a[i], d, n);
		//printf("message.a: %d\n",message.a[i]);
		plain[i] = (char)message.a[i];
	}
	outputmsglen = strlen(plain);
	outfp = fopen("Output Message.txt", "w");
	if (outfp == NULL) {
		printf("Error!");
		exit(1);
	}

	fprintf_s(outfp, "%s ", plain);
	fclose(outfp);
	printf("Finnally message decrypted As: %s\n", plain);

}
int main() {
	FILE *fp, *decryptfp;
	int prime[400];

	char mydst[1024] = "";
	int i, len = 0;

	fp = fopen("primnumberlist.txt", "r");
	//srand(time(NULL));
	fscanf(fp, "%d", &i);
	while (!feof(fp)) {
		prime[len++] = i;
		fscanf(fp, "%d", &i);
	}

	fclose(fp);


	p = prime[rand() % len];
	printf("First Prime Number: %d\n", p);
	q = prime[rand() % len];
	printf("First Prime Number: %d\n", q);

	if (p == q) {
		printf("\nWRONG INPUT\n");
		exit(1);
	}
	n = p*q;
	t = (p - 1)*(q - 1);
	printf("\nGet n and t:%d  %d \n", n, t);
	calculate_e();
	printf("\nPOSSIBLE VALUES OF e AND d ARE\n");
	for (i = 0; i<j - 1; i++)
		printf("%d \t %d\n", e[i], d[i]);

	fp = fopen("Input Message.txt", "r");
	len = 0;
	fscanf(fp, "%c", &i);
	while (!feof(fp)) {
		msg[len++] = i;
		fscanf(fp, "%c", &i);
	}

	fclose(fp);
	//gets(msg);
	printf("msg=%s\n", msg);
	encrypt(msg, cipher, n, e[0]);

	decryptfp = fopen("Base64Encoded.txt", "r");

	fscanf(decryptfp, "%s", cipher);
	while (!feof(decryptfp)) {

		fscanf(decryptfp, "%s", cipher);
	}

	fclose(decryptfp);
	printf("We get in cipher:%s\n", cipher);

	b64_decode(cipher, mydst);
	printf("\n%s\ndecodes from base64 as:\n%s\n", cipher, mydst);

	decryptfp = fopen("Base64Decoded.txt", "w");
	if (decryptfp == NULL) {
		printf("Error!");
		exit(1);
	}
	printf("\nSend to Base64 Decoded file is :");

	fprintf_s(decryptfp, "%s", mydst);
	printf("%s ", mydst);

	fclose(decryptfp);
	decrypt(mydst, msg, n, d[0]);



	system("pause");
	return(0);
}

