# BankSimulator
A bank simulator using mutexes and semaphores.

In this little project, I implemented a parallel bank simulator that maintains any number of accounts, In addition to a variable number of ATMs, that are used to access the accounts.

Description:
  - An Operation:
      The Bank offers a variety of transactions: Account opening, Cash withdrawal, Cash deposit, Balance inquiry, Transfer and turning an account into VIP.
      Each transaction takes a second, This means that two ATMs can not access the same account at the same time.
  - 

Each ATM can open an account, 
