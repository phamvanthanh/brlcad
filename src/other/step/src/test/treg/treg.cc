/*
 * treg.cc
 *
 * Ian Soboroff, NIST
 * June, 1994
 *
 * This test uses a registry, instance manager, and STEPfile.  The
 * registry keeps dictionary information on the schema, such as lists and
 * reference counts of defined types, entity types, etc.  The instance
 * manager is used to keep track of instantiated entities.  This program may
 * serve as a basic tutorial... for more information, see NISTIR 4937,
 * Validation Testing System: Reusable Software Component Design, by Morris,
 * Sauder, and Ressler.
 *
 * treg can take an optional argument, a filename.  If given, the STEPfile
 * is written to this filename at program completion, for importing by
 * another program.
 *
 */

/* A switch for tests.h, because we don't need to schema header file */
#define DONT_NEED_HEADER
#include "tests.h"


// This function, given a pointer to an entity, will populate it, that is,
// put data values in its attributes.  PopulateEntity doesn't care what
// attributes your entity has.  It goes through them, one at a time, checks
// their type, and puts an appropriate random value in.
void PopulateEntity( STEPentity * ent ) {
    int attrCount = ent->AttributeCount();
    cout << "Populating " << ent->EntityName() << " which has ";
    cout << attrCount << " attributes." << endl;

    ent->ResetAttributes();    // start us walking at the top of the list

    STEPattribute * attr = ent->NextAttribute();
    while( attr != 0 ) {
        const AttrDescriptor * attrDesc = attr->aDesc;
        cout << "  attribute " << attrDesc->Name();
        cout << " [" << attrDesc->TypeName() << "] = ";
        int needOutput = 1;  // true if we need to output the value
        // that is, if it's anything but 'none'

        // Here's how we do this... set up a string stream to put the value
        // into, because STEPattribute has this StrToVal() function to put
        // a string value as the value of the attribute.  Then, depending on
        // the type of the attribute, put something nearly appropriate in.
        ostringstream valstr;
        switch( attrDesc->NonRefType() ) {
            case INTEGER_TYPE: // for these types, just put in a random number
            case REAL_TYPE:    // from 0-99.
            case NUMBER_TYPE:
                cout << "(int/real/num) ";
                valstr << rand() % 100;
                break;
            case STRING_TYPE:  // for strings, put in the name of the entity,
                cout << "(string) ";    // followed by a number from 1 to 10.
                valstr << ent->EntityName() << rand() % 10 + 1;
                break;
            case ENUM_TYPE:    // for enumerations, put a random legal value.
            case BOOLEAN_TYPE: // the trick here is that the value needs to be
            case LOGICAL_TYPE: { // the word, not the int value, because of StrToVal
                cout << "(enum/bool/logi) ";
                STEPenumeration * se = attr->ptr.e; // grab the enumeration...
                valstr << se->element_at( rand() % se->no_elements() );
            }
            break;
            default:   // for other stuff like aggregates and selects, just leave
                cout << "none (" << attrDesc->NonRefType();  // 'em blank...
                cout << ")" << endl;
                needOutput = 0;
        }
        valstr << ends;  // flush and null-terminate the stream
        /*** char *val = valstr.str(); ***/  // fix stream into char* string
        char * val = &( valstr.str()[0] );
        if( needOutput ) {
            cout << val << endl;
        }
        attr->StrToVal( val ); // and assign

        attr = ent->NextAttribute();
    }
}


int main( int argc, char * argv[] ) {
    int using_outfile = 0;

    if( argc > 2 ) {
        cout << "Syntax:   treg [filename]" << endl;
        exit( 1 );
    } else if( argc > 1 ) {
        using_outfile = 1;    // output filename is in argc[1]
    }

    // This has to be done before anything else.  This initializes
    // all of the registry information for the schema you are using.
    // The SchemaInit() function is generated by fedex_plus... see
    // extern statement above.

    Registry * registry = new Registry( SchemaInit );

    // The nifty thing about the Registry is that it basically keeps a list
    // of everything in your schema.  What this means is that we can go
    // through the Registry and instantiate, say, one of everything, without
    // knowing at coding-time what entities there are to instantiate.  So,
    // this test could be linked with other class libraries produced from
    // other schema, rather than the example, and run happily.

    InstMgr instance_list;
    STEPfile * sfile = new STEPfile( *registry, instance_list );

    // The STEPfile is actually an object that manages the relationship
    // between what's instantiated in the instance manager, and how that
    // information gets passed to the outside, e.g., a file on disk.

    // Here's what's going to happen below: we're going to figure out
    // how many different entities there are, instantiate one of each and
    // keep an array of pointers to each.  We'll stick some random data in
    // them as we go.  When we're done, we'll print out everything by walking
    // the array, and then write out the STEPfile information to the screen.

    // Figure outhow many entities there are, then allocate an array
    // to store a pointer to one of each.

    int num_ents = registry->GetEntityCnt();
    STEPentity ** SEarray = new STEPentity*[num_ents];

    // "Reset" the Schema and Entity hash tables... this sets things up
    // so we can walk through the table using registry->NextEntity()

    registry->ResetSchemas();
    registry->ResetEntities();

    // Print out what schema we're running through.

    const SchemaDescriptor * schema = registry->NextSchema();
    cout << "Building entities in schema " << schema->Name() << endl;

    // "Loop" through the schema, building one of each entity type.

    const EntityDescriptor * ent;  // needs to be declared const...
    for( int i = 0; i < num_ents; i++ ) {
        ent = registry->NextEntity();
        cout << "  Building entity " << ent->Name() << endl;

        // Build object, using its name, through the registry
        SEarray[i] = registry->ObjCreate( ent->Name() );

        // Add each realized entity to the instance list
        instance_list.Append( SEarray[i], completeSE );

        // Put some data into each instance
        PopulateEntity( SEarray[i] );
    }

    // Print out all entities

    SEarrIterator SEitr( ( const STEPentity ** ) SEarray, num_ents );
    // the above cast is needed because the SEarrIterator
    // constructor takes a const entity array pointer argument

    cout << endl << "Here are the entities instantiated, via the SEarray:";
    cout << endl;
    for( SEitr = 0; !SEitr; ++SEitr ) {
        SEitr()->STEPwrite( cout );
    }

    cout << endl << "Here are all the entities via the STEPfile:" << endl;
    sfile->WriteExchangeFile( cout );

    if( using_outfile ) {
        cout << "\nWriting STEPfile to output file " << argv[1] << endl;
        ofstream step_out( argv[1] );
        sfile->WriteExchangeFile( step_out );
    }
    exit( 0 );
}

