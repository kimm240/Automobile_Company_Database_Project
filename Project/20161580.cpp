#include <stdio.h>
#include <string.h>
#include <winsock2.h>
#include "mysql.h"
#pragma comment(lib, "libmySQL.lib")

#define DB_HOST "localhost"
#define DB_USER "root"
#define DB_PASS "gusrb13579!"
#define DB_NAME "test"
//제가 기존에 썼던 DB가 3306을 port_num으로 가지고 있어서 ...
#define DB_PORT 3307

#define QUERY_MAX_SIZE 1000

using namespace std;

//menu를 출력
void print_menu();
//sub menu를 출력
void print_sub_menu(int idx);
//DDL을 Create하고 관련 data를 Insert한다.
int Create_Insert();
//가지고 있는 table을 Drop한다.
int Delete_Drop();
//Query1
int activateTYPE1(char brand2[], int k);
//Query1_1
int activateTYPE1_1(char brand2[], int k);
//Query1_2
int activateTYPE1_2(char brand2[], int k);
//Query2
int activateTYPE2(int k);
//Query2_1
int activateTYPE2_1(int k);
//Query2_2
int activateTYPE2_2(int k);
//Query3
int activateTYPE3(char date1_1[], char date1_2[]);
//Query3_1
int activateTYPE3_1(char date1_1[], char date1_2[]);
//Query3_2
int activateTYPE3_2(char date1_1[], char date1_2[]);
//Query4
int activateTYPE4(int k, char year_str[]);
//Query5
int activateTYPE5(int k, char year_str[]);
//Query6
int activateTYPE6();
//Query7
int activateTYPE7();
//income으로부터 code를 return한다. (code는 income으로 data를 분류할 때 사용함.)
const int THIS_YEAR = 2021;
const int THIS_MONTH = 6;
const int brand_cnt = 15; //00001 ~ 00015

int main() {
	int res = Create_Insert();

	while (1) {
		print_menu();
		int menu_idx = -1;
		int sub_idx = -1;
		int flag = 0;
		scanf("%d", &menu_idx);
		if (menu_idx == 0) break;
		else {
			int k, year;
			char brand2[15];
			char date1_1[10];
			char date1_2[10];
			char year_str[6];

			switch (menu_idx) {
			case 1:
				printf("---------- TYPE 1 ----------\n");
				printf("** Show the sales trends for a particular brand over the past k year. **\n");
				printf("What brand? : ");
				scanf("%s", brand2);
				printf("Which k? : ");
				scanf("%d", &k);

				activateTYPE1(brand2, k);
				print_sub_menu(menu_idx);
				scanf("%d", &sub_idx);

				if (sub_idx == 1) {
					printf("---------- TYPE 1-1 ----------\n");
					printf("** Then break these data out by gender of the buyer. **\n");
					flag = activateTYPE1_1(brand2, k);
				}
				if (sub_idx == 2) {
					printf("---------- TYPE 1-2 ----------\n");
					printf("** Then by income range. **\n");
					flag = activateTYPE1_2(brand2, k);
				}
				break;
			case 2:
				printf("---------- TYPE 2 ----------\n");
				printf("** Show the sales trends for various brands over the past k months. **\n");
				printf("Which k? : ");
				scanf("%d", &k);
				activateTYPE2(k);
				print_sub_menu(menu_idx);
				scanf("%d", &sub_idx);
				if (sub_idx == 1) {
					printf("---------- TYPE 2-1 ----------\n");
					printf("** Then break these data out by gender of the buyer. **\n");
					flag = activateTYPE2_1(k);
				}
				if (sub_idx == 2) {
					printf("---------- TYPE 2-2 ----------\n");
					printf("** Then by income range. **\n");
					flag = activateTYPE2_2(k);
				}
				break;
			case 3:
				printf("---------- TYPE 3 ----------\n");
				printf("** Find that transmissions made by supplier (company name) between two given dates are defective. **\n");
				printf("First date? : (Date FORMAT: YYYYMMDD)\n");
				printf("Example: 20201120 | 20190302\n");
				scanf("%s", date1_1);
				printf("Second date? : (Date FORMAT: YYYYMMDD)\n");
				printf("Example: 20201120 | 20190302\n");
				scanf("%s", date1_2);

				activateTYPE3(date1_1, date1_2);
				print_sub_menu(menu_idx);
				scanf("%d", &sub_idx);
				if (sub_idx == 1) {
					printf("---------- TYPE 3-1 ----------\n");
					printf("** Find the VIN of each car containing such a transmission and the customer to which it was sold. **\n");
					flag = activateTYPE3_1(date1_1, date1_2);
				}
				if (sub_idx == 2) {
					printf("---------- TYPE 3-2 ----------\n");
					printf("** Find the dealer who sold the VIN and transmission for each vehicle containing these transmissions. **\n");
					flag = activateTYPE3_2(date1_1, date1_2);
				}
				break;
			case 4:
				printf("---------- TYPE 4 ----------\n");
				printf("** Find the top k brands by dollar-amount sold by the year. **\n");
				printf("Which k? : ");
				scanf("%d", &k);
				printf("What year? :");
				scanf("%s", &year_str);
				flag = activateTYPE4(k, year_str);
				break;
			case 5:
				printf("---------- TYPE 5 ----------\n");
				printf("** Find the top k brands by unit sales by the year. **\n");
				printf("Which k? : ");
				scanf("%d", &k);
				printf("What year? : ");
				scanf("%s", &year_str);
				flag = activateTYPE5(k, year_str);
				break;
			case 6:
				printf("---------- TYPE 6 ----------\n");
				printf("** In what month(s) do convertibles sell best? **\n");
				flag = activateTYPE6();
				break;
			case 7:
				printf("---------- TYPE 7 ----------\n");
				printf("** Find those dealers who keep a vehicle in inventory for the longest average time. **\n");
				printf("** Time Unit: Day **\n");
				flag = activateTYPE7();
				break;
			}
		}
	}

	res = Delete_Drop();
	return 0;
}

int Create_Insert() {
	FILE* fp;
	fp = fopen("20161580_1.txt", "r");
	while (feof(fp) == 0) {
		MYSQL Conn;				//MySQL 정보를 담을 구조체
		MYSQL* ConnPtr = NULL;	//MySQL 핸들
		MYSQL_RES* Result;	    //쿼리 성공 시 결과를 담는 구조체 포인터
		MYSQL_ROW Row;			//쿼리 성공 시 결과로 나온 행의 정보를 담는 구조체
		int Stat;

		mysql_init(&Conn);		//MySQL 정보 초기화

		//DB와 연결
		ConnPtr = mysql_real_connect(&Conn, DB_HOST, DB_USER, DB_PASS, DB_NAME, DB_PORT, (char*)NULL, 0);

		//연결 결과 확인. NULL일 경우 연결 실패한 것.
		if (ConnPtr == NULL) {
			fprintf(stderr, "Mysql connection error : %s\n", mysql_error(&Conn));
			return 1;
		}

		char Query2[QUERY_MAX_SIZE];
		fgets(Query2, QUERY_MAX_SIZE, fp);
		Stat = mysql_query(ConnPtr, Query2);

		//쿼리 요청 실패 시 예외 처리
		if (Stat != 0) {
			fprintf(stderr, "Mysql query error: %s\n", mysql_error(&Conn));
			return 1;
		}


		//결과 확인하기
		Result = mysql_store_result(ConnPtr);
		mysql_free_result(Result);
		mysql_close(ConnPtr);
	}
	return 0;
}

int Delete_Drop() {
	FILE* fp;
	fp = fopen("20161580_2.txt", "r");
	while (feof(fp) == 0) {
		MYSQL Conn;				//MySQL 정보를 담을 구조체
		MYSQL* ConnPtr = NULL;	//MySQL 핸들
		MYSQL_RES* Result;	    //쿼리 성공 시 결과를 담는 구조체 포인터
		MYSQL_ROW Row;			//쿼리 성공 시 결과로 나온 행의 정보를 담는 구조체
		int Stat;

		mysql_init(&Conn);		//MySQL 정보 초기화

		//DB와 연결
		ConnPtr = mysql_real_connect(&Conn, DB_HOST, DB_USER, DB_PASS, DB_NAME, DB_PORT, (char*)NULL, 0);

		//연결 결과 확인. NULL일 경우 연결 실패한 것.
		if (ConnPtr == NULL) {
			fprintf(stderr, "Mysql connection error : %s\n", mysql_error(&Conn));
			return 1;
		}

		char Query2[QUERY_MAX_SIZE];
		fgets(Query2, QUERY_MAX_SIZE, fp);
		Stat = mysql_query(ConnPtr, Query2);

		//쿼리 요청 실패 시 예외 처리
		if (Stat != 0) {
			fprintf(stderr, "Mysql query error: %s\n", mysql_error(&Conn));
			return 1;
		}

		//결과 확인하기
		Result = mysql_store_result(ConnPtr);
		mysql_free_result(Result);
		mysql_close(ConnPtr);
	}
	return 0;
}

void print_menu() {
	printf("---------- SELECT QUERY TYPES ----------");
	printf("\n\n");
	for (int i = 1; i <= 7; i++) {
		printf("           %d. TYPE %d\n", i, i);
	}
	printf("           0. QUIT\n");
}

void print_sub_menu(int idx) {
	printf("---------- SELECT QUERY TYPES ----------");
	printf("\n\n");
	printf("           1. TYPE %d-1\n", idx);
	printf("           2. TYPE %d-2\n", idx);

}

int activateTYPE1(char brand2[], int k) {
	MYSQL Conn;				//MySQL 정보를 담을 구조체
	MYSQL* ConnPtr = NULL;	//MySQL 핸들
	MYSQL_RES* Result;	    //쿼리 성공 시 결과를 담는 구조체 포인터
	MYSQL_ROW Row;			//쿼리 성공 시 결과로 나온 행의 정보를 담는 구조체
	int Stat;

	mysql_init(&Conn);		//MySQL 정보 초기화

	//DB와 연결
	ConnPtr = mysql_real_connect(&Conn, DB_HOST, DB_USER, DB_PASS, DB_NAME, DB_PORT, (char*)NULL, 0);

	//연결 결과 확인. NULL일 경우 연결 실패한 것.
	if (ConnPtr == NULL) {
		fprintf(stderr, "Mysql connection error : %s\n", mysql_error(&Conn));
		return 1;
	}

	//Make a Query 
	char Query2[QUERY_MAX_SIZE] = "SELECT customer_of_vehicle.buy_when, COUNT(*) AS sell_count FROM customers, customer_of_vehicle, vehicles, models, brand WHERE customers.customer_ID = customer_of_vehicle.customer_ID_fk2 AND customer_of_vehicle.VID_fk2 = vehicles.VID AND vehicles.model_ID = models.model_ID AND models.brand_ID = brand.brand_ID AND(brand.brand_name = '";
	char Query2_back[] = "') GROUP BY customer_of_vehicle.buy_when ORDER BY customer_of_vehicle.buy_when DESC";
	strcat(Query2, brand2);
	strcat(Query2, Query2_back);

	Stat = mysql_query(ConnPtr, Query2);

	//쿼리 요청 실패 시 예외 처리
	if (Stat != 0) {
		fprintf(stderr, "Mysql query error: %s\n", mysql_error(&Conn));
		return 1;
	}

	/* 출력은 this_year - k + 1까지 */
	int final_year = THIS_YEAR - k + 1;

	//결과 확인하기
	Result = mysql_store_result(ConnPtr);
	printf("buy_when sell_count\n");
	while ((Row = mysql_fetch_row(Result)) != NULL) {
		int year = strtol(Row[0], NULL, 10);

		if (year < final_year) break;
		else printf("%s %s\n", Row[0], Row[1]);
	}
	mysql_free_result(Result);
	mysql_close(ConnPtr);
	return 0;
}

int activateTYPE1_1(char brand2[], int k) {
	MYSQL Conn;				//MySQL 정보를 담을 구조체
	MYSQL* ConnPtr = NULL;	//MySQL 핸들
	MYSQL_RES* Result;	    //쿼리 성공 시 결과를 담는 구조체 포인터
	MYSQL_ROW Row;			//쿼리 성공 시 결과로 나온 행의 정보를 담는 구조체
	int Stat;

	mysql_init(&Conn);		//MySQL 정보 초기화

	//DB와 연결
	ConnPtr = mysql_real_connect(&Conn, DB_HOST, DB_USER, DB_PASS, DB_NAME, DB_PORT, (char*)NULL, 0);

	//연결 결과 확인. NULL일 경우 연결 실패한 것.
	if (ConnPtr == NULL) {
		fprintf(stderr, "Mysql connection error : %s\n", mysql_error(&Conn));
		return 1;
	}

	//Make a Query
	char Query2[QUERY_MAX_SIZE] = "SELECT customer_of_vehicle.buy_when, customers.gender, COUNT(*) AS sell_count FROM customers, customer_of_vehicle, vehicles, models, brand WHERE customers.customer_ID = customer_of_vehicle.customer_ID_fk2 AND customer_of_vehicle.VID_fk2 = vehicles.VID AND vehicles.model_ID = models.model_ID AND models.brand_ID = brand.brand_ID AND(brand.brand_name = '";
	char Query2_back[] = "') GROUP BY customer_of_vehicle.buy_when, customers.gender ORDER BY customer_of_vehicle.buy_when DESC";
	strcat(Query2, brand2);
	strcat(Query2, Query2_back);

	//쿼리 요청 및 성공 여부 받아오기
	Stat = mysql_query(ConnPtr, Query2);
	//쿼리 요청 실패 시 예외 처리
	if (Stat != 0) {
		fprintf(stderr, "Mysql query error: %s\n", mysql_error(&Conn));
		return 1;
	}

	// 출력은 final_year까지
	int final_year = THIS_YEAR - k + 1;

	//결과 확인하기
	printf(" ** gender: M -> Male | F -> Female **\n");
	printf("buy_when gender sell_count\n");
	Result = mysql_store_result(ConnPtr);
	while ((Row = mysql_fetch_row(Result)) != NULL) {
		int year = strtol(Row[0], NULL, 10);
		// 출력은 final_year까지
		if (year < final_year) break;
		else printf("%s %s %s\n", Row[0], Row[1], Row[2]);
	}
	mysql_free_result(Result);
	mysql_close(ConnPtr);
	return 0;
}

int activateTYPE1_2(char brand2[], int k) {
	MYSQL Conn;				//MySQL 정보를 담을 구조체
	MYSQL* ConnPtr = NULL;	//MySQL 핸들
	MYSQL_RES* Result;	    //쿼리 성공 시 결과를 담는 구조체 포인터
	MYSQL_ROW Row;			//쿼리 성공 시 결과로 나온 행의 정보를 담는 구조체
	int Stat;

	mysql_init(&Conn);		//MySQL 정보 초기화

	//DB와 연결
	ConnPtr = mysql_real_connect(&Conn, DB_HOST, DB_USER, DB_PASS, DB_NAME, DB_PORT, (char*)NULL, 0);

	//연결 결과 확인. NULL일 경우 연결 실패한 것.
	if (ConnPtr == NULL) {
		fprintf(stderr, "Mysql connection error : %s\n", mysql_error(&Conn));
		return 1;
	}

	//Make a Query
	char Query2[QUERY_MAX_SIZE] = "SELECT customer_of_vehicle.buy_when, customers.annual_income, COUNT(*) AS sell_count FROM customers, customer_of_vehicle, vehicles, models, brand WHERE customers.customer_ID = customer_of_vehicle.customer_ID_fk2 AND customer_of_vehicle.VID_fk2 = vehicles.VID AND vehicles.model_ID = models.model_ID AND models.brand_ID = brand.brand_ID AND(brand.brand_name = '";
	char Query2_back[] = "') GROUP BY customer_of_vehicle.buy_when, customers.annual_income ORDER BY customer_of_vehicle.buy_when DESC";
	strcat(Query2, brand2);
	strcat(Query2, Query2_back);

	printf("buy_when annual_income count\n");
	//쿼리 요청 및 성공 여부 받아오기
	Stat = mysql_query(ConnPtr, Query2);
	//쿼리 요청 실패 시 예외 처리
	if (Stat != 0) {
		fprintf(stderr, "Mysql query error: %s\n", mysql_error(&Conn));
		return 1;
	}
	/* 출력은 this_year - k + 1까지 */
	int final_year = THIS_YEAR - k + 1;

	//결과 확인하기
	Result = mysql_store_result(ConnPtr);

	while ((Row = mysql_fetch_row(Result)) != NULL) {
		int year_str_num;
		year_str_num = strtol(Row[0], NULL, 10);
		/* 출력은 this_year - k + 1까지 */
		if (year_str_num < final_year) continue;
		else printf("%s %s %s\n", Row[0], Row[1], Row[2]);
	}

	mysql_free_result(Result);
	mysql_close(ConnPtr);
	return 0;
}

int activateTYPE2(int k) {
	MYSQL Conn;				//MySQL 정보를 담을 구조체
	MYSQL* ConnPtr = NULL;	//MySQL 핸들
	MYSQL_RES* Result;	    //쿼리 성공 시 결과를 담는 구조체 포인터
	MYSQL_ROW Row;			//쿼리 성공 시 결과로 나온 행의 정보를 담는 구조체
	int Stat;

	mysql_init(&Conn);		//MySQL 정보 초기화

	//DB와 연결
	ConnPtr = mysql_real_connect(&Conn, DB_HOST, DB_USER, DB_PASS, DB_NAME, DB_PORT, (char*)NULL, 0);

	//연결 결과 확인. NULL일 경우 연결 실패한 것.
	if (ConnPtr == NULL) {
		fprintf(stderr, "Mysql connection error : %s\n", mysql_error(&Conn));
		return 1;
	}

	//쿼리문 작성
	char Query2[QUERY_MAX_SIZE] = "SELECT brand.brand_name, customer_of_vehicle.buy_when, customer_of_vehicle.buy_when_month, COUNT(*) AS sell_count FROM customers, customer_of_vehicle, vehicles, models, brand WHERE customers.customer_ID = customer_of_vehicle.customer_ID_fk2 AND customer_of_vehicle.VID_fk2 = vehicles.VID AND vehicles.model_ID = models.model_ID AND models.brand_ID = brand.brand_ID GROUP BY brand.brand_name, customer_of_vehicle.buy_when, customer_of_vehicle.buy_when_month ORDER BY brand.brand_name";

	//쿼리 요청 및 성공 여부 받아오기
	Stat = mysql_query(ConnPtr, Query2);

	//쿼리 요청 실패 시 예외 처리
	if (Stat != 0) {
		fprintf(stderr, "Mysql query error: %s\n", mysql_error(&Conn));
		return 1;
	}

	//몇 년(final_year) 몇 월(final_month)까지 출력해야하는지 계산
	int THIS_YEAR_MONTH = THIS_YEAR * 12 + THIS_MONTH;
	int final_YEAR_MONTH = THIS_YEAR_MONTH - k;
	int final_year = final_YEAR_MONTH / 12;
	int final_month = final_YEAR_MONTH % 12;
	if (final_month == 0) {
		final_year--;
		final_month = 12;
	}

	printf("brand_name buy_when_year buy_when_month sell_count\n");

	//결과 확인하기
	Result = mysql_store_result(ConnPtr);
	while ((Row = mysql_fetch_row(Result)) != NULL) {
		int year_str_num = strtol(Row[1], NULL, 10);
		int month_str_num = strtol(Row[2], NULL, 10);

		//final_year년 final_month월 까지 출력
		if (year_str_num < final_year || (year_str_num == final_year && month_str_num < final_month)) continue;
		else printf("%s %s %s %s\n", Row[0], Row[1], Row[2], Row[3]);
	}
	mysql_free_result(Result);
	mysql_close(ConnPtr);
	return 0;
}

int activateTYPE2_1(int k) {
	MYSQL Conn;				//MySQL 정보를 담을 구조체
	MYSQL* ConnPtr = NULL;	//MySQL 핸들
	MYSQL_RES* Result;	    //쿼리 성공 시 결과를 담는 구조체 포인터
	MYSQL_ROW Row;			//쿼리 성공 시 결과로 나온 행의 정보를 담는 구조체
	int Stat;

	mysql_init(&Conn);		//MySQL 정보 초기화

	//DB와 연결
	ConnPtr = mysql_real_connect(&Conn, DB_HOST, DB_USER, DB_PASS, DB_NAME, DB_PORT, (char*)NULL, 0);

	//연결 결과 확인. NULL일 경우 연결 실패한 것.
	if (ConnPtr == NULL) {
		fprintf(stderr, "Mysql connection error : %s\n", mysql_error(&Conn));
		return 1;
	}

	//쿼리문 작성
	char Query2[QUERY_MAX_SIZE] = "SELECT brand.brand_name, customer_of_vehicle.buy_when, customer_of_vehicle.buy_when_month, customers.gender, COUNT(*) AS sell_count FROM customers, customer_of_vehicle, vehicles, models, brand WHERE customers.customer_ID = customer_of_vehicle.customer_ID_fk2 AND customer_of_vehicle.VID_fk2 = vehicles.VID AND vehicles.model_ID = models.model_ID AND models.brand_ID = brand.brand_ID GROUP BY brand.brand_name, customer_of_vehicle.buy_when, customer_of_vehicle.buy_when_month, customers.gender ORDER BY brand.brand_name";

	//쿼리 요청 및 성공 여부 받아오기
	Stat = mysql_query(ConnPtr, Query2);
	//쿼리 요청 실패 시 예외 처리
	if (Stat != 0) {
		fprintf(stderr, "Mysql query error: %s\n", mysql_error(&Conn));
		return 1;
	}

	//몇 년(final_year) 몇 월(final_month)까지 출력해야하는지 계산
	int THIS_YEAR_MONTH = THIS_YEAR * 12 + THIS_MONTH;
	int final_YEAR_MONTH = THIS_YEAR_MONTH - k;
	int final_year = final_YEAR_MONTH / 12;
	int final_month = final_YEAR_MONTH % 12;
	if (final_month == 0) {
		final_year--;
		final_month = 12;
	}

	printf("buy_when_year buy_when_month gender sell_count\n");

	//결과 확인하기
	Result = mysql_store_result(ConnPtr);
	while ((Row = mysql_fetch_row(Result)) != NULL) {
		int year_str_num = strtol(Row[1], NULL, 10);
		int month_str_num = strtol(Row[2], NULL, 10);

		//final_year년 final_month월 까지 출력
		if (year_str_num < final_year || (year_str_num == final_year && month_str_num < final_month)) continue;
		else printf("%s %s %s %s %s\n", Row[0], Row[1], Row[2], Row[3], Row[4]);
	}
	mysql_free_result(Result);
	mysql_close(ConnPtr);
	return 0;
}

int activateTYPE2_2(int k) {
	MYSQL Conn;				//MySQL 정보를 담을 구조체
	MYSQL* ConnPtr = NULL;	//MySQL 핸들
	MYSQL_RES* Result;	    //쿼리 성공 시 결과를 담는 구조체 포인터
	MYSQL_ROW Row;			//쿼리 성공 시 결과로 나온 행의 정보를 담는 구조체
	int Stat;

	mysql_init(&Conn);		//MySQL 정보 초기화

	//DB와 연결
	ConnPtr = mysql_real_connect(&Conn, DB_HOST, DB_USER, DB_PASS, DB_NAME, DB_PORT, (char*)NULL, 0);

	//연결 결과 확인. NULL일 경우 연결 실패한 것.
	if (ConnPtr == NULL) {
		fprintf(stderr, "Mysql connection error : %s\n", mysql_error(&Conn));
		return 1;
	}

	//쿼리문 작성
	char Query2[QUERY_MAX_SIZE] = "SELECT brand.brand_name, customer_of_vehicle.buy_when, customer_of_vehicle.buy_when_month, customers.annual_income, COUNT(*) AS sell_count FROM  customers, customer_of_vehicle, vehicles, models, brand WHERE customers.customer_ID = customer_of_vehicle.customer_ID_fk2 AND customer_of_vehicle.VID_fk2 = vehicles.VID AND vehicles.model_ID = models.model_ID AND models.brand_ID = brand.brand_ID GROUP BY brand.brand_name, customer_of_vehicle.buy_when, customer_of_vehicle.buy_when_month, customers.annual_income ORDER BY brand.brand_name";

	//쿼리 요청 및 성공 여부 받아오기
	Stat = mysql_query(ConnPtr, Query2);
	//쿼리 요청 실패 시 예외 처리
	if (Stat != 0) {
		fprintf(stderr, "Mysql query error: %s\n", mysql_error(&Conn));
		return 1;
	}

	//몇 년(final_year) 몇 월(final_month)까지 출력해야하는지 계산
	int THIS_YEAR_MONTH = THIS_YEAR * 12 + THIS_MONTH;
	int final_YEAR_MONTH = THIS_YEAR_MONTH - k;
	int final_year = final_YEAR_MONTH / 12;
	int final_month = final_YEAR_MONTH % 12;
	if (final_month == 0) {
		final_year--;
		final_month = 12;
	}

	//결과 확인하기
	printf("brand_name buy_when_year buy_when_month annual_income\n");
	Result = mysql_store_result(ConnPtr);
	while ((Row = mysql_fetch_row(Result)) != NULL) {
		int year_str_num;
		year_str_num = strtol(Row[1], NULL, 10);

		//ex)year_str_num: 2020 year_idx: 20
		int year_idx = year_str_num % 100;

		int month_str_num;
		month_str_num = strtol(Row[2], NULL, 10);
		int sell_count;
		sell_count = strtol(Row[4], NULL, 10);

		//final_year년 final_month월 까지 출력
		if (year_str_num < final_year || (year_str_num == final_year && month_str_num < final_month)) continue;
		else printf("%s %s %s %s\n", Row[0], Row[1], Row[2], Row[3]);
	}
	mysql_free_result(Result);
	mysql_close(ConnPtr);
	return 0;
}

int activateTYPE3(char date1_1[], char date1_2[]) {
	MYSQL Conn;				//MySQL 정보를 담을 구조체
	MYSQL* ConnPtr = NULL;	//MySQL 핸들
	MYSQL_RES* Result;	    //쿼리 성공 시 결과를 담는 구조체 포인터
	MYSQL_ROW Row;			//쿼리 성공 시 결과로 나온 행의 정보를 담는 구조체
	int Stat;

	mysql_init(&Conn);		//MySQL 정보 초기화

	//DB와 연결
	ConnPtr = mysql_real_connect(&Conn, DB_HOST, DB_USER, DB_PASS, DB_NAME, DB_PORT, (char*)NULL, 0);

	//연결 결과 확인. NULL일 경우 연결 실패한 것.
	if (ConnPtr == NULL) {
		fprintf(stderr, "Mysql connection error : %s\n", mysql_error(&Conn));
		return 1;
	}

	//쿼리문 작성
	char Query2[QUERY_MAX_SIZE] = "SELECT supplier_ID, supplier_name, supply_part, date_year_month_day, defection FROM  supplier WHERE(date_year_month_day BETWEEN '";
	char Query2_1[] = "' AND '";
	char Query2_2[] = "') and defection = 'yes'";
	strcat(Query2, date1_1);
	strcat(Query2, Query2_1);
	strcat(Query2, date1_2);
	strcat(Query2, Query2_2);

	//쿼리 요청 및 성공 여부 받아오기
	Stat = mysql_query(ConnPtr, Query2);
	//쿼리 요청 실패 시 예외 처리
	if (Stat != 0) {
		fprintf(stderr, "Mysql query error: %s\n", mysql_error(&Conn));
		return 1;
	}

	printf("ID name supply_part year_month_day defection\n");
	//결과 확인하기
	Result = mysql_store_result(ConnPtr);
	while ((Row = mysql_fetch_row(Result)) != NULL) {
		printf("%s %s %s %s %s\n", Row[0], Row[1], Row[2], Row[3], Row[4]);
	}
	mysql_free_result(Result);
	mysql_close(ConnPtr);
	return 0;
}

int activateTYPE3_1(char date1_1[], char date1_2[]) {
	MYSQL Conn;				//MySQL 정보를 담을 구조체
	MYSQL* ConnPtr = NULL;	//MySQL 핸들
	MYSQL_RES* Result;	    //쿼리 성공 시 결과를 담는 구조체 포인터
	MYSQL_ROW Row;			//쿼리 성공 시 결과로 나온 행의 정보를 담는 구조체
	int Stat;

	mysql_init(&Conn);		//MySQL 정보 초기화

	//DB와 연결
	ConnPtr = mysql_real_connect(&Conn, DB_HOST, DB_USER, DB_PASS, DB_NAME, DB_PORT, (char*)NULL, 0);

	//연결 결과 확인. NULL일 경우 연결 실패한 것.
	if (ConnPtr == NULL) {
		fprintf(stderr, "Mysql connection error : %s\n", mysql_error(&Conn));
		return 1;
	}

	//쿼리문 작성
	char Query2[QUERY_MAX_SIZE] = "SELECT supplier_for_vehicle.VID, customers.name, supplier.supply_part FROM  supplier_for_vehicle, supplier, vehicles, customer_of_vehicle, customers WHERE supplier_for_vehicle.supplier_ID = supplier.supplier_ID AND supplier_for_vehicle.VID = vehicles.VID AND vehicles.VID = customer_of_vehicle.VID_fk2 AND customer_of_vehicle.customer_ID_fk2 = customers.customer_ID AND(supplier.defection = 'YES') AND (supplier.date_year_month_day BETWEEN '";
	char Query2_1[] = "' AND '";
	char Query2_2[] = "')";
	strcat(Query2, date1_1);
	strcat(Query2, Query2_1);
	strcat(Query2, date1_2);
	strcat(Query2, Query2_2);

	//쿼리 요청 및 성공 여부 받아오기
	Stat = mysql_query(ConnPtr, Query2);
	//쿼리 요청 실패 시 예외 처리
	if (Stat != 0) {
		fprintf(stderr, "Mysql query error: %s\n", mysql_error(&Conn));
		return 1;
	}

	printf("VID customer_name defected_supply_part\n");
	//결과 확인하기
	Result = mysql_store_result(ConnPtr);
	while ((Row = mysql_fetch_row(Result)) != NULL) {
		printf("%s %s %s\n", Row[0], Row[1], Row[2]);
	}
	mysql_free_result(Result);
	mysql_close(ConnPtr);
	return 0;
}

int activateTYPE3_2(char date1_1[], char date1_2[]) {
	MYSQL Conn;				//MySQL 정보를 담을 구조체
	MYSQL* ConnPtr = NULL;	//MySQL 핸들
	MYSQL_RES* Result;	    //쿼리 성공 시 결과를 담는 구조체 포인터
	MYSQL_ROW Row;			//쿼리 성공 시 결과로 나온 행의 정보를 담는 구조체
	int Stat;

	mysql_init(&Conn);		//MySQL 정보 초기화

	//DB와 연결
	ConnPtr = mysql_real_connect(&Conn, DB_HOST, DB_USER, DB_PASS, DB_NAME, DB_PORT, (char*)NULL, 0);

	//연결 결과 확인. NULL일 경우 연결 실패한 것.
	if (ConnPtr == NULL) {
		fprintf(stderr, "Mysql connection error : %s\n", mysql_error(&Conn));
		return 1;
	}

	//쿼리문 작성
	char Query2[QUERY_MAX_SIZE] = "SELECT dealers.dealer_name, supplier.supply_part, vehicles.VID FROM  supplier, supplier_for_vehicle, vehicles, vehicle_list_for_dealer, dealers WHERE (supplier.date_year_month_day BETWEEN '";
	char Query2_1[] = "' AND '";
	char Query2_2[] = "') and supplier.supplier_ID = supplier_for_vehicle.supplier_ID AND supplier_for_vehicle.VID = vehicles.VID AND vehicles.VID = vehicle_list_for_dealer.VID_FK3 AND vehicle_list_for_dealer.dealer_ID_FK = dealers.dealer_ID AND(supplier.defection = 'yes')";
	strcat(Query2, date1_1);
	strcat(Query2, Query2_1);
	strcat(Query2, date1_2);
	strcat(Query2, Query2_2);

	//쿼리 요청 및 성공 여부 받아오기
	Stat = mysql_query(ConnPtr, Query2);
	//쿼리 요청 실패 시 예외 처리
	if (Stat != 0) {
		fprintf(stderr, "Mysql query error: %s\n", mysql_error(&Conn));
		return 1;
	}

	//결과 확인하기
	Result = mysql_store_result(ConnPtr);
	printf("dealer_name defected_supply_part defected_VID\n");
	while ((Row = mysql_fetch_row(Result)) != NULL) {
		printf("%s %s %s\n", Row[0], Row[1], Row[2]);
	}
	mysql_free_result(Result);
	mysql_close(ConnPtr);
	return 0;
}

int activateTYPE4(int k, char year_char[]) {
	MYSQL Conn;				//MySQL 정보를 담을 구조체
	MYSQL* ConnPtr = NULL;	//MySQL 핸들
	MYSQL_RES* Result;	    //쿼리 성공 시 결과를 담는 구조체 포인터
	MYSQL_ROW Row;			//쿼리 성공 시 결과로 나온 행의 정보를 담는 구조체
	int Stat;

	mysql_init(&Conn);		//MySQL 정보 초기화

	//DB와 연결
	ConnPtr = mysql_real_connect(&Conn, DB_HOST, DB_USER, DB_PASS, DB_NAME, DB_PORT, (char*)NULL, 0);

	//연결 결과 확인. NULL일 경우 연결 실패한 것.
	if (ConnPtr == NULL) {
		fprintf(stderr, "Mysql connection error : %s\n", mysql_error(&Conn));
		return 1;
	}

	//쿼리문 작성
	char Query2[QUERY_MAX_SIZE] = "SELECT brand.brand_name, SUM(vehicles.price) AS total_profit FROM  customer_of_vehicle, customers, vehicles, models, brand WHERE customer_of_vehicle.customer_ID_fk2 = customers.customer_ID AND customer_of_vehicle.VID_fk2 = vehicles.VID AND vehicles.model_ID = models.model_ID AND models.brand_ID = brand.brand_ID AND(customer_of_vehicle.buy_when = '";
	char Query2_back[] = "') GROUP BY brand.brand_name ORDER BY total_profit DESC";
	strcat(Query2, year_char);
	strcat(Query2, Query2_back);

	//쿼리 요청 및 성공 여부 받아오기
	Stat = mysql_query(ConnPtr, Query2);
	//쿼리 요청 실패 시 예외 처리
	if (Stat != 0) {
		fprintf(stderr, "Mysql query error: %s\n", mysql_error(&Conn));
		return 1;
	}

	printf("brand_name total_profit\n");
	//결과 확인하기
	Result = mysql_store_result(ConnPtr);
	int cnt = 0;
	while ((Row = mysql_fetch_row(Result)) != NULL) {
		printf("%s %s\n", Row[0], Row[1]);
		if (++cnt == k) break;
	}
	mysql_free_result(Result);
	mysql_close(ConnPtr);
	return 0;
}

int activateTYPE5(int k, char year_char[]) {
	MYSQL Conn;				//MySQL 정보를 담을 구조체
	MYSQL* ConnPtr = NULL;	//MySQL 핸들
	MYSQL_RES* Result;	    //쿼리 성공 시 결과를 담는 구조체 포인터
	MYSQL_ROW Row;			//쿼리 성공 시 결과로 나온 행의 정보를 담는 구조체
	int Stat;

	mysql_init(&Conn);		//MySQL 정보 초기화

	//DB와 연결
	ConnPtr = mysql_real_connect(&Conn, DB_HOST, DB_USER, DB_PASS, DB_NAME, DB_PORT, (char*)NULL, 0);

	//연결 결과 확인. NULL일 경우 연결 실패한 것.
	if (ConnPtr == NULL) {
		fprintf(stderr, "Mysql connection error : %s\n", mysql_error(&Conn));
		return 1;
	}

	//쿼리문 작성
	char Query2[QUERY_MAX_SIZE] = "SELECT brand.brand_name, COUNT(*) AS sell_count FROM  customer_of_vehicle, customers, vehicles, models, brand WHERE customer_of_vehicle.customer_ID_fk2 = customers.customer_ID AND customer_of_vehicle.VID_fk2 = vehicles.VID AND vehicles.model_ID = models.model_ID AND models.brand_ID = brand.brand_ID AND(customer_of_vehicle.buy_when = '";
	char Query2_back[] = "') GROUP BY brand.brand_name";
	strcat(Query2, year_char);
	strcat(Query2, Query2_back);

	//쿼리 요청 및 성공 여부 받아오기
	Stat = mysql_query(ConnPtr, Query2);
	//쿼리 요청 실패 시 예외 처리
	if (Stat != 0) {
		fprintf(stderr, "Mysql query error: %s\n", mysql_error(&Conn));
		return 1;
	}

	printf("brand_name sell_count\n");
	//결과 확인하기
	int cnt = 0;
	Result = mysql_store_result(ConnPtr);
	while ((Row = mysql_fetch_row(Result)) != NULL) {
		printf("%s %s\n", Row[0], Row[1]);
		if (++cnt == k) break;
	}
	mysql_free_result(Result);
	mysql_close(ConnPtr);
	return 0;
}

int activateTYPE6() {
	MYSQL Conn;				//MySQL 정보를 담을 구조체
	MYSQL* ConnPtr = NULL;	//MySQL 핸들
	MYSQL_RES* Result;	    //쿼리 성공 시 결과를 담는 구조체 포인터
	MYSQL_ROW Row;			//쿼리 성공 시 결과로 나온 행의 정보를 담는 구조체
	int Stat;

	mysql_init(&Conn);		//MySQL 정보 초기화

	//DB와 연결
	ConnPtr = mysql_real_connect(&Conn, DB_HOST, DB_USER, DB_PASS, DB_NAME, DB_PORT, (char*)NULL, 0);

	//연결 결과 확인. NULL일 경우 연결 실패한 것.
	if (ConnPtr == NULL) {
		fprintf(stderr, "Mysql connection error : %s\n", mysql_error(&Conn));
		return 1;
	}

	//쿼리문 작성
	char Query2[QUERY_MAX_SIZE] = "with month_conv_cnt(month, cnt) as(SELECT customer_of_vehicle.buy_when_month, COUNT(*) AS convertible_total_count FROM  customer_of_vehicle, customers, vehicles WHERE customer_of_vehicle.customer_ID_fk2 = customers.customer_ID AND customer_of_vehicle.VID_fk2 = vehicles.VID AND(vehicles.convertible = 'yes') GROUP BY customer_of_vehicle.buy_when_month), max_cnt(value) as (select max(cnt) from month_conv_cnt) select month_conv_cnt.month from month_conv_cnt, max_cnt where month_conv_cnt.cnt = max_cnt.value";

	//쿼리 요청 및 성공 여부 받아오기
	Stat = mysql_query(ConnPtr, Query2);
	//쿼리 요청 실패 시 예외 처리
	if (Stat != 0) {
		fprintf(stderr, "Mysql query error: %s\n", mysql_error(&Conn));
		return 1;
	}

	printf("month\n");
	//결과 확인하기
	Result = mysql_store_result(ConnPtr);
	while ((Row = mysql_fetch_row(Result)) != NULL) {
		printf("%s\n", Row[0]);
	}
	mysql_free_result(Result);
	mysql_close(ConnPtr);
	return 0;
}

int activateTYPE7() {
	MYSQL Conn;				//MySQL 정보를 담을 구조체
	MYSQL* ConnPtr = NULL;	//MySQL 핸들
	MYSQL_RES* Result;	    //쿼리 성공 시 결과를 담는 구조체 포인터
	MYSQL_ROW Row;			//쿼리 성공 시 결과로 나온 행의 정보를 담는 구조체
	int Stat;

	mysql_init(&Conn);		//MySQL 정보 초기화

	//DB와 연결
	ConnPtr = mysql_real_connect(&Conn, DB_HOST, DB_USER, DB_PASS, DB_NAME, DB_PORT, (char*)NULL, 0);

	//연결 결과 확인. NULL일 경우 연결 실패한 것.
	if (ConnPtr == NULL) {
		fprintf(stderr, "Mysql connection error : %s\n", mysql_error(&Conn));
		return 1;
	}

	//쿼리문 작성
	const char* Query = "with dealer_time(name, time) as (SELECT dealers.dealer_name, AVG(vehicle_list_for_dealer.time_in_inventory) AS avg_time FROM  vehicles, vehicle_list_for_dealer, dealers WHERE vehicles.VID = vehicle_list_for_dealer.VID_FK3 AND vehicle_list_for_dealer.dealer_ID_FK = dealers.dealer_ID GROUP BY dealers.dealer_name), max_time(value) as (select max(time) from dealer_time) select dealer_time.name, dealer_time.time from dealer_time, max_time where dealer_time.time = max_time.value";
	//쿼리 요청 및 성공 여부 받아오기
	Stat = mysql_query(ConnPtr, Query);
	//쿼리 요청 실패 시 예외 처리
	if (Stat != 0) {
		fprintf(stderr, "Mysql query error: %s\n", mysql_error(&Conn));
		return 1;
	}

	printf("dealer_name avg_time_max\n");
	//결과 확인하기
	Result = mysql_store_result(ConnPtr);
	while ((Row = mysql_fetch_row(Result)) != NULL) {
		printf("%s %s\n", Row[0], Row[1]);
	}
	mysql_free_result(Result);
	mysql_close(ConnPtr);
	return 0;
}