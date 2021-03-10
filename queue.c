#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "harness.h"
#include "queue.h"

/*
 * Create empty queue.
 * Return NULL if could not allocate space.
 */
queue_t *q_new()
{
    queue_t *q = malloc(sizeof(queue_t));
    if (q == NULL)
        return NULL;
    q->head = NULL;
    q->tail = NULL;
    q->size = 0;
    return q;
}

/* Free all storage used by queue */
void q_free(queue_t *q)
{
    if (q == NULL)
        return;
    while (q->head != NULL) {
        list_ele_t *tmp = q->head;
        q->head = q->head->next;
        free(tmp->value);
        free(tmp);
    }
    free(q);
}

/*
 * Attempt to insert element at head of queue.
 * Return true if successful.
 * Return false if q is NULL or could not allocate space.
 * Argument s points to the string to be stored.
 * The function must explicitly allocate space and copy the string into it.
 */

bool q_insert_head(queue_t *q, char *s)
{
    if (q == NULL)
        return false;
    list_ele_t *newh;
    newh = malloc(sizeof(list_ele_t));
    if (newh == NULL)
        return false;

    newh->value = malloc(sizeof(char) * (strlen(s) + 1));
    if (newh->value == NULL) {
        free(newh);
        return false;
    }
    memset(newh->value, '\0', (strlen(s) + 1));
    strncpy(newh->value, s, strlen(s));

    newh->next = q->head;
    q->head = newh;
    if (q->size == 0)
        q->tail = newh;

    q->size++;
    return true;
}

/*
 * Attempt to insert element at tail of queue.
 * Return true if successful.
 * Return false if q is NULL or could not allocate space.
 * Argument s points to the string to be stored.
 * The function must explicitly allocate space and copy the string into it.
 */
bool q_insert_tail(queue_t *q, char *s)
{
    if (q == NULL)
        return false;

    list_ele_t *newt;
    newt = malloc(sizeof(list_ele_t));
    if (newt == NULL)
        return false;

    newt->value = malloc(sizeof(char) * (strlen(s) + 1));
    if (newt->value == NULL) {
        free(newt);
        return false;
    }
    memset(newt->value, '\0', (strlen(s) + 1));
    strncpy(newt->value, s, strlen(s));

    newt->next = NULL;
    if (q->head == NULL) {
        q->head = newt;
    } else {
        q->tail->next = newt;
    }
    q->tail = newt;
    q->size++;
    return true;
}

/*
 * Attempt to remove element from head of queue.
 * Return true if successful.
 * Return false if queue is NULL or empty.
 * If sp is non-NULL and an element is removed, copy the removed string to *sp
 * (up to a maximum of bufsize-1 characters, plus a null terminator.)
 * The space used by the list element and the string should be freed.
 */
bool q_remove_head(queue_t *q, char *sp, size_t bufsize)
{
    if (q == NULL || q->head == NULL || sp == NULL)
        return false;
    memset(sp, '\0', bufsize);
    strncpy(sp, q->head->value, bufsize - 1);

    list_ele_t *tmp = q->head;
    q->head = q->head->next;
    free(tmp->value);
    free(tmp);


    q->size--;
    return true;
}

int q_size(queue_t *q)
{
    if (q == NULL || q->head == NULL)
        return 0;
    return q->size;
}

/*
 * Reverse elements in queue
 * No effect if q is NULL or empty
 * This function should not allocate or free any list elements
 * (e.g., by calling q_insert_head, q_insert_tail, or q_remove_head).
 * It should rearrange the existing ones.
 */
void q_reverse(queue_t *q)
{
    if (q == NULL || q->head == NULL)
        return;

    q->tail = q->head;
    list_ele_t *cur = q->head;
    list_ele_t *next = NULL;
    list_ele_t *prev = NULL;

    while (cur != NULL) {
        next = cur->next;
        cur->next = prev;
        prev = cur;
        cur = next;
    }
    q->head = prev;
}

/*
 * Sort elements of queue in ascending order
 * No effect if q is NULL or empty. In addition, if q has only one
 * element, do nothing.
 */

/* pull off the front node of the source and put it in dest */
void move_node(list_ele_t **destRef, list_ele_t **sourceRef)
{
    list_ele_t *newNode = *sourceRef;

    *sourceRef = newNode->next;

    newNode->next = *destRef;

    *destRef = newNode;
}

list_ele_t *sortedmerge(list_ele_t *a, list_ele_t *b)
{
    list_ele_t tmp;
    list_ele_t *tail = &tmp;

    tmp.next = NULL;

    while (1) {
        if (a == NULL) {
            tail->next = b;
            break;
        } else if (b == NULL) {
            tail->next = a;
            break;
        }
        if (strcmp(a->value, b->value) < 0) {
            move_node(&(tail->next), &a);
        } else {
            move_node(&(tail->next), &b);
        }

        tail = tail->next;
    }

    return tmp.next;
}

void frontbacksplit(list_ele_t *head, list_ele_t **front, list_ele_t **back)
{
    if (head == NULL || head->next == NULL) {
        *front = head;
        *back = NULL;
        return;
    }

    list_ele_t *slow = head;
    list_ele_t *fast = head->next;

    while (fast != NULL) {
        fast = fast->next;
        if (fast != NULL) {
            slow = slow->next;
            fast = fast->next;
        }
    }

    *front = head;
    *back = slow->next;
    slow->next = NULL;
}

void merge_sort(list_ele_t **head)
{
    if (*head == NULL || (*head)->next == NULL) {
        return;
    }

    list_ele_t *a;
    list_ele_t *b;

    frontbacksplit(*head, &a, &b);

    merge_sort(&a);
    merge_sort(&b);

    *head = sortedmerge(a, b);
}

void q_sort(queue_t *q)
{
    if (q == NULL)
        return;
    if (q->head == NULL)
        return;
    merge_sort(&q->head);

    list_ele_t *tmp = q->head;
    while (tmp->next != NULL)
        tmp = tmp->next;

    q->tail = tmp;
}
