# Automobile_Company_Database_Sogang
## _For What?_
1. The application is for an automobile company. In out 'hypothetical' company, it has been decided to redesign a major part of the database that underlies company operations.

2. The Queries listed below are those that your client wants turned in. We should execute these queries within MySQL DBMS what you managed.

## _Query_Lists_
(TYPE 1) Show the sales trends for a particular brand over the past k years.


(TYPE 1-1) Then break these data out by gender of the buyer. 


(TYPE 1-2) Then by income range. 


(TYPE 2) Show sales trends for various brands over the past k months.


(TYPE 2-1) Then break these data out by gender of the buyer. 


(TYPE 2-2) Then by income range. 


(TYPE 3) Find that transmissions made by supplier (company name) between two 
given dates are defective. 


(TYPE 3-1) Find the VIN of each car containing such a transmission and the 
customer to which it was sold. 


(TYPE 3-2) Find the dealer who sold the VIN and transmission for each vehicle 
containing these transmissions. 


(TYPE 4) Find the top k brands by dollar-amount sold by the year. 


(TYPE 5) Find the top k brands by unit sales by the year. 


(TYPE 6) In what month(s) do convertibles sell best?


(TYPE 7) Find those dealers who keep a vehicle in inventory for the longest 
average time. 



## _How to solve?_
| what? | where? |
| ------ | ------ |
| Logical Schema | Database_Logical_Schema.png |
| Physical Schema | Database_Physical_Schema.erwin |
| Query X(X는 1~7) | Projects/20161580.cpp의 function activateTYPEX |
| Query X-Y | Projects/20161580.cpp의 function activateTYPEX_Y |

## _etc_
| Name | What is it? |
| ------ | ------ |
| Projects/20161580_1.txt | Schema를 Create/Tuple의 Data를 Insert하는 SQL Query문 |
| Projects/20161580_2.txt | 기존에 만들었던 Schema를 Drop하는 SQL Query문 |
