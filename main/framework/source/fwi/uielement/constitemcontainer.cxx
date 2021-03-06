/**************************************************************
 * 
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
 * 
 *   http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied.  See the License for the
 * specific language governing permissions and limitations
 * under the License.
 * 
 *************************************************************/



// MARKER(update_precomp.py): autogen include statement, do not remove
#include "precompiled_framework.hxx"
//_________________________________________________________________________________________________________________
//	my own includes
//_________________________________________________________________________________________________________________
#include <uielement/constitemcontainer.hxx>
#include <uielement/rootitemcontainer.hxx>
#include <uielement/itemcontainer.hxx>
#include <threadhelp/resetableguard.hxx>

//_________________________________________________________________________________________________________________
//	interface includes
//_________________________________________________________________________________________________________________
#include <com/sun/star/beans/PropertyAttribute.hpp>

//_________________________________________________________________________________________________________________
//	other includes
//_________________________________________________________________________________________________________________
#include <cppuhelper/implbase1.hxx>

using namespace cppu;
using namespace com::sun::star::uno;
using namespace com::sun::star::lang;
using namespace com::sun::star::beans;
using namespace com::sun::star::container;

const char WRONG_TYPE_EXCEPTION[] = "Type must be com::sun::star::uno::Sequence< com::sun::star::beans::PropertyValue >";

const int PROPHANDLE_UINAME     = 1;
const int PROPCOUNT             = 1;
const rtl::OUString PROPNAME_UINAME( RTL_CONSTASCII_USTRINGPARAM( "UIName" ));

namespace framework
{

/**
 * The class which implements the PropertySetInfo interface.
 */
extern "C"
{
static int SAL_CALL compare_OUString_Property_Impl( const void *arg1, const void *arg2 ) SAL_THROW( () )
{
   return ((::rtl::OUString *)arg1)->compareTo( ((Property *)arg2)->Name );
}
}

class OPropertySetHelperInfo_Impl
    : public WeakImplHelper1< ::com::sun::star::beans::XPropertySetInfo >
{
	Sequence < Property > aInfos;

public:
	OPropertySetHelperInfo_Impl( IPropertyArrayHelper & rHelper_ ) SAL_THROW( () );

	// XPropertySetInfo-Methoden
    virtual Sequence< Property > SAL_CALL getProperties(void) throw(::com::sun::star::uno::RuntimeException);
    virtual Property SAL_CALL getPropertyByName(const ::rtl::OUString& PropertyName) throw(::com::sun::star::beans::UnknownPropertyException, ::com::sun::star::uno::RuntimeException);
    virtual sal_Bool SAL_CALL hasPropertyByName(const ::rtl::OUString& PropertyName) throw(::com::sun::star::uno::RuntimeException);
};


/**
 * Create an object that implements XPropertySetInfo IPropertyArrayHelper.
 */
OPropertySetHelperInfo_Impl::OPropertySetHelperInfo_Impl(
	IPropertyArrayHelper & rHelper_ )
	SAL_THROW( () )
	:aInfos( rHelper_.getProperties() )
{
}

/**
 * Return the sequence of properties, which are provided throug the constructor.
 */
Sequence< Property > OPropertySetHelperInfo_Impl::getProperties(void) throw(::com::sun::star::uno::RuntimeException)
{
	return aInfos;
}

/**
 * Return the sequence of properties, which are provided throug the constructor.
 */
Property OPropertySetHelperInfo_Impl::getPropertyByName( const ::rtl::OUString & PropertyName ) throw(::com::sun::star::beans::UnknownPropertyException, ::com::sun::star::uno::RuntimeException)
{
	Property * pR;
	pR = (Property *)bsearch( &PropertyName, aInfos.getConstArray(), aInfos.getLength(),
                              sizeof( Property ),
							  compare_OUString_Property_Impl );
	if( !pR ) {
		throw UnknownPropertyException();
	}

	return *pR;
}

/**
 * Return the sequence of properties, which are provided throug the constructor.
 */
sal_Bool OPropertySetHelperInfo_Impl::hasPropertyByName( const ::rtl::OUString & PropertyName ) throw(::com::sun::star::uno::RuntimeException)
{
	Property * pR;
	pR = (Property *)bsearch( &PropertyName, aInfos.getConstArray(), aInfos.getLength(),
                              sizeof( Property ),
							  compare_OUString_Property_Impl );
	return pR != NULL;
}

//*****************************************************************************************************************
//	XInterface, XTypeProvider
//*****************************************************************************************************************
DEFINE_XINTERFACE_6     (   ConstItemContainer                                              ,
                            OWeakObject                                                     ,
                            DIRECT_INTERFACE( ::com::sun::star::lang::XTypeProvider         ),
                            DIRECT_INTERFACE( ::com::sun::star::container::XElementAccess   ),
                            DIRECT_INTERFACE( ::com::sun::star::container::XIndexAccess     ),
                            DIRECT_INTERFACE( ::com::sun::star::beans::XFastPropertySet		),
                            DIRECT_INTERFACE( ::com::sun::star::beans::XPropertySet		    ),
                            DIRECT_INTERFACE( ::com::sun::star::lang::XUnoTunnel            )
						)

DEFINE_XTYPEPROVIDER_6  (   ConstItemContainer                          ,
                            ::com::sun::star::lang::XTypeProvider       ,
                            ::com::sun::star::container::XIndexAccess   ,
                            ::com::sun::star::container::XElementAccess ,
                            ::com::sun::star::beans::XFastPropertySet   ,
                            ::com::sun::star::beans::XPropertySet       ,
                            ::com::sun::star::lang::XUnoTunnel
						)

ConstItemContainer::ConstItemContainer() : ::cppu::OWeakObject()
{
}

ConstItemContainer::ConstItemContainer( const RootItemContainer& rRootItemContainer, sal_Bool bFastCopy )
{
    ShareGuard( rRootItemContainer.m_aShareMutex );

    // If bFastCopy is set the onwer of the root item container will transfer ownership to us. So
    // it is possible to copy only the root part.
    m_aUIName = rRootItemContainer.m_aUIName;
    if ( bFastCopy )
        m_aItemVector = rRootItemContainer.m_aItemVector;
    else
        copyItemContainer( rRootItemContainer.m_aItemVector );
}

ConstItemContainer::ConstItemContainer( const ItemContainer& rItemContainer )
{
    ShareGuard( rItemContainer.m_aShareMutex );
    copyItemContainer( rItemContainer.m_aItemVector );
}

ConstItemContainer::ConstItemContainer( const Reference< XIndexAccess >& rSourceContainer, sal_Bool bFastCopy )
{
    // We also have to copy the UIName property
    try
    {
        Reference< XPropertySet > xPropSet( rSourceContainer, UNO_QUERY );
        if ( xPropSet.is() )
        {
            rtl::OUString aUIName;
            xPropSet->getPropertyValue( rtl::OUString( RTL_CONSTASCII_USTRINGPARAM( "UIName" ))) >>= m_aUIName;
        }
    }
    catch ( Exception& )
    {
    }

    if ( rSourceContainer.is() )
    {
        try
        {
            sal_Int32 nCount = rSourceContainer->getCount();
            m_aItemVector.reserve(nCount);
            if ( bFastCopy )
            {
                for ( sal_Int32 i = 0; i < nCount; i++ )
                {
                    Sequence< PropertyValue > aPropSeq;
                    if ( rSourceContainer->getByIndex( i ) >>= aPropSeq )
                        m_aItemVector.push_back( aPropSeq );
                }
            }
            else
            {
                for ( sal_Int32 i = 0; i < nCount; i++ )
                {
                    Sequence< PropertyValue > aPropSeq;
                    if ( rSourceContainer->getByIndex( i ) >>= aPropSeq )
                    {
                        sal_Int32 nContainerIndex = -1;
                        Reference< XIndexAccess > xIndexAccess;
                        for ( sal_Int32 j = 0; j < aPropSeq.getLength(); j++ )
                        {
                            if ( aPropSeq[j].Name.equalsAscii( "ItemDescriptorContainer" ))
                            {
                                aPropSeq[j].Value >>= xIndexAccess;
                                nContainerIndex = j;
                                break;
                            }
                        }

                        if ( xIndexAccess.is() && nContainerIndex >= 0 )
                            aPropSeq[nContainerIndex].Value <<= deepCopyContainer( xIndexAccess );

                        m_aItemVector.push_back( aPropSeq );
                    }
                }
            }
        }
        catch ( IndexOutOfBoundsException& )
        {
        }
    }
}

ConstItemContainer::~ConstItemContainer()
{
}

// private
void ConstItemContainer::copyItemContainer( const std::vector< Sequence< PropertyValue > >& rSourceVector )
{
    const sal_uInt32 nCount = rSourceVector.size();
    for ( sal_uInt32 i = 0; i < nCount; i++ )
    {
        sal_Int32 nContainerIndex = -1;
        Sequence< PropertyValue > aPropSeq( rSourceVector[i] );
        Reference< XIndexAccess > xIndexAccess;
        for ( sal_Int32 j = 0; j < aPropSeq.getLength(); j++ )
        {
            if ( aPropSeq[j].Name.equalsAscii( "ItemDescriptorContainer" ))
            {
                aPropSeq[j].Value >>= xIndexAccess;
                nContainerIndex = j;
                break;
            }
        }

        if ( xIndexAccess.is() && nContainerIndex >= 0 )
            aPropSeq[nContainerIndex].Value <<= deepCopyContainer( xIndexAccess );

        m_aItemVector.push_back( aPropSeq );
    }
}

Reference< XIndexAccess > ConstItemContainer::deepCopyContainer( const Reference< XIndexAccess >& rSubContainer )
{
    Reference< XIndexAccess > xReturn;
    if ( rSubContainer.is() )
    {
        ItemContainer*      pSource = ItemContainer::GetImplementation( rSubContainer );
        ConstItemContainer* pSubContainer( 0 );
        if ( pSource )
            pSubContainer = new ConstItemContainer( *pSource );
        else
            pSubContainer = new ConstItemContainer( rSubContainer );
        xReturn = Reference< XIndexAccess >( static_cast< OWeakObject* >( pSubContainer ), UNO_QUERY );
    }

    return xReturn;
}

// XUnoTunnel
sal_Int64 ConstItemContainer::getSomething( const ::com::sun::star::uno::Sequence< sal_Int8 >& rIdentifier ) throw(::com::sun::star::uno::RuntimeException)
{
    if( ( rIdentifier.getLength() == 16 ) && ( 0 == rtl_compareMemory( ConstItemContainer::GetUnoTunnelId().getConstArray(), rIdentifier.getConstArray(), 16 ) ) )
	{
		return reinterpret_cast< sal_Int64 >( this );
	}
	return 0;
}

const Sequence< sal_Int8 >& ConstItemContainer::GetUnoTunnelId() throw()
{
	static ::com::sun::star::uno::Sequence< sal_Int8 > * pSeq = NULL;
	if( !pSeq )
	{
		::osl::Guard< ::osl::Mutex > aGuard( ::osl::Mutex::getGlobalMutex() );
		if( !pSeq )
		{
			static ::com::sun::star::uno::Sequence< sal_Int8 > aSeq( 16 );
			rtl_createUuid( (sal_uInt8*)aSeq.getArray(), 0, sal_True );
			pSeq = &aSeq;
		}
	}
	return *pSeq;
}

ConstItemContainer* ConstItemContainer::GetImplementation( const ::com::sun::star::uno::Reference< ::com::sun::star::uno::XInterface >& rxIFace ) throw()
{
	::com::sun::star::uno::Reference< ::com::sun::star::lang::XUnoTunnel > xUT( rxIFace, ::com::sun::star::uno::UNO_QUERY );
	return xUT.is() ? reinterpret_cast< ConstItemContainer* >(sal::static_int_cast< sal_IntPtr >(
                          xUT->getSomething( ConstItemContainer::GetUnoTunnelId() ))) : NULL;
}

// XElementAccess
sal_Bool SAL_CALL ConstItemContainer::hasElements()
throw ( RuntimeException )
{
    return ( !m_aItemVector.empty() );
}

// XIndexAccess
sal_Int32 SAL_CALL ConstItemContainer::getCount()
throw ( RuntimeException )
{
    return m_aItemVector.size();
}

Any SAL_CALL ConstItemContainer::getByIndex( sal_Int32 Index )
throw ( IndexOutOfBoundsException, WrappedTargetException, RuntimeException )
{
    if ( sal_Int32( m_aItemVector.size()) > Index )
        return makeAny( m_aItemVector[Index] );
    else
		throw IndexOutOfBoundsException( ::rtl::OUString(), (OWeakObject *)this );
}

// XPropertySet
Reference< XPropertySetInfo > SAL_CALL ConstItemContainer::getPropertySetInfo()
throw (::com::sun::star::uno::RuntimeException)
{
	// Optimize this method !
	// We initialize a static variable only one time. And we don't must use a mutex at every call!
	// For the first call; pInfo is NULL - for the second call pInfo is different from NULL!
    static Reference< XPropertySetInfo >* pInfo = NULL;

    if( pInfo == NULL )
	{
		// Ready for multithreading
		osl::MutexGuard aGuard( osl::Mutex::getGlobalMutex() ) ;
		// Control this pointer again, another instance can be faster then these!
        if( pInfo == NULL )
		{
			// Create structure of propertysetinfo for baseclass "OPropertySetHelper".
			// (Use method "getInfoHelper()".)
            static Reference< XPropertySetInfo > xInfo( createPropertySetInfo( getInfoHelper() ) );
			pInfo = &xInfo;
		}
	}

	return (*pInfo);
}

void SAL_CALL ConstItemContainer::setPropertyValue( const ::rtl::OUString&, const Any& )
throw (::com::sun::star::beans::UnknownPropertyException, ::com::sun::star::beans::PropertyVetoException, ::com::sun::star::lang::IllegalArgumentException, ::com::sun::star::lang::WrappedTargetException, ::com::sun::star::uno::RuntimeException)
{
}

Any SAL_CALL ConstItemContainer::getPropertyValue( const ::rtl::OUString& PropertyName )
throw (::com::sun::star::beans::UnknownPropertyException, ::com::sun::star::lang::WrappedTargetException, ::com::sun::star::uno::RuntimeException)
{
    if ( PropertyName.equals( PROPNAME_UINAME ))
        return makeAny( m_aUIName );

    throw UnknownPropertyException();
}

void SAL_CALL ConstItemContainer::addPropertyChangeListener( const ::rtl::OUString&, const ::com::sun::star::uno::Reference< ::com::sun::star::beans::XPropertyChangeListener >& )
throw (::com::sun::star::beans::UnknownPropertyException, ::com::sun::star::lang::WrappedTargetException, ::com::sun::star::uno::RuntimeException)
{
}

void SAL_CALL ConstItemContainer::removePropertyChangeListener( const ::rtl::OUString&, const ::com::sun::star::uno::Reference< ::com::sun::star::beans::XPropertyChangeListener >& )
throw (::com::sun::star::beans::UnknownPropertyException, ::com::sun::star::lang::WrappedTargetException, ::com::sun::star::uno::RuntimeException)
{
    // Only read-only properties - do nothing
}

void SAL_CALL ConstItemContainer::addVetoableChangeListener( const ::rtl::OUString&, const ::com::sun::star::uno::Reference< ::com::sun::star::beans::XVetoableChangeListener >& )
throw (::com::sun::star::beans::UnknownPropertyException, ::com::sun::star::lang::WrappedTargetException, ::com::sun::star::uno::RuntimeException)
{
    // Only read-only properties - do nothing
}

void SAL_CALL ConstItemContainer::removeVetoableChangeListener( const ::rtl::OUString&, const ::com::sun::star::uno::Reference< ::com::sun::star::beans::XVetoableChangeListener >& )
throw (::com::sun::star::beans::UnknownPropertyException, ::com::sun::star::lang::WrappedTargetException, ::com::sun::star::uno::RuntimeException)
{
    // Only read-only properties - do nothing
}

// XFastPropertySet
void SAL_CALL ConstItemContainer::setFastPropertyValue( sal_Int32, const ::com::sun::star::uno::Any& )
throw (::com::sun::star::beans::UnknownPropertyException, ::com::sun::star::beans::PropertyVetoException, ::com::sun::star::lang::IllegalArgumentException, ::com::sun::star::lang::WrappedTargetException, ::com::sun::star::uno::RuntimeException)
{
}

Any SAL_CALL ConstItemContainer::getFastPropertyValue( sal_Int32 nHandle )
throw (::com::sun::star::beans::UnknownPropertyException, ::com::sun::star::lang::WrappedTargetException, ::com::sun::star::uno::RuntimeException)
{
    if ( nHandle == PROPHANDLE_UINAME )
        return makeAny( m_aUIName );

    throw UnknownPropertyException();
}

::cppu::IPropertyArrayHelper& SAL_CALL ConstItemContainer::getInfoHelper()
{
	// Optimize this method !
	// We initialize a static variable only one time. And we don't must use a mutex at every call!
	// For the first call; pInfoHelper is NULL - for the second call pInfoHelper is different from NULL!
    static ::cppu::OPropertyArrayHelper* pInfoHelper = NULL;

    if( pInfoHelper == NULL )
	{
		// Ready for multithreading
        osl::MutexGuard aGuard( osl::Mutex::getGlobalMutex() ) ;

		// Control this pointer again, another instance can be faster then these!
        if( pInfoHelper == NULL )
		{
			// Define static member to give structure of properties to baseclass "OPropertySetHelper".
			// "impl_getStaticPropertyDescriptor" is a non exported and static function, who will define a static propertytable.
			// "sal_True" say: Table is sorted by name.
            static ::cppu::OPropertyArrayHelper aInfoHelper( impl_getStaticPropertyDescriptor(), sal_True );
			pInfoHelper = &aInfoHelper;
		}
	}

    return(*pInfoHelper);
}

const com::sun::star::uno::Sequence< com::sun::star::beans::Property > ConstItemContainer::impl_getStaticPropertyDescriptor()
{
	// Create a new static property array to initialize sequence!
	// Table of all predefined properties of this class. Its used from OPropertySetHelper-class!
	// Don't forget to change the defines (see begin of this file), if you add, change or delete a property in this list!!!
	// It's necessary for methods of OPropertySetHelper.
	// ATTENTION:
    //      YOU MUST SORT FOLLOW TABLE BY NAME ALPHABETICAL !!!

    static const com::sun::star::beans::Property pProperties[] =
	{
        com::sun::star::beans::Property( PROPNAME_UINAME, PROPHANDLE_UINAME ,
                                         ::getCppuType((const rtl::OUString*)NULL),
                                         com::sun::star::beans::PropertyAttribute::TRANSIENT | com::sun::star::beans::PropertyAttribute::READONLY  )
	};
	// Use it to initialize sequence!
    static const com::sun::star::uno::Sequence< com::sun::star::beans::Property > lPropertyDescriptor( pProperties, PROPCOUNT );
	// Return static "PropertyDescriptor"
    return lPropertyDescriptor;
}

Reference < XPropertySetInfo > ConstItemContainer::createPropertySetInfo(
	IPropertyArrayHelper & rProperties ) SAL_THROW( () )
{
	return static_cast< XPropertySetInfo * >( new OPropertySetHelperInfo_Impl( rProperties ) );
}

} // namespace framework

