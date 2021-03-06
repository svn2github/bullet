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

#include <dae/daeDom.h>
#include <dom/domFx_annotate_type_common.h>

daeElementRef
domFx_annotate_type_common::create(daeInt bytes)
{
	domFx_annotate_type_commonRef ref = new(bytes) domFx_annotate_type_common;
	return ref;
}


daeMetaElement *
domFx_annotate_type_common::registerElement()
{
    if ( _Meta != NULL ) return _Meta;
    
    _Meta = new daeMetaElement;
    _Meta->setName( "fx_annotate_type_common" );
	_Meta->setStaticPointerAddress(&domFx_annotate_type_common::_Meta);
	_Meta->registerConstructor(domFx_annotate_type_common::create);

	_Meta->setIsTransparent( true );
	// Add elements: bool, bool2, bool3, bool4, int, int2, int3, int4, float, float2, float3, float4, float2x2, float3x3, float4x4, string
    _Meta->appendElement(domFx_annotate_type_common::domBool::registerElement(),daeOffsetOf(domFx_annotate_type_common,elemBool));
    _Meta->appendElement(domFx_annotate_type_common::domBool2::registerElement(),daeOffsetOf(domFx_annotate_type_common,elemBool2));
    _Meta->appendElement(domFx_annotate_type_common::domBool3::registerElement(),daeOffsetOf(domFx_annotate_type_common,elemBool3));
    _Meta->appendElement(domFx_annotate_type_common::domBool4::registerElement(),daeOffsetOf(domFx_annotate_type_common,elemBool4));
    _Meta->appendElement(domFx_annotate_type_common::domInt::registerElement(),daeOffsetOf(domFx_annotate_type_common,elemInt));
    _Meta->appendElement(domFx_annotate_type_common::domInt2::registerElement(),daeOffsetOf(domFx_annotate_type_common,elemInt2));
    _Meta->appendElement(domFx_annotate_type_common::domInt3::registerElement(),daeOffsetOf(domFx_annotate_type_common,elemInt3));
    _Meta->appendElement(domFx_annotate_type_common::domInt4::registerElement(),daeOffsetOf(domFx_annotate_type_common,elemInt4));
    _Meta->appendElement(domFx_annotate_type_common::domFloat::registerElement(),daeOffsetOf(domFx_annotate_type_common,elemFloat));
    _Meta->appendElement(domFx_annotate_type_common::domFloat2::registerElement(),daeOffsetOf(domFx_annotate_type_common,elemFloat2));
    _Meta->appendElement(domFx_annotate_type_common::domFloat3::registerElement(),daeOffsetOf(domFx_annotate_type_common,elemFloat3));
    _Meta->appendElement(domFx_annotate_type_common::domFloat4::registerElement(),daeOffsetOf(domFx_annotate_type_common,elemFloat4));
    _Meta->appendElement(domFx_annotate_type_common::domFloat2x2::registerElement(),daeOffsetOf(domFx_annotate_type_common,elemFloat2x2));
    _Meta->appendElement(domFx_annotate_type_common::domFloat3x3::registerElement(),daeOffsetOf(domFx_annotate_type_common,elemFloat3x3));
    _Meta->appendElement(domFx_annotate_type_common::domFloat4x4::registerElement(),daeOffsetOf(domFx_annotate_type_common,elemFloat4x4));
    _Meta->appendElement(domFx_annotate_type_common::domString::registerElement(),daeOffsetOf(domFx_annotate_type_common,elemString));
	// Ordered list of sub-elements
    _Meta->addContents(daeOffsetOf(domFx_annotate_type_common,_contents));

	
	
	_Meta->setElementSize(sizeof(domFx_annotate_type_common));
	_Meta->validate();

	return _Meta;
}

daeElementRef
domFx_annotate_type_common::domBool::create(daeInt bytes)
{
	domFx_annotate_type_common::domBoolRef ref = new(bytes) domFx_annotate_type_common::domBool;
	return ref;
}


daeMetaElement *
domFx_annotate_type_common::domBool::registerElement()
{
    if ( _Meta != NULL ) return _Meta;
    
    _Meta = new daeMetaElement;
    _Meta->setName( "bool" );
	_Meta->setStaticPointerAddress(&domFx_annotate_type_common::domBool::_Meta);
	_Meta->registerConstructor(domFx_annotate_type_common::domBool::create);

	//	Add attribute: _value
 	{
		daeMetaAttribute *ma = new daeMetaAttribute;
		ma->setName( "_value" );
		ma->setType( daeAtomicType::get("Bool"));
		ma->setOffset( daeOffsetOf( domFx_annotate_type_common::domBool , _value ));
		ma->setContainer( _Meta );
		_Meta->appendAttribute(ma);
	}
	
	
	_Meta->setElementSize(sizeof(domFx_annotate_type_common::domBool));
	_Meta->validate();

	return _Meta;
}

daeElementRef
domFx_annotate_type_common::domBool2::create(daeInt bytes)
{
	domFx_annotate_type_common::domBool2Ref ref = new(bytes) domFx_annotate_type_common::domBool2;
	return ref;
}


daeMetaElement *
domFx_annotate_type_common::domBool2::registerElement()
{
    if ( _Meta != NULL ) return _Meta;
    
    _Meta = new daeMetaElement;
    _Meta->setName( "bool2" );
	_Meta->setStaticPointerAddress(&domFx_annotate_type_common::domBool2::_Meta);
	_Meta->registerConstructor(domFx_annotate_type_common::domBool2::create);

	//	Add attribute: _value
 	{
		daeMetaAttribute *ma = new daeMetaArrayAttribute;
		ma->setName( "_value" );
		ma->setType( daeAtomicType::get("Bool2"));
		ma->setOffset( daeOffsetOf( domFx_annotate_type_common::domBool2 , _value ));
		ma->setContainer( _Meta );
		_Meta->appendAttribute(ma);
	}
	
	
	_Meta->setElementSize(sizeof(domFx_annotate_type_common::domBool2));
	_Meta->validate();

	return _Meta;
}

daeElementRef
domFx_annotate_type_common::domBool3::create(daeInt bytes)
{
	domFx_annotate_type_common::domBool3Ref ref = new(bytes) domFx_annotate_type_common::domBool3;
	return ref;
}


daeMetaElement *
domFx_annotate_type_common::domBool3::registerElement()
{
    if ( _Meta != NULL ) return _Meta;
    
    _Meta = new daeMetaElement;
    _Meta->setName( "bool3" );
	_Meta->setStaticPointerAddress(&domFx_annotate_type_common::domBool3::_Meta);
	_Meta->registerConstructor(domFx_annotate_type_common::domBool3::create);

	//	Add attribute: _value
 	{
		daeMetaAttribute *ma = new daeMetaArrayAttribute;
		ma->setName( "_value" );
		ma->setType( daeAtomicType::get("Bool3"));
		ma->setOffset( daeOffsetOf( domFx_annotate_type_common::domBool3 , _value ));
		ma->setContainer( _Meta );
		_Meta->appendAttribute(ma);
	}
	
	
	_Meta->setElementSize(sizeof(domFx_annotate_type_common::domBool3));
	_Meta->validate();

	return _Meta;
}

daeElementRef
domFx_annotate_type_common::domBool4::create(daeInt bytes)
{
	domFx_annotate_type_common::domBool4Ref ref = new(bytes) domFx_annotate_type_common::domBool4;
	return ref;
}


daeMetaElement *
domFx_annotate_type_common::domBool4::registerElement()
{
    if ( _Meta != NULL ) return _Meta;
    
    _Meta = new daeMetaElement;
    _Meta->setName( "bool4" );
	_Meta->setStaticPointerAddress(&domFx_annotate_type_common::domBool4::_Meta);
	_Meta->registerConstructor(domFx_annotate_type_common::domBool4::create);

	//	Add attribute: _value
 	{
		daeMetaAttribute *ma = new daeMetaArrayAttribute;
		ma->setName( "_value" );
		ma->setType( daeAtomicType::get("Bool4"));
		ma->setOffset( daeOffsetOf( domFx_annotate_type_common::domBool4 , _value ));
		ma->setContainer( _Meta );
		_Meta->appendAttribute(ma);
	}
	
	
	_Meta->setElementSize(sizeof(domFx_annotate_type_common::domBool4));
	_Meta->validate();

	return _Meta;
}

daeElementRef
domFx_annotate_type_common::domInt::create(daeInt bytes)
{
	domFx_annotate_type_common::domIntRef ref = new(bytes) domFx_annotate_type_common::domInt;
	return ref;
}


daeMetaElement *
domFx_annotate_type_common::domInt::registerElement()
{
    if ( _Meta != NULL ) return _Meta;
    
    _Meta = new daeMetaElement;
    _Meta->setName( "int" );
	_Meta->setStaticPointerAddress(&domFx_annotate_type_common::domInt::_Meta);
	_Meta->registerConstructor(domFx_annotate_type_common::domInt::create);

	//	Add attribute: _value
 	{
		daeMetaAttribute *ma = new daeMetaAttribute;
		ma->setName( "_value" );
		ma->setType( daeAtomicType::get("Int"));
		ma->setOffset( daeOffsetOf( domFx_annotate_type_common::domInt , _value ));
		ma->setContainer( _Meta );
		_Meta->appendAttribute(ma);
	}
	
	
	_Meta->setElementSize(sizeof(domFx_annotate_type_common::domInt));
	_Meta->validate();

	return _Meta;
}

daeElementRef
domFx_annotate_type_common::domInt2::create(daeInt bytes)
{
	domFx_annotate_type_common::domInt2Ref ref = new(bytes) domFx_annotate_type_common::domInt2;
	return ref;
}


daeMetaElement *
domFx_annotate_type_common::domInt2::registerElement()
{
    if ( _Meta != NULL ) return _Meta;
    
    _Meta = new daeMetaElement;
    _Meta->setName( "int2" );
	_Meta->setStaticPointerAddress(&domFx_annotate_type_common::domInt2::_Meta);
	_Meta->registerConstructor(domFx_annotate_type_common::domInt2::create);

	//	Add attribute: _value
 	{
		daeMetaAttribute *ma = new daeMetaArrayAttribute;
		ma->setName( "_value" );
		ma->setType( daeAtomicType::get("Int2"));
		ma->setOffset( daeOffsetOf( domFx_annotate_type_common::domInt2 , _value ));
		ma->setContainer( _Meta );
		_Meta->appendAttribute(ma);
	}
	
	
	_Meta->setElementSize(sizeof(domFx_annotate_type_common::domInt2));
	_Meta->validate();

	return _Meta;
}

daeElementRef
domFx_annotate_type_common::domInt3::create(daeInt bytes)
{
	domFx_annotate_type_common::domInt3Ref ref = new(bytes) domFx_annotate_type_common::domInt3;
	return ref;
}


daeMetaElement *
domFx_annotate_type_common::domInt3::registerElement()
{
    if ( _Meta != NULL ) return _Meta;
    
    _Meta = new daeMetaElement;
    _Meta->setName( "int3" );
	_Meta->setStaticPointerAddress(&domFx_annotate_type_common::domInt3::_Meta);
	_Meta->registerConstructor(domFx_annotate_type_common::domInt3::create);

	//	Add attribute: _value
 	{
		daeMetaAttribute *ma = new daeMetaArrayAttribute;
		ma->setName( "_value" );
		ma->setType( daeAtomicType::get("Int3"));
		ma->setOffset( daeOffsetOf( domFx_annotate_type_common::domInt3 , _value ));
		ma->setContainer( _Meta );
		_Meta->appendAttribute(ma);
	}
	
	
	_Meta->setElementSize(sizeof(domFx_annotate_type_common::domInt3));
	_Meta->validate();

	return _Meta;
}

daeElementRef
domFx_annotate_type_common::domInt4::create(daeInt bytes)
{
	domFx_annotate_type_common::domInt4Ref ref = new(bytes) domFx_annotate_type_common::domInt4;
	return ref;
}


daeMetaElement *
domFx_annotate_type_common::domInt4::registerElement()
{
    if ( _Meta != NULL ) return _Meta;
    
    _Meta = new daeMetaElement;
    _Meta->setName( "int4" );
	_Meta->setStaticPointerAddress(&domFx_annotate_type_common::domInt4::_Meta);
	_Meta->registerConstructor(domFx_annotate_type_common::domInt4::create);

	//	Add attribute: _value
 	{
		daeMetaAttribute *ma = new daeMetaArrayAttribute;
		ma->setName( "_value" );
		ma->setType( daeAtomicType::get("Int4"));
		ma->setOffset( daeOffsetOf( domFx_annotate_type_common::domInt4 , _value ));
		ma->setContainer( _Meta );
		_Meta->appendAttribute(ma);
	}
	
	
	_Meta->setElementSize(sizeof(domFx_annotate_type_common::domInt4));
	_Meta->validate();

	return _Meta;
}

daeElementRef
domFx_annotate_type_common::domFloat::create(daeInt bytes)
{
	domFx_annotate_type_common::domFloatRef ref = new(bytes) domFx_annotate_type_common::domFloat;
	return ref;
}


daeMetaElement *
domFx_annotate_type_common::domFloat::registerElement()
{
    if ( _Meta != NULL ) return _Meta;
    
    _Meta = new daeMetaElement;
    _Meta->setName( "float" );
	_Meta->setStaticPointerAddress(&domFx_annotate_type_common::domFloat::_Meta);
	_Meta->registerConstructor(domFx_annotate_type_common::domFloat::create);

	//	Add attribute: _value
 	{
		daeMetaAttribute *ma = new daeMetaAttribute;
		ma->setName( "_value" );
		ma->setType( daeAtomicType::get("Float"));
		ma->setOffset( daeOffsetOf( domFx_annotate_type_common::domFloat , _value ));
		ma->setContainer( _Meta );
		_Meta->appendAttribute(ma);
	}
	
	
	_Meta->setElementSize(sizeof(domFx_annotate_type_common::domFloat));
	_Meta->validate();

	return _Meta;
}

daeElementRef
domFx_annotate_type_common::domFloat2::create(daeInt bytes)
{
	domFx_annotate_type_common::domFloat2Ref ref = new(bytes) domFx_annotate_type_common::domFloat2;
	return ref;
}


daeMetaElement *
domFx_annotate_type_common::domFloat2::registerElement()
{
    if ( _Meta != NULL ) return _Meta;
    
    _Meta = new daeMetaElement;
    _Meta->setName( "float2" );
	_Meta->setStaticPointerAddress(&domFx_annotate_type_common::domFloat2::_Meta);
	_Meta->registerConstructor(domFx_annotate_type_common::domFloat2::create);

	//	Add attribute: _value
 	{
		daeMetaAttribute *ma = new daeMetaArrayAttribute;
		ma->setName( "_value" );
		ma->setType( daeAtomicType::get("Float2"));
		ma->setOffset( daeOffsetOf( domFx_annotate_type_common::domFloat2 , _value ));
		ma->setContainer( _Meta );
		_Meta->appendAttribute(ma);
	}
	
	
	_Meta->setElementSize(sizeof(domFx_annotate_type_common::domFloat2));
	_Meta->validate();

	return _Meta;
}

daeElementRef
domFx_annotate_type_common::domFloat3::create(daeInt bytes)
{
	domFx_annotate_type_common::domFloat3Ref ref = new(bytes) domFx_annotate_type_common::domFloat3;
	return ref;
}


daeMetaElement *
domFx_annotate_type_common::domFloat3::registerElement()
{
    if ( _Meta != NULL ) return _Meta;
    
    _Meta = new daeMetaElement;
    _Meta->setName( "float3" );
	_Meta->setStaticPointerAddress(&domFx_annotate_type_common::domFloat3::_Meta);
	_Meta->registerConstructor(domFx_annotate_type_common::domFloat3::create);

	//	Add attribute: _value
 	{
		daeMetaAttribute *ma = new daeMetaArrayAttribute;
		ma->setName( "_value" );
		ma->setType( daeAtomicType::get("Float3"));
		ma->setOffset( daeOffsetOf( domFx_annotate_type_common::domFloat3 , _value ));
		ma->setContainer( _Meta );
		_Meta->appendAttribute(ma);
	}
	
	
	_Meta->setElementSize(sizeof(domFx_annotate_type_common::domFloat3));
	_Meta->validate();

	return _Meta;
}

daeElementRef
domFx_annotate_type_common::domFloat4::create(daeInt bytes)
{
	domFx_annotate_type_common::domFloat4Ref ref = new(bytes) domFx_annotate_type_common::domFloat4;
	return ref;
}


daeMetaElement *
domFx_annotate_type_common::domFloat4::registerElement()
{
    if ( _Meta != NULL ) return _Meta;
    
    _Meta = new daeMetaElement;
    _Meta->setName( "float4" );
	_Meta->setStaticPointerAddress(&domFx_annotate_type_common::domFloat4::_Meta);
	_Meta->registerConstructor(domFx_annotate_type_common::domFloat4::create);

	//	Add attribute: _value
 	{
		daeMetaAttribute *ma = new daeMetaArrayAttribute;
		ma->setName( "_value" );
		ma->setType( daeAtomicType::get("Float4"));
		ma->setOffset( daeOffsetOf( domFx_annotate_type_common::domFloat4 , _value ));
		ma->setContainer( _Meta );
		_Meta->appendAttribute(ma);
	}
	
	
	_Meta->setElementSize(sizeof(domFx_annotate_type_common::domFloat4));
	_Meta->validate();

	return _Meta;
}

daeElementRef
domFx_annotate_type_common::domFloat2x2::create(daeInt bytes)
{
	domFx_annotate_type_common::domFloat2x2Ref ref = new(bytes) domFx_annotate_type_common::domFloat2x2;
	return ref;
}


daeMetaElement *
domFx_annotate_type_common::domFloat2x2::registerElement()
{
    if ( _Meta != NULL ) return _Meta;
    
    _Meta = new daeMetaElement;
    _Meta->setName( "float2x2" );
	_Meta->setStaticPointerAddress(&domFx_annotate_type_common::domFloat2x2::_Meta);
	_Meta->registerConstructor(domFx_annotate_type_common::domFloat2x2::create);

	//	Add attribute: _value
 	{
		daeMetaAttribute *ma = new daeMetaArrayAttribute;
		ma->setName( "_value" );
		ma->setType( daeAtomicType::get("Float2x2"));
		ma->setOffset( daeOffsetOf( domFx_annotate_type_common::domFloat2x2 , _value ));
		ma->setContainer( _Meta );
		_Meta->appendAttribute(ma);
	}
	
	
	_Meta->setElementSize(sizeof(domFx_annotate_type_common::domFloat2x2));
	_Meta->validate();

	return _Meta;
}

daeElementRef
domFx_annotate_type_common::domFloat3x3::create(daeInt bytes)
{
	domFx_annotate_type_common::domFloat3x3Ref ref = new(bytes) domFx_annotate_type_common::domFloat3x3;
	return ref;
}


daeMetaElement *
domFx_annotate_type_common::domFloat3x3::registerElement()
{
    if ( _Meta != NULL ) return _Meta;
    
    _Meta = new daeMetaElement;
    _Meta->setName( "float3x3" );
	_Meta->setStaticPointerAddress(&domFx_annotate_type_common::domFloat3x3::_Meta);
	_Meta->registerConstructor(domFx_annotate_type_common::domFloat3x3::create);

	//	Add attribute: _value
 	{
		daeMetaAttribute *ma = new daeMetaArrayAttribute;
		ma->setName( "_value" );
		ma->setType( daeAtomicType::get("Float3x3"));
		ma->setOffset( daeOffsetOf( domFx_annotate_type_common::domFloat3x3 , _value ));
		ma->setContainer( _Meta );
		_Meta->appendAttribute(ma);
	}
	
	
	_Meta->setElementSize(sizeof(domFx_annotate_type_common::domFloat3x3));
	_Meta->validate();

	return _Meta;
}

daeElementRef
domFx_annotate_type_common::domFloat4x4::create(daeInt bytes)
{
	domFx_annotate_type_common::domFloat4x4Ref ref = new(bytes) domFx_annotate_type_common::domFloat4x4;
	return ref;
}


daeMetaElement *
domFx_annotate_type_common::domFloat4x4::registerElement()
{
    if ( _Meta != NULL ) return _Meta;
    
    _Meta = new daeMetaElement;
    _Meta->setName( "float4x4" );
	_Meta->setStaticPointerAddress(&domFx_annotate_type_common::domFloat4x4::_Meta);
	_Meta->registerConstructor(domFx_annotate_type_common::domFloat4x4::create);

	//	Add attribute: _value
 	{
		daeMetaAttribute *ma = new daeMetaArrayAttribute;
		ma->setName( "_value" );
		ma->setType( daeAtomicType::get("Float4x4"));
		ma->setOffset( daeOffsetOf( domFx_annotate_type_common::domFloat4x4 , _value ));
		ma->setContainer( _Meta );
		_Meta->appendAttribute(ma);
	}
	
	
	_Meta->setElementSize(sizeof(domFx_annotate_type_common::domFloat4x4));
	_Meta->validate();

	return _Meta;
}

daeElementRef
domFx_annotate_type_common::domString::create(daeInt bytes)
{
	domFx_annotate_type_common::domStringRef ref = new(bytes) domFx_annotate_type_common::domString;
	return ref;
}


daeMetaElement *
domFx_annotate_type_common::domString::registerElement()
{
    if ( _Meta != NULL ) return _Meta;
    
    _Meta = new daeMetaElement;
    _Meta->setName( "string" );
	_Meta->setStaticPointerAddress(&domFx_annotate_type_common::domString::_Meta);
	_Meta->registerConstructor(domFx_annotate_type_common::domString::create);

	//	Add attribute: _value
 	{
		daeMetaAttribute *ma = new daeMetaAttribute;
		ma->setName( "_value" );
		ma->setType( daeAtomicType::get("xsString"));
		ma->setOffset( daeOffsetOf( domFx_annotate_type_common::domString , _value ));
		ma->setContainer( _Meta );
		_Meta->appendAttribute(ma);
	}
	
	
	_Meta->setElementSize(sizeof(domFx_annotate_type_common::domString));
	_Meta->validate();

	return _Meta;
}


daeMetaElement * domFx_annotate_type_common::_Meta = NULL;
daeMetaElement * domFx_annotate_type_common::domBool::_Meta = NULL;
daeMetaElement * domFx_annotate_type_common::domBool2::_Meta = NULL;
daeMetaElement * domFx_annotate_type_common::domBool3::_Meta = NULL;
daeMetaElement * domFx_annotate_type_common::domBool4::_Meta = NULL;
daeMetaElement * domFx_annotate_type_common::domInt::_Meta = NULL;
daeMetaElement * domFx_annotate_type_common::domInt2::_Meta = NULL;
daeMetaElement * domFx_annotate_type_common::domInt3::_Meta = NULL;
daeMetaElement * domFx_annotate_type_common::domInt4::_Meta = NULL;
daeMetaElement * domFx_annotate_type_common::domFloat::_Meta = NULL;
daeMetaElement * domFx_annotate_type_common::domFloat2::_Meta = NULL;
daeMetaElement * domFx_annotate_type_common::domFloat3::_Meta = NULL;
daeMetaElement * domFx_annotate_type_common::domFloat4::_Meta = NULL;
daeMetaElement * domFx_annotate_type_common::domFloat2x2::_Meta = NULL;
daeMetaElement * domFx_annotate_type_common::domFloat3x3::_Meta = NULL;
daeMetaElement * domFx_annotate_type_common::domFloat4x4::_Meta = NULL;
daeMetaElement * domFx_annotate_type_common::domString::_Meta = NULL;


