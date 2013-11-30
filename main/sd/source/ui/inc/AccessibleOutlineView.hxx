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



#ifndef SD_ACCESSIBILITY_ACCESSIBLE_OUTLINE_VIEW_HXX
#define SD_ACCESSIBILITY_ACCESSIBLE_OUTLINE_VIEW_HXX

#include "AccessibleDocumentViewBase.hxx"
#include <svx/AccessibleTextHelper.hxx>

namespace sd {
class OutlineViewShell;
class Window;
}

namespace accessibility {


/** This class makes the Impress outline view accessible.

    Please see the documentation of the base class for further
    explanations of the individual methods. This class is a mere
    wrapper around the AccessibleTextHelper class; as basically the
    Outline View is a big Outliner.
*/
class AccessibleOutlineView
    : public AccessibleDocumentViewBase
{
public:
    AccessibleOutlineView (
        ::sd::Window* pSdWindow,
        ::sd::OutlineViewShell* pViewShell,
        const ::com::sun::star::uno::Reference<
        ::com::sun::star::frame::XController>& rxController,
        const ::com::sun::star::uno::Reference<
        ::com::sun::star::accessibility::XAccessible>& rxParent);

    virtual ~AccessibleOutlineView	(void);

    /** Complete the initialization begun in the constructor.
    */
    virtual void Init (void);


    //=====  IAccessibleViewForwarderListener  ================================

    virtual void ViewForwarderChanged (ChangeType aChangeType, 
        const IAccessibleViewForwarder* pViewForwarder);

    //=====  XAccessibleContext  ==============================================

    virtual sal_Int32 SAL_CALL
        getAccessibleChildCount (void)
        throw (::com::sun::star::uno::RuntimeException);

    virtual ::com::sun::star::uno::Reference< ::com::sun::star::accessibility::XAccessible> SAL_CALL
        getAccessibleChild (sal_Int32 nIndex)
        throw (::com::sun::star::uno::RuntimeException);
    virtual ::rtl::OUString SAL_CALL
        getAccessibleName(void)
        throw (::com::sun::star::uno::RuntimeException);
    //=====  XAccessibleEventBroadcaster  ========================================

    virtual void SAL_CALL 
        addEventListener (
            const ::com::sun::star::uno::Reference< 
                ::com::sun::star::accessibility::XAccessibleEventListener >& xListener) 
        throw (::com::sun::star::uno::RuntimeException);

    virtual void SAL_CALL 
        removeEventListener ( 
            const ::com::sun::star::uno::Reference< 
                ::com::sun::star::accessibility::XAccessibleEventListener >& xListener) 
        throw (::com::sun::star::uno::RuntimeException);

	using cppu::WeakComponentImplHelperBase::addEventListener;
	using cppu::WeakComponentImplHelperBase::removeEventListener;

    //=====  XServiceInfo  ====================================================
    
    /**	Returns an identifier for the implementation of this object.
    */
	virtual ::rtl::OUString SAL_CALL
    	getImplementationName (void)
	    throw (::com::sun::star::uno::RuntimeException);

    //=====  lang::XEventListener  ============================================

	virtual void SAL_CALL
		disposing (const ::com::sun::star::lang::EventObject& rEventObject)
        throw (::com::sun::star::uno::RuntimeException);

    //=====  XPropertyChangeListener  =========================================

	virtual void SAL_CALL
		propertyChange (const ::com::sun::star::beans::PropertyChangeEvent& rEventObject)
        throw (::com::sun::star::uno::RuntimeException);


protected:

    // overridden, as we hold the listeners ourselves
    virtual void FireEvent (const ::com::sun::star::accessibility::AccessibleEventObject& aEvent);

    // overridden to detect focus changes
    virtual void Activated (void);

    // overridden to detect focus changes
    virtual void Deactivated (void);

    // declared, but not defined
    AccessibleOutlineView( const AccessibleOutlineView& );
    AccessibleOutlineView& operator= ( const AccessibleOutlineView& );

    // This method is called from the component helper base class while disposing.
    virtual void SAL_CALL disposing (void);

    ///	Create an accessible name that contains the current view mode.
    virtual ::rtl::OUString
    	CreateAccessibleName ()
        throw (::com::sun::star::uno::RuntimeException);

    ///	Create an accessible description that contains the current
    ///	view mode.
    virtual ::rtl::OUString
    	CreateAccessibleDescription ()
        throw (::com::sun::star::uno::RuntimeException);

private:

    /// Invalidate text helper, updates visible children
    void UpdateChildren();

    AccessibleTextHelper maTextHelper;

};

} // end of namespace accessibility

#endif
