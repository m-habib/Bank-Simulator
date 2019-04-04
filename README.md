# BankSimulator
A bank simulator using mutexes and semaphores.<br/>
Dec, 2017<br/><br/>

In this little project, I implemented a parallel bank simulator that maintains any number of accounts, In addition to a variable number of ATMs, that are used to access the accounts.

**Description:**
  - Transaction:<br/>
    The Bank offers a variety of transactions: Account opening, Cash withdrawal, Cash deposit, Balance inquiry, Transfer and turning an account into VIP.
    Each transaction takes a second, This means that two ATMs cannot access the same account at the same time.
  
  - Account:<br/>
    Each account has the following properties: Unique ID, Password, Balance.
  
  - ATM:<br/>
    An ATM can perform one transaction every 100msec. Each ATM receives the transactions in a file given by the user.
    Each transaction is written to a shared log file.
  
  - The Bank:  
    The bank charges a commission from the non-VIP accounts every 3 seconds. 
    The commission is at a random rate in the range of 2%-4%.
    An ATM can't open a new account at the same time as the bank charges the commission .
  
  - Snapshot:  
    The bank displays the current balance of all the accounts, sorted by account number. the display is updated every 0.5sec.  
      <p align="center">
        <img src="Images/Snapshot.gif"> <br/>
        Bank Snapshot
      </p>

<br/>
**Data Structure Diagram:**

<p align="center">
<img src="Images/Data_Structure_Diagram.jpg" width="40%" height="40%">
</p>
