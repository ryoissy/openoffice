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



#ifndef _DAVTYPES_HXX_
#define _DAVTYPES_HXX_

#include <rtl/ustring.hxx>
#include <com/sun/star/uno/Any.hxx>

namespace http_dav_ucp
{
/* RFC 2518

15.1 Class 1

   A class 1 compliant resource MUST meet all "MUST" requirements in all
   sections of this document.

   Class 1 compliant resources MUST return, at minimum, the value "1" in
   the DAV header on all responses to the OPTIONS method.

15.2 Class 2

   A class 2 compliant resource MUST meet all class 1 requirements and
   support the LOCK method, the supportedlock property, the
   lockdiscovery property, the Time-Out response header and the Lock-
   Token request header.  A class "2" compliant resource SHOULD also
   support the Time-Out request header and the owner XML element.

   Class 2 compliant resources MUST return, at minimum, the values "1"
   and "2" in the DAV header on all responses to the OPTIONS method.
*/

struct DAVCapabilities
{
	bool class1;
	bool class2;
	bool executable; // supports "executable" property (introduced by mod_dav)

	DAVCapabilities() : class1( false ), class2( false ), executable( false ) {}
};

enum Depth { DAVZERO = 0, DAVONE = 1, DAVINFINITY = -1 };

enum ProppatchOperation { PROPSET = 0, PROPREMOVE = 1 };

struct ProppatchValue
{
	ProppatchOperation 		 operation;
	rtl::OUString            name;
	com::sun::star::uno::Any value;

	ProppatchValue( const ProppatchOperation o,
					const rtl::OUString & n,
					const com::sun::star::uno::Any & v )
	: operation( o ), name( n ), value( v ) {}
};

} // namespace http_dav_ucp

#endif // _DAVTYPES_HXX_
