/*
 * Bank.h
 *
 *  Created on: Dec 8, 2017
 *      Author: Mohammad HabibAllah
 */

#ifndef BANK_H_
#define BANK_H_
#include "account.h"
#include <vector>
#include <fstream>

using std::vector;
using std::ifstream;
using std::ofstream;

extern sem_t log_mutex;

typedef enum bankresult_s {
	BankSuccess,
	BankFailed,
	AccountDoesNotExist,
	AccountAlreadyExists,
	SourceAccountDoesNotExist,
	DistAccountDoesNotExist,
	IllegalPassword,
	IllegalBalance,
	LowBalance,
	IncorrectPassword
} BankResult;

class Bank {
	int bank_balance;
	int vector_rd_count;
	sem_t vector_rd_lock;
	sem_t vector_wr_lock;

public:
	vector<Account*> accounts;
	Bank();
	~Bank();
	void collect(ofstream& log);
	void print();
	int getBalance(int number, string password);
	int getBalanceWithoutPassword(int number);
	Account* containsAccount(int number);
	BankResult addAccount(int number, int balance, string password);
	bool correctPassword(int number, string password);
	BankResult makeVip(int number, string password);
	BankResult deposit(int number, int ammount, string password);
	BankResult withdrawal(int number, int ammount, string password);
	BankResult transfer(int from, int to, string password, int ammount);

private:
	void reader_down_vector();
	void reader_up_vector();
	void writer_up_vector();
	void writer_down_vector();
};

#endif /* BANK_H_ */
