/*
NAME: LEONG CHANG YUNG
ASSIGNMENT TITLE: Grocery Store Database
DATE: 15/12/2020
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#define SIZE 100

struct item {
	int id;
	char name[20];
	double cost;
	char category;
	struct item* next;
};

int instruction(struct item** Ptr);
int append(struct item** Ptr);
int del(struct item** Ptr);
int priceC(struct item** Ptr);
void search(struct item** Ptr);
int asort(struct item** Ptr);
int dsort(struct item** Ptr);
void view(struct item** Ptr);
void saving(struct item** Ptr, char file[SIZE]);

int main(void) {
	char fname[SIZE];
	int choice;
	int changes = 0;
	int S;
	int start = 0;
	char string[SIZE];
	char husk[1];

	typedef struct item node;
	typedef node* nodePtr;

	nodePtr N;
	nodePtr sPtr;
	nodePtr cPtr;
	nodePtr pPtr;
	nodePtr cleaner;

	sPtr = malloc(sizeof(node));

	N = sPtr;
	cPtr = sPtr;

	FILE* read;

	printf("Welcome to the grocery store!\n");
	printf("Please input the file you'd like to load into stock.\n");
	scanf("%s", fname);
	read = fopen(fname, "r");

	//if the file is not found.
	if (read == NULL) {
		printf("%s could not be found", fname);
		sleep(5);
		return 0;
	}
	else {
		// start loading it into a linked list.
		while (!feof(read)) {
			char readline[SIZE];
			fgets(readline, sizeof(readline), read);
			sscanf(readline, "%d %s %lf %c", &N->id, N->name, &N->cost, &N->category);
			if (!feof(read)) {
				N->next = malloc(sizeof(node));
				N = N->next;
			} // end if
		} // end while
	} // end else

	N->next = NULL;
	fclose(read);

	printf("%s has been properly loaded into the database!\n", fname);

	// it keeps going until user goes for mode 6 which is quit.
	while (1) {
		// it prompts the user which mode it wants.
		choice = instruction(&sPtr);

    		scanf("%c", husk);
		
		// and based on the user input it will go to the mode based on the switch.
		switch (choice) {
		case 1:
			changes += append(&sPtr);
			break;
		case 2:
			changes += del(&sPtr);
			break;
		case 3:
			changes += priceC(&sPtr);
			break;
		case 4:
			search(&sPtr);
			break;
		case 5:

			do {
				printf("Sort based on ascending (1) or descending (2)?\n");
				gets(string);
				S = atoi(string);
			} while (!(S >= 1 && S <= 2) || strlen(string) != 1);
			if (S == 1) {
				changes += asort(&sPtr);
				view(&sPtr);
			}
			else if (S == 2) {
				changes += dsort(&sPtr);
				view(&sPtr);
			}
			S = 0;
			break;
		case 6:
			if (changes != 0) {
				saving(&sPtr, fname);
			}
			else {
				printf("Thanks for shopping\n");
			}

			// frees the node in the linked list.
			while (sPtr != NULL) {
				cleaner = sPtr;
				sPtr = sPtr->next;
				free(cleaner);
			}
			sleep(5);
			return 1;
		} // end switch
	} // end while
} // end main

// A menu for the user
int instruction(struct item** Ptr) {
	// to ensure the first while is false
	int input = 7;
	int flag = 0;
	char Input[2];
	struct item* cPtr = *Ptr;

	// prompt the user for the right input between 1 to 6. if list empty prompt the user to add more or quit
	do {
		flag = 0;

		if (cPtr == NULL) {
			printf("\nThere is no data in the list.");
		}

		printf("\nPlease make one of the following selections:\n");
		printf("1. Add new item\n");
		printf("2. Delete item\n");
		printf("3. Change the cost of an item\n");
		printf("4. Search for item\n");
		printf("5. Display inventory details\n");
		printf("6. Quit\n");
		scanf("%s", Input);

		input = atoi(Input);

		if (cPtr == NULL && input == 6) {
			return input;
		}

		for (int x = 0; x < strlen(Input); x++) {
			flag += !(isdigit(Input[x]));
		}

		if (!(input >= 1 && input <= 6) || flag != 0) {
			printf("\nThat is not an option. Please select from 1 to 6.");
		}

	} while (!(input > 0 && input <= 6) || (cPtr == NULL && !(input == 1)) || strlen(Input) != 1);

	return input;

} // end function instruction

// adds more stuff the the linked list.
int append(struct item** Ptr) {

	struct item* temp = malloc(sizeof(struct item));
	struct item* cPtr = *Ptr;
	struct item* bFinal;

	int length;
	char* check;
	double value;
	char string[SIZE] = "";

	// to flag if the input is valid
	int flag = 0;

	// inputting the valid data into a temp node for adding
	// getting the id. (id)
	while (flag != 0 || !strcmp(string, "")) {
		flag = 0;
		printf("What is the ID number of the item to add?\n");
		gets(string);

		length = strlen(string);

		// assuring the maximum value that can be inputted is up to 5 digits.
		if (length > 5) {
			printf("Please enter numbers from 0 to 99999 only\n");
			flag++;
		}

		// this ensures that there is no other char other than numbers inputted. prompting the user for only digits. 
		for (int x = 0; x < length; x++) {
			flag += !(isdigit(string[x]));
		}

		// converts the string to int if there is no other characters other than digits.
		temp->id = atoi(string);

		// ensure that there is no negative value
		if (temp->id < 0) {
			printf("Please enter numbers from 0 to 99999 only\n");
			flag++;
		}
	}

	// getting the name of the product and capitalizing the first char of the name (name) and ensuring that the first char is capitalized.
	do {
		// reset flag counter/flag.
		flag = 0;

		printf("What is the name of the item to be added?\n");
		gets(temp->name);
		temp->name[0] = toupper(temp->name[0]);

		for (int x = 0; x < strlen(temp->name); x++) {
			if (temp->name[x] == ' ') {
				flag++;
			}
		}

		if (flag != 0) {
			printf("No spaces allowed consider changing the spaces with '_'.\n");
		}

	} while (!strcmp(temp->name, "") || flag != 0);

	// ensures that there is only double inputted into the string. (cost)
	do {
		printf("What is the cost of the item to be added?\n");
		gets(string);

		temp->cost = strtod(string, &check);
		length = strlen(check);
	} while (!strcmp(string, "") || length != 0 || temp->cost < 0);

	// resetting the length variable.
	length = 0;

	// this it to ensure that only 1 char is inputted and only if it have C,N,P,D,M inserted. (category)
	while (!(temp->category == 'C' || temp->category == 'N' || temp->category == 'P' || temp->category == 'D' || temp->category == 'M') || length != 1) {
		printf("What is the product category of the item to be added?\n");
		gets(string);
		length = strlen(string);
		temp->category = toupper(string[0]);
	}

	// the temp is not pointing to anything (NULL)
	temp->next = NULL;

	// if linked list is empty add the temp node in. since there is no need for checking if there is any other nodes with same id
	if (cPtr == NULL) {
		*Ptr = temp;
		printf("%s has been added into the database.\n", temp->name);
		return 1;
	}

	// iterate through the linked list to find the node with the same id
	while (cPtr != NULL) {

		// if there is same id in the temp node and the one of the node in the linked list then don't add.
		if (temp->id == cPtr->id) {
			printf("Sorry, an item with the ID number %i is already in the database.\n", temp->id);
			return 0;
		} // end if

		// if there is same name in the temp node and the one of the node in the linked list then don't add.
		if (!strcasecmp(temp->name, cPtr->name)) {
			printf("Sorry, an item with the similar/same name (%s & %s) is already in the database.\n", temp->name, cPtr->name);
			return 0;
		} // end if
		bFinal = cPtr;
		cPtr = cPtr->next;

	} // end while

		// if not able to find then add the node at (the) end.
	bFinal->next = temp;
	printf("%s has been added into the database.\n", temp->name);

	return 1;

}// end function append.

// remove a node from the linked list
int del(struct item** Ptr) {


	char string[SIZE] = "";
	int val;
	int found = 0;

	struct item* Del = NULL;
	struct item* cPtr = *Ptr;
	struct item* bDel = NULL;
	struct item* bFinal = NULL;
	struct item* aDel = NULL;
	struct item* first = *Ptr;

	// to flag if the input is valid
	int flag = 0;

	while (!strcmp(string, "") || flag != 0) {
		flag = 0;
		printf("What is the ID number of the product to delete?\n");
		gets(string);

		for (int x = 0; x < strlen(string); x++) {
			flag += !(isdigit(string[x]));
		}

		// converts the string to int if there is no other characters other than digits.
		val = atoi(string);
	}

	// searching for the right one (node) until the next node is NULL.
	while (cPtr->next != NULL) {

		// if it's found, then take the node next to the targetted node as aDel
		if (found == 1) {
			aDel = cPtr;
			found++;
		}

		// if the targetted node is found it's location will be stored in Del.
		if (cPtr->id == val) {
			Del = cPtr;
			found++;
		}

		// this takes the node before the targetted node
		if (cPtr->next->id == val) {
			bDel = cPtr;
		}

		// this will get the 2nd final node in the linked list
		if (cPtr->next->next == NULL) {
			bFinal = cPtr;
		}

		// moves the pointer
		cPtr = cPtr->next;

	}// end while

	// if it's the node the user is searching for is on the first node in the linked list.
	if (first->id == val) {
		printf("Product with the number ID of %i is deleted\n", first->id);

		// if the list only have one node and that node is the one having the same id.
		if (first->next == NULL) {
			free(first);
			*Ptr = NULL;
			return 1;
		}
		else {
			*Ptr = cPtr;
			bFinal->next = NULL;
			cPtr->next = aDel;
			return 1;
		}
	}

	// if it's at the last node in the linked list.
	if (cPtr->id == val) {
		printf("Product with the number ID of %i is deleted\n", cPtr->id);
		bDel->next = NULL;
		free(cPtr);
		return 1;
	}

	// if the node is not pressent in the linked list
	if (found == 0) {
		printf("Sorry, there is no item in the database with that ID number.\n");
		return 0;
	}
	else {
		// if the node is found in the linked list.
		printf("Product with the number ID of %i is deleted\n", Del->id);
		free(Del);
		bFinal->next = NULL;
		bDel->next = cPtr;
		cPtr->next = aDel;
		return 1;
	}
}// end function del.

// changing the price/cost of a specific node 
int priceC(struct item** Ptr) {

	int code;
	int found = 0;
	int changes = 0;
	int inicost;
	char string[SIZE] = "";
	char* check;

	struct item* cPtr = *Ptr;

	// to flag if the input is valid
	int flag = 0;

	while (!strcmp(string, "") || flag != 0) {
		flag = 0;
		printf("What is the ID number of the item in question?\n");
		gets(string);

		for (int x = 0; x < strlen(string); x++) {
			flag += !(isdigit(string[x]));
		}

		// converts the string to int if there is no other characters other than digits.
		code = atoi(string);
	}

	// searching for the right node.
	while (cPtr != NULL && found == 0) {

		// if found then get the initial cost of the node.
		if (cPtr->id == code) {
			inicost = cPtr->cost;
			found++;
		}
		else {
			cPtr = cPtr->next;
		}
	}

	if (found == 1) {
		do {
			printf("What is the new price for %s\n", cPtr->name);
			gets(string);

			// this ensures that there is no other char other than numbers inputted. prompting the user for only digits. 
			for (int x = 0; x < strlen(string); x++) {
				flag += !(isdigit(string[x]));
			}

			cPtr->cost = strtod(string, &check);
		} while (cPtr->cost < 0 || !strcmp(string, "") || strlen(check) != 0);

		printf("%s now costs %.2f.\n", cPtr->name, cPtr->cost);

		// if the node and the price inputted by the user is the same then don't increase the changes in main.
		int v1 = cPtr->cost * 100;
		int v2 = inicost * 100;
		if (v1 == v2) {
			return 0;
		}
		else {
			return 1;
		}
	}
	else {
		printf("Sorry, there is no item in the database with that ID number.\n");
		return 0;
	}
}

// finding the node based on the ID given
void search(struct item** Ptr) {

	char item[20] = "";
	int found = 0;

	struct item* cPtr = *Ptr;

	// this ensures that the user doesn't type in enter without any other characters in it.
	do {
		printf("Which item would you like to search for?\n");
		gets(item);
	} while (!strcmp(item, ""));

	// iterating the linked list.
	while (cPtr != NULL) {

		// if found the name that is the same as the node then it will break. if not keep going to the next node. 
		if (strcasecmp(item, cPtr->name) == 0) {
			found++;
			break;
		}
		else {
			cPtr = cPtr->next;
		}
	}// end while

	if (found == 1) {
		printf("Here is the product information:\n");
		printf("%05d %s %.2lf %c\n", cPtr->id, cPtr->name, cPtr->cost, cPtr->category);
	}
	else {
		printf("Sorry, there is no item in the database with that name.\n");
	}

}// end function search.

// sorting based on ascending order and will promt the user for the sort based on What.
int asort(struct item** Ptr) {

	// m stands for medium
	int mi;
	double mcost;
	char mcat;
	char mn[20];
	char input[SIZE];

	int sel = 0;
	int start = 0;
	int changes = 0;

	struct item* anchor = *Ptr;
	struct item* cPtr;

	while (!(sel >= 1 && sel <= 4)) {
		printf("Ascending sort according to ID (1), name (2), cost (3), category (4)?\n");
		scanf("%s", input);

		sel = atoi(input);
	}

	// this will follow the path based on the sel value if there are any swapping it will flag there are changes and save into changes in main.
	switch (sel) {
	case 1:
		while (anchor->next != NULL) {

			// if start is 0 it means a new/start pass have begun
			if (start == 0) {
				cPtr = anchor;
				start++;
			}// end if

			// if the anchor id have a greater value than cPtr id then swap nodes.
			if (anchor->id > cPtr->id) {
				mi = cPtr->id;
				mcost = cPtr->cost;
				mcat = cPtr->category;
				strcpy(mn, cPtr->name);

				cPtr->id = anchor->id;
				cPtr->category = anchor->category;
				cPtr->cost = anchor->cost;
				strcpy(cPtr->name, anchor->name);

				anchor->category = mcat;
				anchor->cost = mcost;
				anchor->id = mi;
				strcpy(anchor->name, mn);

				changes++;
			}// end if

			// if reach the cPtr reach the end of the list the anchor move to the next and reset start to 0, else move cPtr
			if (cPtr->next == NULL) {
				anchor = anchor->next;
				start = 0;
			}
			else {
				cPtr = cPtr->next;
			}
		}// end while
		return changes;
		break;

	case 2:
		while (anchor->next != NULL) {

			// if start is 0 it means a new/start pass have begun
			if (start == 0) {
				cPtr = anchor;
				start++;
			}// end if

			// if the anchor name first char have a greater value than cPtr name first char then swap nodes.
			if (anchor->name[0] > cPtr->name[0]) {
				mi = cPtr->id;
				mcost = cPtr->cost;
				mcat = cPtr->category;
				strcpy(mn, cPtr->name);

				cPtr->id = anchor->id;
				cPtr->category = anchor->category;
				cPtr->cost = anchor->cost;
				strcpy(cPtr->name, anchor->name);

				anchor->category = mcat;
				anchor->cost = mcost;
				anchor->id = mi;
				strcpy(anchor->name, mn);

				changes++;
			}// end if

			// if reach the cPtr reach the end of the list the anchor move to the next and reset start to 0, else move cPtr
			if (cPtr->next == NULL) {
				anchor = anchor->next;
				start = 0;
			}
			else {
				cPtr = cPtr->next;
			}
		}// end while
		return changes;
		break;
	case 3:
		while (anchor->next != NULL) {
			if (start == 0) {
				cPtr = anchor;
				start++;
			}// end if

			// if the anchor cost have a greater value than cPtr cost then swap nodes.
			if (anchor->cost > cPtr->cost) {
				mi = cPtr->id;
				mcost = cPtr->cost;
				mcat = cPtr->category;
				strcpy(mn, cPtr->name);

				cPtr->id = anchor->id;
				cPtr->category = anchor->category;
				cPtr->cost = anchor->cost;
				strcpy(cPtr->name, anchor->name);

				anchor->category = mcat;
				anchor->cost = mcost;
				anchor->id = mi;
				strcpy(anchor->name, mn);

				changes++;
			}// end if

			// if reach the cPtr reach the end of the list the anchor move to the next and reset start to 0, else move cPtr
			if (cPtr->next == NULL) {
				anchor = anchor->next;
				start = 0;
			}
			else {
				cPtr = cPtr->next;
			}
		}
		return changes;
		break;
	case 4:
		while (anchor->next != NULL) {
			if (start == 0) {
				cPtr = anchor;
				start++;
			}// end if

			// if the anchor category have a greater value than cPtr category then swap nodes.
			if (anchor->category > cPtr->category) {
				mi = cPtr->id;
				mcost = cPtr->cost;
				mcat = cPtr->category;
				strcpy(mn, cPtr->name);

				cPtr->id = anchor->id;
				cPtr->category = anchor->category;
				cPtr->cost = anchor->cost;
				strcpy(cPtr->name, anchor->name);

				anchor->category = mcat;
				anchor->cost = mcost;
				anchor->id = mi;
				strcpy(anchor->name, mn);

				changes++;
			}// end if

			// if reach the cPtr reach the end of the list the anchor move to the next and reset start to 0, else move cPtr
			if (cPtr->next == NULL) {
				anchor = anchor->next;
				start = 0;
			}
			else {
				cPtr = cPtr->next;
			}
		}
		return changes;
		break;
	default:
		printf("Wrong input");
		return 0;
	}
}// end function asort

// sorting based on descending order and will prompt the user sort based on what. 
int dsort(struct item** Ptr) {

	// m stands for medium
	int mi;
	double mcost;
	char mcat;
	char mn[20];
	char input[SIZE];

	int sel = 0;
	int start = 0;
	int changes = 0;

	struct item* anchor = *Ptr;
	struct item* cPtr;

	while (!(sel >= 1 && sel <= 4)) {
		printf("Descending sort according to ID (1), name (2), cost (3), category (4)?\n");
		scanf("%s", input);

		sel = atoi(input);
	}

	// this will follow the path based on the sel value if there are any swapping it will increase the changes to save into changes in main.
	switch (sel) {
	case 1:
		while (anchor->next != NULL) {
			if (start == 0) {
				cPtr = anchor;
				start++;
			}// end if


			// if anchor id have a smaller value than cPtr id then swap
			if (anchor->id < cPtr->id) {
				mi = cPtr->id;
				mcost = cPtr->cost;
				mcat = cPtr->category;
				strcpy(mn, cPtr->name);

				cPtr->id = anchor->id;
				cPtr->category = anchor->category;
				cPtr->cost = anchor->cost;
				strcpy(cPtr->name, anchor->name);

				anchor->category = mcat;
				anchor->cost = mcost;
				anchor->id = mi;
				strcpy(anchor->name, mn);
				changes++;
			}// end if

			// if reach the cPtr reach the end of the list the anchor move to the next and reset start to 0 else move the cPtr
			if (cPtr->next == NULL) {
				anchor = anchor->next;
				start = 0;
			}
			else {
				cPtr = cPtr->next;
			}
		} // end while
		return changes;
		break;
	case 2:
		while (anchor->next != NULL) {
			if (start == 0) {
				cPtr = anchor;
				start++;
			} // end if

			// if anchor name first char have a smaller value than cPtr name first char then swap
			if ((int)anchor->name[0] < (int)cPtr->name[0]) {
				mi = cPtr->id;
				mcost = cPtr->cost;
				mcat = cPtr->category;
				strcpy(mn, cPtr->name);

				cPtr->id = anchor->id;
				cPtr->category = anchor->category;
				cPtr->cost = anchor->cost;
				strcpy(cPtr->name, anchor->name);

				anchor->category = mcat;
				anchor->cost = mcost;
				anchor->id = mi;
				strcpy(anchor->name, mn);

				changes++;
			} // end if

			// if reach the cPtr reach the end of the list the anchor move to the next and reset start to 0 else move the cPtr
			if (cPtr->next == NULL) {
				anchor = anchor->next;
				start = 0;
			}
			else {
				cPtr = cPtr->next;
			}
		} // end while
		return changes;
		break;
	case 3:
		while (anchor->next != NULL) {
			if (start == 0) {
				cPtr = anchor;
				start++;
			}// end if

			// if anchor cost have a smaller value than cPtr cost then swap
			if (anchor->cost < cPtr->cost) {
				mi = cPtr->id;
				mcost = cPtr->cost;
				mcat = cPtr->category;
				strcpy(mn, cPtr->name);

				cPtr->id = anchor->id;
				cPtr->category = anchor->category;
				cPtr->cost = anchor->cost;
				strcpy(cPtr->name, anchor->name);

				anchor->category = mcat;
				anchor->cost = mcost;
				anchor->id = mi;
				strcpy(anchor->name, mn);

				changes++;
			}// end if

			// if reach the cPtr reach the end of the list the anchor move to the next and reset start to 0 else move the cPtr
			if (cPtr->next == NULL) {
				anchor = anchor->next;
				start = 0;
			}
			else {
				cPtr = cPtr->next;
			}
		}// end while
		return changes;
		break;
	case 4:
		while (anchor->next != NULL) {
			if (start == 0) {
				cPtr = anchor;
				start++;
			}// end if

			// if anchor category have a smaller value than cPtr category then swap
			if (anchor->category < cPtr->category) {
				mi = cPtr->id;
				mcost = cPtr->cost;
				mcat = cPtr->category;
				strcpy(mn, cPtr->name);

				cPtr->id = anchor->id;
				cPtr->category = anchor->category;
				cPtr->cost = anchor->cost;
				strcpy(cPtr->name, anchor->name);

				anchor->category = mcat;
				anchor->cost = mcost;
				anchor->id = mi;
				strcpy(anchor->name, mn);

				changes++;
			}// end if

			// if reach the cPtr reach the end of the list the anchor move to the next and reset start to 0 else move the cPtr
			if (cPtr->next == NULL) {
				anchor = anchor->next;
				start = 0;
			}
			else {
				cPtr = cPtr->next;
			}
		}
		return changes;
		break;
	default:
		printf("Wrong input");
		return 0;
	}
}// end function dsort

// browsing the linked list
void view(struct item** Ptr) {

	struct item* cPtr = *Ptr;

	// iterate through the linked list and printing of the contents of each nodes.
	printf("Here is a listing of all the products in stock:\n");
	printf("%-5s %-20s %-6s %s\n", "ID", "Product", "Price", "Category");
	while (cPtr != NULL) {
		printf("%05i %-20s %-6.2lf %c\n", cPtr->id, cPtr->name, cPtr->cost, cPtr->category);
		cPtr = cPtr->next;
	}
}// end function view

// saving the file if there are changes
void saving(struct item** Ptr, char file[SIZE]) {
	char saving[3];
	FILE* write;
	char fname[SIZE];

	struct item* cPtr = *Ptr;

	// ensures the user input is either yes or no
	while (strcasecmp(saving, "yes") && strcasecmp(saving, "no")) {
		printf("Would you like to save the changes made to the database? (yes/no)\n");
		scanf("%s", saving);
	}

	// if user input is yes it will start loading the linked list to a file specified by user.
	if (!strcasecmp(saving, "yes")) {
		printf("What file would you like to save the new database?\n");
		scanf("%s", fname);

		//this ensures that the file that is used for loading is not the same as the one saving.
		while (!strcmp(fname, file)) {
			printf("You can't save into the same file you used to load.\n");
			printf("What file would you like to save the new database?\n");
			scanf("%s", fname);
		}

		write = fopen(fname, "w");

		// start loading into the file.
		while (cPtr != NULL) {

			// for the last node
			if (cPtr->next != NULL) {
				fprintf(write, "%i %s %0.2lf %c\n", cPtr->id, cPtr->name, cPtr->cost, cPtr->category);
			}
			else {
				fprintf(write, "%i %s %0.2lf %c", cPtr->id, cPtr->name, cPtr->cost, cPtr->category);
			}
			cPtr = cPtr->next;
		} // end while

		printf("The file has been saved. Thanks for shopping!\n");

		// if user input no
	}
	else if (!strcasecmp(saving, "no")) {
		printf("Thanks for shopping\n");
	}
}// end function saving
