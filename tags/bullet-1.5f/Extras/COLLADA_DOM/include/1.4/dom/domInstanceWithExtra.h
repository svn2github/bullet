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
#ifndef __domInstanceWithExtra_h__
#define __domInstanceWithExtra_h__

#include <dom/domTypes.h>
#include <dom/domElements.h>

#include <dom/domExtra.h>

/**
 * The InstanceWithExtra type is used for all generic instance elements. A
 * generic instance element  is one which does not have any specific child
 * elements declared.
 */
class domInstanceWithExtra_complexType 
{
protected:  // Attribute
/**
 *  The url attribute refers to resource to instantiate. This may refer to
 * a local resource using a  relative URL fragment identifier that begins
 * with the “#” character. The url attribute may refer  to an external
 * resource using an absolute or relative URL. 
 */
	xsAnyURI attrUrl;

protected:  // Element
/**
 *  The extra element may occur any number of times.  @see domExtra
 */
	domExtra_Array elemExtra_array;

public:	//Accessors and Mutators
	/**
	 * Gets the url attribute.
	 * @return Returns a xsAnyURI reference of the url attribute.
	 */
	xsAnyURI &getUrl() { return attrUrl; }
	/**
	 * Gets the url attribute.
	 * @return Returns a constant xsAnyURI reference of the url attribute.
	 */
	const xsAnyURI &getUrl() const { return attrUrl; }
	/**
	 * Sets the url attribute.
	 * @param atUrl The new value for the url attribute.
	 */
	void setUrl( const xsAnyURI &atUrl ) { attrUrl.setURI( atUrl.getURI() ); }

	/**
	 * Gets the extra element array.
	 * @return Returns a reference to the array of extra elements.
	 */
	domExtra_Array &getExtra_array() { return elemExtra_array; }
	/**
	 * Gets the extra element array.
	 * @return Returns a constant reference to the array of extra elements.
	 */
	const domExtra_Array &getExtra_array() const { return elemExtra_array; }
protected:
	/**
	 * Constructor
	 */
	domInstanceWithExtra_complexType() : attrUrl(), elemExtra_array() {}
	/**
	 * Destructor
	 */
	virtual ~domInstanceWithExtra_complexType() {}
	/**
	 * Copy Constructor
	 */
	domInstanceWithExtra_complexType( const domInstanceWithExtra_complexType &cpy ) { (void)cpy; }
	/**
	 * Overloaded assignment operator
	 */
	virtual domInstanceWithExtra_complexType &operator=( const domInstanceWithExtra_complexType &cpy ) { (void)cpy; return *this; }
};

/**
 * An element of type domInstanceWithExtra_complexType.
 */
class domInstanceWithExtra : public daeElement, public domInstanceWithExtra_complexType
{
protected:
	/**
	 * Constructor
	 */
	domInstanceWithExtra() {}
	/**
	 * Destructor
	 */
	virtual ~domInstanceWithExtra() {}
	/**
	 * Copy Constructor
	 */
	domInstanceWithExtra( const domInstanceWithExtra &cpy ) : daeElement(), domInstanceWithExtra_complexType() { (void)cpy; }
	/**
	 * Overloaded assignment operator
	 */
	virtual domInstanceWithExtra &operator=( const domInstanceWithExtra &cpy ) { (void)cpy; return *this; }

public: // STATIC METHODS
	/**
	 * Creates an instance of this class and returns a daeElementRef referencing it.
	 * @param bytes The size allocated for this instance.
	 * @return a daeElementRef referencing an instance of this object.
	 */
	static daeElementRef create(daeInt bytes);
	/**
	 * Creates a daeMetaElement object that describes this element in the meta object reflection framework.
	 * If a daeMetaElement already exists it will return that instead of creating a new one. 
	 * @return A daeMetaElement describing this COLLADA element.
	 */
	static daeMetaElement* registerElement();

public: // STATIC MEMBERS
	/**
	 * The daeMetaElement that describes this element in the meta object reflection framework.
	 */
	static daeMetaElement* _Meta;
};


#endif
