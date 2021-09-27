/*++

Copyright (c) 2004 - 2006, Intel Corporation. All rights reserved.<BR>
This program and the accompanying materials                          
are licensed and made available under the terms and conditions of the BSD License         
which accompanies this distribution.  The full text of the license may be found at        
http://opensource.org/licenses/bsd-license.php                                            
                                                                                          
THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,                     
WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.  


Module Name:

  LinkedList.c
  
Abstract: 

  Linked List Library Functions.

--*/

#include "BaseLibInternals.h"

/**
  Worker function that locates the Node in the List

  By searching the List, finds the location of the Node in List. At the same time,
  verifies the validity of this list.

  If List is NULL, then ASSERT().
  If List->ForwardLink is NULL, then ASSERT().
  If List->backLink is NULL, then ASSERT().
  If Node is NULL, then ASSERT();
  If PcdMaximumLinkedListLenth is not zero, and prior to insertion the number
  of nodes in ListHead, including the ListHead node, is greater than or
  equal to PcdMaximumLinkedListLength, then ASSERT().

  @param  List  A pointer to a node in a linked list.
  @param  Node  A pointer to one nod.

  @retval TRUE   Node is in List
  @retval FALSE  Node isn't in List, or List is invalid

**/
BOOLEAN
IsNodeInList (
  IN      CONST LIST_ENTRY      *List,
  IN      CONST LIST_ENTRY      *Node
  )
{
  UINTN                         Count;
  CONST LIST_ENTRY              *Ptr;
  BOOLEAN                       Found;

  //
  // Test the validity of List and Node
  //
  ASSERT (List != NULL);
  ASSERT (List->ForwardLink != NULL);
  ASSERT (List->BackLink != NULL);
  ASSERT (Node != NULL);

  Count = PcdGet32 (PcdMaximumLinkedListLength);

  Ptr = List;
  do {
    Ptr = Ptr->ForwardLink;
    Count--;
  } while ((Ptr != List) && (Ptr != Node) && (Count > 0));
  Found = (BOOLEAN)(Ptr == Node);

  if (PcdGet32 (PcdMaximumLinkedListLength) > 0) {
    while ((Count > 0) && (Ptr != List)) {
      Ptr = Ptr->ForwardLink;
      Count--;
    }
    ASSERT (Count > 0);
  }

  return Found;
}

/**
  Initializes the head node of a doubly linked list, and returns the pointer to
  the head node of the doubly linked list.

  Initializes the forward and backward links of a new linked list. After
  initializing a linked list with this function, the other linked list
  functions may be used to add and remove nodes from the linked list. It is up
  to the caller of this function to allocate the memory for ListHead.

  If ListHead is NULL, then ASSERT().

  @param  ListHead  A pointer to the head node of a new doubly linked list.

  @return ListHead

**/
LIST_ENTRY *
EFIAPI
GlueInitializeListHead (
  IN OUT  LIST_ENTRY            *List
  )

{
  ASSERT (List != NULL);

  List->ForwardLink = List;
  List->BackLink = List;
  return List;
}

/**
  Adds a node to the beginning of a doubly linked list, and returns the pointer
  to the head node of the doubly linked list.

  Adds the node Entry at the beginning of the doubly linked list denoted by
  ListHead, and returns ListHead.

  If ListHead is NULL, then ASSERT().
  If Entry is NULL, then ASSERT().
  If ListHead was not initialized with InitializeListHead(), then ASSERT().
  If PcdMaximumLinkedListLenth is not zero, and prior to insertion the number
  of nodes in ListHead, including the ListHead node, is greater than or
  equal to PcdMaximumLinkedListLength, then ASSERT().

  @param  ListHead  A pointer to the head node of a doubly linked list.
  @param  Entry     A pointer to a node that is to be inserted at the beginning
                    of a doubly linked list.

  @return ListHead

**/
LIST_ENTRY *
EFIAPI
GlueInsertHeadList (
  IN OUT  LIST_ENTRY            *List,
  IN OUT  LIST_ENTRY            *Entry
  )
{
  //
  // ASSERT List not too long and Entry is not one of the nodes of List
  //
  ASSERT (!IsNodeInList (List, Entry));

  Entry->ForwardLink = List->ForwardLink;
  Entry->BackLink = List;
  Entry->ForwardLink->BackLink = Entry;
  List->ForwardLink = Entry;
  return List;
}

/**
  Adds a node to the end of a doubly linked list, and returns the pointer to
  the head node of the doubly linked list.

  Adds the node Entry to the end of the doubly linked list denoted by ListHead,
  and returns ListHead.

  If ListHead is NULL, then ASSERT().
  If Entry is NULL, then ASSERT().
  If ListHead was not initialized with InitializeListHead(), then ASSERT().
  If PcdMaximumLinkedListLenth is not zero, and prior to insertion the number
  of nodes in ListHead, including the ListHead node, is greater than or
  equal to PcdMaximumLinkedListLength, then ASSERT().

  @param  ListHead  A pointer to the head node of a doubly linked list.
  @param  Entry     A pointer to a node that is to be added at the end of the
                    doubly linked list.

  @return ListHead

**/
LIST_ENTRY *
EFIAPI
GlueInsertTailList (
  IN OUT  LIST_ENTRY            *List,
  IN OUT  LIST_ENTRY            *Entry
  )
{
  //
  // ASSERT List not too long and Entry is not one of the nodes of List
  //
  ASSERT (!IsNodeInList (List, Entry));

  Entry->ForwardLink = List;
  Entry->BackLink = List->BackLink;
  Entry->BackLink->ForwardLink = Entry;
  List->BackLink = Entry;
  return List;
}

/**
  Retrieves the first node of a doubly linked list.

  Returns the first node of a doubly linked list. List must have been
  initialized with InitializeListHead(). If List is empty, then NULL is
  returned.

  If List is NULL, then ASSERT().
  If List was not initialized with InitializeListHead(), then ASSERT().
  If PcdMaximumLinkedListLenth is not zero, and the number of nodes
  in List, including the List node, is greater than or equal to
  PcdMaximumLinkedListLength, then ASSERT().

  @param  List  A pointer to the head node of a doubly linked list.

  @return The first node of a doubly linked list.
  @retval NULL  The list is empty.

**/
LIST_ENTRY *
EFIAPI
GlueGetFirstNode (
  IN CONST LIST_ENTRY  *List
  )
{
  //
  // ASSERT List not too long
  //
  ASSERT (IsNodeInList (List, List));

  return List->ForwardLink;
}

/**
  Retrieves the next node of a doubly linked list.

  Returns the node of a doubly linked list that follows Node. List must have
  been initialized with InitializeListHead(). If List is empty, then List is
  returned.

  If List is NULL, then ASSERT().
  If Node is NULL, then ASSERT().
  If List was not initialized with InitializeListHead(), then ASSERT().
  If PcdMaximumLinkedListLenth is not zero, and List contains more than
  PcdMaximumLinkedListLenth nodes, then ASSERT().
  If Node is not a node in List, then ASSERT().

  @param  List  A pointer to the head node of a doubly linked list.
  @param  Node  A pointer to a node in the doubly linked list.

  @return Pointer to the next node if one exists. Otherwise a null value which
          is actually List is returned.

**/
LIST_ENTRY *
EFIAPI
GlueGetNextNode (
  IN CONST LIST_ENTRY  *List,
  IN CONST LIST_ENTRY  *Node
  )
{
  //
  // ASSERT List not too long and Node is one of the nodes of List
  //
  ASSERT (IsNodeInList (List, Node));

  return Node->ForwardLink;
}

/**
  Checks to see if a doubly linked list is empty or not.

  Checks to see if the doubly linked list is empty. If the linked list contains
  zero nodes, this function returns TRUE. Otherwise, it returns FALSE.

  If ListHead is NULL, then ASSERT().
  If ListHead was not initialized with InitializeListHead(), then ASSERT().
  If PcdMaximumLinkedListLenth is not zero, and the number of nodes
  in List, including the List node, is greater than or equal to
  PcdMaximumLinkedListLength, then ASSERT().

  @param  ListHead  A pointer to the head node of a doubly linked list.

  @retval TRUE  The linked list is empty.
  @retval FALSE The linked list is not empty.

**/
BOOLEAN
EFIAPI
GlueIsListEmpty (
  IN      CONST LIST_ENTRY      *List
  )
{
  //
  // ASSERT List not too long
  //
  ASSERT (IsNodeInList (List, List));

  return (BOOLEAN)(List->ForwardLink == List);
}

/**
  Determines if a node in a doubly linked list is null.

  Returns FALSE if Node is one of the nodes in the doubly linked list specified
  by List. Otherwise, TRUE is returned. List must have been initialized with
  InitializeListHead().

  If List is NULL, then ASSERT().
  If Node is NULL, then ASSERT().
  If List was not initialized with InitializeListHead(), then ASSERT().
  If PcdMaximumLinkedListLenth is not zero, and the number of nodes
  in List, including the List node, is greater than or equal to
  PcdMaximumLinkedListLength, then ASSERT().
  If Node is not a node in List and Node is not equal to List, then ASSERT().

  @param  List  A pointer to the head node of a doubly linked list.
  @param  Node  A pointer to a node in the doubly linked list.

  @retval TRUE  Node is one of the nodes in the doubly linked list.
  @retval FALSE Node is not one of the nodes in the doubly linked list.

**/
BOOLEAN
EFIAPI
GlueIsNull (
  IN      CONST LIST_ENTRY      *List,
  IN      CONST LIST_ENTRY      *Node
  )
{
  //
  // ASSERT List not too long and Node is one of the nodes of List
  //
  ASSERT (IsNodeInList (List, Node));

  return (BOOLEAN)(Node == List);
}

/**
  Determines if a node the last node in a doubly linked list.

  Returns TRUE if Node is the last node in the doubly linked list specified by
  List. Otherwise, FALSE is returned. List must have been initialized with
  InitializeListHead().

  If List is NULL, then ASSERT().
  If Node is NULL, then ASSERT().
  If List was not initialized with InitializeListHead(), then ASSERT().
  If PcdMaximumLinkedListLenth is not zero, and the number of nodes
  in List, including the List node, is greater than or equal to
  PcdMaximumLinkedListLength, then ASSERT().
  If Node is not a node in List, then ASSERT().

  @param  List  A pointer to the head node of a doubly linked list.
  @param  Node  A pointer to a node in the doubly linked list.

  @retval TRUE  Node is the last node in the linked list.
  @retval FALSE Node is not the last node in the linked list.

**/
BOOLEAN
EFIAPI
GlueIsNodeAtEnd (
  IN      CONST LIST_ENTRY      *List,
  IN      CONST LIST_ENTRY      *Node
  )
{
  //
  // ASSERT List not too long and Node is one of the nodes of List
  //
  ASSERT (IsNodeInList (List, Node));

  return (BOOLEAN)(!IsNull (List, Node) && List->BackLink == Node);
}

/**
  Swaps the location of two nodes in a doubly linked list, and returns the
  first node after the swap.

  If FirstEntry is identical to SecondEntry, then SecondEntry is returned.
  Otherwise, the location of the FirstEntry node is swapped with the location
  of the SecondEntry node in a doubly linked list. SecondEntry must be in the
  same double linked list as FirstEntry and that double linked list must have
  been initialized with InitializeListHead(). SecondEntry is returned after the
  nodes are swapped.

  If FirstEntry is NULL, then ASSERT().
  If SecondEntry is NULL, then ASSERT().
  If SecondEntry and FirstEntry are not in the same linked list, then ASSERT().
  If PcdMaximumLinkedListLength is not zero, and the number of nodes in the
  linked list containing the FirstEntry and SecondEntry nodes, including
  the FirstEntry and SecondEntry nodes, is greater than or equal to
  PcdMaximumLinkedListLength, then ASSERT().

  @param  FirstEntry  A pointer to a node in a linked list.
  @param  SecondEntry A pointer to another node in the same linked list.

**/
LIST_ENTRY *
EFIAPI
GlueSwapListEntries (
  IN OUT  LIST_ENTRY            *FirstEntry,
  IN OUT  LIST_ENTRY            *SecondEntry
  )
{
  LIST_ENTRY                    *Ptr;

  if (FirstEntry == SecondEntry) {
    return SecondEntry;
  }

  //
  // ASSERT Entry1 and Entry2 are in the same linked list
  //
  ASSERT (IsNodeInList (FirstEntry, SecondEntry));

  //
  // Ptr is the node pointed to by FirstEntry->ForwardLink
  //
  Ptr = RemoveEntryList (FirstEntry);

  //
  // If FirstEntry immediately follows SecondEntry, FirstEntry willl be placed
  // immediately in front of SecondEntry
  //
  if (Ptr->BackLink == SecondEntry) {
    return InsertTailList (SecondEntry, FirstEntry);
  }

  //
  // Ptr == SecondEntry means SecondEntry immediately follows FirstEntry,
  // then there are no further steps necessary
  //
  if (Ptr == InsertHeadList (SecondEntry, FirstEntry)) {
    return Ptr;
  }

  //
  // Move SecondEntry to the front of Ptr
  //
  RemoveEntryList (SecondEntry);
  InsertTailList (Ptr, SecondEntry);
  return SecondEntry;
}

/**
  Removes a node from a doubly linked list, and returns the node that follows
  the removed node.

  Removes the node Entry from a doubly linked list. It is up to the caller of
  this function to release the memory used by this node if that is required. On
  exit, the node following Entry in the doubly linked list is returned. If
  Entry is the only node in the linked list, then the head node of the linked
  list is returned.

  If Entry is NULL, then ASSERT().
  If Entry is the head node of an empty list, then ASSERT().
  If PcdMaximumLinkedListLength is not zero, and the number of nodes in the
  linked list containing Entry, including the Entry node, is greater than
  or equal to PcdMaximumLinkedListLength, then ASSERT().

  @param  Entry A pointer to a node in a linked list

  @return Entry

**/
LIST_ENTRY *
EFIAPI
GlueRemoveEntryList (
  IN      CONST LIST_ENTRY      *Entry
  )
{
  ASSERT (!IsListEmpty (Entry));

  Entry->ForwardLink->BackLink = Entry->BackLink;
  Entry->BackLink->ForwardLink = Entry->ForwardLink;
  return Entry->ForwardLink;
}
