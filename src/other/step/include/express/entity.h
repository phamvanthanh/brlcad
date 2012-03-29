#ifndef ENTITY_H
#define ENTITY_H

/* $Id: entity.h,v 1.12 1997/01/21 19:17:11 dar Exp $ */

/************************************************************************
** Module:  Entity
** Description: This module represents Express entity definitions.  An
**  entity definition consists of a name, a list of attributes, a
**  collection of uniqueness sets, a specification of the entity's
**  position in a class hierarchy (i.e., sub- and supertypes), and
**  a list of constraints which must be satisfied by instances of
**  the entity.  A uniquess set is a set of one or more attributes
**  which, when taken together, uniquely identify a specific instance
**  of the entity.  Thus, the uniqueness set { name, ssn } indicates
**  that no two instances of the entity may share both the same
**  values for both name and ssn.
** Constants:
**  ENTITY_NULL - the null entity
**
************************************************************************/

/*
 * This software was developed by U.S. Government employees as part of
 * their official duties and is not subject to copyright.
 *
 * $Log: entity.h,v $
 * Revision 1.12  1997/01/21 19:17:11  dar
 * made C++ compatible
 *
 * Revision 1.11  1994/11/10  19:20:03  clark
 * Update to IS
 *
 * Revision 1.10  1994/05/11  19:51:05  libes
 * numerous fixes
 *
 * Revision 1.9  1993/10/15  18:48:24  libes
 * CADDETC certified
 *
 * Revision 1.7  1993/02/16  03:14:47  libes
 * simplified find calls
 *
 * Revision 1.6  1993/01/19  22:45:07  libes
 * *** empty log message ***
 *
 * Revision 1.5  1992/08/18  17:12:41  libes
 * rm'd extraneous error messages
 *
 * Revision 1.4  1992/06/08  18:06:24  libes
 * prettied up interface to print_objects_when_running
 *
 */

/*************/
/* constants */
/*************/

#define ENTITY_NULL             (Entity)0
#define ENTITY_INHERITANCE_UNINITIALIZED    -1

/*****************/
/* packages used */
/*****************/

#include "expbasic.h"   /* get basic definitions */
#include "symbol.h"
#include "scope.h"

/************/
/* typedefs */
/************/

typedef struct Scope_ * Entity;

/****************/
/* modules used */
/****************/

#include "expr.h"
#include "variable.h"
#include "schema.h"

/***************************/
/* hidden type definitions */
/***************************/

struct Entity_ {
    Linked_List supertype_symbols; /* linked list of original symbols*/
    /* as generated by parser */
    Linked_List supertypes; /* linked list of supertypes (as entities) */
    Linked_List subtypes;   /* simple list of subtypes */
    /* useful for simple lookups */
    Expression  subtype_expression; /* DAG of subtypes, with complete */
    /* information including, OR, AND, and ONEOF */
    Linked_List attributes; /* explicit attributes */
    int     inheritance;    /* total number of attributes */
    /* inherited from supertypes */
    int     attribute_count;
    Linked_List unique; /* list of identifiers that are unique */
    Linked_List instances;  /* hook for applications */
    int     mark;   /* usual hack - prevent traversing sub/super */
    /* graph twice */
    bool     abstract;/* is this an abstract supertype? */
    Type        type;   /* type pointing back to ourself */
    /* Useful to have when evaluating */
    /* expressions involving entities */
};

/********************/
/* global variables */
/********************/

extern struct freelist_head ENTITY_fl;
extern int ENTITY_MARK;

/******************************/
/* macro function definitions */
/******************************/

#define ENTITY_new()        (struct Entity_ *)MEM_new(&ENTITY_fl)
#define ENTITY_destroy(x)   MEM_destroy(&ENTITY_fl,(Freelist *)(char *)x)

#define ENTITYget_symbol(e) SCOPEget_symbol(e)
/* returns a function (i.e., which can be passed to other functions) */
#define ENTITY_get_symbol   SCOPE_get_symbol

#define ENTITYget_attributes(e) ((e)->u.entity->attributes)
#define ENTITYget_subtypes(e)   ((e)->u.entity->subtypes)
#define ENTITYget_supertypes(e) ((e)->u.entity->supertypes)
#define ENTITYget_name(e)   ((e)->symbol.name)
#define ENTITYget_uniqueness_list(e) ((e)->u.entity->unique)
#define ENTITYget_where(e)  ((e)->where)
#define ENTITYput_where(e,w)    ((e)->where = (w))

#define ENTITYget_abstract(e)   ((e)->u.entity->abstract)
#define ENTITYget_mark(e)   ((e)->u.entity->mark)
#define ENTITYput_mark(e,m) ((e)->u.entity->mark = (m))
#define ENTITYput_inheritance_count(e,c) ((e)->u.entity->inheritance = (c))
#define ENTITYget_inheritance_count(e)  ((e)->u.entity->inheritance)
#define ENTITYget_size(e)   ((e)->u.entity->attribute_count + (e)->u.entity->inheritance)
#define ENTITYget_attribute_count(e)    ((e)->u.entity->attribute_count)
#define ENTITYput_attribute_count(e,c)  ((e)->u.entity->attribute_count = (c))

#define ENTITYget_clientData(e)     ((e)->clientData)
#define ENTITYput_clientData(e,d)       ((e)->clientData = (d))

/***********************/
/* function prototypes */
/***********************/

extern struct Scope_  * ENTITYcreate PROTO( ( struct Symbol_ * ) );
extern void     ENTITYinitialize PROTO( ( void ) );
extern void     ENTITYadd_attribute PROTO( ( struct Scope_ *, struct Variable_ * ) );
extern struct Scope_  * ENTITYcopy PROTO( ( struct Scope_ * ) );
extern Entity       ENTITYfind_inherited_entity PROTO( ( struct Scope_ *, char *, int ) );
extern Variable     ENTITYfind_inherited_attribute PROTO( ( struct Scope_ *, char *, struct Symbol_ ** ) );
extern Variable     ENTITYresolve_attr_ref PROTO( ( Entity, Symbol *, Symbol * ) );
extern bool      ENTITYhas_immediate_supertype PROTO( ( Entity, Entity ) );
extern Variable     ENTITYget_named_attribute PROTO( ( Entity, char * ) );
extern Linked_List  ENTITYget_all_attributes PROTO( ( Entity ) );
extern bool      ENTITYhas_supertype PROTO( ( Entity, Entity ) );
extern void     ENTITYadd_instance PROTO( ( Entity, Generic ) );
extern int      ENTITYget_initial_offset PROTO( ( Entity ) );
extern int      ENTITYdeclares_variable PROTO( ( Entity, struct Variable_ * ) );

#endif    /*  ENTITY_H  */
