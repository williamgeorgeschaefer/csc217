// William Schaefer (N00857559@students.ncc.edu)
// CSC 217
// Project 2: "Bookstore Part 1"
// Due November 25, 2020 at 11:59 pm
// booklist.c 
//
// This code contains functions to add books to the
// linked list, print the items in the list, search for books
// and compare two books.  The nodes in the linked list are
// stored alphabetically. 

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "booklist.h"
#include "invalidlist.h"
#include "createbook.h"
#include "textbook.h"

// Add function - Accepts two struct book arguments, the former is
// the head of the list and the latter is the book to be added.  The
// nodes of the linked list are stored alphabetically.  It also accepts
// an integer argument to denote whether the author sort is active or not.
// This method will return the header of the list.
struct book* add(struct book *header, struct book *newBook, int authorSortActive){
    struct book *node = (struct book*)malloc(sizeof(struct book));
    node->numCopies = 1;
    strcpy(node->isbn, newBook->isbn);
    strcpy(node->title, newBook->title);
    strcpy(node->last, newBook->last);
    strcpy(node->first, newBook->first);
    node->next = 0;

    // List is empty.
    if(header == 0){
        return node;
    }

    // Is the node the first item in the list?  If yes, return the new header of the list...

    // Author sort is inactive, and the title of the book node being 
    // added to the list comes first alphabetically.
    if(authorSortActive == 0 && bookCompareTitle(node, header) < 0){
        node->next = header;
        return node;
    }
    // Author sort is active, and the last name of the author of the book
    // node being added to the list comes first alphabetically.
    else if(authorSortActive != 0 && bookCompareLast(node, header) < 0){
        node->next = header;
        return node;
    }
    struct book *current = header;
    struct book *previous = header;

    // ...if no, search the list to find where it belongs.

    // If the author sorting command is inactive, this method will
    // alphabetize the list of books by title.
    if(authorSortActive == 0){
        while(current != 0 && bookCompareTitle(current, node) < 0){
            previous = current;
            current = current->next;
        }
    }
    // If the author sorting command is active, this method will
    // alphabetize the list of books by author's last name.
    else{
        while(current != 0 && bookCompareLast(current, node) < 0){
            previous = current;
            current = current->next;
        }
    }
    

    node->next = current;
    previous->next = node;
    return header;
}

// Print List function - Accepts one struct book argument, the header
// of the linked list.  It will print the nodes in order.
void printList(struct book* header) {
    struct book *current = header;
    while(current != 0){
        printf("%s%s%s%s%d%s\n", current->title, " (", current->last, "): ", current->numCopies, " copies");
        current = current->next;
    }
}

void writeInventory(struct book* header, FILE *output){
    struct book *current = header;
    while(current != 0){
        for(int i = 0; i < current->numCopies; i++){
            fprintf(output, "%s \"%s\" %s, %s\n", current->isbn, current->title, current->last, current->first);
        }
        current = current->next;
    }
}

// Book Search function - Accepts two arguments, the former being a
// struct book representing the head of the linked list of books, the
// latter being a character array representing the ISBN of the book
// that is being searched.
struct book* bookSearch(struct book *header, char targetisbn[]){
    struct book *current = header;
    while(current != 0){
        if(strcmp(current->isbn, targetisbn) == 0){
            return current;
        }
        current = current->next;
    }
    return 0;
}

// Partial Search function - Accepts four arguments,  a struct book
// representing the head of the linked list of books, a character array 
// representing the partial ISBN of the book that is being searched, a
// struct book array to store each book with a matching prefix and an integer
// representing the number of matches the array has room for.  This function
// returns an integer representing the number of prefix matches in the inventory.
// The array of matches ends with a null terminator.
int partialSearch(struct book *header, char targetisbn[], struct book* matches[], int lim){
    struct book *current = header;
    int i = 0;
    int stdmatches = 0;
    while(current != 0){
        if(strncmp(current->isbn, targetisbn, strlen(targetisbn)) == 0){
            if(i < lim - 1){
                matches[i] = current;
                i++;
            }
            stdmatches++;
        }
        current = current->next;
    }
    matches[i] = 0;
    return stdmatches;
}

// Book Compare Title function - Accepts two struct book arguments
// and compares the title, last name and first name.  This function
// is not called until after we know that both books have the same ISBN.
//
// Compares book1 to book2; if book1 < book2, return -1; if book1 > book2,
// return 1, otherwise, return 0
int bookCompareTitle(struct book *book1, struct book *book2){
    int val = strcmp(book1->title, book2->title); // val is equal to what is returned by the call to
    // String Compare, +/- 1 or 0
    if(val != 0){
        return val;
    }
    val = strcmp(book1->last, book2->last); //Comparing Authors' Last Names
    if(val != 0){
        return val;
    }
    val = strcmp(book1->first, book2->first); //Comparing Authors' First Names
    return val;
}

// Book Compare Last Function - Accepts two struct book arguments and compares
// author's last name, first name and title.
int bookCompareLast(struct book *book1, struct book *book2){
    int val = strcmp(book1->last, book2->last); // val is equal to what is returned by the call to
    // String Compare, +/- 1 or 0
    if(val != 0){
        return val;
    }
    val = strcmp(book1->first, book2->first); //Comparing Authors' First Names
    if(val != 0){
        return val;
    }
    val = strcmp(book1->title, book2->title); //Comparing Titles
    return val;
}

// Delete Function - Accepts two struct book pointer arguments, the header of the list and the node
// to be removed from the list. 
struct book* delete(struct book* header, struct book* target){
    struct book* current = header;
    // Special case for deleting the first node in the list.
    if(target == header){
        header = header->next;
        free(target);
        return header;
    }
    // Checking if current->next exists.  We are looking for a pointer to target.
    while(current->next != target && current->next){
        current = current->next;
    }
    // We are making sure we did not reach the end of the list.
    if(current->next == target){
        current->next = target->next;
        free(target);
        return header;
    }
    // ERROR: Target not found.
    return 0;
}