/*
 * account.h
 *
 *  Created on: Dec 8, 2017
 *      Author: Mohammad HabibAllah
 */

#ifndef ACCOUNT_H_
#define ACCOUNT_H_

#include <string>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>
#include <sys/types.h>
#include <sys/sem.h>
#include <iomanip>

using std::string;

class Account {
	int number;
	string password;
	int balance;
	bool vip;
	sem_t rd_lock;
	sem_t wr_lock;
	sem_t vip_lock;
	int rd_counter;

public:
	Account(int number, string password, int balance);
	int getBalance();
	void setBalance(int balance);
	string getPassword();
	int getNumber();
	bool isVip();
	void setVip();
	void close();
	void deposit(int ammount);
	void withdrawal(int ammount);
	bool isEnough(int ammount);
	void print();
	void up_rdLock();
	void down_rdLock();
	void up_wrLock();
	void down_wrLock();
	void up_vipLock();
	void down_vipLock();
	void incRdCount();
	void decRdCount();
	int rdCount();
};

#endif /* ACCOUNT_H_ */
