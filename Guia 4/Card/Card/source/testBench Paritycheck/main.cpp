#include <iostream>

#define PAN_LENGHT (19)
#define DATA_LENGHT (10)
#define CHAR_LENGHT (5)
#define CARD_KINETIS_PIN (0)
#define FS 0b01101
#define SS 0b01011
#define ES 0b11111
#define LOW_NYBBLE_MASK (0b00001111)
#define CARD_EN_PIN 0
#define CARD_DATA_PIN 0
#define CARD_CLK_PIN 0
static uint8_t pan[PAN_LENGHT];
uint8_t data_test[DATA_LENGHT]; //?
static bool enable;
static bool error;
using namespace std;
bool checkParity(void);
int main(void) {
	/////////////////////SUCCESFULL TEST//////////////////////////
	uint8_t succesfull_test_1[DATA_LENGHT] = {1,1,1,1,1,1,1,1,1,14};
	for(int i=0;i< DATA_LENGHT;i++)
		data_test[i] = succesfull_test_1[i];
	std::string str;
	for (int i : succesfull_test_1) 
		str.push_back(i+'0');
	
	cout << "Using the following test string \t" << str << endl;
	bool parity = checkParity();
	cout << "The parity check result is " << parity << endl;
	if (parity) {
		cout << "This message is OK" << endl;
	}
	//////////////////////PARITY BIT WRONG/////////////////////
	uint8_t parity_bit_wrong_test_1[DATA_LENGHT] = { 1,3,1,1,1,1,1,1,1,28 };
	for (int i = 0; i < DATA_LENGHT; i++)
		data_test[i] = parity_bit_wrong_test_1[i];
	std::string str2;
	for (int i : parity_bit_wrong_test_1) {
		str2.push_back(i + '0');
	}
	cout << "Using the following test string \t" << str2 << endl;
	bool parity2 = checkParity();
	cout << "The parity check result is " << parity2 << endl;
	////////////////////////LCR WRONG/////////////////////
	uint8_t LCR_worng_test_1[DATA_LENGHT] = { 2,1,1,1,1,1,1,1,1,14 };
	for (int i = 0; i < DATA_LENGHT; i++)
		data_test[i] = LCR_worng_test_1[i];
	std::string str3;
	for (int i : LCR_worng_test_1) {
		str3.push_back(i + '0');
	}
	cout << "Using the following test string \t" << str3 << endl;
	cout << "The parity check result is " << checkParity() << endl;
	////////////////////////SUCCESFULL_ALAN/////////////////////
	uint8_t alan_succes_test[DATA_LENGHT] = { 16,1,2,19,4,21 ,22,7,8,25 };
	//10000 00001 00010 10011 00100 10101 10110 00111 01000 11001<--- BIEN
	/*
	1 0 0 0 0
	0 0 0 0 1
	0 0 0 1 0
	1 0 0 1 1
	0 0 1 0 0
	1 0 1 0 1
	1 0 1 1 0
	0 0 1 1 1
	0 1 0 0 0 
	  3 2 1 0
 	1 1 0 l 1 
	*/
	for (int i = 0; i < DATA_LENGHT; i++)
		data_test[i] = alan_succes_test[i];
	std::string str4;
	for (int i : alan_succes_test) {
		str4.push_back(i + '0');
	}
	cout << "Using the following test string \t" << str4 << endl;
	cout << "The parity check result is " << checkParity() << endl;
	char s;
	cin >> s;
	return 0;
}

bool checkParity(void) {
	uint8_t i = 0, j;
	uint8_t char_parity = false;
	uint8_t lrc_parity[CHAR_LENGHT] = { 0 };
	bool is_ok = true;
	for (i = 0; i < DATA_LENGHT ; i++) {
		for (j = 0; j < CHAR_LENGHT; j++) {
			char_parity ^= ((data_test[i] & (1 << j)) >> j); //we test the parity of the chars
			lrc_parity[j] ^= ((data_test[i] & (1 << j)) >> j);
		}
		if (char_parity == 0) { //if it wasnt ODD parity.
			error = true;
			is_ok &= false;
			//////////////////////////////////
			cout << "Parity error detected in character " << (int)i << endl;
			//////////////////////////////////
		}
		char_parity = false;
	}
	for (i = 0; i < CHAR_LENGHT - 1; i++)
		if (lrc_parity[i] != 1) {
			is_ok &= false;
			//////////////////////////////////
			cout << "Parity error in the character "<< (int) i << " of the LCR check" << endl;
			//////////////////////////////////
		}
	return is_ok;
}