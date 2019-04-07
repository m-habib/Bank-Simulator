# Bank Simulator
A bank simulator using threads, mutexes and semaphores.<br/>
Dec, 2017<br/><br/>


<p align="center">
<img src="Images/BankSimulator.jpeg" width="70%" height="70%">
</p>


In this little project, I developed a bank simulator. The bank maintains any number of accounts, In addition to a variable number of ATMs, that are used to access the accounts and can be used simultaneously.

**Description:**
  - Transaction:<br/>
    The Bank offers a variety of transactions: Account opening, Cash withdrawal, Cash deposit, Balance inquiry, Transfer money between accounts, and turning an account into VIP.  
    Each transaction takes a second, This means that two ATMs cannot access the same account at the same time.
  
  - Account:<br/>
    Each account has the following properties: Unique ID, Password, Balance.
  
  - ATM:<br/>
    An ATM can perform one transaction every 100msec. Each ATM receives the transactions in a file given by the user.  
    Input file format:  
    &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;**Account opening:**&nbsp;O \<account id\> \<password\> \<initial balance\>.  
    &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;**Cash withdrawal:**&nbsp;&nbsp;W \<account id\> \<password\> \<amount\>.  
    &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;**Cash deposit:**&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;D \<account id\> \<password\> \<amount\>.  
    &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;**Balance inquiry:**&nbsp;&nbsp;&nbsp;&nbsp;B \<account id\> \<password\>.  
    &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;**Transfer:**&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;T \<account id\> \<password\> \<target account\> \<amount\>.  
    You can find an example [here](7_atms_example/atm1.txt).
  
  - The Bank:  
    The bank charges its non-VIP accounts a commission every 3 seconds.  
    The commission is at a random rate in the range of 2%-4%.  
    An ATM can't open a new account at the same time as the bank charges the commission.
  
  - Log File:  
    The ATMs write each transaction to a shared log file.  
    [Here](log_example.txt) you can find Log file example. 
    
  - Snapshot:  
    The bank displays the current balance of all the accounts, sorted by account number. the display is updated every 0.5sec.  
      <p align="center">
        <img src="Images/Snapshot.gif"> <br/>
        Bank Snapshot, Wait for it!
      </p>
<br/>

**Data Structure Diagram:**

<p align="center">
<img src="Images/Data_Structure_Diagram.jpg" width="40%" height="40%">
</p>


**How to run?**  
&nbsp;&nbsp;&nbsp;1) make  
&nbsp;&nbsp;&nbsp;2) ./Bank <Number of ATMs – N> <ATM_1_input_file> <ATM_2_input_file> ... <ATM_N_input_file>

&nbsp;&nbsp;&nbsp;Example:  
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;$ make  
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;$ ./Bank 7 ./7_atms_example/*
