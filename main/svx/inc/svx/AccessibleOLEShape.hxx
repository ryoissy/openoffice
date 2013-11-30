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



#ifndef _SVX_ACCESSIBILITY_ACCESSIBLE_OLE_SHAPE_HXX
#define _SVX_ACCESSIBILITY_ACCESSIBLE_OLE_SHAPE_HXX

#ifndef _SVX_ACCESSIBILITY_ACCESSIBLE_SHAPE_MANAGER_HXX
#include <svx/AccessibleShape.hxx>
#endif

#ifndef _COM_SUN_STAR_ACCESSIBILITY_XACCESSIBLE_ACTION_HPP_
#include <com/sun/star/accessibility/XAccessibleAction.hpp>
#endif
#include "svx/svxdllapi.h"

namespace accessibility {

/**	@descr
        This class makes OLE objects accessible.  With respect to its
        base class <type>AccessibleShape</type> it supports the additional
        <type>XAccessibleAction</type> interface.
*/
class SVX_DLLPUBLIC AccessibleOLEShape
    :	public AccessibleShape,
        public ::com::sun::star::accessibility::XAccessibleAction 
{
public:
	//=====  internal  ========================================================
	AccessibleOLEShape (
        const AccessibleShapeInfo& rShapeInfo,
        const AccessibleShapeTreeInfo& rShapeTreeInfo);
	virtual ~AccessibleOLEShape (void);

    //=====  XAccessibleAction  ===============================================

    sal_Int32 SAL_CALL getAccessibleActionCount (void)
        throw (::com::sun::star::uno::RuntimeException);

    sal_Bool SAL_CALL doAccessibleAction (sal_Int32 nIndex) 
        throw (::com::sun::star::lang::IndexOutOfBoundsException, 
            ::com::sun::star::uno::RuntimeException);

    ::rtl::OUString SAL_CALL getAccessibleActionDescription (sal_Int32 nIndex)
        throw (::com::sun::star::lang::IndexOutOfBoundsException, 
            ::com::sun::star::uno::RuntimeException);

    ::com::sun::star::uno::Reference< 
        ::com::sun::star::accessibility::XAccessibleKeyBinding> SAL_CALL getAccessibleActionKeyBinding (
            sal_Int32 nIndex)
        throw (::com::sun::star::lang::IndexOutOfBoundsException, 
            ::com::sun::star::uno::RuntimeException);

    //=====  XInterface  ======================================================

    virtual com::sun::star::uno::Any SAL_CALL
        queryInterface (const com::sun::star::uno::Type & rType)
        throw (::com::sun::star::uno::RuntimeException);

    virtual void SAL_CALL
        acquire (void) 
        throw ();

    virtual void SAL_CALL
        release (void)
        throw ();

	//=====  XServiceInfo  ====================================================

	virtual ::rtl::OUString SAL_CALL
    	getImplementationName (void)
	    throw (::com::sun::star::uno::RuntimeException);

    virtual ::com::sun::star::uno::Sequence< ::rtl::OUString> SAL_CALL
        getSupportedServiceNames (void)
        throw (::com::sun::star::uno::RuntimeException);

    //=====  XTypeProvider  ===================================================

    virtual ::com::sun::star::uno::Sequence< ::com::sun::star::uno::Type> SAL_CALL
        getTypes (void)
        throw (::com::sun::star::uno::RuntimeException);
// ====== XAccessibleExtendedAttributes =====================================
    virtual ::com::sun::star::uno::Any SAL_CALL getExtendedAttributes() throw (::com::sun::star::lang::IndexOutOfBoundsException, ::com::sun::star::uno::RuntimeException) ;
protected:
    ///	Create a name string that contains the accessible name.
	virtual ::rtl::OUString
    	CreateAccessibleBaseName ()
        throw (::com::sun::star::uno::RuntimeException);

    ///	Create a description string that contains the accessible description.
	virtual ::rtl::OUString
    	CreateAccessibleDescription ()
        throw (::com::sun::star::uno::RuntimeException);

private:
    /** Don't use the default constructor.  Use the public constructor that 
        takes the original shape and the parent as arguments instead.
    */
    SVX_DLLPRIVATE AccessibleOLEShape (void);

    /// Don't use the constructor.  Not yet implemented.
    SVX_DLLPRIVATE AccessibleOLEShape (const AccessibleOLEShape&);

    /// Don't use the assignment operator.  Not yet implemented.
    SVX_DLLPRIVATE AccessibleOLEShape& operator= (const AccessibleOLEShape&);
};

} // end of namespace accessibility

#endif
