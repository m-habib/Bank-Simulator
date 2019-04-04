/*
 * bank.cpp
 *
 *  Created on: Dec 8, 2017
 *      Author: Mohammad HabibAllah
 */

#include "bank.h"
#include <math.h>
#include <algorithm>

using std::cout;

#define RAND_FACTOR 100
#define RAND_MOD 3
#define RAND_BIAS 2
#define PASS_LENGTH 4
#define ERROR -1

void Bank::reader_down_vector() {
	sem_wait(&vector_rd_lock);
	vector_rd_count++;
	if (vector_rd_count == 1)
		sem_wait(&vector_wr_lock);
	sem_post(&vector_rd_lock);
}

void Bank::reader_up_vector() {
	sem_wait(&vector_rd_lock);
	vector_rd_count--;
	if (vector_rd_count == 0)
		sem_post(&vector_wr_lock);
	sem_post(&vector_rd_lock);
}

void Bank::writer_up_vector() {
	sem_post(&vector_wr_lock);
}

void Bank::writer_down_vector() {
	sem_wait(&vector_wr_lock);
}

Bank::Bank() {
	this->bank_balance = 0;
	vector_rd_count = 0;
	sem_init(&vector_rd_lock, 0, 1);
	sem_init(&vector_wr_lock, 0, 1);
}

Bank::~Bank() {
	writer_down_vector();
	int accounts_number = accounts.size();
	if (accounts_number == 0) {
		writer_up_vector();
		return;
	}
	vector<Account*>::iterator it = accounts.begin();
	for (; it != accounts.end(); it++)
		delete (*it);
	writer_up_vector();
	sem_destroy(&vector_rd_lock);
	sem_destroy(&vector_wr_lock);
	return;
}

void Bank::collect(ofstream& log) {
	int rand_ammoun = (rand() % RAND_MOD + RAND_BIAS);
	reader_down_vector();
	vector<Account*>::iterator it = accounts.begin();
	for (; it != accounts.end(); ++it) {
		(*it)->down_vipLock();
		if ((*it)->isVip()) {
			(*it)->up_vipLock();
			continue;
		}
		(*it)->down_wrLock();
		int balance = (*it)->getBalance();
		int withdrawal = round(balance * rand_ammoun / RAND_FACTOR);
		(*it)->setBalance(balance - withdrawal);
		bank_balance += withdrawal;
		(*it)->up_wrLock();
		(*it)->up_vipLock();
		sem_wait(&log_mutex);
		log << "Bank: commissions of " << rand_ammoun
				<< " % were charged, the bank gained " << withdrawal
				<< " $ from account " << (*it)->getNumber() << "\n";
		sem_post(&log_mutex);
	}
	reader_up_vector();
}

struct accountsComperator {
	bool operator()(Account* a, Account* b) {
		return (a->getNumber() < b->getNumber());
	}
};

void Bank::print() {
	cout << "\033[2J";
	cout << "\033[1;1H";
	cout << "Current Bank Status\n";

	reader_down_vector();
	//************* down all writers for all the accounts *************//
	vector<Account*>::iterator it = accounts.begin();
	for (; it != accounts.end(); ++it) {
		(*it)->down_rdLock();
		(*it)->incRdCount();
		if ((*it)->rdCount() == 1)
			(*it)->down_wrLock();
		(*it)->up_rdLock();
	}
	//****************************************************************//

	for (it = accounts.begin(); it != accounts.end(); ++it)
		(*it)->print(); //no need to lock writes
	cout << "The Bank has " << bank_balance << " $\n";

	//************* up all writers for all the accounts ***************//
	for (it = accounts.begin(); it != accounts.end(); ++it) {
		(*it)->down_rdLock();
		(*it)->decRdCount();
		if ((*it)->rdCount() == 0)
			(*it)->up_wrLock();
		(*it)->up_rdLock();
	}
	//***************************************************************//
	reader_up_vector();
}

int Bank::getBalance(int number, string password) {
	if (!correctPassword(number, password))
		return ERROR;
	Account* account = containsAccount(number);
	if (!account)
		return ERROR;
	return account->getBalance();
}

int Bank::getBalanceWithoutPassword(int number) {
	Account* account = containsAccount(number);
	if (!account)
		return ERROR;
	return account->getBalance();
}

Account* Bank::containsAccount(int number) {
	if (this->accounts.size() == 0)
		return NULL;
	reader_down_vector();
	vector<Account*>::iterator it = this->accounts.begin();
	for (; it != accounts.end(); ++it) {
		if ((*it)->getNumber() == number) {
			reader_up_vector();
			return (*it);
		}
	}
	reader_up_vector();
	return NULL;
}

BankResult Bank::addAccount(int number, int balance, string password) {
	if (containsAccount(number))
		return AccountAlreadyExists;
	if (balance < 0)
		return IllegalBalance;
	if (password.length() != PASS_LENGTH)
		return IllegalPassword;
	Account* new_account = new Account(number, password, balance);
	writer_down_vector();
	accounts.push_back(new_account);
	std::sort(accounts.begin(), accounts.end(), accountsComperator());
	writer_up_vector();
	return BankSuccess;
}

bool Bank::correctPassword(int number, string password) {
	return (containsAccount(number)->getPassword() == password);
}

BankResult Bank::makeVip(int number, string password) {
	if (!containsAccount(number))
		return AccountDoesNotExist;
	if (!correctPassword(number, password))
		return IncorrectPassword;
	containsAccount(number)->setVip();
	return BankSuccess;
}

BankResult Bank::deposit(int number, int ammount, string password) {
	if (!containsAccount(number))
		return AccountDoesNotExist;
	if (!correctPassword(number, password))
		return IncorrectPassword;
	containsAccount(number)->deposit(ammount);
	return BankSuccess;
}

BankResult Bank::withdrawal(int number, int ammount, string password) {
	Account* account = containsAccount(number);
	if (!account)
		return AccountDoesNotExist;
	if (!correctPassword(number, password))
		return IncorrectPassword;
	if (!account->isEnough(ammount))
		return LowBalance;
	containsAccount(number)->withdrawal(ammount);
	return BankSuccess;
}

BankResult Bank::transfer(int from, int to, string password, int ammount) {
	reader_down_vector();
	Account* account_from = containsAccount(from);
	Account* account_to = containsAccount(to);
	reader_up_vector();
	if (!account_from)
		return SourceAccountDoesNotExist;
	if(!account_to)
		return DistAccountDoesNotExist;
	if (!correctPassword(from, password))
		return IncorrectPassword;
	if (!account_from->isEnough(ammount))
		return LowBalance;
	account_from->withdrawal(ammount);
	account_to->deposit(ammount);
	return BankSuccess;
}

