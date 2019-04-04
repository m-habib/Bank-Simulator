/*
 * main.cpp
 *
 *  Created on: Dec 9, 2017
 *      Author: Mohammad HabibAllah
 */

#include "bank.h"
#include <iostream>
#include <ostream>
#include <string.h>

using std::cerr;
using std::endl;
using std::cout;

#define MAXLINE 10000
#define ERROR -1

sem_t log_mutex;
ofstream log_file;
bool atms_finished;

class thread_args {
public:
	int atm_id;
	string in_file;
	Bank* bank;
};

void commandHandler(string line, int atmId, Bank* bank) {
	char* saver;
	char line_c[MAXLINE];
	strcpy(line_c, line.data());
	char* op = strtok_r(line_c, " ", &saver);
	int account = atoi(strtok_r(NULL, " ", &saver));
	string password = string(strtok_r(NULL, " ", &saver));
	int amount;
	Account* account_pointer = bank->containsAccount(account);
	if (!account_pointer && (*op != 'O')) {
		sem_wait(&log_mutex);
		log_file << "Error " << atmId
				<< ": Your transaction failed - account id " << account
				<< " does not exist\n";
		sem_post(&log_mutex);
		return;
	}
	BankResult result;
	int balance;
	switch (*op) {

	case 'O':
		amount = atoi(strtok_r(NULL, " ", &saver));
		result = bank->addAccount(account, amount, password);
		sem_wait(&log_mutex);
		if (result == BankSuccess) {
			log_file << atmId << ": New account id is " << account
					<< " with password " << password << " and initial balance "
					<< amount << "\n";
		} else if (result == IllegalBalance)

			log_file << "Error " << atmId
					<< ": Your transaction failed - illegal balance\n";
		else if (result == IllegalPassword)
			log_file << "Error " << atmId
					<< ": Your transaction failed - illegal password\n";
		else
			log_file << "Error " << atmId
					<< ": Your transaction failed - account with the same id exists\n";

		sleep(1);
		sem_post(&log_mutex);
		break;

	case 'L':
		account_pointer->down_vipLock();
		result = bank->makeVip(account, password);
		sleep(1);
		account_pointer->up_vipLock();
		if (result != BankSuccess) {
			sem_wait(&log_mutex);
			log_file << "Error " << atmId
					<< ": Your transaction failed - password for account id "
					<< account << " is incorrect\n";
			sem_post(&log_mutex);
		}
		break;

	case 'D':
		amount = atoi(strtok_r(NULL, " ", &saver));
		account_pointer->down_wrLock();
		result = bank->deposit(account, amount, password);
		balance = bank->getBalance(account, password);
		sleep(1);
		account_pointer->up_wrLock();
		sem_wait(&log_mutex);
		if (result == BankSuccess) {
			log_file << atmId << ": Account " << account << " new balance is "
					<< balance << " after " << amount << " $ was deposited\n";
		} else {
			log_file << "Error " << atmId
					<< ": Your transaction failed - password for account id "
					<< account << " is incorrect\n";
		}
		sem_post(&log_mutex);
		break;

	case 'W':
		amount = atoi(strtok_r(NULL, " ", &saver));
		account_pointer->down_wrLock();
		result = bank->withdrawal(account, amount, password);
		balance = bank->getBalance(account, password);
		sleep(1);
		account_pointer->up_wrLock();
		sem_wait(&log_mutex);
		if (result == BankSuccess)
			log_file << atmId << ": Account " << account << " new balance is "
					<< balance << " after " << amount << " $ was withdrew\n";
		else if (result == LowBalance)
			log_file << "Error " << atmId
					<< ": Your transaction failed - account id " << account
					<< " balance is lower than " << amount << "\n";

		else
			log_file << "Error " << atmId
					<< ": Your transaction failed - password for account id "
					<< account << " is incorrect\n";
		sem_post(&log_mutex);
		break;

	case 'B':
		account_pointer->down_rdLock();
		account_pointer->incRdCount();
		if (account_pointer->rdCount() == 1)
			account_pointer->down_wrLock();
		account_pointer->up_rdLock();

		balance = bank->getBalance(account, password);
		sleep(1);
		account_pointer->down_rdLock();
		account_pointer->decRdCount();
		if (account_pointer->rdCount() == 0)
			account_pointer->up_wrLock();
		account_pointer->up_rdLock();

		sem_wait(&log_mutex);
		if (balance == ERROR)
			log_file << "Error " << atmId
					<< ": Your transaction failed - password for account id "
					<< account << " is incorrect\n";
		else
			log_file << atmId << ": Account " << account << " balance is "
					<< balance << "\n";
		sem_post(&log_mutex);
		break;

	case 'T':
		int targetAccount = atoi(strtok_r(NULL, " ", &saver));
		amount = atoi(strtok_r(NULL, " ", &saver));

		if (account == targetAccount) {
			account_pointer->down_wrLock();
		} else {

			vector<Account*>::iterator it = bank->accounts.begin();
			for (; it != bank->accounts.end(); ++it) {
				if ((*it)->getNumber() == account
						|| (*it)->getNumber() == targetAccount) {
					(*it)->down_wrLock();
				}
			}
		}

		result = bank->transfer(account, targetAccount, password, amount);
		balance = bank->getBalance(account, password);
		int target_balance = bank->getBalanceWithoutPassword(targetAccount);
		sleep(1);
		if (account == targetAccount) {
			account_pointer->up_wrLock();
		} else {

			vector<Account*>::iterator it = bank->accounts.begin();
			for (; it != bank->accounts.end(); ++it) {
				if ((*it)->getNumber() == account
						|| (*it)->getNumber() == targetAccount) {
					(*it)->up_wrLock();
				}
			}
		}
		sem_wait(&log_mutex);
		if (result == BankSuccess)
			log_file << atmId << ": Transfer " << amount << " from account "
					<< account << " to account " << targetAccount
					<< " new account balance is " << balance
					<< " new target account balance is " << target_balance
					<< "\n";
		else if (result == LowBalance)
			log_file << "Error " << atmId
					<< ": Your transaction failed - account id " << account
					<< " balance is lower than " << amount << "\n";
		else if (result == DistAccountDoesNotExist)
			log_file << "Error " << atmId
					<< ": Your transaction failed - account id "
					<< targetAccount << " does not exist\n";
		else
			log_file << "Error " << atmId
					<< ": Your transaction failed - password for account id "
					<< account << " is incorrect\n";
		sem_post(&log_mutex);
		break;
	}
}

void readFromFile(int atmId, string commandFile, Bank* bank) {
	ifstream inFile;
	inFile.open((commandFile).c_str());
	string line;
	while (!(inFile).eof()) {
		getline(inFile, line);
		if (line.length() == 0)
			break;
		usleep(100000);
		commandHandler(line, atmId, bank);
	}
	inFile.close();
}

void* atmStart(void* args) {
	readFromFile(((thread_args*) args)->atm_id, ((thread_args*) args)->in_file,
			((thread_args*) args)->bank);
	return NULL;
}

void* collecting(void* bank_arg) {
	while (1) {
		Bank* bank = (Bank*) bank_arg;
		bank->collect(log_file);
		sleep(3);
		if (atms_finished)
			return NULL;
	}
}

void* printing_status(void* args) {
	while (1) {
		((Bank*) args)->print();
		usleep(500000);
		if (atms_finished)
			return NULL;
	}

}

int main(int argc, char** argv) {
	atms_finished = false;
	sem_init(&log_mutex, 0, 1);
	log_file.open("log.txt", std::ios::trunc);
	if (!log_file) {
		cout << "illegal arguments\n";
	}
	int atms_N = atoi(argv[1]);
	pthread_t* threads = new pthread_t[atms_N];
	pthread_t collect_thread;
	thread_args* args = new thread_args[atms_N];
	Bank bank;
	int result = pthread_create(&collect_thread, NULL, collecting,
			(void*) &bank);
	if (result) {
		cout << "illegal arguments\n";
	}
	pthread_t print_thread;
	result = pthread_create(&print_thread, NULL, printing_status,
			(void*) &bank);
	if (result) {
		cout << "illegal arguments\n";
	}
	for (int i = 0; i < atms_N; i++) {
		args[i].atm_id = i + 1;
		args[i].bank = &bank;
		args[i].in_file = argv[i + 2];
		int result = pthread_create(&threads[i], NULL, atmStart,
				(void*) &args[i]);
		if (result) {
			cout << "illegal arguments\n";
		}
	}
	for (int i = 0; i < atms_N; i++)
		pthread_join(threads[i], NULL);
	atms_finished = true;
	pthread_join(collect_thread, NULL);
	pthread_join(print_thread, NULL);
	log_file.close();
	sem_destroy(&log_mutex);
	delete[] threads;
	delete[] args;
	return 0;
}

