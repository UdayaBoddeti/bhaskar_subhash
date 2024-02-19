// Item catalouge with doubly linked list

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// #include <time.h>
// #include <windows.h>
// #include <psapi.h>

#define NAME_LENGTH 30
#define ID_LENGTH 20
#define DATAFILE "items.dat"
#define PRINT_SEPERATOR() (printf("-------------------------------------------------------\n"))
#define PRINT_SUCCESS_MESSAGE(OP, ID) (printf("%s: Successfully %s item.\n", ID, OP))
#define PRINT_FAILED_MESSAGE(OP, ID) (printf("%s: Failed to %s item.\n", ID, OP))
#define PRINT_NOT_FOUND_MESSAGE() (printf("%s: Item not found.\n", ID))
#define GET_NEW_PRICE() (printf("Enter new price: "), scanf("%d", &new_price)) 
#define GETID(OP) (printf("Enter ID to %s: ", OP), scanf("%s", ID))
#define GET_CHILD(NODE) (NODE -> left_child != NULL ? NODE -> left_child : NODE -> right_child)

typedef struct 
{
	char id[ID_LENGTH];
	char name[NAME_LENGTH];
	long long int price;
}item;

struct node
{
	struct node *left_child;
	item item_details;
	struct node *right_child;
};


struct node *new_node, *root = NULL, *current;
FILE *fp_data;
char ID[ID_LENGTH];
int cmp_result;


int add_node_to_list(struct node *parent)
{
	if (root == NULL)
	{
		root = new_node;
	}
	else
	{
		if (strcmp(parent -> item_details.id, new_node -> item_details.id) < 0)
		{
			if (parent -> right_child == NULL)
			{
				parent -> right_child = new_node;
				return 1;
			}
			else
			{
				return add_node_to_list(parent -> right_child);
			}
		}

		if (strcmp(parent -> item_details.id, new_node -> item_details.id) > 0)
		{
			if (parent -> left_child == NULL)
			{
				parent -> left_child = new_node;
				return 1;
			}
			else
			{
				return add_node_to_list(parent -> left_child);
			}
		}
		return 0;
	}
}

void create_node()
{
	new_node = malloc(sizeof(item));
	new_node -> left_child = new_node -> right_child = NULL;
}

void load_list()
{
	fp_data = fopen(DATAFILE, "r");

	while ("Do until EOF")
	{
		create_node();
		if (fread(&(new_node -> item_details), sizeof(item), 1, fp_data))
		{
			add_node_to_list(root);
		}
		else
		{
			break;
		}
	}
	fclose(fp_data);
}

void print_details(struct node *node)
{
	PRINT_SEPERATOR();
	printf("Item ID: %s\n", node -> item_details.id);
	printf("Item name: %s\n", node -> item_details.name);
	printf("Item price: %d\n", node -> item_details.price);
	PRINT_SEPERATOR();
}

void view_all_records(struct node* parent)
{
	if (parent == NULL)
	{
		return;
	}

	view_all_records(parent -> left_child);
	print_details(parent);
	view_all_records(parent -> right_child);
}

void read_item_details(struct node *new_node)
{
	printf("Enter item name: ");
	scanf("%[^\n]s", new_node -> item_details.name);
	printf("Enter item price: ");
	scanf("%d", &(new_node -> item_details.price));
}

void save_data_to_file(struct node *parent)
{
	if (parent == NULL)
	{
		return;
	}

	fwrite(&(parent -> item_details), sizeof(item), 1, fp_data);
	save_data_to_file(parent -> left_child);
	save_data_to_file(parent -> right_child);
}

void save_list_to_file()
{
	fp_data = fopen(DATAFILE, "w");
	save_data_to_file(root);
	fclose(fp_data);
}

void add_record()
{
	create_node();
	read_item_details(new_node);

	id_generation: sprintf(new_node -> item_details.id , "%d", (rand() % 999));

	if (add_node_to_list(root))
	{
		PRINT_SUCCESS_MESSAGE("added", new_node -> item_details.id);
		save_list_to_file();
	}
	else
	{
		goto id_generation;
	}
}

struct node* get_matching_node(struct node *parent, char ID[])
{
	if (parent == NULL)
	{
		return NULL;
	}

	cmp_result = strcmp(parent -> item_details.id , ID);

	if (! cmp_result)
	{
		return parent;
	}
	else if(cmp_result < 0)
	{
		return get_matching_node(parent -> right_child, ID);
	}
	else
	{
		return get_matching_node(parent -> left_child, ID);
	}
}

void update_price(char ID[])
{
	struct node *match_node = get_matching_node(root, ID);
	int new_price;

	if (match_node)
	{
		GET_NEW_PRICE();
		match_node -> item_details.price = new_price;
		save_list_to_file();
	}
	else
	{
		PRINT_NOT_FOUND_MESSAGE();
	}
}

void search(char ID[])
{
	struct node *match_node = get_matching_node(root, ID);

	if (match_node)
	{
		print_details(match_node);
	}
	else
	{
		PRINT_NOT_FOUND_MESSAGE();
	}
}

struct node* get_inorder_successor(struct node* node)
{
	struct node *walk = node -> right_child;

	while (walk -> left_child != NULL)
	{
		walk = walk -> left_child;
	}
	return walk;
}

struct node* get_parent_node(struct node* child)
{
	struct node* walk = root;

	if (child == root)
	{
		return NULL;
	}
	while ("Do until parent found")
	{
		if (walk -> left_child == child || walk -> right_child == child)
		{
			return walk;
		}
		else
		{
			if (strcmp(walk -> item_details.id, child -> item_details.id) < 0)
			{
				walk = walk -> right_child;
			}
			else
			{
				walk = walk -> right_child;
			}
		}
	}
}

void delete(char ID[])
{
	struct node* match_node = get_matching_node(root, ID);

	if (match_node)
	{
		char confirm_delete;
		printf("are you sure that you want to delete item with ID %s? [Y/N]: ", ID);
		confirm_delete = getch();

		if (confirm_delete == 'y' || confirm_delete == 'Y')
		{
			printf("\n");
			struct node* match_node_parent = get_parent_node(match_node);

			if (match_node -> left_child == NULL && match_node -> right_child == NULL)
			{
				if (match_node != root)
				{
					if (match_node_parent -> left_child == match_node)
					{
						match_node_parent -> left_child = NULL;
					}
					else
					{
						match_node_parent -> right_child = NULL;
					}
				}
				else
				{
					root = NULL;
				}

				PRINT_SUCCESS_MESSAGE("deleted", ID);
			}

			else if (match_node -> right_child && match_node -> left_child)
			{
				struct node *inorder_successor = get_inorder_successor(match_node);

				match_node -> item_details = inorder_successor -> item_details;

				struct node* inorder_successor_parent = get_parent_node(inorder_successor);
				
				if (inorder_successor_parent -> left_child == inorder_successor)
				{
					inorder_successor_parent -> left_child = GET_CHILD(inorder_successor);
				}
				else
				{
					inorder_successor_parent -> right_child = GET_CHILD(inorder_successor);
				}
				
				PRINT_SUCCESS_MESSAGE("deleted", ID);
				return;
			}

			else
			{
				struct node* child = GET_CHILD(match_node);

				if (match_node != root)
				{

					if (match_node_parent == match_node)
					{
						match_node_parent -> left_child = child;
					}
					else
					{
						match_node_parent -> right_child = child;
					}
				}
				else
				{
					root = child;
				}

				PRINT_SUCCESS_MESSAGE("deleted", ID);
			}
		}
	}
	else
	{
		PRINT_NOT_FOUND_MESSAGE();
	}
}

void menu()
{
	int choice;

	printf("-------------------Item Catalouge-------------------\n");
	printf("Which action would you like to perform?\n");
	printf("0. Exit\n");
	printf("1. Add item to inventory\n");
	printf("2. View items in inventory\n");
	printf("3. Update price of an item\n");
	printf("4. Remove item from inventory\n");
	printf("5. Serach for item details\n");
	PRINT_SEPERATOR();
	printf("Enter your choice: ");
	scanf("%d", &choice);
	getchar();

	switch (choice)
	{
		case 0: exit(0);

		case 1: add_record();
				break;

		case 2: view_all_records(root);
				break;

		case 3: GETID("update");
				update_price(ID);
				break;
		case 4: GETID("remove");
				delete(ID);
				break;

		case 5: GETID("search");
				search(ID);
				break;

		default: printf("Invalid choice.\n");
				 break;
	}
}

void main()
{
	load_list();
	while ("Do until exit")
	{
		menu();
	}
}