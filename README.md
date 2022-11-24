# Automobile_Company_Database_Project
## _For What?_
1. The application is for an automobile company. In out 'hypothetical' company, it has been decided to redesign a major part of the database that underlies company operations.

2. The Queries listed below are those that your client wants turned in. We should execute these queries within MySQL DBMS what you managed.

## BCNF
Schema가 중복성을 가지지 않게 하기 위해, BCNF가 아닌 relation은 BCNF로 분해하였다. </br>
그러기 위해 아래의 개념을 활용했다.
![image](https://user-images.githubusercontent.com/67453494/203731443-f974a206-153f-4ce5-b89d-e3e8d7615959.png)
</br>

## Schema
![image](https://user-images.githubusercontent.com/67453494/203731306-84d98421-981d-4723-97ab-3300542cfdd3.png)

1.	brand: 회사의 brand들의 id(varchar(5))와 name(varchar(18))을 저장한다. </br>
2.	models: 회사의 model들의 id(varchar(5))와 name(varchar(18))을 저장한다. </br>
3.	vehicles: 회사의 vehicle들의 id(varchar(5))와 name(varchar(18)), 가격, convertible 여부(varchar(5))를 저장한다. </br>
4.	supplier: 회사의 부품을 조달하는 supplier들의 정보를 저장한다. 정보로 id(var char(5)), name(varchar(18)), supply_part(부품의 정보. varchar(18)), date_year_month_day(부품을 제공한 날짜로, 연/월/일의 형식을 띤다. 예를 들어, 2021년 4월 20일에 생산한 부품일 경우, date_year_mont_day는 20210420. varchar(18))), defection(결함 여부. varchar(18)). yes와 no가 있다.)가 있다. </br>
5.	company_owned_manufacturing_plants: 회사에서 스스로 운영하는 부품 업체의 정보를 저장한다. 정보로 id, name, 생산 파트가 있다. </br> 
6.	dealers: 회사의 차를 파는 딜러들의 정보가 있다. 정보로 id(varchar(5)), 이름(vachar(18))이 있다. </br>
7.	supplier_for_model: supplier table과 model table을 연동해서 사용하는 Query문을 실행할 때 필요한 table이다. 특정 부품을 제공하는 supplier의 id와, 해당 부품을 사용하는 model의 id가 하나의 tuple로 묶여 있다. </br>
8.	supplier_for_vehicle: supplier table과 vehicle table을 연동해서 사용하는 Query문을 실행할 때 필요한 table이다. 특정 부품을 제공하는 supplier의 id와, 해당 부품을 사용하는 vehicle의 id가 하나의 tuple로 묶여 있다. </br>
9.	option_list_for_model: option table과 model table을 연동해서 사용하는 Query문을 실행할 때 필요한 table이다. 특정 option을 의미하는 option의 id와, 해당 option을 사용하는 model의 id가 하나의 tuple로 묶여 있다. </br>
10.	customer_of_vehicle: customer table과 vehicle table을 연동해서 사용하는 Query문을 실행할 때 필요한 table이다. 특정 vehicle을 의미하는 vehicle의 id와, 해당 vehicle을 구매한 customer의 id, 그리고 구매한 날짜의 연(buy_when. varchar(45))), 월(buy_when_month. int)이 하나의 tuple로 묶여 있다. </br>
11.	vehicle_list_for_dealer: vehicle table과 dealer table을 연동해서 사용하는 Query문을 실행할 때 필요한 table이다. 특정 vehicle을 의미하는 vehicle의 id와, 해당 vehicle을 담당한 dealer의 id, 그리고 해당 vehicle이 inventory에 저장된 기간(time_in_inventory. 단위는 ‘일’이다. int)이 하나의 tuple로 묶여 있다. </br>
12.	own_plant_for_model: company_owned_manufacturing_plants table과 model table을 연동해서 사용하는 Query문을 실행할 때 필요한 table이다. 특정 model을 의미하는 model의 id와, 해당 model의 부품을 생산하는 company_owned_manufacturing_plant의 id가 하나의 tuple로 묶여 있다. </br>


## _Query_List_
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
