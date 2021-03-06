/*
 * Copyright 2006 Sony Computer Entertainment Inc.
 *
 * Licensed under the SCEA Shared Source License, Version 1.0 (the "License"); you may not use this 
 * file except in compliance with the License. You may obtain a copy of the License at:
 * http://research.scea.com/scea_shared_source_license.html
 *
 * Unless required by applicable law or agreed to in writing, software distributed under the License 
 * is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or 
 * implied. See the License for the specific language governing permissions and limitations under the 
 * License. 
 */

#ifndef __DAE_META_ATTRIBUTE_H__
#define __DAE_META_ATTRIBUTE_H__

#include <dae/daeTypes.h>
#include <dae/daeStringRef.h>
#include <dae/daeAtomicType.h>
#include <dae/daeElement.h>
#include <dae/daeArray.h>

class daeElement;
class daeMetaElement;
class daeMetaAttribute;
class daeMetaElementAttribute;

typedef daeSmartRef<daeMetaElementAttribute> daeMetaElementAttributeRef;
typedef daeSmartRef<daeMetaElement> daeMetaElementRef;
typedef daeSmartRef<daeMetaAttribute> daeMetaAttributeRef;

typedef daeTArray<daeMetaAttributeRef> daeMetaAttributeRefArray;
typedef daeTArray<daeMetaAttribute*> daeMetaAttributePtrArray;
typedef daeTArray<daeMetaElementAttributeRef> daeMetaElementAttributeRefArray;

/**
 * The @c daeMetaAttribute class describes one attribute in a C++ COLLADA dom element.
 *
 * In the case of the C++ object model a conceptual attribute can be
 * either a dom attribute, a dom element, or a dom value.
 * Essentially, the meta attribute describes fields on the C++ class.
 * However these attributes are stored separately in the containing meta
 * @c daeMetaElement.
 * @c daeMetaAttributes always exist inside of @c daeMetaElements.
 * Each @c daeMetaAttribute has certain symantic operations it is capable of
 * including @c set(), @c get(), @c print(), and @c resolve().
 * @c daeMetaAttributes use the @c daeAtomicType system as their underlying semantic
 * implementation, but contain additional information about the packaging
 * of the atomic types into the C++ dom classes such as offset, and
 * array information.
 */
class daeMetaAttribute : public daeElement
{
protected:
	daeStringRef			_name;
	daeInt					_offset;
	daeAtomicType*			_type;
	daeMetaElement*			_container;
	daeString				_default;
	daeBool					_isValid;
	daeBool					_isRequired;

public:
	/**
	 * Constructor
	 */
	daeMetaAttribute();

	/**
	 * Destructor
	 */
	~daeMetaAttribute() {}
public:
	/** 
	 * Determines if the value of this attribute was ever set. 
	 * This will be the case if @c setDefault() was
	 * called or if the attribute was assigned a value by the input file.  If you set the value yourself,
	 * you need to call @c setIsValid() to set this flag.
	 * @return Returns true if the value in this attribute is valid.
	 */
	daeBool getIsValid() {return _isValid; }
	/**
	 * Sets the value that indicates if this attribute contains a valid value.  
	 * If you don't set this on an optional
	 * attribute, that attribute will not be written.
	 * @param isValid Indicates if the value in this attribute valid, true if it is, false if not.
	 */
	void setIsValid(daeBool isValid) {_isValid = isValid;}
	/** 
	 * Determines if the schema indicates that this is a required attribute.
	 * @return Returns true if this is a required attribute, false if not.
	 */
	daeBool getIsRequired() {return _isRequired; }
	/**
	 * Sets the value that indicates that this attribute is required by the schema.  If set, the attribute
	 * will always be exported by the API regardless of its value.
	 * @param isRequired Indicates if the schema says this attribute is required, true if it is, false if not.
	 */
	void setIsRequired(daeBool isRequired) {_isRequired = isRequired;}
	/**
	 * Sets the byte offset (from @c this) where this attribute's storage is
	 * found in its container element class.
	 * @param offset Integer byte offset from @c this pointer.
	 */
	void setOffset(daeInt offset) { _offset = offset; }

	/**
	 * Gets the byte offset (from @ this) where this attribute's storage is
	 * found in its container element class.
	 * @return Returns the integer byte offset from @c this pointer for this attribute.
	 */
	daeInt getOffset() { return _offset; }
	 
	/**
	 * Sets the name of the attribute.
	 * @param name @c daeString that is directly stored as a pointer
	 * without being copied.
	 */
	void setName(daeString name) { _name = name; }
	
	/**
	 * Gets the name of this attribute.
	 * @return Returnsthe name of this attribute.
	 */
	daeStringRef getName() { return _name; }

	/**
	 * Sets the type of the attribute.
	 * @param type @c daeAtomicType to use for interacting with this
	 * attribute in a containing @c daeElement.
	 */
	void setType(daeAtomicType* type) { _type = type; }
	
	/**
	 * Gets the @c daeAtomicType used by this attribute.
	 * @return Returns the @c daeAtomicType that this attribute uses for its
	 * implementation.
	 */
	daeAtomicType* getType() { return _type; }

	/**
	 * Sets the default for this attribute via a string.  The attribute's
	 * type is used to convert the string into a binary value
	 * inside of an element.
	 * @param defaultVal @c daeString representing the default value.
	 */
	void setDefault(daeString defaultVal) { _default = defaultVal; }

	/**
	 * Gets the default for this attribute via a string.  The attribute's
	 * type is used to convert the string into a binary value
	 * inside of an element.
	 * @return Returns a @c daeString representing the default value.
	 */
	daeString getDefault() { return _default; }

	/**
	 * Sets the containing @c daeMetaElement for this attribute.
	 * @param container Element on which this @c daeMetaAttribute belongs.
	 */
	void setContainer(daeMetaElement* container) { _container = container; }

	/**
	 * Gets the containing @c daeMetaElement for this attribute.
	 * @return Returns the @c daeMetaElement to which this @c daeAttribute belongs.
	 */
	daeMetaElement* getContainer() { return _container; }
	  
	/**
	 * Gets the number of particles associated with this attribute in instance <tt><i>e.</i></tt> 
	 * @param e Containing element to run the operation on.
	 * @return Returns the number of particles associated with this attribute
	 * in instance <tt><i>e.</i></tt> 
	 */
	virtual daeInt getCount(daeElement* e);

	/**
	 * Gets a particle from containing element <tt><i>e</i></tt> based on <tt><i>index.</i></tt> 
	 * @param e Containing element from which to get the element.
	 * @param index Index of the particle to retrieve if indeed
	 * there is an array of elements rather than a singleton.
	 * @return Returns the associated particle out of parent element e, based on index, if necessary.
	 */
	virtual daeMemoryRef get(daeElement* e, daeInt index);
	  
public: // STATIC MEMBERS
	/**
	 * Lists the type names that can be created by factories and indicates which _FactoryTemplates to use for each type.
	 */
	static daeStringRefArrayArray		_NameBindings;
	/**
	 * Points to the factory objects used to construct various types of attributes, _NameBindings specifies which type names are bound to which factories.
	 */
	static daeMetaAttributeRefArray		_FactoryTemplates;
	
public:	//STATIC INTERFACE
	/**
	 * Obsolete
	 */
	static daeMetaAttributeRef Factory(daeStringRef xmlTypeName);
	/**
	 * Obsolete
	 */
	static void InitializeKnownTypes();

public:
	/**
	 * Clones the @c daeMetaAttribute.
	 * @return Returns a duplicate of this @c daeMetaAttribute.
	 * @note Not Implemented.
	 */
	virtual daeMetaAttributeRef clone();

	/**
	 * Resolves a reference (if there is one) in the attribute type;
	 * only useful for reference types.
	 * @param elem Containing element on which this attribute
	 * should be resolved.
	 */
	virtual void resolve(daeElementRef elem);

	/**
	 * Gets the number of bytes for this attribute.
	 * @return Returns the number of bytes in the C++ COLLADA dom element for this
	 * attribute.
	 */
	virtual daeInt getSize();

	/**
	 * Gets the alignment in bytes on the class of this meta attribute type.
	 * @return Returns the alignment in bytes.
	 */
	virtual daeInt getAlignment();

	/**
	 * Sets the value of this attribute on <tt><i>element</i></tt> by converting string <tt><i>s</i></tt> 
	 * to a binary value and assigning it via the underlying @c daeAtomicType
	 * system.
	 * @param element Element on which to set this attribute.
	 * @param s String containing the value to be converted via the
	 * atomic type system.
	 */
	virtual void set(daeElement* element, daeString s);

	/**
	 * Copys the value of this attribute from fromElement into toElement.
	 * @param toElement Pointer to a @c daeElement to copy this attribute to.
	 * @param fromElement Pointer to a @c daeElement to copy this attribute from.
	 */
	virtual void copy(daeElement* toElement, daeElement* fromElement);
	
public:
	/**
	 * Gets the storage inside of <tt><i>e</i></tt> associated with
	 * this attribute.  It is very useful for performing generic processing
	 * on elements and attributes from external tools regardless of element
	 * and attribute type.
	 * @param e Element from which to apply this attributes offset.
	 * @return Returns the storage associate with this attribute in <tt><i>e.</i></tt> 
	 */
	daeChar* getWritableMemory(daeElement* e) {
		return (daeChar*)e+_offset; }
};
/**
* The @c daeMetaElementAttribute class represents a single attribute whose value is an element.
*/
class daeMetaElementAttribute : public daeMetaAttribute
{
public:
	/** Minimum number of times this meta element can occur. */
	daeInt					_minOccurs;
	/** Maximum number of times this meta element can occur. */
	daeInt					_maxOccurs;
	/** If this element is found in a choice group in the schema */
	daeBool					_isInChoice;
	/** If this element is found in a sequence group in the schema */
	daeBool					_isInSequence;
	
	/** The element found before this one in the sequence group in the schema */
	daeMetaElement* 		_previousInSequence;
	/** The metaElement that describes the element type of this attribute */
	daeMetaElement* 		_elementType;
public:
	/**
	 * Constructor
	 */
	daeMetaElementAttribute();
	/**
	 * Destructor
	 */
	~daeMetaElementAttribute() {}
public:
	/**
	 * Sets the element type for the element that this attribute points to.
	 * @param elementType @c daeMetaElement representing the type.
	 */
	void setElementType(daeMetaElement *elementType) {
		_elementType = elementType; }

	/**
	 * Gets the element type for the element that this attribute points to.
	 * @return Returns the @c daeMetaElement representing the type.
	 */
	daeMetaElement* getElementType() { return _elementType; }
	
	/**
	 * Defines the override version of base method.
	 * @see daeMetaAttribute::clone()
	 */
	virtual daeMetaAttributeRef clone();
	
	/**
	 * Places element <tt><i>child</i></tt> in element <tt><i>parent</i></tt> using @c this element attribute.
	 * @param parent The Element in which to place child.
	 * @param child The Element to place in parent.
	 */
	virtual void placeElement(daeElement* parent, daeElement* child);
	/**
	 * Removes element <tt><i>child</i></tt> from element <tt><i>parent</i></tt> using @c this element attribute.
	 * @param parent The Element in which to remove child.
	 * @param child The Element to remove from parent.
	 */
	virtual void removeElement(daeElement* parent, daeElement* child);
	/**
	 * Sets the database document associated with this element.
	 * @param parent The daeElement to set the document.
	 * @param c The @c daeDocument to associate with this element.
	 */
	virtual void setDocument(daeElement *parent, daeDocument* c );
	inline void setCollection(daeElement *parent, daeDocument* c ) {
		setDocument( parent, c );
	}

	/**
	 * Gets the number of elements associated with this attribute in instance <tt><i>e.</i></tt> 
	 * @param e Containing element to run the operation on.
	 * @return Returns the number of elements associated with this attribute
	 * in instance <tt><i>e.</i></tt> 
	 */
	virtual daeInt getCount(daeElement* e);

	/**
	 * Gets an element from containing element <tt><i>e</i></tt> based on <tt><i>index.</i></tt> 
	 * @param e Containing element from which to get the element.
	 * @param index Index of the element to retrieve if indeed
	 * there is an array of elements rather than a singleton.
	 * @return Returns the associated element out of parent element e, based on index, if necessary.
	 */
	virtual daeMemoryRef get(daeElement* e, daeInt index);

	/**
	 * Defines the override version of base method.
	 * @param element Element on which to set this attribute.
	 * @param s String containing the value to be converted via the
	 * atomic type system.
	 */
	virtual void set(daeElement* element, daeString s);
	/**
	 * Defines the override version of base method.
	 * @param toElement Pointer to a @c daeElement to copy this attribute to.
	 * @param fromElement Pointer to a @c daeElement to copy this attribute from.
	 */
	virtual void copy(daeElement* toElement, daeElement* fromElement);
};
typedef daeSmartRef<daeMetaElementAttribute> daeMetaElementAttributeRef;
typedef daeTArray<daeMetaElementAttributeRef> daeMetaElementAttributeArray;


/**
 * The @c daeMetaElementArrayAttribute class is similar to daeMetaElementAttribute 
 * except that this meta attribute
 * describes an array of elements rather than a singleton.
 */
class daeMetaElementArrayAttribute : public daeMetaElementAttribute
{
public:
	/**
	 * Constructor
	 */
	daeMetaElementArrayAttribute();
public:
	/**
	 * Defines the override version of this method from @c daeMetaElement.
	 * @return Returns a duplicate of this @c daeMetaAttribute.
	 * @note Not Implemented.
	 */
	virtual daeMetaAttributeRef clone();
	/**
	 * Defines the override version of this method from @c daeMetaElement.
	 */
	virtual void placeElement(daeElement* parent, daeElement* child);
    /**
	 * Defines the override version of this method from @c daeMetaElement.
	 */
	virtual void removeElement(daeElement* parent, daeElement* child);
	/**
	 * Sets the database document associated with this element.
	 * @param c The @c daeDocument to associate with this element.
	 */
	virtual void setDocument(daeElement *parent, daeDocument* c );
	inline void setCollection(daeElement *parent, daeDocument* c ) {
		setDocument( parent, c );
	}

	/**
	 * Defines the override version of this method from @c daeMetaElement.
	 * @param e Containing element to run the operation on.
	 * @return Returns the number of particles associated with this attribute
	 * in instance <tt><i>e.</i></tt> 
	 */
	virtual daeInt getCount(daeElement* e);
	/**
	 * Defines the override version of this method from @c daeMetaElement.
	 * @param e Containing element from which to get the element.
	 * @param index Index of the particle to retrieve if indeed
	 * there is an array of elements rather than a singleton.
	 * @return Returns the associated particle out of parent element e, based on index, if necessary.
	 */
	virtual daeMemoryRef get(daeElement* e, daeInt index);
	/**
	 * Defines the override version of this method from @c daeMetaElement.
	 * @param toElement Pointer to a @c daeElement to copy this attribute to.
	 * @param fromElement Pointer to a @c daeElement to copy this attribute from.
	 */
	virtual void copy(daeElement* toElement, daeElement* fromElement);
};
typedef daeSmartRef<daeMetaElementArrayAttribute> daeMetaElementArrayAttributeRef;
typedef daeTArray<daeMetaElementArrayAttributeRef> daeMetaElementArrayAttributeArray;

/**
 * The @c daeMetaArrayAttribute class is simple a wrapper that implements
 * an array of atomic types rather than a singleton.
 * The corresponding storage is an array
 * and the corresponding operations are implemented on the array
 * data structure rather than on inlined storage in elements.
 */
class daeMetaArrayAttribute : public daeMetaAttribute
{
	daeMetaAttributeRef arrayType;
public:
	/**
	 * Defines the override version of this method from @c daeMetaAttribute.
	 * @return Returns a duplicate of this @c daeMetaAttribute.
	 * @note Not Implemented.
	 */
	virtual daeMetaAttributeRef clone();
	/**
	 * Defines the override version of this method from @c daeMetaAttribute.
	 * @param element Element on which to set this attribute.
	 * @param s String containing the value to be converted via the
	 * atomic type system.
	 */
	virtual void set(daeElement* element, daeString s);
	/**
	 * Defines the override version of this method from @c daeMetaAttribute.
	 * @param toElement Pointer to a @c daeElement to copy this attribute to.
	 * @param fromElement Pointer to a @c daeElement to copy this attribute from.
	 */
	virtual void copy(daeElement* toElement, daeElement* fromElement);
	/**
	 * Defines the override version of this method from @c daeMetaElement.
	 * @param e Containing element to run the operation on.
	 * @return Returns the number of particles associated with this attribute
	 * in instance <tt><i>e.</i></tt> 
	 */
	virtual daeInt getCount(daeElement* e);
	/**
	 * Defines the override version of this method from @c daeMetaElement.
	 * @param e Containing element from which to get the element.
	 * @param index Index of the particle to retrieve if indeed
	 * there is an array of elements rather than a singleton.
	 * @return Returns the associated particle out of parent element e, based on index, if necessary.
	 */
	virtual daeMemoryRef get(daeElement* e, daeInt index);
};

#endif //__DAE_META_ATTRIBUTE_H__






