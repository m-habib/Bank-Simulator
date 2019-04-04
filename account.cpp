/*
 * account.cpp
 *
 * Created on: Dec 8, 2017
 *      Author: Mohammad HabibAllah
 */
#include "account.h"

Account::Account(int number, string password, int balance) {
	sem_init(&wr_lock, 0, 1);
	sem_init(&rd_lock, 0, 1);
	sem_init(&vip_lock, 0, 1);
	this->number = number;
	this->password = password;
	this->balance = balance;
	this->vip = false;
	this->rd_counter = 0;
	sem_post(&vip_lock);
	sem_post(&rd_lock);
	sem_post(&wr_lock);
}

void Account::up_rdLock() {
	sem_post(&rd_lock);
}

void Account::down_rdLock() {
	sem_wait(&rd_lock);
}

void Account::up_wrLock() {
	sem_post(&wr_lock);
}

void Account::down_wrLock() {
	sem_wait(&wr_lock);
}

void Account::up_vipLock() {
	sem_post(&vip_lock);
}

void Account::down_vipLock() {
	sem_wait(&vip_lock);
}

void Account::incRdCount() {
	rd_counter++;
}
void Account::decRdCount() {
	rd_counter--;
}

int Account::rdCount() {
	return rd_counter;
}

int Account::getBalance() {
	sem_wait(&rd_lock);
	rd_counter++;
	if (rd_counter == 1) {
		sem_wait(&wr_lock);
	}
	sem_post(&rd_lock);
	return balance;
	sem_wait(&rd_lock);
	rd_counter--;
	if (rd_counter == 0) {
		sem_post(&wr_lock);
	}
	sem_post(&rd_lock);
}

void Account::setBalance(int balance) {
	this->balance = balance;
}
string Account::getPassword() {
	return password;
}
int Account::getNumber() {
	return number;
}
bool Account::isVip() {
	return vip;
}
void Account::setVip() {
	this->vip = true;
}

void Account::withdrawal(int ammount) {
	this->balance -= ammount;
}

void Account::deposit(int ammount) {
	this->balance += ammount;
}

bool Account::isEnough(int ammount) {
	if (this->balance >= ammount)
		return true;
	return false;
}

void Account::print() {
	std::cout << "Account " << number << ": Balance - " << balance
			<< " $ , Account Password - " << password << "\n";
}
