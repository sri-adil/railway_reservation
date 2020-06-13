
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define PORT 15755
#define PASS_LENGTH 20

int user_function(int sock, int choice);
int irctc(int sock);
int list(int sock, int type);
int admin_function(int sock, int action);
int view_booking(int sock);

int main(int argc, char * argv[]){
	char *ip = "127.0.0.1";
	if(argc==2){
		ip = argv[1];
	}
	int cli_fd = socket(AF_INET, SOCK_STREAM, 0);
	if(cli_fd == -1){
		printf("Socket Creation Failed.\n");
		exit(0);
	}
	struct sockaddr_in ca;
	ca.sin_family=AF_INET;
	ca.sin_port= htons(PORT);
	ca.sin_addr.s_addr = inet_addr(ip);
	if(connect(cli_fd, (struct sockaddr *)&ca, sizeof(ca))==-1){
		printf("Connection Failed.\n");
		exit(0);
	}
	printf("Connection Established.\n");
	
	while(irctc(cli_fd)!=3);
	close(cli_fd);

	return 0;
}

int irctc(int sock){
	int choice;
	system("clear");
	printf("\n\n\t\t\tTRAIN RESERVATION SYSTEM\n");
	printf("\t\t\t1. Login\n");
	printf("\t\t\t2. Sign Up\n");
	printf("\t\t\t3. Exit\n");
	printf("\t\t\tEnter Your Choice: ");
	scanf("%d", &choice);
	write(sock, &choice, sizeof(choice));
	if(choice==1){
		int type, acc_no;
		char password[PASS_LENGTH];
		system("clear");
		printf("\n\t\t\tEnter the type of account:\n");
		printf("\t\t\t1. Customer\n\t\t\t2. Agent\n\t\t\t3. Admin\n");
		printf("\t\t\tEnter your Choice: ");
		scanf("%d", &type);
		printf("\t\t\tEnter Your Account Number: ");
		scanf("%d", &acc_no);
		strcpy(password,getpass("\t\t\tEnter the Password: "));

		write(sock, &type, sizeof(type));
		write(sock, &acc_no, sizeof(acc_no));
		write(sock, &password, strlen(password));

		int valid_login;
		read(sock, &valid_login, sizeof(valid_login));
		if(valid_login == 1){
			while(list(sock, type)!=-1);
			system("clear");
			return 1;
		}
		else if(valid_login == 0){
			printf("\t\t\tInvalid Credentials\n");
			while(getchar()!='\n');
			getchar();
			return 1;
		}
		else{
			printf("\t\t\tLogin Failed : Multiple Login For This User.\n");
			while(getchar()!='\n');
			getchar();
			return 1;
		}
	}
	else if(choice==2){
		int type, acc_no;
		char password[PASS_LENGTH], secret_pin[5], name[10];
		system("clear");
		printf("\n\t\t\tEnter The Type Of Account: \n");
		printf("\t\t\t1. Customer\n\t\t\t2. Agent\n\t\t\t3. Admin\n");
		printf("\t\t\tEnter Your Choice: ");
		scanf("%d", &type);
		printf("\t\t\tEnter Your Name: ");scanf("%s", name);
		strcpy(password,getpass("\t\t\tEnter Your Password: "));
		if(type == 3){
			int attempt = 1;
			while(1){
				strcpy(secret_pin, getpass("\t\t\tEnter secret key: "));attempt++;
				if(strcmp(secret_pin, "sri")!=0 && attempt<=3) printf("\t\t\tInvalid key. Please Try Again.\n");
				else break;
			}
			if(!strcmp(secret_pin, "sri"));
			else exit(0);
		}
		write(sock, &type, sizeof(type));
		write(sock, &name, sizeof(name));
		write(sock, &password, strlen(password));

		read(sock, &acc_no, sizeof(acc_no));
		printf("\t\t\tAccount Number For Further Logins: %d\n", acc_no);
		while(getchar()!='\n');
		getchar();		
		return 2;
	}
	else
		return 3;
}

int list(int sock, int type){
	int choice = 0;
	if(type == 1 || type == 2){
		system("clear");
		printf("\t\t\tOPTIONS\n");
		printf("\t\t\t1. Book Ticket\n\t\t\t2. Update Booking\n\t\t\t3. Cancel Booking\n\t\t\t4. View Bookings\n\t\t\t5. Logout\n\t\t\tEnter Your Choice: ");
		scanf("%d", &choice);
		return user_function(sock, choice);
		return -1;
	}
	else{
		system("clear");
		printf("\t\t\tOPTIONS\n\t\t\t1. Add Train\n\t\t\t2. Delete Train\n\t\t\t3. Modify Train\n\t\t\t4. Add Root User\n\t\t\t5. Delete User\n\t\t\t6. View all bookings\n\t\t\t7. Logout\n\t\t\tEnter Your Choice: ");
		scanf("%d", &choice);
		return admin_function(sock, choice);
	}
}

int admin_function(int sock, int choice){
	switch(choice){
		case 1:{
			int tno;
			char tname[50];
			write(sock, &choice, sizeof(choice));
			printf("\t\t\tEnter Train Name: ");scanf("%s", tname);
			fflush(stdin);
			printf("\t\t\tEnter Train Number: ");scanf("%d", &tno);
			write(sock, &tname, sizeof(tname));
			write(sock, &tno, sizeof(tno));
			read(sock, &choice, sizeof(choice));
			if(choice == 1 ) printf("\t\t\tTrain Added Successfully.\n");
			while(getchar()!='\n');
			getchar();
			return choice;
			break;
		}
		case 2:{
			int no_of_trains;
			write(sock, &choice, sizeof(choice));
			read(sock, &no_of_trains, sizeof(int));
			//printf("no of train:%d",no_of_trains);
			while(no_of_trains>0){
				int tid, tno;
				char tname[20];
				read(sock, &tid, sizeof(tid));
				read(sock, &tname, sizeof(tname));
				read(sock, &tno, sizeof(tno));
				if(strcmp(tname, "deleted")!=0)
					printf("\t\t\t%d.\t\t%d\t\t%s\n", tid+1, tno, tname);
				no_of_trains--;
			}
			printf("\n\n\t\t\tEnter -2 To Cancel.\n\t\t\tEnter The Train ID To Delete: ");
			scanf("%d", &no_of_trains);
			printf("no of train:%d",no_of_trains);
			write(sock, &no_of_trains, sizeof(int));
			read(sock, &choice, sizeof(choice));
			if(choice != -2) printf("\t\t\tTrain Deleted Successfully\n");
			else printf("\t\t\tOperation Cancelled!");
			while(getchar()!='\n');
			return choice;
			break;
		}
		case 3:{
			int no_of_trains;
			write(sock, &choice, sizeof(choice));
			read(sock, &no_of_trains, sizeof(int));
			while(no_of_trains>0){
				int tid, tno;
				char tname[20];
				read(sock, &tid, sizeof(tid));
				read(sock, &tname, sizeof(tname));
				read(sock, &tno, sizeof(tno));
				if(!strcmp(tname, "deleted"));
				else
				printf("\t\t\t%d.\t\t%d\t\t%s\n", tid+1, tno, tname);
				no_of_trains--;
			}
			printf("\n\n\t\t\tEnter Train ID To Modify: "); scanf("%d", &no_of_trains);
			write(sock, &no_of_trains, sizeof(int));
			printf("\n\t\t\tModifying Parameter:\n\t\t\t1. Train Name\n\t\t\t2. Train No.\n\t\t\t3. Available Seats\n");
			printf("\n\t\t\tEnter Your Choice: ");scanf("%d", &no_of_trains);
			write(sock, &no_of_trains, sizeof(int));
			if(no_of_trains == 2 || no_of_trains == 3){
				read(sock, &no_of_trains, sizeof(int));
				printf("\n\t\t\tCurrent Value: %d\n", no_of_trains);				
				printf("\t\t\tEnter New Value: ");scanf("%d", &no_of_trains);
				write(sock, &no_of_trains, sizeof(int));
			}
			else{
				char name[20];
				read(sock, &name, sizeof(name));
				printf("\n\t\t\tCurrent Value: %s\n", name);
				printf("\t\t\tEnter New Value: ");scanf("%s", name);
				write(sock, &name, sizeof(name));
			}
			read(sock, &choice, sizeof(choice));
			if(choice == 3) printf("\t\t\tTrain Data Modified Successfully\n");
			while(getchar()!='\n');
			return choice;
			break;
		}
		case 4:{
			write(sock, &choice, sizeof(choice));
			char pass[PASS_LENGTH],name[10];
			printf("\t\t\tEnter Name For New Admin: ");scanf("%s", name);
			strcpy(pass, getpass("\t\t\tEnter Password For The New Admin Account: "));
			write(sock, &name, sizeof(name));
			write(sock, &pass, sizeof(pass));
			read(sock, &choice, sizeof(choice));
			printf("\n\n\t\t\tThe Account Number For New Admin Is: %d\n", choice);
			read(sock, &choice, sizeof(choice));
			if(choice == 4)printf("\n\t\t\tSuccessfully Created Admin Account\n");
			while(getchar()!='\n');
			return choice;
			break;
		}
		case 5: {
			int choice, users, id;
			write(sock, &choice, sizeof(choice));
			printf("\n\t\t\tSelect Account Type To Delete\n");
			printf("\t\t\t1. Customer\n\t\t\t2. Agent\n\t\t\t3. Admin\n");
			printf("\n\t\t\tYour Choice: ");
			scanf("%d", &choice);
			write(sock, &choice, sizeof(choice));
			read(sock, &users, sizeof(users));
			while(users--){
				char name[10];
				read(sock, &id, sizeof(id));
				read(sock, &name, sizeof(name));
				if(strcmp(name, "deleted")!=0)
				printf("%d\t%s\n", id, name);
			}
			printf("\n\t\t\tEnter The ID To Delete: ");scanf("%d", &id);
			write(sock, &id, sizeof(id));
			read(sock, &choice, sizeof(choice));
			if(choice == 5) printf("\n\t\t\tSuccessfully Deleted User\n");
			else break;
			while(getchar()!='\n');
			return choice;
		}
		case 7: {
			write(sock, &choice, sizeof(choice));
			read(sock, &choice, sizeof(choice));
			if(choice==6) printf("\n\t\t\tLogged Out Successfully.\n");
			while(getchar()!='\n');
			getchar();
			return -1;
			break;
		}

		case 6:{
			write(sock, &choice, sizeof(choice));
			view_booking(sock);
			read(sock, &choice, sizeof(choice));
			return 6;
		}
		default: return -1;
	}
}

int user_function(int sock, int choice){
	switch(choice){
		case 1:{
			//book a ticket
			int trains, trainid, trainseats, trainno, newseats;
			char trainname[20];
			write(sock, &choice, sizeof(choice));
			read(sock, &trains, sizeof(trains));
			printf("\n\t\t\tID\tT_NO\tAV_SEAT\tTRAIN NAME\n");
			while(trains--){
				read(sock, &trainid, sizeof(trainid));
				read(sock, &trainno, sizeof(trainno));
				read(sock, &trainseats, sizeof(trainseats));
				read(sock, &trainname, sizeof(trainname));
				if(strcmp(trainname, "deleted")!=0)
				printf("\t\t\t%d\t%d\t%d\t%s\n", trainid, trainno, trainseats, trainname);
			}
			printf("\n\t\t\tEnter Train ID: "); scanf("%d", &trainid);
			write(sock, &trainid, sizeof(trainid));
			read(sock, &trainseats, sizeof(trainseats));
			printf("\n\t\t\tEnter the number of seats: "); scanf("%d", &newseats);
			if(trainseats>=newseats && newseats>0)
				write(sock, &newseats, sizeof(newseats));
			else{
				newseats = -1;
				write(sock, &newseats, sizeof(newseats));
			}
			read(sock, &choice, sizeof(choice));
			
			if(choice == 1) printf("\n\n\t\t\tTicket Booked Successfully\n");
			else printf("\n\n\t\t\tTickets Were Not Booked. Please Try Again.\n");
			printf("\n\n\t\t\tPress Any Key To Continue...\n");
			while(getchar()!='\n');
			while(!getchar());
			return 1;
		}
		case 4:{
			//View your bookings
			write(sock, &choice, sizeof(choice));
			view_booking(sock);
			read(sock, &choice, sizeof(choice));
			return 2;
		}
		case 2:{
			//update bookings
			int val;
			write(sock, &choice, sizeof(choice));
			int ret_val=view_booking(sock);
			if(ret_val!=-2)
			{
				printf("\n\t\t\tEnter The Booking ID To Be Updated: "); scanf("%d", &val);
				write(sock, &val, sizeof(int));	//Booking ID
				printf("\n\t\t\tInformation To Update:\n\t\t\t1. Increase Number of Seats\n\t\t\t2. Decrease Number of Seats\n\t\t\tEnter Your Choice: ");
				scanf("%d", &val);
				write(sock, &val, sizeof(int));	//Increase or Decrease
				if(val == 1){
					printf("\n\t\t\tNumber of Tickets To Increase: ");scanf("%d",&val);
					write(sock, &val, sizeof(int));	//No of Seats
				}else if(val == 2){
					printf("\n\t\t\tNumber of Tickets To Decrease: ");scanf("%d",&val);
					write(sock, &val, sizeof(int));	//No of Seats		
				}
				read(sock, &choice, sizeof(choice));
				if(choice == -2)
					printf("\n\n\t\t\tOperation Failed. No More Available Seats.\n");
				else printf("\n\n\t\t\tUpdate Successfull.\n");
			}
			while(getchar()!='\n');
			return 3;
		}
		case 3: {
			write(sock, &choice, sizeof(choice));
			view_booking(sock);
			int val;
			printf("\n\t\t\tEnter Booking ID To Be Deleted: "); scanf("%d", &val);
			write(sock, &val, sizeof(int));	//Booking ID
			read(sock, &choice, sizeof(choice));
			if(choice == -2)
				printf("\n\n\t\t\tOperation Failed. No More Available Seats.\n");
			else printf("\n\n\t\t\tDeletion Successfull.\n");
			while(getchar()!='\n');
			return 3;
			}
		case 5: {
			write(sock, &choice, sizeof(choice));
			read(sock, &choice, sizeof(choice));
			if(choice == 5) printf("\n\n\t\t\tLogged Out Successfully.\n");
			while(getchar()!='\n');
			return -1;
			break;
		}
		default: return -1;
	}
}

int view_booking(int sock){
	int entries;
	read(sock, &entries, sizeof(int));
	if(entries == 0)
	{ 
		printf("\n\n\t\t\tNo Records Found.\n");
		return -2;
	}
	else printf("\n\n\t\t\tYour Recent %d Bookings Are :\n", entries);
	while(!getchar());
	while(entries--){
		int bid, bks_seat, bke_seat, cancelled;
		char trainname[20];
		read(sock,&bid, sizeof(bid));
		read(sock,&trainname, sizeof(trainname));
		read(sock,&bks_seat, sizeof(int));
		read(sock,&bke_seat, sizeof(int));
		read(sock,&cancelled, sizeof(int));
		if(!cancelled)
		printf("\t\tBookingID: %d\t1st Ticket: %d\tLast Ticket: %d\tTRAIN :%s\n", bid, bks_seat, bke_seat, trainname);
	}
	printf("\n\n\t\t\tPress Any Key To Continue...\n");
	while(getchar()!='\n');

	return 0;
}
