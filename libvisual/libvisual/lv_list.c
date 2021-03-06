/* Libvisual - The audio visualisation framework.
 * 
 * Copyright (C) 2004, 2005, 2006 Dennis Smit <ds@nerds-incorporated.org>
 *
 * List implementation from RCL.
 * Copyright (C) 2002, 2003, 2004
 *				Dennis Smit <ds@nerds-incorporated.org>,
 *				Sepp Wijnands <mrrazz@nerds-incorporated.org>,
 *				Tom Wimmenhove <nohup@nerds-incorporated.org>
 *
 * Authors: Dennis Smit <ds@nerds-incorporated.org>
 *	    Sepp Wijnands <mrrazz@nerds-incorporated.org>,
 *	    Tom Wimmenhove <nohup@nerds-incorporated.org>
 *
 * $Id: lv_list.c,v 1.30 2006/01/22 13:23:37 synap Exp $
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as
 * published by the Free Software Foundation; either version 2.1
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 */

#include "config.h"
#include "lv_list.h"
#include "lv_common.h"

#define LIST_ITERCONTEXT(obj)				(VISUAL_CHECK_CAST ((obj), ListIterContext))


typedef struct _ListIterContext ListIterContext;

struct _ListIterContext {
	VisObject	*object;

	VisListEntry	*cur;
};


static int list_destroy (VisCollection *collection);
static int list_size (VisCollection *collection);
static VisCollectionIter *list_iter (VisCollection *collection);

static void list_iter_assign (VisCollectionIter *iter, VisCollection *collection, VisObject *itercontext, int index);
static int list_iter_has_more (VisCollectionIter *iter, VisCollection *collection, VisObject *itercontext);
static void list_iter_next (VisCollectionIter *iter, VisCollection *collection, VisObject *itercontext);
static void *list_iter_getdata (VisCollectionIter *iter, VisCollection *collection, VisObject *itercontext);


static int list_destroy (VisCollection *collection)
{
	VisCollectionDestroyerFunc destroyer;
	VisList *list = VISUAL_LIST (collection);
	VisListEntry *le = NULL;
	void *elem;

	visual_return_val_if_fail (list != NULL, -VISUAL_ERROR_COLLECTION_NULL);

	destroyer = visual_collection_get_destroyer (collection);

	/* Walk through the given list, possibly calling the destroyer for it */
	if (destroyer == NULL) {
		while ((elem = visual_list_next (list, &le)) != NULL)
			visual_list_delete (list, &le);
	} else {
		while ((elem = visual_list_next (list, &le)) != NULL) {
			destroyer (elem);
			visual_list_delete (list, &le);
		}
	}

	return VISUAL_OK;
}

static int list_size (VisCollection *collection)
{
	VisList *list = VISUAL_LIST (collection);

	visual_return_val_if_fail (list != NULL, -VISUAL_ERROR_COLLECTION_NULL);

	return list->count;
}

static VisCollectionIter *list_iter (VisCollection *collection)
{
	VisCollectionIter *iter;
	ListIterContext *context;
	VisList *list = VISUAL_LIST (collection);

	context = visual_mem_new0 (ListIterContext, 1);

	/* Do the VisObject initialization for the ListIterContext */
	visual_object_initialize (VISUAL_OBJECT (context), TRUE, NULL);
	context->cur = list->head;

	iter = visual_collection_iter_new (list_iter_assign, list_iter_next, list_iter_has_more,
			list_iter_getdata, collection, VISUAL_OBJECT (context));

	return iter;
}

static void list_iter_assign (VisCollectionIter *iter, VisCollection *collection, VisObject *itercontext, int index)
{
	ListIterContext *context = LIST_ITERCONTEXT (itercontext);
	VisList *list = VISUAL_LIST (collection);
	int i;

	context->cur = list->head;

	if (context->cur == NULL)
		return;

	for (i = 0; i < index; i++) {
		context->cur = context->cur->next;

		if (context->cur == NULL)
			return;
	}
}

static void list_iter_next (VisCollectionIter *iter, VisCollection *collection, VisObject *itercontext)
{
	ListIterContext *context = LIST_ITERCONTEXT (itercontext);
	VisListEntry *le = context->cur;

	if (le == NULL)
		return;

	context->cur = le->next;
}

static int list_iter_has_more (VisCollectionIter *iter, VisCollection *collection, VisObject *itercontext)
{
	ListIterContext *context = LIST_ITERCONTEXT (itercontext);

	if (context->cur == NULL)
		return FALSE;

	return TRUE;
}

static void *list_iter_getdata (VisCollectionIter *iter, VisCollection *collection, VisObject *itercontext)
{
	ListIterContext *context = LIST_ITERCONTEXT (itercontext);
	VisListEntry *le = context->cur;

	if (le == NULL)
		return NULL;

	return le->data;
}

VisList *visual_list_new (VisCollectionDestroyerFunc destroyer)
{
	VisList *list;

	list = visual_mem_new0 (VisList, 1);

	visual_list_init (list, destroyer);

	/* do the visobject initialization */
	visual_object_set_allocated (VISUAL_OBJECT (list), TRUE);
	visual_object_ref (VISUAL_OBJECT (list));

	return list;
}

int visual_list_count(VisList *list)
{
	visual_return_val_if_fail (list != NULL, -VISUAL_ERROR_COLLECTION_NULL);
	return list->count;
}

int visual_list_init (VisList *list, VisCollectionDestroyerFunc destroyer)
{
	visual_return_val_if_fail (list != NULL, -VISUAL_ERROR_LIST_NULL);

	/* Do the VisObject initialization */
	visual_object_clear (VISUAL_OBJECT (list));
	visual_object_set_dtor (VISUAL_OBJECT (list), visual_collection_dtor);
	visual_object_set_allocated (VISUAL_OBJECT (list), FALSE);

	/* Set the VisCollection data */
	visual_collection_set_destroyer (VISUAL_COLLECTION (list), destroyer);
	visual_collection_set_destroy_func (VISUAL_COLLECTION (list), list_destroy);
	visual_collection_set_size_func (VISUAL_COLLECTION (list), list_size);
	visual_collection_set_iter_func (VISUAL_COLLECTION (list), list_iter);

	/* Set the VisList data */
	list->head = NULL;
	list->tail = NULL;
	list->count = 0;

	return VISUAL_OK;
}

void *visual_list_next (VisList *list, VisListEntry **le)
{
	visual_return_val_if_fail (list != NULL, NULL);
	visual_return_val_if_fail (le != NULL, NULL);

	if (*le == NULL)
		*le = list->head;
	else
		*le = (*le)->next;

	if (*le != NULL)
		return (*le)->data;

	return NULL;
}

void *visual_list_prev (VisList *list, VisListEntry **le)
{
	visual_return_val_if_fail (list != NULL, NULL);
	visual_return_val_if_fail (le != NULL, NULL);

	if (!*le)
		*le = list->tail;
	else
		*le = (*le)->prev;

	if (*le)
		return (*le)->data;

	return NULL;
}

void *visual_list_get (VisList *list, int index)
{
	VisListEntry *le = NULL;
	void *data = NULL;
	int i, lc;

	visual_return_val_if_fail (list != NULL, NULL);
	visual_return_val_if_fail (index >= 0, NULL);

	lc = visual_collection_size (VISUAL_COLLECTION (list));

	if (lc - 1 < index)
		return NULL;

	for (i = 0; i <= index; i++) {
		data = visual_list_next (list, &le);

		if (data == NULL)
			return NULL;
	}

	return data;
}

int visual_list_add_at_begin (VisList *list, void *data)
{
	VisListEntry *le;

	visual_return_val_if_fail (list != NULL, -VISUAL_ERROR_LIST_NULL);

	/* Allocate memory for new list entry */
	le = visual_mem_new0 (VisListEntry, 1);

	/* Assign data element */
	le->data = data;

	visual_list_chain_at_begin (list, le);

	return VISUAL_OK;
}

int visual_list_add (VisList *list, void *data)
{
	VisListEntry *le;

	visual_return_val_if_fail (list != NULL, -VISUAL_ERROR_LIST_NULL);

	le = visual_mem_new0 (VisListEntry, 1);

	/* Assign data element */
	le->data = data;

	visual_list_chain (list, le);

	return VISUAL_OK;
}

int visual_list_chain_at_begin (VisList *list, VisListEntry *le)
{
	VisListEntry *next;

	visual_return_val_if_fail (list != NULL, -VISUAL_ERROR_LIST_NULL);
	visual_return_val_if_fail (le != NULL, -VISUAL_ERROR_LIST_ENTRY_NULL);

	if (list->head == NULL) {
		list->head = le;
		list->tail = le;

		le->prev = NULL;
		le->next = NULL;
	} else {
		next = list->head;

		le->next = next;
		list->head = le;

		le->prev = NULL;
	}

	/* Done */
	list->count++;

	return VISUAL_OK;
}

int visual_list_chain (VisList *list, VisListEntry *le)
{
	VisListEntry *prev;

	visual_return_val_if_fail (list != NULL, -VISUAL_ERROR_LIST_NULL);
	visual_return_val_if_fail (le != NULL, -VISUAL_ERROR_LIST_ENTRY_NULL);

	/* Add list entry to list */
	/* Is this the first entry for this list ? */
	if (list->head == NULL) {
		list->head = le;
		list->tail = le;

		le->prev = NULL;
		le->next = NULL;
	} else {
		/* Nope, add to tail of this list */
		prev = list->tail;

		/* Exchange pointers */
		prev->next = le;
		le->prev = prev;

		le->next = NULL;

		/* Point tail to new entry */
		list->tail = le;
	}

	/* Done */
	list->count++;

	return VISUAL_OK;
}

int visual_list_unchain (VisList *list, VisListEntry *le)
{
	VisListEntry *prev;
	VisListEntry *next;

	visual_return_val_if_fail (list != NULL, -VISUAL_ERROR_LIST_NULL);
	visual_return_val_if_fail (le != NULL, -VISUAL_ERROR_LIST_ENTRY_NULL);

	/* Point new to le's previous entry */
	prev = le->prev;
	next = le->next;

	/* Does it have a previous entry ? */
	if (prev != NULL)
		prev->next = next;
	else
		list->head = next;

	if (next != NULL) /* It does have a next entry ? */
		next->prev = prev;
	else
		list->tail = prev;

	list->count--;

	return VISUAL_OK;
}

int visual_list_insert (VisList *list, VisListEntry **le, void *data)
{
	VisListEntry *prev, *next, *current;

	visual_return_val_if_fail (list != NULL, -VISUAL_ERROR_LIST_NULL);
	visual_return_val_if_fail (le != NULL, -VISUAL_ERROR_LIST_ENTRY_NULL);
	visual_return_val_if_fail (data != NULL, -VISUAL_ERROR_NULL);

	current = visual_mem_new0 (VisListEntry, 1);

	/* Assign data element */
	current->data = data;

	/* Add entry to list */
	if (list->head == NULL && *le == NULL) {
		/* First entry */
		list->head = current;
		list->tail = current;
	} else if (*le == NULL) {
		/* Insert entry at first position */
		next = list->head;
		/* Exchange pointers */
		current->next = next;
		next->prev = current;
		/* Point head to current pointer */
		list->head = current;
	} else {
		/* Insert entry at *le's position */
		prev = *le;
		next = prev->next;

		current->prev = prev;
		current->next = next;

		prev->next = current;
		if (next != NULL)
			next->prev = current;
		else
			list->tail = current;
	}

	/* Hop to new entry */
	*le = current;

	/* Done */
	list->count++;

	return VISUAL_OK;
}

int visual_list_delete (VisList *list, VisListEntry **le)
{
	VisListEntry *next;

	visual_return_val_if_fail (list != NULL, -VISUAL_ERROR_LIST_NULL);
	visual_return_val_if_fail (le != NULL, -VISUAL_ERROR_LIST_ENTRY_NULL);

	/* Valid list entry ? */
	if (*le == NULL) {
		visual_log (VISUAL_LOG_ERROR, "There is no list entry to delete");

		return -VISUAL_ERROR_LIST_ENTRY_INVALID; /* Nope */
	}

	next = (*le)->next;
	visual_list_unchain (list, *le);

	visual_mem_free (*le);

	*le = next;

	return VISUAL_OK;
}

int visual_list_destroy (VisList *list, VisListEntry **le)
{
	VisCollectionDestroyerFunc destroyer;

	visual_return_val_if_fail (list != NULL, -VISUAL_ERROR_LIST_NULL);
	visual_return_val_if_fail (le != NULL, -VISUAL_ERROR_LIST_ENTRY_NULL);

	destroyer = visual_collection_get_destroyer (VISUAL_COLLECTION (list));

	if (destroyer != NULL)
		destroyer ((*le)->data);

	return visual_list_delete (list, le);
}
