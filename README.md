# BankSimulator
A bank simulator using mutexes and semaphores.

In this little project, I implemented a parallel bank simulator that maintains any number of accounts, In addition to a variable number of ATMs, that are used to access the accounts.

Description:
  - Operation:
      The Bank offers a variety of transactions: Account opening, Cash withdrawal, Cash deposit, Balance inquiry, Transfer and turning an account into VIP.
      Each transaction takes a second, This means that two ATMs can not access the same account at the same time.
  - Account:
      Each account has the following properties: Unique ID, Password, Balance.
  - ATM:
      An ATM can perform one transaction every 100msec. Each ATM receives the transactions in a file given by the user.
  - The Bank:
      The bank charges a commision from the non-VIP accounts every 3 seconds. 
      The commision is at a random rate in the range of 2%-4%.
      An ATM can't open a new account at the same time as the bank charges a fee.
  - Snapshot:
      The bank displays the current balance of all the accounts, sorted by account number. the display is updated every 0.5 sec.
      
      

